/*************************************************************************
	> File Name: fchat.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月01日 星期六 15时05分27秒
 ************************************************************************/

#include"chat.h"
void *fchat(void *arg)
{
    printf("fchat start\n");
    int len=0;
    //获取用户id
    char uid[10];
    if((len=get_arg(arg,uid,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    uid[len]=0;
    printf("uid is %s\n",uid);//
    //获取好友id
    char fid[10];
    if((len=get_arg(arg,fid,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    fid[len]=0;
    printf("fid is %s\n",fid);//
    //获取消息
    char msg[500];
    if((len=get_arg(arg,msg,500))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    msg[len]=0;
    printf("msg is %s\n",msg);//
    //获取用户socket
    char fd[4];
    if((len=get_arg(arg,fd,4))<0){
        fprintf(log,"get_arg failed\n");
    }
    fd[len]=0;
    printf("fd is %s\n",fd);    
    char cmd[1024];
    memset(cmd,0,sizeof(cmd));
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
    char data[1024];
    //将消息储存起来
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"insert into chat_msg(send_id,recv_id,msg) values(%s,%s,'%s')",uid,fid,msg);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    //查询好友fd
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select state,socket from user_data where id = %s",fid);
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    //若不在线
    if(strcmp(row[0],"0")==0){
        printf("对方不在线\n");
        free(arg);
        printf("fchat over\n");
        return NULL;
    }
    //若在线
    //发送消息
    memset(data,0,sizeof(data));
    sprintf(data,"%s\n%s\n%s\n",uid,fid,msg);
    if(send_pack(atoi(row[1]),FCHAT,strlen(data),data)<0){
        my_err("write",__LINE__);
    }
    //将消息状态改为1已发送
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"update chat_msg set state = 1 where send_id = %s && recv_id = %s && msg = '%s'",uid,fid,msg);
    printf("cmd is %s\n",cmd);
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    printf("fchat over\n");
}
