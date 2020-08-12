/*************************************************************************
	> File Name: realfile.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月07日 星期五 09时52分11秒
 ************************************************************************/

#include"client.h"
void *realfile(void *arg)
{
    //printf("realfile start\n");
    int len=0;
    realfile_read_len=0;
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
    //链接服务器
    if(connect(filefd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in))<0){
        my_err("connect",__LINE__);
    }
    FILE *fp=fopen(pathname,"r");
    //发送发文件请求
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n",filename);
    if(send_pack(filefd,REALFILE,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    //接收服务器就绪消息
    char recv_buf[5];
    memset(recv_buf,0,sizeof(recv_buf));
    unpack(filefd,recv_buf,sizeof(recv_buf));
    if(recv_buf[0]!='1'){
        pthread_exit(NULL);
    }
    //发送文件
    File_pack buffer;
    memset(&buffer,0,sizeof(buffer));
    while((len=fread(buffer.data,sizeof(char),512,fp))>0)
    {
        //printf("len is %d\n",len);
        buffer.type='1';
        sprintf(buffer.len,"%d",len);
        //printf("buffer.data is %s",buffer.data);
        if(send(filefd,&buffer,len+5,0)<0)
        {
            printf("Send File:%s Failed./n", filename);
            break;
        }
        memset(&buffer,0,sizeof(buffer));
    }
    //发送END包
    memset(&buffer,0,sizeof(buffer));
    buffer.type='0';
    if(send(filefd,&buffer,1,0)<0){
        printf("Send File:%s Failed./n", filename);
    }
    // 关闭文件和套接字
    close(filefd);
    fclose(fp);
    P_LOCK;
    printf("\t\t\t\t\t文件已发送至服务器，等待好友接收\n");
    P_UNLOCK;
    free(arg);
    pthread_exit(NULL);
}
