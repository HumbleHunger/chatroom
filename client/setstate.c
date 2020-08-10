/*************************************************************************
	> File Name: setstate.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月04日 星期二 22时19分53秒
 ************************************************************************/

#include"client.h"
int gsetstate()
{
    P_LOCK;
    printf("请输入需要设置的群ID\n");
    P_UNLOCK;
    char gid[10];
    memset(gid,0,sizeof(gid));
    //S_LOCK;
    scanf("%s",gid);
    P_LOCK;
    printf("请输入需要设置的成员ID\n");
    P_UNLOCK;
    char mid[10];
    memset(mid,0,sizeof(mid));
    //S_LOCK;
    scanf("%s",mid);
    //S_UNLOCK;
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n%s\n",user_id,mid,gid);
    //printf("setstate send_buf is %s",send_buf);//
    if(send_pack(connfd,SETSTATE,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    return 0;
}
