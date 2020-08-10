/*************************************************************************
	> File Name: groupmember.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月06日 星期四 14时38分06秒
 ************************************************************************/

#include"client.h"
int groupmember()
{
    P_LOCK;
    printf("请输入您要查询的群ID\n");
    P_UNLOCK;
    char gid[10];
    scanf("%s",gid);
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n",user_id,gid);
    //printf("groupmember send_buf is %s",send_buf);//
    if(send_pack(connfd,GROUPMEMBER,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
}
