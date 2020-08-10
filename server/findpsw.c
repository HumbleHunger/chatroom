/*************************************************************************
	> File Name: findpsw.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月05日 星期三 21时58分30秒
 ************************************************************************/

#include"chat.h"
void *findpsw(void *arg)
{
    printf("findpsw start\n");
    int len=0;
    //获取用户id
    char uid[10];
    if((len=get_arg(arg,uid,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    uid[len]=0;
    printf("uid is %s\n",uid);//
    //获取新密码
    char psw[32];
    if((len=get_arg(arg,psw,32))<0){
        fprintf(log,"get_arg failed\n");
    }
    psw[len]=0;
    printf("psw is %s\n",psw);
    //获取connfd
    char fd[4];
    if((len=get_arg(arg,fd,4))<0){
        fprintf(log,"get_arg failed\n");
    }
    fd[len]=0;
    printf("fd is %s\n",fd);
    //调用mysql修改用户信息
    char cmd[1024];
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"update user_data set password = '%s' where id = '%s'",psw,uid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    //发送成功信息
    char data[3];
    memset(data,0,sizeof(data));
    sprintf(data,"1\n");
    if(send_pack(atoi(fd),FINDPSW,strlen(data),data)<0){
        my_err("write",__LINE__);
    }
    free(arg);
    printf("findpsw over\n");
    return NULL;
}
