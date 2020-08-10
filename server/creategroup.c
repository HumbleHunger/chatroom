/*************************************************************************
	> File Name: creategroup.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月03日 星期一 21时17分29秒
 ************************************************************************/

#include"chat.h"
void *creategroup(void *arg)
{
    printf("creategroup start\n");
    int len=0;
    //获取用户id
    char id[10];
    if((len=get_arg(arg,id,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    id[len]=0;
    printf("id is %s\n",id);//
    //获取群名
    char name[20];
    if((len=get_arg(arg,name,20))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    name[len]=0;
    printf("name is %s\n",name);//
    //获取connfd
    char fd[4];
    if((len=get_arg(arg,fd,4))<0){
        fprintf(log,"get_arg failed\n");
    }
    fd[len]=0;
    printf("fd is %s\n",fd);
    //查询群名是否存在
    char cmd[1024];
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select group_id from my_groups where group_name = '%s'",name);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
    char data[10];
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    if(row!=NULL){
        memset(data,0,sizeof(data));
        sprintf(data,"0\n");
        if(send_pack(atoi(fd),CREATEGROUP,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("creategroup over\n");
        return NULL;
    }
    //调用mysql增加群信息
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"insert into my_groups(group_name) values('%s')",name);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    memset(cmd,0,sizeof(cmd));
    //查询群id并返回
    sprintf(cmd,"select group_id from my_groups where group_name = '%s'",name);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    result=mysql_store_result(&mysql);
    if((row=mysql_fetch_row(result))==NULL){
        my_err("mysql_fetch_row",__LINE__);
    }
    memset(data,0,sizeof(data));
    sprintf(data,"%s\n",row[0]);
    if(send_pack(atoi(fd),CREATEGROUP,strlen(data),data)<0){
        my_err("write",__LINE__);
    }
    //增加群成员信息
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"insert into group_member(group_id,group_name,member_id,member_state,link) values('%s','%s','%s','%d','%d')",row[0],name,id,2,1);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    free(arg);
    printf("creategroup over\n");
    return 0;
}
