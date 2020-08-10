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
    //查询好友是否存在以及在线情况
    char cmd[1024];
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select state from user_data where id = %s",fid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	//MYSQL_FIELD *field;
    //printf("result\n");//
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    if(row==NULL){
        printf("row == NULL\n");//
        sprintf(msg,"0\n");//好友不存在
        free(arg);
        if(send_pack(atoi(fd),ADDFRIEND,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
        //close(atoi(fd));
        return 0;
    }
    //好友存在
    char state=row[0][0];//记录在线状态
    //查询好友表中有几条好友关系
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select * from friend where (user='%s'&&friend='%s') || (user='%s'&&friend='%s')",uid,fid,fid,uid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    result=mysql_store_result(&mysql);
    int count=0;//好友关系数
    while(row=mysql_fetch_row(result)){
        count++;
    }
    if(count==2){
        sprintf(msg,"3\n");
        if(send_pack(atoi(fd),ADDFRIEND,strlen(msg),msg)<0){
            my_err("write",__LINE__);
        }
        return 0;
    }
    //判断好友是否存在
    //好友存在
    memset(cmd,0,sizeof(cmd));
    //添加好友关系
    sprintf(cmd,"insert into friend(user,friend) values(%s,%s)",uid,fid);
    printf("cmd is %s\n",cmd);//
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    if(count==0){
        memset(cmd,0,sizeof(cmd));
        //添加好友申请消息
        sprintf(cmd,"insert into friend_request(send_id,recv_id) values(%s,%s)",uid,fid);
        if(mysql_query(&mysql, cmd)<0){
            my_err("mysql_query",__LINE__);
        }
    }
    if(1){
        if(state=='1'){
            //在线
            //0条好友关系
            if(count==0){
                //发送消息
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
                memset(cmd,0,sizeof(cmd));
                sprintf(cmd,"update friend_request set state = 1 where send_id='%s' && recv_id='%s'",uid,fid);
                printf("%s\n",cmd);//
                if(mysql_query(&mysql, cmd)<0){
                    my_err("mysql_query",__LINE__);
                }
            }
            else if(count==1){
                //将link改为1
                memset(cmd,0,sizeof(cmd));
                sprintf(cmd,"update friend set link = 1 where (user='%s' && friend='%s')||(user='%s' && friend='%s')",uid,fid,fid,uid);
                if(mysql_query(&mysql, cmd)<0){
                    my_err("mysql_query",__LINE__);
                }
                //查询好友fd
                memset(cmd,0,sizeof(cmd));
                sprintf(cmd,"select socket from user_data where id = %s",fid);
                if(mysql_query(&mysql, cmd)<0){
                    my_err("mysql_query",__LINE__);
                }
                result=mysql_store_result(&mysql);
                row=mysql_fetch_row(result);
                //发送成功消息
                memset(msg,0,sizeof(msg));
                sprintf(msg,"2\n%s\n",uid);
                if(send_pack(atoi(row[0]),ADDFRIEND,strlen(msg),msg)<0){
                    my_err("write",__LINE__);
                }
            }
            
        }
        else{
            //不在线
            if(count==1){
                //将link改为1
                memset(cmd,0,sizeof(cmd));
                sprintf(cmd,"update friend set link = 1 where (user='%s' && friend='%s')||(user='%s' && friend='%s')",uid,fid,fid,uid);
                if(mysql_query(&mysql, cmd)<0){
                    my_err("mysql_query",__LINE__);
                }
            }
        }
    }
    free(arg);
    printf("addfriend over\n");
    return 0;
}

