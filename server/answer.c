/*************************************************************************
	> File Name: answer.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月05日 星期三 22时37分36秒
 ************************************************************************/

#include"chat.h"
void *answer(void *arg)
{
    printf("answer start\n");
    int len=0;
    //获取用户id
    char uid[10];
    if((len=get_arg(arg,uid,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    uid[len]=0;
    printf("uid is %s\n",uid);//
    //获取密保答案
    char answer[32];
    if((len=get_arg(arg,answer,32))<0){
        fprintf(log,"get_arg failed\n");
    }
    answer[len]=0;
    printf("answer is %s\n",answer);
    //获取connfd
    char fd[4];
    if((len=get_arg(arg,fd,4))<0){
        fprintf(log,"get_arg failed\n");
    }
    fd[len]=0;
    printf("fd is %s\n",fd);
    //调用mysql查询用户信息
    char cmd[1024];
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select answer from user_data where id = %s",uid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    char data[5];
    if(strcmp(answer,row[0])!=0){
        memset(data,0,sizeof(data));
        sprintf(data,"0\n");//答案错误
        if(send_pack(atoi(fd),ANSWER,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("answer over\n");
        return NULL;
    }
    else{
        memset(data,0,sizeof(data));
        sprintf(data,"1\n");//答案正确
        if(send_pack(atoi(fd),ANSWER,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("answer over\n");
        return NULL;
    }
}
