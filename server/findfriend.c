/*************************************************************************
	> File Name: findfriend.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月31日 星期五 20时57分47秒
 ************************************************************************/

#include"chat.h"
void *findfriend(void *arg)
{
    printf("findfriend start\n");//
    int len=0;
    //获取目标用户id
    char id[10];
    if((len=get_arg(arg,id,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    id[len]=0;
    printf("id is %s\n",id);//
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
    sprintf(cmd,"select name,state from user_data where id = '%s'",id);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    char data[1024];
    if(row==NULL){
        memset(data,0,sizeof(data));
        sprintf(data,"0\n");
        if(send_pack(atoi(fd),FINDFRIEND,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("findfriend over\n");//
        return 0;
    }
    memset(data,0,sizeof(data));
    sprintf(data,"%s\n%s\n%s\n",id,row[0],row[1]);
    if(send_pack(atoi(fd),FINDFRIEND,strlen(data),data)<0){
        my_err("write",__LINE__);
    }
    free(arg);
    printf("findfriend over\n");//
    return 0;
}

