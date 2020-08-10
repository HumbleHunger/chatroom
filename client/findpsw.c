/*************************************************************************
	> File Name: findpsw.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月05日 星期三 22时15分08秒
 ************************************************************************/

#include"client.h"
int findpsw()
{
    printf("请输入您的用户ID\n");
    static char uid[10];
    scanf("%s",uid);
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n",uid);
    //printf("user send_buf is %s",send_buf);//
    if(send_pack(connfd,USER,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    char recv_buf[1024];
    memset(recv_buf,0,sizeof(recv_buf));
    if(unpack(connfd,recv_buf,sizeof(recv_buf))<0){
        my_err("read",__LINE__);
    }
    //user
    read_len=0;
    char flag[2];
    memset(flag,0,sizeof(flag));
    if(get_arg(recv_buf,flag,sizeof(flag))<0){
        my_err("read",__LINE__);
    }
    if(flag[0]=='0'){
        printf("\t\t\t\t\t用户不存在\n");
        return 0;
    }
    char request[32];
    memset(request,0,sizeof(request));
    if(get_arg(recv_buf,request,sizeof(request))<0){
        my_err("read",__LINE__);
    }
    else if(flag[0]=='1'){
        printf("您的密保问题是%s\n",request);
    }

    printf("请输入您的密保答案\n");
    char answer[32];
    scanf("%s",answer);
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n",uid,answer);
    //printf("answer send_buf is %s",send_buf);//
    if(send_pack(connfd,ANSWER,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    memset(recv_buf,0,sizeof(recv_buf));
    if(unpack(connfd,recv_buf,sizeof(recv_buf))<0){
        my_err("read",__LINE__);
    }
    //answer
    read_len=0;
    memset(flag,0,sizeof(flag));
    if(get_arg(recv_buf,flag,sizeof(flag))<0){
        my_err("read",__LINE__);
    }
    if(flag[0]=='0'){
        printf("答案错误\n");
        return 0;
    }
    //psw
    read_len=0;
    printf("请输入您的新密码\n");
    char psw[32];
    scanf("%s",psw);
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n",uid,psw);
    //printf("findpsw send_buf is %s",send_buf);//
    if(send_pack(connfd,FINDPSW,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    memset(recv_buf,0,sizeof(recv_buf));
    if(unpack(connfd,recv_buf,sizeof(recv_buf))<0){
        my_err("read",__LINE__);
    }
    memset(flag,0,sizeof(flag));
    if(get_arg(recv_buf,flag,sizeof(flag))<0){
        my_err("read",__LINE__);
    }
    if(flag[0]=='0'){
        printf("密码重置失败\n");
        return 0;
    }
    else if(flag[0]=='1'){
        printf("密码重置成功\n");
        return 0;
    }

}
