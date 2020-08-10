/*************************************************************************
	> File Name: over.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月31日 星期五 15时02分48秒
 ************************************************************************/

#include"chat.h"
#include"thread_pool.h"
void *over(void *arg)
{
    printf("over start\n");
    int len=0;
    //获取用户id
    char uid[10];
    if((len=get_arg(arg,uid,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    uid[len]=0;
    printf("uid is %s\n",uid);//
    //获取用户fd
    char fd[4];
    if((len=get_arg(arg,fd,4))<0){
        fprintf(log,"get_arg failed\n");
    }
    fd[len]=0;
    printf("fd is %s\n",fd);
    char cmd[1024];
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"update user_data set socket = 0 , state = 0 where id = %s",uid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    //发送消息
    char data[5];
    memset(data,0,sizeof(data));
    sprintf(data,"1\n");
    if(send_pack(atoi(fd),OVER,strlen(data),data)<0){
        my_err("write",__LINE__);
    }
    printf("over over\n");
}

