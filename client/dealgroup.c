/*************************************************************************
	> File Name: dealgroup.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月04日 星期二 20时32分13秒
 ************************************************************************/

#include"client.h"
int dealgroup()
{
    GM_LOCK;
    if(Gm.num==0){
        printf("您暂时没有群验证消息\n");
        GM_UNLOCK;
        return 0;
    }
    GM *operate=Gm.head;
    int chose=0;
    char send_buf[1024];
    while(operate){
        memset(send_buf,0,sizeof(send_buf));
        P_LOCK;
        printf("%s申请加入群%s\n",operate->member_id,operate->group_id);
        printf("1.同意\t\t2.拒绝\n");
        P_UNLOCK;
        scanf("%d",&chose);
        switch(chose){
            case 1:
            sprintf(send_buf,"%s\n%s\n1\n",operate->member_id,operate->group_id);
            if(send_pack(connfd,DEALGROUP,strlen(send_buf),send_buf)<0){
                my_err("write",__LINE__);
            }
            break;
            case 2:
            sprintf(send_buf,"%s\n%s\n2\n",operate->member_id,operate->group_id);
            if(send_pack(connfd,DEALGROUP,strlen(send_buf),send_buf)<0){
                my_err("write",__LINE__);
            }
            break;
        }
        operate=operate->next;
    }
    delgnode();
    GM_UNLOCK;
}
