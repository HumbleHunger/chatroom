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
    Pack send_buf;
    memset(&send_buf,0,sizeof(Pack));
    sprintf(send_buf.type,"%d",type);
    sprintf(send_buf.len,"%d",len);
    int lenth=atoi(send_buf.len)+4;
    sprintf(send_buf.data,"%s",value);
    //printf("向%d发送数据%s\n",connfd,send_buf.data);
    if(my_write(connfd,(char *)&send_buf,lenth)<0){
        return -1;
    }
    return 0;
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

