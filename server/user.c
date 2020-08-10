/*************************************************************************
	> File Name: user.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月05日 星期三 22时06分49秒
 ************************************************************************/

#include"chat.h"
void *user(void *arg)
{
    printf("user start\n");
    int len=0;
    //获取用户id
    char uid[10];
    if((len=get_arg(arg,uid,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    uid[len]=0;
    printf("uid is %s\n",uid);//
    //获取用户socket
    char fd[4];
    if((len=get_arg(arg,fd,4))<0){
        fprintf(log,"get_arg failed\n");
    }
    fd[len]=0;
    printf("fd is %s\n",fd);
    char cmd[1024];
    memset(cmd,0,sizeof(cmd));
    //查询用户是否存在
    sprintf(cmd,"select question from user_data where id = %s",uid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
    char data[20];
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    if(row==NULL){
        memset(data,0,sizeof(data));
        sprintf(data,"0\n");//用户不存在
        if(send_pack(atoi(fd),USER,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("user over\n");
        return NULL;
    }
    else{
        memset(data,0,sizeof(data));
        sprintf(data,"1\n%s\n",row[0]);//用户存在
        if(send_pack(atoi(fd),USER,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("user over\n");
        return NULL;
    }
}
