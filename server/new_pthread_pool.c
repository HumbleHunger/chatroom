/*************************************************************************
	> File Name: new_pthread_pool.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月18日 星期六 11时32分06秒
 ************************************************************************/

#include "thread_pool.h"
#include"chat.h"
int full()
{
    if(pool->work_num==pool->max_work_num){
        return 1;
    }
    else{
        return 0;
    }
}
int add_work(void *(*fun)(void *arg),void *arg)
{
    Work *operate,*record;
    operate=(Work *)malloc(sizeof(Work));
    operate->fun=fun;
    operate->arg=arg;
    operate->next=NULL;
    
    pthread_mutex_lock(&mutex);
    while(full()==1){
        pthread_cond_wait(&work_not_full,&mutex);
    }
    pool->work_num++;
    if(pool->queue_head==NULL){
        pool->queue_head=operate;
        pool->queue_tail=operate;
    }
    else{
        record=pool->queue_tail;
        record->next=operate;
        pool->queue_tail=operate;
    }
    //printf("add_work Namei\n");
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);
}
int del_work()
{
    Work *operate=pool->queue_head;
    pool->queue_head=operate->next;
    if(pool->queue_head==NULL){
        pool->queue_tail=NULL;
    }
    pool->work_num--;
}
void *thread(void *a)
{
    while(1){
        pthread_mutex_lock(&mutex);
        while (pool->work_num == 0 && !pool->shutdown)
        {
            printf ("thread %ld is waiting\n", pthread_self ());
            pthread_cond_wait(&cond,&mutex);
        }
        if(pool->shutdown==1){
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
        //重新设置本线程TSD为0
        int tsd;
        tsd=0;
        pthread_setspecific(key,(void *)tsd);
        printf("thread %ld is working\n",pthread_self());
        Work *operate=pool->queue_head;
        del_work();
        pthread_mutex_unlock(&mutex);
        //printf("start\n");//
        (*(operate->fun))(operate->arg);
        free(operate);
        pthread_cond_signal(&work_not_full);
        operate=NULL;
        /*        if(pool->shutdown==1){
            pthread_exit(NULL);
        }
*/
    }
}
void pool_init(int thread_num)
{
    pthread_cond_init(&cond,NULL);
    pthread_mutex_init(&mutex,NULL);

    pool=(Thread_pool *)malloc(sizeof(Thread_pool));

    pool->queue_head=NULL;
    pool->queue_tail=NULL;
    pool->work_num=0;
    pool->max_work_num=20;

    pool->thid=(pthread_t *)malloc(10 * sizeof(pthread_t));
    memset(pool->thid,0,10 * sizeof(pthread_t));
    pool->thread_num=thread_num;
    pool->max_thread_num=10;

    pool->shutdown=0;
    
    for(int i=0;i<thread_num;i++){
        pthread_create(&pool->thid[i],NULL,thread,NULL);
    }
}
void destroy_queue()
{
    Work *operate,*record;
    operate=pool->queue_head;
    while(operate){
        record=operate->next;
        free(operate);
        operate=record;
    }
}
void pool_destroy()
{
    pool->shutdown=1;
    pthread_cond_broadcast(&cond);
    for(int i=0;i<pool->thread_num;i++){
        pthread_join(pool->thid[i],NULL);
    }
    free(pool->thid);
    destroy_queue();
    free(pool);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}
