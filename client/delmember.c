/*************************************************************************
	> File Name: delmember.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月05日 星期三 20时41分03秒
 ************************************************************************/

#include"client.h"
int delmember()
{
    P_LOCK;
    printf("请输入需要进行踢人操作的群ID\n");
    P_UNLOCK;
    char gid[10];
    memset(gid,0,sizeof(gid));
    //S_LOCK;
    Scanf(gid);
    P_LOCK;
    printf("请输入需要踢出的成员ID\n");
    P_UNLOCK;
    char mid[10];
    memset(mid,0,sizeof(mid));
    //S_LOCK;
    Scanf(mid);
    //S_UNLOCK;
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n%s\n",user_id,mid,gid);
    //printf("delmember send_buf is %s",send_buf);//
    if(send_pack(connfd,DELMEMBER,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    return 0;
}
