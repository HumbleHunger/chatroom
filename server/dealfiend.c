/*************************************************************************
	> File Name: dealfiend.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月13日 星期四 15时21分07秒
 ************************************************************************/

#include"chat.h"
void *dealfriend(void *arg)
{
    printf("dealfriend start\n");
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
    //获取操作
    char flag[2];
    if((len=get_arg(arg,flag,2))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    flag[len]=0;
    printf("flag is %s\n",flag);//
    //获取用户socket
    char fd[4];
    if((len=get_arg(arg,fd,4))<0){
        fprintf(log,"get_arg failed\n");
    }
    fd[len]=0;
    printf("fd is %s\n",fd);
    char cmd[1024];
    char msg[1024];
    //查询好友fd
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select socket from user_data where id = %s",fid);
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    if(flag[0]=='1'){
        //更新好友LINK
        memset(cmd,0,sizeof(cmd));
        sprintf(cmd,"update friend set link = 1 where (user = %s && friend = %s) || (user = %s && friend = %s)",fid,uid,uid,fid);
        printf("cmd is %s\n",cmd);//
        if(mysql_query(&mysql, cmd)<0){
            my_err("mysql_query",__LINE__);
        }
        //发送消息
        memset(msg,0,sizeof(msg));
        sprintf(msg,"1\n%s\n",uid);
        if(send_pack(atoi(row[0]),DEALFRIEND,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
    }
    else if(flag[0]=='2'){
        //删除好友记录
        memset(cmd,0,sizeof(cmd));
        sprintf(cmd,"delete from friend where (user = %s && friend = %s) || (user = %s && friend = %s)",fid,uid,uid,fid);
        printf("cmd is %s\n",cmd);//
        if(mysql_query(&mysql, cmd)<0){
            my_err("mysql_query",__LINE__);
        }
        //发送消息
        memset(msg,0,sizeof(msg));
        sprintf(msg,"2\n%s\n",uid);
        if(send_pack(atoi(row[0]),DEALFRIEND,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
    }
    //删除好友请求记录
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"delete from friend_request where send_id = %s && recv_id = %s",fid,uid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    free(arg);
    printf("dealfriend over\n");
}
