/*************************************************************************
	> File Name: recv_file.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月07日 星期五 10时40分17秒
 ************************************************************************/

#include"chat.h"
void *recv_file(void *arg)
{
    printf("recvfile start\n");
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
    //获取文件名
    char filename[256];
    if((len=get_arg(arg,filename,256))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    filename[len]=0;
    printf("filename is %s\n",filename);//
    //获取用户socket
    char fd[4];
    if((len=get_arg(arg,fd,4))<0){
        fprintf(log,"get_arg failed\n");
    }
    fd[len]=0;
    printf("fd is %s\n",fd);
    char cmd[1024];
    //查询是否有文件记录
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"select * from file where send_id = %s && recv_id = %s && filename = '%s'",fid,uid,filename);
    if(mysql_query(&mysql, cmd)<0){
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=NULL;
	MYSQL_ROW row;
    result=mysql_store_result(&mysql);
    row=mysql_fetch_row(result);
    char data[1024];
    if(row==NULL){
        memset(data,0,sizeof(data));
        sprintf(data,"0\n");
        if(send_pack(atoi(fd),RECVFILE,strlen(data),data)<0){
            my_err("write",__LINE__);
        }
        free(arg);
        return NULL;
    }
    FILE *fp=fopen(filename,"r");
    char send_buf[1024];
    // 每读取一段数据，便将其发送给服务器，循环直到文件读完为止
    char buffer[100];
    memset(buffer,0,sizeof(buffer));
    while((len=fread(buffer,sizeof(char),96-strlen(filename),fp)) > 0)
    {
        memset(send_buf,0,sizeof(send_buf));
        sprintf(send_buf,"1\n%s\n%s",filename,buffer);
        //printf("recvfile send_buf is %s",send_buf);//
        if(send_pack(atoi(fd),RECVFILE,len+3+strlen(filename),send_buf)<0){
            my_err("write",__LINE__);
        }
        memset(buffer,0,sizeof(buffer));
    }
    // 关闭文件
    fclose(fp);
    free(arg);
    printf("recvfile over\n");
}
