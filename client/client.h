/*************************************************************************
	> File Name: client.h
	> Author: 
	> Mail: 
	> Created Time: 2020年07月28日 星期二 16时49分42秒
 ************************************************************************/

#ifndef _CLIENT_H
#define _CLIENT_H
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
#include<pthread.h>
#include<errno.h>

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
#define GM_LOCK pthread_mutex_lock(&gm_mutex)
#define GM_UNLOCK pthread_mutex_unlock(&gm_mutex)
#define LOCK pthread_mutex_lock(&mutex)
#define UNLOCK pthread_mutex_unlock(&mutex)
#define P_LOCK pthread_mutex_lock(&p_mutex)
#define S_LOCK pthread_mutex_lock(&s_mutex)
#define P_UNLOCK pthread_mutex_unlock(&p_mutex)
#define S_UNLOCK pthread_mutex_unlock(&s_mutex)
#define C_WAIT pthread_cond_wait(&cond,&s_mutex)
#define C_SIGNAL pthread_cond_signal(&cond)

typedef struct {
    char type[2];
    char len[2];
    char data[1024];
}Pack;
typedef struct {
    char type;
    char len[4];
    char data[1024];
}File_pack;
//好友消息
typedef struct fmsg{
    char send_id[10];
    char recv_id[10];
    struct fmsg* next;
    char msg[500];
}Fmsg;
//好友消息链表
typedef struct fmsgh{
    Fmsg *head;
    Fmsg *tail;
    int num;
}Msghead;
//群成员申请
typedef struct gm{
    char member_id[10];
    char group_id[10];
    struct gm* next;
}GM;
//群申请链表
typedef struct gmember{
    GM *head;
    GM *tail;
    int num;
}GMhead;
//群消息
typedef struct gmsg{
    char mid[10];
    char gid[10];
    struct gmsg* next;
    char msg[500];
}Gmsg;
//群消息链表
typedef struct gmsgh{
    Gmsg *head;
    Gmsg *tail;
    int num;
}Gmsghead;

Msghead Msg;
GMhead Gm;
Gmsghead Gsg;

int exit_flag;//用户注销标识
char user_id[10];//用户id
int connfd;//socket
int filefd;//file socket
int chat_flag;//聊天标志1好友2群
int findpsw_flag;//找回密码标志
int file_flag;
char chat_id[10];//聊天对象
int read_len;
int realfile_read_len;
struct sockaddr_in serv_addr;//服务器地址

//读写锁
pthread_mutex_t p_mutex,s_mutex,mutex,gm_mutex;
pthread_cond_t cond;

int send_pack(int connfd,int type,int len,char *value);
int unpack(int connfd,char *recv_buf,int lenth);
int my_write(int connfd,char *send_buf,int len);
int my_read(int conn_fd,void *buf,int len);
void my_err(const char *str,int line);

void *msgbox(void *arg);
void *filemsgbox(void *arg);
int get_arg(char *read_buf,char *recv_buf,int len);

int addfmsg(char *send_id,char *recv_id,char *msg);
int delnode();
int printnode();

int addgmsg(char *mid,char *gid,char *msg);
int printgnode();

int addgnode(char *member_id,char *group_id);
int delgnode();

int print_main();
int login();
int uregister();
int findpsw();
int print_meau();

int print_friend();
int friendlist();
int addfriend();
int findfriend();
int delfriend();
int blockfriend();
int fchat();
int fchatmsg();

int print_group();
int creategroup();
int grouplist();
int addgroup();
int dealgroup();
int exitgroup();
int gsetstate();
int gchat();
int delmember();
int delgroup();
int groupmember();
int gchatmsg();

int print_file();
int send_file();
int recv_file();
void *realfile(void *arg);
void *recvfile(void *arg);
int realfile_get_arg(char *read_buf,char *recv_buf,int len);

#endif
