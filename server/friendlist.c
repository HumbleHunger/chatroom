/*************************************************************************
	> File Name: friendlist.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月31日 星期五 16时36分52秒
 ************************************************************************/

#include"chat.h"
void *friendlist(void *arg)
{
    printf("start friendlist\n");//
    int len=0;
    //获取用户id
    char id[10];
    if((len=get_arg(arg,id,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    id[len]=0;
    printf("id is %s\n",id);//
    //获取connfd
    char fd[4];
    if((len=get_arg(arg,fd,4))<0){
        fprintf(log,"get_arg failed\n");
    }
    fd[len]=0;
    printf("fd is %s\n",fd);
    //调用mysql查询用户信息
    char cmd[1024];
    memset(cmd,0,sizeof(cmd));
    //查询并储存所有好友id
    sprintf(cmd,"select friend from friend where user = '%s'",id);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
    char friend[100][10];
    memset(friend,0,sizeof(friend));
    result=mysql_store_result(&mysql);
    int i=0;
    while(row=mysql_fetch_row(result)){
        strcpy(friend[i],row[0]);
        printf("friend[%d] is %s\n",i,friend[i]);
        i++;
    }
    char data[1024];
    if(i==0){
        memset(data,0,sizeof(data));
        sprintf(data,"0\n");
        if(send_pack(atoi(fd),FRIENDLIST,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        printf("friendlist over\n");//
        return 0;
    }
    printf("依次查询好友信息\n");//
    //依次查询好友信息
    for(int j=0;j<i;j++){
        memset(cmd,0,sizeof(cmd));
        sprintf(cmd,"select name,state from user_data where id = '%s'",friend[j]);
        if(mysql_query(&mysql, cmd)<0){
            my_err("mysql_query",__LINE__);
        }
        result=mysql_store_result(&mysql);
        row=mysql_fetch_row(result);
        memset(data,0,sizeof(data));
        sprintf(data,"%s\n%s\n%s\n",friend[j],row[0],row[1]);
        if(send_pack(atoi(fd),FRIENDLIST,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
    }
    free(arg);
    printf("friendlist over\n");//
    return 0;
}

