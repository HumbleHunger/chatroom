/*************************************************************************
	> File Name: get_arg.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月28日 星期二 11时34分53秒
 ************************************************************************/

#include"chat.h"
#include"thread_pool.h"
int get_arg(char *read_buf,char *recv_buf,int len)
{
    //获取已读长度
    //printf("read_buf is %s",read_buf);
    //printf("len start is %d\n",len);//
    int read_len=(int)pthread_getspecific(key);
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
    pthread_setspecific(key,(void *)read_len);
    //printf("TSD is %d\n",pthread_getspecific(key));//
    //返回所读字节数
    return i;
}

