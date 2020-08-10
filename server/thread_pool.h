/*************************************************************************
	> File Name: thread_pool.h
	> Author: 
	> Mail: 
	> Created Time: 2020年07月27日 星期一 21时10分02秒
 ************************************************************************/

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
typedef struct work{
    void *(*fun)(void *arg);
    void *arg;
    struct work* next;
}Work;
typedef struct
{
    Work *queue_head;
    Work *queue_tail;
    int work_num;
    int max_work_num;
    
    pthread_t *thid;
    int thread_num;
    int max_thread_num;
    
    int shutdown;
}Thread_pool;
Thread_pool *pool;
pthread_mutex_t mutex;
pthread_cond_t cond,work_not_full;
int full();
int add_work(void *(*fun)(void *arg),void *arg);
int del_work();
void *thread(void *a);
void pool_init(int max_thread_num);
void destroy_queue();
void pool_destroy();

#endif
