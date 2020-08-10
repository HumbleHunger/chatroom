/*************************************************************************
	> File Name: gchatmsg.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月06日 星期四 15时33分24秒
 ************************************************************************/

#include"chat.h"
void *gchatmsg(void *arg)
{
    printf("gchatmsg start\n");
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
    char data[1024];

    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    if(row==NULL){
        memset(data,0,sizeof(data));
        sprintf(data,"0\n");
        if(send_pack(atoi(fd),GCHATMSG,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("gchatmsg over\n");
        return NULL;
    }
    if(row[0][0]=='0'){
        memset(data,0,sizeof(data));
        sprintf(data,"0\n");
        if(send_pack(atoi(fd),GCHATMSG,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("gchatmsg over\n");
        return NULL;
    }
    //查询非用户群成员
    sprintf(cmd,"select member_id from group_member where member_id != %s && group_id = %s",uid,gid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    //查询群消息记录
    sprintf(cmd,"select send_id,msg from group_chatmsg where (recv_id = %s && group_id = %s) || (send_id = %s && group_id = %s && recv_id = %s)",uid,gid,uid,gid,row[0]);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    result=mysql_store_result(&mysql);
    while(row=mysql_fetch_row(result)){
        memset(data,0,sizeof(data));
        sprintf(data,"%s\n%s\n",row[0],row[1]);
        if(send_pack(atoi(fd),GCHATMSG,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
    }
   free(arg);
   printf("gchatmsg over\n");
}
