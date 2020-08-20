/*************************************************************************
	> File Name: rigister.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月28日 星期二 16时53分05秒
 ************************************************************************/

#include"client.h"
int uregister()
{
    printf("请输入您的用户名\n");
    char name[20];
    memset(name,0,sizeof(name));
    Scanfs(name);
    printf("请输入您的密码\n");
    char psw[20];
    memset(psw,0,sizeof(psw));
    Scanfs(psw);    
    printf("请输入您的密保问题\n");
    char question[25];
    memset(question,0,sizeof(question));
    Scanfs(question);
    printf("请输入您的密保答案\n");
    char answer[25];
    memset(answer,0,sizeof(answer));
    Scanfs(answer);
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n%s\n%s\n",name,psw,question,answer);
    //printf("%s",send_buf);//
    if(send_pack(connfd,UREGISTER,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    char recv_buf[11];
    memset(recv_buf,0,sizeof(recv_buf));
    if(unpack(connfd,recv_buf,sizeof(recv_buf))<0){
        my_err("read",__LINE__);
    }
    printf("您的ID为%s",recv_buf);
    return 0;
}

