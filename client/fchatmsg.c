/*************************************************************************
	> File Name: fchatmsg.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月06日 星期四 15时19分20秒
 ************************************************************************/

#include"client.h"
int fchatmsg()
{
    P_LOCK;
    printf("请输入好友ID\n");
    P_UNLOCK;
    char fid[10];
    S_LOCK;
    scanf("%s",fid);
    S_UNLOCK;
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n",user_id,fid);
    //printf("fchatmsg send_buf is %s",send_buf);//
    if(send_pack(connfd,FCHATMSG,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    return 0;
}
