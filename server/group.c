/*************************************************************************
	> File Name: group.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月05日 星期三 15时13分41秒
 ************************************************************************/

#include"chat.h"
void *group(void *arg)
{
    printf("group start\n");
    int len=0;
    //获取用户id
    char uid[10];
    if((len=get_arg(arg,uid,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    uid[len]=0;
    printf("uid is %s\n",uid);//
    //获取群id
    char gid[10];
    if((len=get_arg(arg,gid,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    gid[len]=0;
    printf("gid is %s\n",gid);//
    //获取用户socket
    char fd[4];
    if((len=get_arg(arg,fd,4))<0){
        fprintf(log,"get_arg failed\n");
    }
    fd[len]=0;
    printf("fd is %s\n",fd);
    char cmd[1024];
    memset(cmd,0,sizeof(cmd));
    //查询群成员记录是否存在
    sprintf(cmd,"select link from group_member where member_id = %s && group_id = %s",uid,gid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
    char data[3];

    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    if(row==NULL){
        memset(data,0,sizeof(data));
        sprintf(data,"0\n");
        if(send_pack(atoi(fd),GROUP,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("group over\n");
        return NULL;
    }
    else if(row[0][0]=='0'){
        memset(data,0,sizeof(data));
        sprintf(data,"0\n");
        if(send_pack(atoi(fd),GROUP,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("group over\n");
        return NULL;
    }
    else{
        memset(data,0,sizeof(data));
        sprintf(data,"1\n");
        if(send_pack(atoi(fd),GROUP,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("group over\n");
        return NULL;
    }
}
