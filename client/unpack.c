/*************************************************************************
	> File Name: unpack.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月28日 星期二 17时16分05秒
 ************************************************************************/

#include"client.h"
int my_read(int conn_fd,void *buf,int len)
{
    int nleft=len;
    int nread=0;
    char *bufp=buf;
    //循环读数据防止操作被中断
    while(nleft>0){
        if((nread=read(conn_fd,bufp,nleft))==-1){
            if(errno==EINTR){
                nread=0;
            }else{
                return -1;
            }
        }else if(nread==0){
            continue;
        }
        nleft-=nread;
        bufp+=nread;
    }

    return (len-nleft);
}
int unpack(int connfd,char *recv_buf,int lenth)
{
    //接收数据类型
    char ty[3];
    memset(ty,0,sizeof(ty));
    if(read(connfd,ty,2)<0){
        my_err("read",__LINE__);
    }

    //接收数据长度
    char len[3];
    memset(len,0,sizeof(len));
    if(read(connfd,len,2)<0){
        my_err("read",__LINE__);
    }
    //printf("pack len is %s\n",len);
    //接收数据
    int ret;
    memset(recv_buf,0,sizeof(recv_buf));
    if((ret=my_read(connfd,recv_buf,atoi(len)))<0){
        my_err("recv",__LINE__);
    }
    return ret;
}
