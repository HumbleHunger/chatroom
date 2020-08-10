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
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n%s\n",user_id,fid,filename);
    //printf("recv_file send_buf is %s",send_buf);//
    if(send_pack(connfd,RECVFILE,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
}
