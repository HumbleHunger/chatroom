/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月27日 星期一 19时34分13秒
 ************************************************************************/

#include"client.h"
void my_err(const char *str,int line)
{
    fprintf(stderr,"line:%d",line);
    perror(str);
    exit(1);
}
int main(int argc,char **argv)
{
    //Msg=(Msghead *)malloc(sizeof(Msghead));
    memset(&Msg,0,sizeof(Msghead));
    Msg.head=NULL;
    Msg.tail=NULL;
    Msg.num=0;
    struct sockaddr_in serv_addr;
    int ret;
    int serv_port;
    char recv_buf[1024];
    memset(recv_buf,0,sizeof(recv_buf));
    //检查参数个数
    if(argc!=5){
        printf("Usage: [-p] [serv_port] [-a] [serv_addr]\n");
        exit(1);
    }
    //初始化服务器地址
    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family=AF_INET;
    //解析参数，获取服务器地址以及端口号
    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"-p")==0){
            serv_port=atoi(argv[i+1]);
            //检查参数合法性并记录
            if(serv_port<0 || serv_port>65535){
                printf("invalid serv_port\n");
                exit(1);
            }
            else{
                serv_addr.sin_port=htons(serv_port);
            }
            continue;
        }
        if(strcmp(argv[i],"-a")==0){
            if(inet_aton(argv[++i],&serv_addr.sin_addr)==0){
                printf("invaild serv_addr\n");
                exit(1);
            }
            continue;
        }
    }
    //再次检查参数是否正确
    if(serv_addr.sin_port==0 || serv_addr.sin_addr.s_addr==0){
        printf("Usage: [-p] [serv_port] [-a] [serv_addr]\n");
        exit(1);
    }
    //创建TCP套接字
    connfd=socket(AF_INET,SOCK_STREAM,0);
    if(connfd<0){
        my_err("socket",__LINE__);
    }
    //链接服务器
    if(connect(connfd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in))<0){
        my_err("connect",__LINE__);
    }
    pthread_mutex_init(&p_mutex,NULL);
    pthread_mutex_init(&s_mutex,NULL);
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    print_main();
    close(connfd);
    pthread_mutex_destroy(&p_mutex);
    pthread_mutex_destroy(&s_mutex);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}
