/*************************************************************************
	> File Name: exitgroup.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月04日 星期二 21时23分26秒
 ************************************************************************/

#include"chat.h"
void *exitgroup(void *arg)
{
    printf("exitgroup start\n");
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
    //查询是否为群成员
    sprintf(cmd,"select link from group_member where group_id = %s && member_id = %s",gid,uid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
    char data[1024];

    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    if(row==NULL){
        memset(data,0,sizeof(data));
        sprintf(data,"0\n");
        if(send_pack(atoi(fd),EXITGROUP,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("exitgroup over\n");
        return NULL;
    }
    if(strcmp(row[0],"0")==0){
        memset(data,0,sizeof(data));
        sprintf(data,"0\n");
        if(send_pack(atoi(fd),EXITGROUP,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("exitgroup over\n");
        return NULL;
    }
    //删除成员记录
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"delete from group_member where group_id = %s && member_id = %s",gid,uid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    memset(data,0,sizeof(data));
    sprintf(data,"1\n");
    if(send_pack(atoi(fd),EXITGROUP,strlen(data),data)<0){
        my_err("write",__LINE__);
    }
    free(arg);
    printf("exitgroup over\n");
}
