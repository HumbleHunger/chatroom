/*************************************************************************
	> File Name: delgroup.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月05日 星期三 21时35分25秒
 ************************************************************************/

#include"client.h"
int delgroup()
{
    P_LOCK;
    printf("请输入需要解散的群ID\n");
    P_UNLOCK;
    char gid[10];
    memset(gid,0,sizeof(gid));
    scanf("%s",gid);
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n",user_id,gid);
    //printf("delgroup send_buf is %s",send_buf);//
    if(send_pack(connfd,DELGROUP,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    return 0;
}
