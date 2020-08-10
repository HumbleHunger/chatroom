/*************************************************************************
	> File Name: gchatmsg.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月06日 星期四 15时57分35秒
 ************************************************************************/

#include"client.h"
int gchatmsg()
{
    P_LOCK;
    printf("请输入群ID\n");
    P_UNLOCK;
    char gid[10];
    S_LOCK;
    scanf("%s",gid);
    S_UNLOCK;
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n",user_id,gid);
    //printf("gchatmsg send_buf is %s",send_buf);//
    if(send_pack(connfd,GCHATMSG,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    return 0;
}
