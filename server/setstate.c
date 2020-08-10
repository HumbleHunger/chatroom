/*************************************************************************
	> File Name: setstate.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月04日 星期二 21时57分54秒
 ************************************************************************/

#include"chat.h"
void *gsetstate(void *arg)
{
    printf("setstate start\n");
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
    //判断是否为群管理
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
        if(send_pack(atoi(fd),SETSTATE,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
        printf("setstate over\n");
        return NULL;
    }
    if(strcmp(row[0],"2")!=0){
        memset(msg,0,sizeof(msg));
        sprintf(msg,"1\n");//非群主
        if(send_pack(atoi(fd),SETSTATE,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("setstate over\n");
        return NULL;
    }
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
        if(send_pack(atoi(fd),SETSTATE,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
        printf("setstate over\n");
        return NULL;
    }
    if(strcmp(row[0],"0")!=0){
        memset(msg,0,sizeof(msg));
        sprintf(msg,"3\n");//已是管理员
        if(send_pack(atoi(fd),SETSTATE,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("setstate over\n");
        return NULL;
    }
    //修改成员地位
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"update group_member set member_state = 1 where member_id = %s && group_id = %s",mid,gid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    //查询成员fd
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select socket from user_data where id = %s",mid);
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    sprintf(msg,"4\n%s\n",gid);//
    free(arg);
    if(send_pack(atoi(row[0]),SETSTATE,strlen(msg),msg)<0){
        my_err("write",__LINE__);
    }
    printf("setstate over\n");
}
