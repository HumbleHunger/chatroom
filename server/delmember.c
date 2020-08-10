/*************************************************************************
	> File Name: delmember.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月05日 星期三 20时22分50秒
 ************************************************************************/

#include"chat.h"
void *delmember(void *arg)
{
    printf("delmember start\n");
    int len=0;
    //获取用户id
    char uid[10];
    if((len=get_arg(arg,uid,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    uid[len]=0;
    printf("uid is %s\n",uid);//
    //获取成员id
    char mid[10];
    if((len=get_arg(arg,mid,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    mid[len]=0;
    printf("mid is %s\n",mid);//
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
    //判断用户是否为群管理
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
        if(send_pack(atoi(fd),DELMEMBER,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
        printf("delmember over\n");
        return NULL;
    }
    if(strcmp(row[0],"0")==0){
        memset(msg,0,sizeof(msg));
        sprintf(msg,"1\n");//用户非管理
        if(send_pack(atoi(fd),DELMEMBER,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("delmember over\n");
        return NULL;
    }
    int user_state=atoi(row[0]);
    //查询该成员是否存在
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select member_state from group_member where member_id = %s && group_id = %s",mid,gid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    if(row==NULL){
        sprintf(msg,"2\n");//不是成员
        free(arg);
        if(send_pack(atoi(fd),DELMEMBER,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
        printf("delmember over\n");
        return NULL;
    }
    int member_state=atoi(row[0]);
    if(user_state<member_state){
        memset(msg,0,sizeof(msg));
        sprintf(msg,"3\n");//权限不够
        if(send_pack(atoi(fd),DELMEMBER,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("delmember over\n");
        return NULL;
    }
    //删除成员记录
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"delete from group_member where member_id = %s && group_id = %s",mid,gid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    //向用户返回成功信息
    memset(msg,0,sizeof(msg));
    sprintf(msg,"4\n%s\n%s\n",mid,gid);
    if(send_pack(atoi(fd),DELMEMBER,strlen(msg),msg)<0){
        my_err("write",__LINE__);
    }
    //查询成员fd
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select socket from user_data where id = %s",mid);
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    //向成员发送通知
    sprintf(msg,"5\n%s\n",gid);//
    if(send_pack(atoi(row[0]),DELMEMBER,strlen(msg),msg)<0){
        my_err("write",__LINE__);
    }
    free(arg);
    printf("delmember over\n");
}
