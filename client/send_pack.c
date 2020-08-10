/*************************************************************************
	> File Name: send_pack.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月28日 星期二 17时02分10秒
 *************************************************************************/

#include"client.h"
int send_pack(int connfd,int type,int len,char *value)
{
    Pack send_buf;
    memset(&send_buf,0,sizeof(Pack));
    sprintf(send_buf.type,"%d",type);
    P_LOCK;
    //printf("发送数据type为%s\n",send_buf.type);
    sprintf(send_buf.len,"%d",len);
    //printf("发送数据len为%s\n",send_buf.len);
    int lenth=atoi(send_buf.len)+4;
    sprintf(send_buf.data,"%s",value);
    //printf("send_pack is %d byte\n",lenth);//
    //printf("发送数据为%s",send_buf.data);
    P_UNLOCK;
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

