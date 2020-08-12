/*************************************************************************
	> File Name: recvfile.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月11日 星期二 17时03分11秒
 ************************************************************************/

#include"client.h"
void *recvfile(void *arg)
{
    int len=0;
    realfile_read_len=0;
    //获取好友ID
    char fid[10];
    if((len=realfile_get_arg(arg,fid,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    fid[len]=0;
    //printf("fid is %s\n",fid);//
    //获取文件路径
    char pathname[256];
    if((len=realfile_get_arg(arg,pathname,10))<0){
        fprintf(stderr,"get_arg failed\n");
    }
    pathname[len]=0;
    //printf("pathname is %s\n",pathname);//
    //从路径名中解析出文件名
    char filename[257];
    len=0;
    for(int i=0;i<strlen(pathname);i++){
        if(pathname[i]=='/'){
            len=0;
            continue;
        }
        filename[len++]=pathname[i];
    }
    filename[len]=0;
    //创建TCP套接字用于传输文件
    filefd=socket(AF_INET,SOCK_STREAM,0);
    if(filefd<0){
        my_err("socket",__LINE__);
    }
    //printf("filefd is %d\n",filefd);
    //链接服务器
    if(connect(filefd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in))<0){
        my_err("connect",__LINE__);
    }
    //查询是否有文件传送
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n%s\n",user_id,fid,filename);
    if(send_pack(filefd,RECVFILE,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    //接收服务器消息
    char recv_buf[5];
    memset(recv_buf,0,sizeof(recv_buf));
    unpack(filefd,recv_buf,sizeof(recv_buf));
    printf("recv_buf[0] is %c\n",recv_buf[0]);
    if(recv_buf[0]=='0'){
        printf("\t\t\t\t\t对方未向您发送文件\n");
        close(filefd);
        pthread_exit(NULL);
    }
    //接收服务器就绪消息
    memset(recv_buf,0,sizeof(recv_buf));
    unpack(filefd,recv_buf,sizeof(recv_buf));
    printf("recv_buf[0] is %c\n",recv_buf[0]);
    if(recv_buf[0]=='0'){
        close(filefd);
        pthread_exit(NULL);
    }
    FILE *fp;
    fp=fopen(filename,"w");
    P_LOCK;
    printf("\t\t\t\t\t正在接收文件\n");
    P_UNLOCK;
    int size=0;
    while(1){
        //接收数据类型
        char type[2];
        memset(type,0,sizeof(type));
        if(my_read(filefd,type,1)<0){
            my_err("read",__LINE__);
        }
        if(type[0]=='0'){
            break;
        }
        //fprintf(stderr,"数据type为%s\n",type);
        //接收数据长度
        char len[5];
        memset(len,0,sizeof(len));
        if(my_read(filefd,len,4)<0){
            my_err("read",__LINE__);
        }
        //fprintf(stderr,"数据len为%s\n",len);
        //接收数据
        char buffer[1024];
        memset(buffer,0,sizeof(buffer));
        if(my_read(filefd,buffer,atoi(len))<0){
            my_err("read",__LINE__);
        }
       // printf("buffer is %s\n",buffer);
        if((size=fwrite(buffer,sizeof(char),atoi(len),fp))<atoi(len))
        {
            printf("File:\t%s Write Failed\n", filename);
            break;
        }
        //printf("fwrite size is %d\n",size);
    }
    close(filefd);
    fclose(fp);
    P_LOCK;
    printf("\t\t\t\t\t文件接收完成\n");
    P_UNLOCK;
    pthread_exit(NULL);
}
