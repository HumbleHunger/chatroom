/*************************************************************************
	> File Name: send_file.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月06日 星期四 23时44分08秒
 ************************************************************************/

#include"client.h"
int send_file()
{
    P_LOCK;
    printf("请输入文件的接收者ID\n");
    P_UNLOCK;
    char fid[10];
    scanf("%s",fid);
    P_LOCK;
    printf("请输入您要发送的文件路径\n");
    P_UNLOCK;
    char filename[1024];
    scanf("%s",filename);
    FILE *fp;
    if((fp=fopen(filename,"r"))==NULL){
        printf("请输入正确的文件路径\n");
        return 0;
    }
    fclose(fp);
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n",user_id,fid);
    //printf("friend send_buf is %s",send_buf);//
    if(send_pack(connfd,FRIEND,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    P_LOCK;
    printf("正在处理中\n");
    P_UNLOCK;
    S_LOCK;
    //printf("wait?\n");
    C_WAIT;
    //printf("char_flag = %d\n",chat_flag);
    if(chat_flag==0){
        S_UNLOCK;
        return 0;
    }
    chat_flag=0;
    S_UNLOCK;
    pthread_t tid;
    char *arg=(char *)malloc(1024*sizeof(char));
    memset(arg,0,sizeof(arg));
    sprintf(arg,"%s\n%s\n",fid,filename);
    P_LOCK;
    printf("\t\t\t\t\t正在发送文件至服务器\n");
    P_UNLOCK;
    pthread_create(&tid,NULL,realfile,(void *)arg);
}
