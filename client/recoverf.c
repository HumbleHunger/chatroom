/*************************************************************************
	> File Name: recoverf.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月13日 星期四 21时13分09秒
 ************************************************************************/

#include"client.h"
int recoverf()
{
    P_LOCK;
    printf("请输入需要恢复的好友ID\n");
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
    if(send_pack(connfd,RECOVERF,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    return 0;
}
