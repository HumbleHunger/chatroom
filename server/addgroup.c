/*************************************************************************
	> File Name: addgroup.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月04日 星期二 14时51分55秒
 ************************************************************************/

#include"chat.h"
void *addgroup(void *arg)
{
    printf("addgroup start\n");
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
    char data[1024];
    memset(data,0,sizeof(data));
    //查询群是否存在
    char cmd[1024];
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select * from group_member where group_id = %s && member_id = %s",gid,uid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    if(row){
        sprintf(data,"a\n");//已是成员
        free(arg);
        if(send_pack(atoi(fd),ADDGROUP,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        return 0;
    }
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select group_name from my_groups where group_id = %s",gid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    if(row==NULL){
        sprintf(data,"0\n");//群不存在
        free(arg);
        if(send_pack(atoi(fd),ADDGROUP,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        return 0;
    }
    //群存在
    //添加成员信息
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"insert into group_member(group_id,group_name,member_id) values('%s','%s','%s')",gid,row[0],uid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    //查询管理人员ID
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select member_id from group_member where group_id = %s && (member_state = 1 || member_state = 2)",gid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    result=mysql_store_result(&mysql);
    MYSQL_RES *results=NULL;
	MYSQL_ROW rows;
    while(row=mysql_fetch_row(result)){
        memset(cmd,0,sizeof(cmd));
        sprintf(cmd,"insert into group_request(send_id,recv_id,group_id) values('%s','%s','%s')",uid,row[0],gid);
        printf("cmd is %s\n",cmd);//
        if(mysql_query(&mysql, cmd)<0){
            my_err("mysql_query",__LINE__);
        }
        memset(cmd,0,sizeof(cmd));
        sprintf(cmd,"select state,socket from user_data where id = %s",row[0]);
        printf("cmd is %s\n",cmd);//
        if(mysql_query(&mysql, cmd)<0){
            my_err("mysql_query",__LINE__);
        }
        results=mysql_store_result(&mysql);
        rows=mysql_fetch_row(results);
        if(strcmp(rows[0],"1")==0){
            memset(data,0,sizeof(data));
            sprintf(data,"%s\n%s\n",uid,gid);
            if(send_pack(atoi(rows[1]),ADDGROUP,strlen(data),data)<0){
                my_err("write",__LINE__);
            }
            memset(cmd,0,sizeof(cmd));
            sprintf(cmd,"update group_request set state = 1 where send_id = '%s' && recv_id = '%s' && group_id = '%s'",uid,row[0],gid);
            printf("cmd is %s\n",cmd);//
            if(mysql_query(&mysql, cmd)<0){
                my_err("mysql_query",__LINE__);
            }
        }
    }
    free(arg);
    printf("addgroup over\n");
}
