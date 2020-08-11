/*************************************************************************
	> File Name: recv_file.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月07日 星期五 11时13分40秒
 ************************************************************************/

#include"client.h"
int recv_file()
{
    P_LOCK;
    printf("请输入文件的发送者ID\n");
    P_UNLOCK;
    char fid[10];
    scanf("%s",fid);
    P_LOCK;
    printf("请输入您要接收的文件名\n");
    P_UNLOCK;
    char filename[256];
    scanf("%s",filename);
    pthread_t tid;
    char *arg=(char *)malloc(1024*sizeof(char));
    memset(arg,0,sizeof(arg));
    sprintf(arg,"%s\n%s\n",fid,filename);
    pthread_create(&tid,NULL,recvfile,(void *)arg);
}
