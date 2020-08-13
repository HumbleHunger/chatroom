/*************************************************************************
	> File Name: unpack.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月28日 星期二 14时50分34秒
 ************************************************************************/

#include"chat.h"
#include"thread_pool.h"
int my_read(int conn_fd,void *buf,int len)
{
    int nleft=len;
    int nread=0;
    char *bufp=buf;
    //循环读数据
    while(nleft>0){
        if((nread=read(conn_fd,bufp,nleft))==-1){
            if(errno==EAGAIN){
                nread=0;
            }else{
                return -1;
            }
        }else if(nread==0){
            continue;
        }
        nleft-=nread;
        bufp+=nread;
    }

    return (len-nleft);
}
void *unpack(void *arg)
{
    printf("unpack start\n");//
    int conn_fd=*(int *)arg;
    
    //接收数据类型
    char ty[3];
    memset(ty,0,sizeof(ty));
    if(my_read(conn_fd,ty,2)<0){
        my_err("read",__LINE__);
    }
    fprintf(stderr,"数据type为%s\n",ty);
    
    //接收数据长度
    char len[3];
    memset(len,0,sizeof(len));
    /*if(atoi(ty)==REALFILE){
        if(my_read(conn_fd,len,3)<0){
            my_err("read",__LINE__);
        }
        fprintf(stderr,"数据len为%s\n",len);
    }
    else{
        if(my_read(conn_fd,len,2)<0){
            my_err("read",__LINE__);
        }
        fprintf(stderr,"数据len为%s\n",len);
    }*/
    if(my_read(conn_fd,len,2)<0){
        my_err("read",__LINE__);
    }
    fprintf(stderr,"数据len为%s\n",len);
    //接收数据
    char recv_buf[1024];
    memset(recv_buf,0,sizeof(recv_buf));
    if(my_read(conn_fd,recv_buf,atoi(len))<0){
        my_err("read",__LINE__);
    }
    fprintf(stderr,"数据为%s\n",recv_buf);
    
    //准备参数
    char *argv=(char *)malloc(atoi(len)+4);
    /*if(atoi(ty)==REALFILE){
        memset(argv,0,sizeof(argv));
        sprintf(argv,"%s",recv_buf);
        fprintf(stderr,"文件数据为%s",argv);
    }
    else{
        memset(argv,0,sizeof(argv));
        sprintf(argv,"%s%d\n",recv_buf,conn_fd);
        fprintf(stderr,"参数为%s",argv);
    }*/
    memset(argv,0,sizeof(argv));
    sprintf(argv,"%s%d\n",recv_buf,conn_fd);
    fprintf(stderr,"参数为%s",argv);
    //判断type，将对应处理函数放进线程池
    int type=atoi(ty);
    pthread_t tid;
    printf("real type is %d\n",type);
    switch(type){
        case LOGIN:
        add_work(login,(void *)argv);
        break;
        case UREGISTER:
        add_work(uregister,(void *)argv);
        break;
        case FCHAT:
        add_work(fchat,(void *)argv);
        break;
        case ADDFRIEND:
        add_work(addfriend,(void *)argv);
        break;
        case DEALFRIEND:
        add_work(dealfriend,(void *)argv);
        break;
        case DELFRIEND:
        add_work(delfriend,(void *)argv);
        break;
        case FINDFRIEND:
        add_work(findfriend,(void *)argv);
        break;
        case FRIENDLIST:
        add_work(friendlist,(void *)argv);
        break;
        case FCHATMSG:
        add_work(fchatmsg,(void *)argv);
        break;
        case BLOCKFRIEND:
        add_work(blockfriend,(void *)argv);
        break;
        case RECOVERF:
        add_work(recoverf,(void *)argv);
        break;
        case GCHAT:
        add_work(gchat,(void *)argv);
        break;
        case GROUPLIST:
        add_work(grouplist,(void *)argv);
        break;
        case CREATEGROUP:
        add_work(creategroup,(void *)argv);
        break;
        case DELGROUP:
        add_work(delgroup,(void *)argv);
        break;
        case ADDGROUP:
        add_work(addgroup,(void *)argv);
        break;
        case EXITGROUP:
        add_work(exitgroup,(void *)argv);
        break;
        case GROUPMEMBER:
        add_work(groupmember,(void *)argv);
        break;
        case GCHATMSG:
        add_work(gchatmsg,(void *)argv);
        break;
        case SETSTATE:
        add_work(gsetstate,(void *)argv);
        break;
        case DELMEMBER:
        add_work(delmember,(void *)argv);
        break;
        case FRIEND:
        add_work(friends,(void *)argv);
        break;
        case GROUP:
        add_work(group,(void *)argv);
        break;
        case DEALGROUP:
        add_work(dealgroup,(void *)argv);
        break;
        case FINDPSW:
        add_work(findpsw,(void *)argv);
        break;
        case USER:
        add_work(user,(void *)argv);
        break;
        case ANSWER:
        add_work(answer,(void *)argv);
        break;
        case SENDFILE:
        add_work(send_file,(void *)argv);
        break;
        case RECVFILE:
        add_work(recv_file,(void *)argv);
        break;
        case REALFILE:
        add_work(realfile,(void *)argv);
        break;
        case OVER:
        add_work(over,(void *)argv);
        break;
    }
    printf("unpack is over\n");//
}
