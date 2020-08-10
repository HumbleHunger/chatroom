/*************************************************************************
	> File Name: chat.h
	> Author: 
	> Mail: 
	> Created Time: 2020年07月27日 星期一 17时33分31秒
 ************************************************************************/

#ifndef _CHAT_H
#define _CHAT_H

#include <stdio.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "thread_pool.h"
#include<pthread.h>
#include<errno.h>
#include<signal.h>
#define SERV_PORT 8848//服务器端口号

enum{
    LOGIN=1,
    UREGISTER,
    FCHAT,
    ADDFRIEND,
    DELFRIEND,
    FINDFRIEND,
    FRIENDLIST,
    FCHATMSG,
    BLOCKFRIEND,
    GCHAT,
    CREATEGROUP,
    DELGROUP,
    ADDGROUP,
    EXITGROUP,
    GROUPMEMBER,
    GCHATMSG,
    SETSTATE,
    DELMEMBER,
    FRIEND,
    OVER,
    GROUPLIST,
    DEALGROUP,
    GROUP,
    FINDPSW,
    USER,
    ANSWER,
    SENDFILE,
    RECVFILE,
    REALFILE,
    START,
};

typedef struct{
    char type[2];
    char len[2];
    char data[1024];
}Pack;
typedef struct {
    char type;
    char len[4];
    char data[1024];
}File_pack;
MYSQL mysql;
FILE* log;
pthread_key_t key;
int epfd;

void setblock(int sock);
void setnoblock(int sock);
void *unpack(void *arg);
int my_read(int conn_fd,void *buf,int len);
int get_arg(char *read_buf,char *recv_buf,int len);
int send_pack(int connfd,int type,int len,char *value);
int my_write(int connfd,char *send_buf,int len);
void my_err(const char *str,int line);

int offline(char *id,int connfd);
void *login(void *arg);
void *uregister(void *arg);
void *addfriend(void *arg);
void *over(void *arg);
void *friendlist(void *arg);
void *findfriend(void *arg);
void *delfriend(void *arg);
void *blockfriend(void *arg);
void *fchat(void *arg);
void *friends(void *arg);
void *creategroup(void *arg);
void *grouplist(void *arg);
void *addgroup(void *arg);
void *dealgroup(void *arg);
void *exitgroup(void *arg);
void *gsetstate(void *arg);
void *gchat(void *arg);
void *group(void *arg);
void *delmember(void *arg);
void *delgroup(void *arg);
void *user(void *arg);
void *answer(void *arg);
void *findpsw(void *arg);
void *groupmember(void *arg);
void *fchatmsg(void *arg);
void *gchatmsg(void *arg);
void *send_file(void *arg);
void *recv_file(void *arg);
void *realfile(void *arg);
#endif
