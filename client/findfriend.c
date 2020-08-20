/*************************************************************************
	> File Name: findfriend.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月31日 星期五 21时12分31秒
 ************************************************************************/

#include"client.h"
int findfriend()
{
    P_LOCK;
    printf("请输入需要查找的ID\n");
    P_UNLOCK;
    char id[10];
    memset(id,0,sizeof(id));
    S_LOCK;
    Scanf(id);
    S_UNLOCK;
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n",id);
    //printf("findfriend send_buf is %s",send_buf);//
    if(send_pack(connfd,FINDFRIEND,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    return 0;
}
