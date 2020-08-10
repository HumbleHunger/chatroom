/*************************************************************************
	> File Name: creategroup.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月03日 星期一 21时56分25秒
 ************************************************************************/

#include"client.h"
int creategroup()
{
    P_LOCK;
    printf("请输入您的群名\n");
    P_UNLOCK;
    char name[32];
    memset(name,0,sizeof(name));
    scanf("%s",name);
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n",user_id,name);
    //printf("%s",send_buf);//
    if(send_pack(connfd,CREATEGROUP,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    return 0;
}
