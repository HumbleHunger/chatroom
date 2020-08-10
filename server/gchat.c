/*************************************************************************
	> File Name: gchat.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月05日 星期三 15时08分22秒
 ************************************************************************/

#include"chat.h"
void *gchat(void *arg)
{
    printf("gchat start\n");
    int len=0;
    //获取用户id
    char mid[10];
    if((len=get_arg(arg,mid,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    mid[len]=0;
    printf("mid is %s\n",mid);//
    //获取群id
    char gid[10];
    if((len=get_arg(arg,gid,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    gid[len]=0;
    printf("gid is %s\n",gid);//
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
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
    char data[1024];
    //查询所有群成员ID
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select member_id from group_member where group_id = %s && member_id != %s",gid,mid);
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    result=mysql_store_result(&mysql);
    MYSQL_RES *results=NULL;
	MYSQL_ROW rows;
    while(row=mysql_fetch_row(result)){
        //将消息储存起来
        memset(cmd,0,sizeof(cmd));
        sprintf(cmd,"insert into group_chatmsg(send_id,recv_id,group_id,msg) values(%s,%s,%s,'%s')",mid,row[0],gid,msg);
        printf("cmd is %s\n",cmd);//
        if(mysql_query(&mysql, cmd)<0){
            my_err("mysql_query",__LINE__);
        }
        //查询群成员fd
        memset(cmd,0,sizeof(cmd));
        sprintf(cmd,"select state,socket from user_data where id = %s",row[0]);
        if(mysql_query(&mysql, cmd)<0){
            my_err("mysql_query",__LINE__);
        }
        results=mysql_store_result(&mysql);
        rows=mysql_fetch_row(results);
        //若不在线
        if(strcmp(rows[0],"0")==0){
            continue;
        }
        //若在线
        //发送消息
        memset(data,0,sizeof(data));
        sprintf(data,"%s\n%s\n%s\n",mid,gid,msg);
        if(send_pack(atoi(rows[1]),GCHAT,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        //将消息状态改为1已发送
        memset(cmd,0,sizeof(cmd));
        sprintf(cmd,"update group_chatmsg set state = 1 where send_id = %s && recv_id = %s && group_id = %s && msg = '%s'",mid,row[0],gid,msg);
        printf("cmd is %s\n",cmd);
        if(mysql_query(&mysql, cmd)<0){
            my_err("mysql_query",__LINE__);
        }
    }
    free(arg);
    printf("gchat over\n");
}
