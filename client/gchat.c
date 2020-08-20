/*************************************************************************
	> File Name: gchat.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月05日 星期三 15时30分19秒
 ************************************************************************/

#include"client.h"
int gchat()
{
    P_LOCK;
    printf("请输入您要群聊的群ID\n");
    P_UNLOCK;
    char gid[10];
    Scanf(gid);
    strcpy(chat_id,gid);
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n",user_id,gid);
    //printf("group send_buf is %s",send_buf);//
    if(send_pack(connfd,GROUP,strlen(send_buf),send_buf)<0){
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
        sprintf(send_buf,"%s\n%s\n%s\n",user_id,gid,msg);
        //printf("gchat send_buf is %s",send_buf);//
        if(send_pack(connfd,GCHAT,strlen(send_buf),send_buf)<0){
            my_err("write",__LINE__);
        }
    }
    S_UNLOCK;
    chat_flag=0;
    memset(chat_id,0,sizeof(chat_id));
    return 0;
}
