/*************************************************************************
	> File Name: send_thread.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月12日 星期三 15时16分27秒
 ************************************************************************/

#include"chat.h"
#include"thread_pool.h"
void *send_thread(void *arg)
{
    while(1){
        pthread_mutex_lock(&mutex1);
        if(pack_queue.head==NULL)
        {
            printf ("send thread %ld is waiting\n", pthread_self ());
            pthread_cond_wait(&cond1,&mutex1);
        }
        if(pool->shutdown==1){
            pthread_mutex_unlock(&mutex1);
            pthread_exit(NULL);
        }
        queue_node *operate=pack_queue.head;
        del_pack();
        pthread_mutex_unlock(&mutex1);
        //printf("connfd is %d\nlen is %d\npack is %s\n",operate->connfd,operate->len,((Pack *)operate->pack)->data);
        if(my_write(operate->connfd,operate->pack,operate->len)<0){
            return NULL;
        }
        free(operate->pack);
        free(operate);
        operate=NULL;
        if(pool->shutdown==1){
            pthread_exit(NULL);
        }
    }
}
