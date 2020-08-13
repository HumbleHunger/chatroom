/*************************************************************************
	> File Name: login.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月28日 星期二 14时48分22秒
 ************************************************************************/

#include"chat.h"
void *login(void *arg)
{
    printf("login start\n");
    int len=0;
    //获取用户id
    char id[10];
    if((len=get_arg(arg,id,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    id[len]=0;
    printf("id is %s\n",id);//
    //获取密码
    char psw[32];
    if((len=get_arg(arg,psw,32))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    psw[len]=0;
    printf("psw is %s\n",psw);//
    //获取connfd
    char fd[4];
    if((len=get_arg(arg,fd,4))<0){
        printf("get_arg failed\n");
    }
    fd[len]=0;
    printf("fd is %s\n",fd);
    //调用mysql查询用户信息
    char cmd[1024];
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select password,state,socket from user_data where id = '%s'",id);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	//MYSQL_FIELD *field;
    //printf("result\n");//
    result=mysql_store_result(&mysql);
    /*if((result=mysql_store_result(&mysql))==NULL){
        my_err("mysql_store_result",__LINE__);
    }*/
    row=mysql_fetch_row(result);
    char data[5];
    memset(data,0,sizeof(data));
    if(row==NULL){
        sprintf(data,"0\n");
        free(arg);
        if(send_pack(atoi(fd),LOGIN,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        return NULL;
        //close(atoi(fd));
    }
    if(row[1][0]=='1' && row[2][0]!='0'){
        sprintf(data,"2\n");
        free(arg);
        if(send_pack(atoi(fd),LOGIN,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        return NULL;
    }
    if(row[0]!=NULL){
        printf("mysql data is %s\n",row[0]);
        if(strcmp(psw,row[0])==0){
            sprintf(data,"1\n");
            if(send_pack(atoi(fd),LOGIN,strlen(data),data)<0){
               my_err("write",__LINE__);
            }
            //修改mysql信息
            memset(cmd,0,sizeof(cmd));
            sprintf(cmd,"update user_data set state = 1 where id = '%s'",id);
            if(mysql_query(&mysql, cmd)<0){
                my_err("mysql_query",__LINE__);
            }
            memset(cmd,0,sizeof(cmd));
            sprintf(cmd,"update user_data set socket = %s where id = '%s'",fd,id);
            printf("cmd is %s\n",cmd);//
            if(mysql_query(&mysql, cmd)<0){
                my_err("mysql_query",__LINE__);
            }
            //检查是否有离线信息
            offline(id,atoi(fd));
            free(arg);
        }
        else{
            sprintf(data,"0\n");
            free(arg);
            printf("send_pack login failed\n");
            if(send_pack(atoi(fd),LOGIN,strlen(data),data)<0){
                my_err("write",__LINE__);
            }
            //close(atoi(fd));
        }
    }
    printf("login over\n");
    return 0;
}

