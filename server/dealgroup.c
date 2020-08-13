/*************************************************************************
	> File Name: dealgroup.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月04日 星期二 19时47分36秒
 ************************************************************************/

#include"chat.h"
void *dealgroup(void *arg)
{
    printf("dealgroup start\n");
    int len=0;
    //获取成员id
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
    //查询请求是否已被处理
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select link from group_request where send_id = %s && group_id = %s",mid,gid);
    printf("%s\n",cmd);
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    if(row==NULL){
        //发送消息
        memset(msg,0,sizeof(msg));
        sprintf(msg,"0\n");//已被处理
        if(send_pack(atoi(fd),DEALGROUP,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("dealgroup over\n");
        return NULL;
    }
    //查询成员fd
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select socket from user_data where id = %s",mid);
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    if(flag[0]=='1'){
        memset(cmd,0,sizeof(cmd));
        sprintf(cmd,"update group_member set link = 1 where member_id = %s && group_id = %s",mid,gid);
        printf("cmd is %s\n",cmd);//
        if(mysql_query(&mysql, cmd)<0){
            my_err("mysql_query",__LINE__);
        }
        //发送消息
        memset(msg,0,sizeof(msg));
        sprintf(msg,"1\n%s\n",gid);
        if(send_pack(atoi(row[0]),DEALGROUP,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
    }
    else if(flag[0]=='2'){
        memset(cmd,0,sizeof(cmd));
        sprintf(cmd,"delete from group_member where member_id = %s && group_id = %s",mid,gid);
        printf("cmd is %s\n",cmd);//
        if(mysql_query(&mysql, cmd)<0){
            my_err("mysql_query",__LINE__);
        }
        memset(msg,0,sizeof(msg));
        sprintf(msg,"2\n%s\n",gid);
        if(send_pack(atoi(row[0]),DEALGROUP,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
    }
    //修改处理状态
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"delete from group_request where send_id = %s && group_id = %s",mid,gid);
    printf("%s\n",cmd);
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    free(arg);
    printf("dealgroup over\n");
}
