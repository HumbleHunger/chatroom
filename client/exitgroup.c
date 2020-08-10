/*************************************************************************
	> File Name: exitgroup.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月04日 星期二 21时37分09秒
 ************************************************************************/

#include"client.h"
int exitgroup()
{
    P_LOCK;
    printf("请输入需要退出的群ID\n");
    P_UNLOCK;
    char gid[10];
    memset(gid,0,sizeof(gid));
    //S_LOCK;
    scanf("%s",gid);
    //S_UNLOCK;
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n",user_id,gid);
    //printf("exitgroup send_buf is %s",send_buf);//
    if(send_pack(connfd,EXITGROUP,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    return 0;
}
