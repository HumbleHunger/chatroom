/*************************************************************************
	> File Name: delgroup.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月05日 星期三 21时25分41秒
 ************************************************************************/

#include"chat.h"
void *delgroup(void *arg)
{
    printf("delgroup start\n");
    int len=0;
    //获取用户id
    char uid[10];
    if((len=get_arg(arg,uid,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    uid[len]=0;
    printf("uid is %s\n",uid);//
    //获取群ID
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
    char msg[1024];
    memset(msg,0,sizeof(msg));
    //判断是否为群主
    char cmd[1024];
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select member_state from group_member where member_id = %s && group_id = %s",uid,gid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    if(row==NULL){
        sprintf(msg,"0\n");//群不存在
        free(arg);
        if(send_pack(atoi(fd),DELGROUP,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
        printf("delgroup over\n");
        return NULL;
    }
    if(strcmp(row[0],"2")!=0){
        memset(msg,0,sizeof(msg));
        sprintf(msg,"1\n");//非群主
        if(send_pack(atoi(fd),DELGROUP,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("delgroup over\n");
        return NULL;
    }
    //删除所有群信息
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"delete from group_member where group_id = %s",gid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"delete from my_groups where group_id = %s",gid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    //发送成功消息
    memset(msg,0,sizeof(msg));
    sprintf(msg,"2\n");
    if(send_pack(atoi(fd),DELGROUP,strlen(msg),msg)<0){
        my_err("write",__LINE__);
    }
    free(arg);
    printf("delgroup over\n");
}
