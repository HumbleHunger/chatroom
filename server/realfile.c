/*************************************************************************
	> File Name: realfile.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月06日 星期四 23时32分39秒
 ************************************************************************/

#include"chat.h"
void *realfile(void *arg)
{
    printf("realfile start\n");
    int len=0;
    int lenth=0;
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
    epoll_ctl(epfd,EPOLL_CTL_DEL,atoi(fd),NULL);
    //printf("filedata is %s\n",buffer);
    FILE *fp=fopen(filename,"w");
    int length = 0;
    char buffer[1024];
    memset(buffer,0,sizeof(buffer));
    char data[5];
    memset(data,0,sizeof(data));
    sprintf(data,"1\n");
    //发送下就绪信息
    if(send_pack(atoi(fd),START,strlen(data),data)<0){
        my_err("write",__LINE__);
    }
    int conn_fd=atoi(fd);
    while(1){
        //接收数据类型
        char type[2];
        memset(type,0,sizeof(type));
        if(my_read(conn_fd,type,1)<0){
            my_err("read",__LINE__);
        }
        if(type[0]=='0'){
            break;
        }
        //fprintf(stderr,"数据type为%s\n",ty);
        //接收数据长度
        char len[5];
        memset(len,0,sizeof(len));
        if(my_read(conn_fd,len,4)<0){
            my_err("read",__LINE__);
        }
        //fprintf(stderr,"数据len为%s\n",len);
        //接收数据
        char buffer[1024];
        memset(buffer,0,sizeof(buffer));
        if(my_read(conn_fd,buffer,atoi(len))<0){
            my_err("read",__LINE__);
        }
        //printf("buffer is %s\n",buffer);
        if(fwrite(buffer,sizeof(char),atoi(len),fp)<atoi(len))
        {
            printf("File:\t%s Write Failed\n", filename);
            break;
        }
    }
    fclose(fp);
    close(atoi(fd));
    free(arg);
    printf("realfile over\n");
}
