/*************************************************************************
	> File Name: msglinked.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月03日 星期一 11时02分43秒
 ************************************************************************/

#include"client.h"
int addfmsg(char *send_id,char *recv_id,char *msg)
{
    Fmsg *operate=(Fmsg*)malloc(sizeof(Fmsg));
    memset(operate,0,sizeof(Fmsg));
    strcpy(operate->send_id,send_id);
    strcpy(operate->recv_id,recv_id);
    strcpy(operate->msg,msg);
    operate->next=NULL;
    LOCK;
    if(Msg.num==0){
        Msg.head=operate;
        Msg.tail=operate;
        Msg.num++;
    }
    else{
        Fmsg *record=Msg.tail;
        record->next=operate;
        Msg.num++;
    }
    UNLOCK;
}
int addgmsg(char *mid,char *gid,char *msg)
{
    Gmsg *operate=(Gmsg*)malloc(sizeof(Gmsg));
    memset(operate,0,sizeof(Gmsg));
    strcpy(operate->mid,mid);
    strcpy(operate->gid,gid);
    strcpy(operate->msg,msg);
    operate->next=NULL;
    LOCK;
    if(Gsg.num==0){
        Gsg.head=operate;
        Gsg.tail=operate;
        Gsg.num++;
    }
    else{
        Gmsg *record=Gsg.tail;
        record->next=operate;
        Gsg.num++;
    }
    UNLOCK;
}
int addgnode(char *member_id,char *group_id)
{
    GM *operate=(GM*)malloc(sizeof(GM));
    memset(operate,0,sizeof(GM));
    strcpy(operate->member_id,member_id);
    strcpy(operate->group_id,group_id);
    operate->next=NULL;
    GM_LOCK;
    if(Gm.num==0){
        Gm.head=operate;
        Gm.tail=operate;
        Gm.num++;
    }
    else{
        GM *record=Gm.tail;
        record->next=operate;
        Gm.num++;
    }
    GM_UNLOCK;
}
int delnode()
{
    LOCK;
    Fmsg *operate=Msg.head;
    Fmsg *record=operate->next;
    free(operate);
    Msg.num--;
    Msg.head=record;
    if(Msg.num==0){
        Msg.tail=NULL;
    }
    UNLOCK;
}
int delgnode()
{
    GM *operate=Gm.head;
    GM *record;
    while(operate){
        record=operate->next;
        free(operate);
        Gm.num--;
        operate=record;
    }
    Gm.head=NULL;
    Gm.tail=NULL;
}
int printnode()
{
    Fmsg *operate;
    LOCK;
    operate=Msg.head;
    for(int i=0;i<Msg.num;i++){
        P_LOCK;
        printf("\t\t\t%s--->%s:%s\n",operate->send_id,operate->recv_id,operate->msg);
        P_UNLOCK;
        operate=operate->next;
    }
    UNLOCK;
}
int printgnode()
{
    Gmsg *operate;
    LOCK;
    operate=Gsg.head;
    for(int i=0;i<Gsg.num;i++){
        P_LOCK;
        printf("\t\t\t%s在群%s中说:%s\n",operate->mid,operate->gid,operate->msg);
        P_UNLOCK;
        operate=operate->next;
    }
    UNLOCK;
}

