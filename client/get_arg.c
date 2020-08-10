/*************************************************************************
	> File Name: get_arg.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月30日 星期四 18时03分41秒
 ************************************************************************/

#include"client.h"
int get_arg(char *read_buf,char *recv_buf,int len)
{
    //获取已读长度
    //printf("read_buf is %s",read_buf);
    //printf("len start is %d\n",len);//
    //printf("%d\n",read_len);//
    //设置指针位置
    char *pread=read_buf+read_len;
    int i;
    for(i=0;*pread!='\n';i++){
        if(i>len){
            //printf("i is %d len is %d",i,len);//
            //printf("return -1\n");//
            break;
        }
        //printf("pread is %c\t",*pread);
        recv_buf[i]=*pread++;
        read_len++;
    }
    read_len++;
    //将更改后已读长度保存
    //printf("read_len is %d\n",read_len);//
    //printf("TSD is %d\n",pthread_getspecific(key));//
    //返回所读字节数
    return i;
}
int realfile_get_arg(char *read_buf,char *recv_buf,int len)
{
    //获取已读长度
    //printf("read_buf is %s",read_buf);
    //printf("len start is %d\n",len);//
    //printf("%d\n",read_len);//
    //设置指针位置
    char *pread=read_buf+realfile_read_len;
    int i;
    for(i=0;*pread!='\n';i++){
        if(i>len){
            //printf("i is %d len is %d",i,len);//
            //printf("return -1\n");//
            break;
        }
        //printf("pread is %c\t",*pread);
        recv_buf[i]=*pread++;
        realfile_read_len++;
    }
    realfile_read_len++;
    //将更改后已读长度保存
    //printf("read_len is %d\n",read_len);//
    //printf("TSD is %d\n",pthread_getspecific(key));//
    //返回所读字节数
    return i;
}
