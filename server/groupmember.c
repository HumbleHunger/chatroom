/*************************************************************************
	> File Name: groupmember.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月06日 星期四 14时39分21秒
 ************************************************************************/

#include"chat.h"
void *groupmember(void *arg)
{
    printf("groupmember start\n");//
    int len=0;
    //获取用户id
    char id[10];
    if((len=get_arg(arg,id,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    id[len]=0;
    printf("id is %s\n",id);//
    //获取群id
    char gid[10];
    if((len=get_arg(arg,gid,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    gid[len]=0;
    printf("gid is %s\n",gid);//
    //获取connfd
    char fd[4];
    if((len=get_arg(arg,fd,4))<0){
        fprintf(log,"get_arg failed\n");
    }
    fd[len]=0;
    printf("fd is %s\n",fd);
    char cmd[1024];
    memset(cmd,0,sizeof(cmd));
    //查询群成员记录是否存在
    sprintf(cmd,"select * from group_member where member_id = %s && group_id = %s && link = 1",id,gid);
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
        sprintf(data,"0\n");//此成员不存在
        if(send_pack(atoi(fd),GROUPMEMBER,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("groupmember over\n");
        return NULL;
    }
    //查询并储存所有成员id
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select member_id from group_member where group_id = '%s' && link = 1",gid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    char member[100][10];
    memset(member,0,sizeof(member));
    result=mysql_store_result(&mysql);
    int i=0;
    while(row=mysql_fetch_row(result)){
        strcpy(member[i],row[0]);
        i++;
    }
    //依次查询成员信息
    for(int j=0;j<i;j++){
        memset(cmd,0,sizeof(cmd));
        sprintf(cmd,"select name,state from user_data where id = '%s'",member[j]);
        if(mysql_query(&mysql, cmd)<0){
            my_err("mysql_query",__LINE__);
        }
        result=mysql_store_result(&mysql);
        row=mysql_fetch_row(result);
        memset(data,0,sizeof(data));
        sprintf(data,"%s\n%s\n%s\n",member[j],row[0],row[1]);
        if(send_pack(atoi(fd),GROUPMEMBER,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
    }
    free(arg);
    printf("groupmember over\n");//
    return NULL;
}
