/*************************************************************************
	> File Name: send_pack.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月28日 星期二 15时56分40秒
 ************************************************************************/

#include"chat.h"
#include"thread_pool.h"
int send_pack(int connfd,int type,int len,char *value)
{
    Pack *send_buf=(Pack *)malloc(sizeof(Pack));
    memset(send_buf,0,sizeof(Pack));
    sprintf(send_buf->type,"%d",type);
    sprintf(send_buf->len,"%d",len);
    int lenth=atoi(send_buf->len)+4;
    sprintf(send_buf->data,"%s",value);
    //printf("向%d发送数据%s\n",connfd,send_buf.data);
    add_pack(connfd,(char *)send_buf,lenth);
    /*if(my_write(connfd,(char *)&send_buf,lenth)<0){
        return -1;
    }*/
    return 0;
}
int send_packs(int connfd,int type,int len,char *value)
{
    Pack send_buf;
    memset(&send_buf,0,sizeof(Pack));
    sprintf(send_buf.type,"%d",type);
    //printf("发送数据type为%s\n",send_buf.type);
    sprintf(send_buf.len,"%d",len);
    //printf("发送数据len为%s\n",send_buf.len);
    int lenth=atoi(send_buf.len)+4;
    sprintf(send_buf.data,"%s",value);
    //printf("send_pack is %d byte\n",lenth);//
    //printf("发送数据为%s",send_buf.data);
    if(my_write(connfd,(char *)&send_buf,lenth)<0){
        return -1;
    }
    return 0;
}
int add_pack(int connfd,char *pack,int len)
{
    queue_node *operate,*record;
    operate=(queue_node *)malloc(sizeof(queue_node));
    operate->connfd=connfd;
    operate->len=len;
    operate->pack=pack;
    operate->next=NULL;

    pthread_mutex_lock(&mutex1);
    if(pack_queue.head==NULL){
        pack_queue.head=operate;
        pack_queue.tail=operate;
    }
    else{
        record=pack_queue.tail;
        record->next=operate;
        pack_queue.tail=operate;
    }
    //printf("add_work Namei\n");
    pthread_mutex_unlock(&mutex1);
    pthread_cond_signal(&cond1);
}
int del_pack()
{
    queue_node *operate=pack_queue.head;
    pack_queue.head=operate->next;
    if(pack_queue.head==NULL){
        pack_queue.tail=NULL;
    }
}
int my_write(int connfd,char *send_buf,int len)
{
    int nleft=len;
    int nwrite=0;
    char *bufp=send_buf;
    //循环写数据
    while(nleft>0){
        if((nwrite=write(connfd,bufp,nleft))==-1){
            if(errno==EWOULDBLOCK){
                nwrite=0;
            }else{
                return -1;
            }
        }else if(nwrite==0){
            continue;
        }
        nleft-=nwrite;
        bufp+=nwrite;
    }
    return (len-nleft);
}

