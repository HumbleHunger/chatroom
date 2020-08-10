/*************************************************************************
	> File Name: register.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月28日 星期二 14时57分09秒
 ************************************************************************/

#include"chat.h"
#include"thread_pool.h"
void *uregister(void *arg)
{
    printf("start register\n");//
    int len=0;
    //获取用户名
    char name[20];
    if((len=get_arg(arg,name,20))<0){
        fprintf(log,"get_arg failed\n");
    }
    name[len]=0;
    printf("name is %s\n",name);
    //获取密码
    char psw[32];
    if((len=get_arg(arg,psw,32))<0){
        fprintf(log,"get_arg failed\n");
    }
    psw[len]=0;
    printf("psw is %s\n",psw);
    //获取密保问题
    char question[32];
    if((len=get_arg(arg,question,32))<0){
        fprintf(log,"get_arg failed\n");
    }
    question[len]=0;
    printf("question is %s\n",question);
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
    //调用mysql增加用户信息
    char cmd[1024];
    memset(cmd,0,sizeof(cmd));
    printf("sprinf cmd\n");
    sprintf(cmd,"insert into user_data(name,password,socket,question,answer) values('%s','%s','%s','%s','%s')",name,psw,fd,question,answer);
    printf("cmd is %s\n",cmd);//

    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    memset(cmd,0,sizeof(cmd));
    //查询用户id并返回
    sprintf(cmd,"select id from user_data where socket = '%s' && name = '%s' && password = '%s'",fd,name,psw);
    printf("cmd is %s",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	//MYSQL_FIELD *field;
    printf("result\n");//
    result=mysql_store_result(&mysql);
    /*if((result=mysql_store_result(&mysql))==NULL){
        my_err("mysql_store_result",__LINE__);
    }*/
    if((row=mysql_fetch_row(result))==NULL){
        my_err("mysql_fetch_row",__LINE__);
    }
    char data[20];
    memset(data,0,sizeof(data));
    sprintf(data,"%s\n",row[0]);
    printf("%s",data);//
    if(send_pack(atoi(fd),UREGISTER,strlen(data),data)<0){
        my_err("write",__LINE__);
    }
    free(arg);
    return 0;
}

