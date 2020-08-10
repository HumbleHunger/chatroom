/*************************************************************************
	> File Name: friendlist.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月31日 星期五 16时58分59秒
 ************************************************************************/

#include"client.h"
int friendlist()
{
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n",user_id);
    //printf("friendlist send_buf is %s",send_buf);//
    /*P_LOCK;
    printf("\t\t\t\t\t用户ID   用户名   在线状态\n");
    P_UNLOCK;*/
    if(send_pack(connfd,FRIENDLIST,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    return 0;
}

