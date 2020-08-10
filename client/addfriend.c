/*************************************************************************
	> File Name: addfriend.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月30日 星期四 10时05分09秒
 ************************************************************************/

#include"client.h"
int addfriend()
{
    P_LOCK;
    printf("请输入对方ID\n");
    P_UNLOCK;
    char fid[10];
    memset(fid,0,sizeof(fid));
    S_LOCK;
    scanf("%s",fid);
    S_UNLOCK;
    if(strcmp(user_id,fid)==0){
        printf("请勿添加自己为好友\n");
        return 0;
    }
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n",user_id,fid);
    //printf("addfriend send_buf is %s",send_buf);//
    if(send_pack(connfd,ADDFRIEND,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    return 0;
}

