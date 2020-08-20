/*************************************************************************
	> File Name: blockfriend.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月01日 星期六 14时46分34秒
 ************************************************************************/

#include"client.h"
int blockfriend()
{
    P_LOCK;
    printf("请输入需要拉黑的好友ID\n");
    P_UNLOCK;
    char id[10];
    memset(id,0,sizeof(id));
    S_LOCK;
    Scanf(id);
    S_UNLOCK;
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n",user_id,id);
    //printf("blockfriend send_buf is %s",send_buf);//
    if(send_pack(connfd,BLOCKFRIEND,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    return 0;
}
