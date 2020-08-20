/*************************************************************************
	> File Name: login.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月29日 星期三 15时05分31秒
 ************************************************************************/

#include"client.h"
int login()
{
    printf("请输入您的用户id\n");
    char id[10];
    memset(id,0,sizeof(id));
    Scanf(id);
    memset(user_id,0,sizeof(user_id));
    sprintf(user_id,"%s",id);//储存id
    printf("请输入您的密码\n");
    char psw[32];
    memset(psw,0,sizeof(psw));
    Scanfs(psw);
    //发送消息
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n",id,psw);
    //printf("send_buf len is %ld",strlen(send_buf));//
    if(send_pack(connfd,LOGIN,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    char recv_buf[5];
    memset(recv_buf,0,sizeof(recv_buf));
    //printf("unpack\n");
    if(unpack(connfd,recv_buf,sizeof(recv_buf))<0){
        my_err("read",__LINE__);
    }
    //printf("login recv_buf is %s\n",recv_buf);
    if(recv_buf[0]=='1'){
        printf("欢迎登录\n");
        printf("user_id:%s\n",user_id);
        pthread_t tid;
        pthread_create(&tid,NULL,msgbox,NULL);
        print_meau();
        return 0;
    }
    else if(recv_buf[0]=='0'){
        printf("用户id或密码错误\n");
        return 0;
    }
    else if(recv_buf[0]=='2'){
        printf("此账号已在线请勿重复登录\n");
        return 0;
    }
}

