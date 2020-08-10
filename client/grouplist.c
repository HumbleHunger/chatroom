/*************************************************************************
	> File Name: grouplist.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月03日 星期一 22时27分19秒
 ************************************************************************/

#include"client.h"
int grouplist()
{
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n",user_id);
    //printf("grouplist send_buf is %s",send_buf);//
    if(send_pack(connfd,GROUPLIST,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    return 0;
}
