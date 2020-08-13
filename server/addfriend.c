/*************************************************************************
	> File Name: addfriend.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月29日 星期三 20时02分17秒
 ************************************************************************/

#include"chat.h"
#include"thread_pool.h"
void *addfriend(void *arg)
{
    printf("addfriend start\n");
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
    //获取用户socket
    char fd[4];
    if((len=get_arg(arg,fd,4))<0){
        fprintf(log,"get_arg failed\n");
    }
    fd[len]=0;
    printf("fd is %s\n",fd);    
    char msg[1024];
    memset(msg,0,sizeof(msg));
    //查询好友关系是否存在
    char cmd[1024];
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select link from friend where user = %s && friend = %s",uid,fid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    if(row!=NULL){
        if(row[0][0]=='0'){
            memset(msg,0,sizeof(msg));
            sprintf(msg,"2\n");//重复发送好友申请
            free(arg);
            if(send_pack(atoi(fd),ADDFRIEND,strlen(msg),msg)<0){
                my_err("write",__LINE__);
            }
        }
        else if(row[0][0]=='1'){
            memset(msg,0,sizeof(msg));
            sprintf(msg,"3\n");//已是好友
            free(arg);
            if(send_pack(atoi(fd),ADDFRIEND,strlen(msg),msg)<0){
                my_err("write",__LINE__);
            }
        }
        return 0;
    }
    //查询好友是否存在以及在线情况
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select state from user_data where id = %s",fid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    if(row==NULL){
        memset(msg,0,sizeof(msg));
        sprintf(msg,"0\n");//用户不存在
        free(arg);
        if(send_pack(atoi(fd),ADDFRIEND,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
        return 0;
    }
    //用户存在
    memset(cmd,0,sizeof(cmd));
    //添加好友关系
    sprintf(cmd,"insert into friend(user,friend) values(%s,%s)",uid,fid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"insert into friend(user,friend) values(%s,%s)",fid,uid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    memset(cmd,0,sizeof(cmd));
    //添加好友申请消息
    sprintf(cmd,"insert into friend_request(send_id,recv_id) values(%s,%s)",uid,fid);
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    if(row[0][0]=='1'){
        //在线
        //查询好友SOCKET发送消息
        memset(cmd,0,sizeof(cmd));
        sprintf(cmd,"select socket from user_data where id = %s",fid);
        if(mysql_query(&mysql, cmd)<0){
            my_err("mysql_query",__LINE__);
        }
        result=mysql_store_result(&mysql);
        row=mysql_fetch_row(result);
        sprintf(msg,"1\n%s\n",uid);
        if(send_pack(atoi(row[0]),ADDFRIEND,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
        //将消息状态改为已发送
        memset(cmd,0,sizeof(cmd));
        sprintf(cmd,"update friend_request set state = 1 where send_id='%s' && recv_id='%s'",uid,fid);
        printf("%s\n",cmd);//
        if(mysql_query(&mysql, cmd)<0){
            my_err("mysql_query",__LINE__);
        }
    }
    free(arg);
    printf("addfriend over\n");
    return 0;
}

