/*************************************************************************
	> File Name: offline.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月06日 星期四 16时08分53秒
 ************************************************************************/

#include"chat.h"
int offline(char *id,int connfd)
{
    char cmd[1024];
    memset(cmd,0,sizeof(cmd));
    //查询是否有离线好友消息
    sprintf(cmd,"select send_id,msg from chat_msg where recv_id = %s && state = 0",id);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
    char data[1024];
    result=mysql_store_result(&mysql);
    while(row=mysql_fetch_row(result)){
        memset(data,0,sizeof(data));
        sprintf(data,"%s\n%s\n%s\n",row[0],id,row[1]);
        if(send_pack(connfd,FCHAT,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
    }
    //将消息改为已发送
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"update chat_msg set state = 1 where recv_id = %s && state = 0",id);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    //查询是否有离线群消息
    sprintf(cmd,"select send_id,group_id,msg from group_chatmsg where recv_id = %s && state = 0",id);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    result=mysql_store_result(&mysql);
    while(row=mysql_fetch_row(result)){
        memset(data,0,sizeof(data));
        sprintf(data,"%s\n%s\n%s\n",row[0],row[1],row[2]);
        if(send_pack(connfd,GCHAT,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
    }
    //将消息改为已发送
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"update group_chatmsg set state = 1 where recv_id = %s && state = 0",id);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    //查询是否有离线好友申请
    sprintf(cmd,"select send_id from friend_request where recv_id = %s && state = 0",id);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    result=mysql_store_result(&mysql);
    while(row=mysql_fetch_row(result)){
        memset(data,0,sizeof(data));
        sprintf(data,"1\n%s\n",row[0]);
        if(send_pack(connfd,ADDFRIEND,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
    }
    //将消息改为已发送
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"update friend_request set state = 1 where recv_id = %s && state = 0",id);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    //查询是否有离线加群申请
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select send_id,group_id from group_request where recv_id = %s && state = 0",id);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    result=mysql_store_result(&mysql);
    while(row=mysql_fetch_row(result)){
        memset(data,0,sizeof(data));
        sprintf(data,"%s\n%s\n",row[0],row[1]);
        if(send_pack(connfd,ADDGROUP,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
    }
    //将消息改为已发送
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"update group_request set state = 1 where recv_id = %s && state = 0",id);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    //查询是否有离线文件消息发送
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select send_id,filename from file where recv_id = %s && state = 0",id);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    result=mysql_store_result(&mysql);
    while(row=mysql_fetch_row(result)){
        memset(data,0,sizeof(data));
        sprintf(data,"%s\n%s\n",row[0],row[1]);
        if(send_pack(connfd,SENDFILE,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
    }
    //将消息改为已发送
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"update file set state = 1 where recv_id = %s && state = 0",id);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    return 0;
}
