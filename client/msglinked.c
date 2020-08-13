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
    if(Msg.head==NULL && Msg.tail==NULL){
        Msg.head=operate;
        Msg.tail=operate;
        Msg.num++;
    }
    else{
        Fmsg *record=Msg.tail;
        record->next=operate;
        Msg.tail=operate;
        Msg.num++;
    }
    UNLOCK;
}
int printnode()
{
    Fmsg *operate;
    LOCK;
    if(Msg.num==0){
        printf("\t\t\t\t\t您暂时没有好友消息\n");
        UNLOCK;
        return 0;
    }
    operate=Msg.head;
    for(int i=0;i<Msg.num;i++){
        P_LOCK;
        printf("\t\t\t\t\t%s--->%s:%s\n",operate->send_id,operate->recv_id,operate->msg);
        P_UNLOCK;
        operate=operate->next;
    }
    delnode();
    UNLOCK;
}
int delnode()
{
    Fmsg *operate=Msg.head;
    Fmsg *record=NULL;
    while(operate){
        record=operate->next;
        free(operate);
        Msg.num--;
        operate=record;
    }
    Msg.num=0;
    Msg.tail=NULL;
    Msg.head=NULL;
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
        Gsg.tail=operate;
        Gsg.num++;
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
        printf("\t\t\t\t\t%s在群%s中说:%s\n",operate->mid,operate->gid,operate->msg);
        P_UNLOCK;
        operate=operate->next;
    }
    UNLOCK;
}
int delgmsgnode()
{
    Gmsg *operate=Gsg.head;
    Gmsg *record=NULL;
    while(operate){
        record=operate->next;
        free(operate);
        Gsg.num--;
        operate=record;
    }
    Gsg.num=0;
    Gsg.head=NULL;
    Gsg.tail=NULL;
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
        Gm.tail=operate;
        Gm.num++;
    }
    GM_UNLOCK;
}
int delgnode()
{
    GM *operate=Gm.head;
    GM *record=NULL;
    while(operate){
        record=operate->next;
        free(operate);
        Gm.num--;
        operate=record;
    }
    Gm.num=0;
    Gm.head=NULL;
    Gm.tail=NULL;
}
int addfrnode(char *send_id)
{
    Freq *operate=(Freq*)malloc(sizeof(Freq));
    memset(operate,0,sizeof(Freq));
    strcpy(operate->send_id,send_id);
    operate->next=NULL;
    FR_LOCK;
    if(Frh.num==0){
        Frh.head=operate;
        Frh.tail=operate;
        Frh.num++;
    }
    else{
        Freq *record=Frh.tail;
        record->next=operate;
        Frh.tail=operate;
        Frh.num++;
    }
    FR_UNLOCK;
}
int delfrnode()
{
    Freq *operate=Frh.head;
    Freq *record=NULL;
    while(operate){
        record=operate->next;
        free(operate);
        Frh.num--;
        operate=record;
    }
    Frh.num=0;
    Frh.head=NULL;
    Frh.tail=NULL;
}
