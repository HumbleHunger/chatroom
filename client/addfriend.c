/*************************************************************************
	> File Name: addfriend.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月30日 星期四 10时05分09秒
 ************************************************************************/

#include"client.h"
int addfriend()
{
    P_LOCK;
    printf("请输入对方ID\n");
    P_UNLOCK;
    char fid[10];
    memset(fid,0,sizeof(fid));
    int c,i=0;
    Scanf(fid);
    /*while(1){
        while(((c=getchar())!='\n') && (c!=EOF)){
            if(c<48 || c>57 || i>8){
                P_LOCK;
                printf("\t\t\t\t\t检测到非法输入!请重新输入\n");
                P_UNLOCK;
                memset(fid,0,sizeof(fid));
                i=0;
                while((c=getchar())!='\n');
                break;
            }
            fid[i]=c;
            i++;
        }
        if(i==0){
            continue;
        }
        else{
            break;
        }
    }*/
    if(strcmp(user_id,fid)==0){
        printf("\t\t\t\t\t请勿添加自己为好友\n");
        return 0;
    }
    char send_buf[1024];
    memset(send_buf,0,sizeof(send_buf));
    sprintf(send_buf,"%s\n%s\n",user_id,fid);
    //printf("addfriend send_buf is %s",send_buf);//
    if(send_pack(connfd,ADDFRIEND,strlen(send_buf),send_buf)<0){
        my_err("write",__LINE__);
    }
    return 0;
}

