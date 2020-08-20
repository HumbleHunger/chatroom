/*************************************************************************
	> File Name: fchat.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月01日 星期六 16时06分25秒
 ************************************************************************/

#include"client.h"
int fchat()
{
    P_LOCK;
    printf("请输入您要私聊的好友ID\n");
    P_UNLOCK;
    char *fid=(char *)malloc(10);
    memset(fid,0,sizeof(fid));
    Scanf(fid);
    if(strcmp(user_id,fid)==0){
        printf("\t\t\t\t\t请不要与自己聊天\n");
        return 0;
    }
    strcpy(chat_id,fid);
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n",user_id,fid);
    //printf("friend send_buf is %s",send_buf);//
    if(send_pack(connfd,FRIEND,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    P_LOCK;
    printf("正在处理中\n");
    P_UNLOCK;
    S_LOCK;
    C_WAIT;
    if(chat_flag==0){
        memset(chat_id,0,sizeof(chat_id));
        S_UNLOCK;
        return 0;
    }
    printf("聊天开始  输入EXITCHAT退出\n");
    char msg[75];
    while(1){
        memset(msg,0,sizeof(msg));
        Scanfs(msg);
        if(strcmp(msg,"EXITCHAT")==0){
            break;
        }
        memset(send_buf,0,sizeof(send_buf));
        sprintf(send_buf,"%s\n%s\n%s\n",user_id,fid,msg);
        //printf("fchat send_buf is %s",send_buf);//
        if(send_pack(connfd,FCHAT,strlen(send_buf),send_buf)<0){
            my_err("write",__LINE__);
        }
    }
    S_UNLOCK;
    chat_flag=0;
    memset(chat_id,0,sizeof(chat_id));
    free(fid);
    return 0;
}
