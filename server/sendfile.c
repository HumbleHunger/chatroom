/*************************************************************************
	> File Name: sendfile.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月06日 星期四 23时02分34秒
 ************************************************************************/

#include"chat.h"
void *send_file(void *arg)
{
    printf("sendfile start\n");
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
    //获取文件名
    char filename[256];
    if((len=get_arg(arg,filename,256))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    filename[len]=0;
    printf("filename is %s\n",filename);//
    //添加文件记录
    char cmd[1024];
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"insert into file(send_id,recv_id,filename) values(%s,%s,'%s')",uid,fid,filename);
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
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    //若不在线
    if(strcmp(row[0],"0")==0){
        printf("对方不在线\n");
        free(arg);
        printf("send_file over\n");
        return NULL;
    }
    //若在线
    //发送消息
    char data[1024];
    memset(data,0,sizeof(data));
    sprintf(data,"%s\n%s\n",uid,filename);
    if(send_pack(atoi(row[1]),SENDFILE,strlen(data),data)<0){
        my_err("write",__LINE__);
    }
    //将消息状态改为1已发送
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"update file set state = 1 where send_id = %s && recv_id = %s && filename = '%s'",uid,fid,filename);
    printf("cmd is %s\n",cmd);
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    free(arg);
    printf("send_file over\n");
}
