/*************************************************************************
	> File Name: msgbox.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月30日 星期四 17时42分32秒
 ************************************************************************/

#include"client.h"
void *msgbox(void *arg)
{
    //printf("msgbox is running\n");
    while(1){
        //接收数据类型
        //printf("msgbox is while\n");
        read_len=0;
        char ty[3];
        memset(ty,0,sizeof(3));
        if(my_read(connfd,ty,2)<0){
            my_err("read",__LINE__);
        }
        //fprintf(stderr,"数据type为%s\n",ty);
    
        //接收数据长度
        char len[3];
        memset(len,0,sizeof(len));
        if(my_read(connfd,len,2)<0){
            my_err("read",__LINE__);
        }
        //fprintf(stderr,"数据len为%s\n",len);
        
        //接收数据
        char recv_buf[1024];
        memset(recv_buf,0,sizeof(recv_buf));
        if(my_read(connfd,recv_buf,atoi(len))<0){
            my_err("read",__LINE__);
        }
        //fprintf(stderr,"数据为%s\n",recv_buf);

        //printf("收到数据%s",recv_buf);
        
        //判断type，分别处理
        int type=atoi(ty);
        //printf("msgbox type is %d\n",type);
        switch(type){
        case OVER:{
            //printf("msgbox is over\n");
            pthread_exit(NULL);
        }
        case ADDFRIEND:{
            char flag[2];
            memset(flag,0,sizeof(flag));
            if(get_arg(recv_buf,flag,sizeof(flag))<0){
                my_err("read",__LINE__);
            }
            if(flag[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t好友不存在\n");
                P_UNLOCK;
            }
            else if(flag[0]=='1'){
                char fid[10];
                memset(fid,0,sizeof(fid));
                if(get_arg(recv_buf,fid,sizeof(fid))<0){
                    my_err("read",__LINE__);
                }
                P_LOCK;
                printf("\t\t\t\t\t%s申请添加您为好友\n",fid);
                printf("\t\t\t\t\t同意请向%s发送好友申请\n\t\t\t\t\t拒绝则忽略\n",fid);
                P_UNLOCK;
                /*
                int ch;
                S_LOCK;
                scanf("%d",&ch);
                S_UNLOCK;
                printf("ch is %d\n",ch);
                if(ch==1){
                    char send_buf[1024];
                    memset(send_buf,0,sizeof(send_buf));
                    sprintf(send_buf,"%s\n%s\n",user_id,fid);
                    printf("addfriend send_buf is %s",send_buf);//
                    if(send_pack(connfd,ADDFRIEND,strlen(send_buf),send_buf)<0){
                        my_err("write",__LINE__);
                    }
                }
                else(chose==2){
                    
            }*/
            }
            else if(flag[0]=='2'){
                char fid[10];
                memset(fid,0,sizeof(fid));
                if(get_arg(recv_buf,fid,sizeof(fid))<0){
                    my_err("read",__LINE__);
                }
                P_LOCK;
                printf("\t\t\t\t\t%s已同意您的好友请求\n",fid);
                P_UNLOCK;
            }
            else if(flag[0]=='3'){
                P_LOCK;
                printf("\t\t\t\t\t请勿重复添加好友\n");
                P_UNLOCK;
            }
            break;
        }
        case FRIENDLIST:{
            char fid[10];
            memset(fid,0,sizeof(fid));
            if(get_arg(recv_buf,fid,sizeof(fid))<0){
                my_err("read",__LINE__);
            }
            if(fid[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t您没有好友\n");
                P_UNLOCK;
                break;
            }
            char name[32];
            memset(name,0,sizeof(name));
            if(get_arg(recv_buf,name,sizeof(name))<0){
                my_err("read",__LINE__);
            }
            char state[2];
            memset(state,0,sizeof(state));
            if(get_arg(recv_buf,state,sizeof(state))<0){
                my_err("read",__LINE__);
            }
            P_LOCK;
            printf("\t\t\t\t\tid:%s  name:%s  state:%s\n",fid,name,state);
            P_UNLOCK;
            break;
        }
        case FINDFRIEND:{
            char fid[10];
            memset(fid,0,sizeof(fid));
            if(get_arg(recv_buf,fid,sizeof(fid))<0){
                my_err("read",__LINE__);
            }
            char name[32];
            memset(name,0,sizeof(name));
            if(get_arg(recv_buf,name,sizeof(name))<0){
                my_err("read",__LINE__);
            }
            char state[2];
            memset(state,0,sizeof(state));
            if(get_arg(recv_buf,state,sizeof(state))<0){
                my_err("read",__LINE__);
            }
            P_LOCK;
            printf("\t\t\t\t\tid:%s  name:%s  state:%s\n",fid,name,state);
            P_UNLOCK;
            break;
        }
        case DELFRIEND:{
            char flag[2];
            memset(flag,0,sizeof(flag));
            if(get_arg(recv_buf,flag,sizeof(flag))<0){
                my_err("read",__LINE__);
            }
            //printf("flag is %c\n",flag[0]);//
            if(flag[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t好友不存在\n");
                P_UNLOCK;
            }
            else if(flag[0]=='1'){
                P_LOCK;
                printf("\t\t\t\t\t删除成功\n");
                P_UNLOCK;
            }
            break;
        }
        case BLOCKFRIEND:{
            char flag[2];
            memset(flag,0,sizeof(flag));
            if(get_arg(recv_buf,flag,sizeof(flag))<0){
                my_err("read",__LINE__);
            }
            //printf("flag is %c\n",flag[0]);//
            if(flag[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t好友不存在\n");
                P_UNLOCK;
            }
            else if(flag[0]=='1'){
                P_LOCK;
                printf("\t\t\t\t\t拉黑成功\n");
                P_UNLOCK;
            }
            break;
        }
        case FRIEND:{
            //printf("friend start!!!\n");
            char flag[2];
            memset(flag,0,sizeof(flag));
            if(get_arg(recv_buf,flag,sizeof(flag))<0){
                my_err("read",__LINE__);
            }
            //printf("flag is %c\n",flag[0]);//
            if(flag[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t红色感叹号！\n");
                P_UNLOCK;
                C_SIGNAL;
                break;
            }
            else if(flag[0]=='1'){
                chat_flag=1;
                C_SIGNAL;
                break;
            }
        }
        case GROUP:{
            char flag[2];
            memset(flag,0,sizeof(flag));
            if(get_arg(recv_buf,flag,sizeof(flag))<0){
                my_err("read",__LINE__);
            }
            //printf("flag is %c\n",flag[0]);//
            if(flag[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t群不存在或您未加入此群\n");
                P_UNLOCK;
                C_SIGNAL;
                break;
            }
            else if(flag[0]=='1'){
                chat_flag=2;
                C_SIGNAL;
                break;
            }
        }
        case FCHAT:{
            char send_id[10];
            memset(send_id,0,sizeof(send_id));
            if(get_arg(recv_buf,send_id,sizeof(send_id))<0){
                my_err("read",__LINE__);
            }
            char recv_id[10];
            memset(recv_id,0,sizeof(recv_id));
            if(get_arg(recv_buf,recv_id,sizeof(recv_id))<0){
                my_err("read",__LINE__);
            }
            char msg[500];
            memset(msg,0,sizeof(msg));
            if(get_arg(recv_buf,msg,sizeof(msg))<0){
                my_err("read",__LINE__);
            }
            if(chat_flag==1 && strcmp(send_id,chat_id)==0){
                P_LOCK;
                printf("\t\t\t%s--->%s:%s\n",send_id,recv_id,msg);
                P_UNLOCK;
            }
            else{
                P_LOCK;
                printf("\t\t\t\t\t您有一条好友消息!!!\n");
                //printf("\t\t\t%s--->%s:%s\n",send_id,recv_id,msg);
                P_UNLOCK;
                addfmsg(send_id,recv_id,msg);
            }
            break;
        }
        case GCHAT:{
            char mid[10];
            memset(mid,0,sizeof(mid));
            if(get_arg(recv_buf,mid,sizeof(mid))<0){
                my_err("read",__LINE__);
            }
            char gid[10];
            memset(gid,0,sizeof(gid));
            if(get_arg(recv_buf,gid,sizeof(gid))<0){
                my_err("read",__LINE__);
            }
            char msg[500];
            memset(msg,0,sizeof(msg));
            if(get_arg(recv_buf,msg,sizeof(msg))<0){
                my_err("read",__LINE__);
            }
            if(chat_flag==2 && strcmp(gid,chat_id)==0){
                P_LOCK;
                printf("\t\t\t%s在群%s中说:%s\n",mid,gid,msg);
                P_UNLOCK;
            }
            else{
                P_LOCK;
                printf("\t\t\t\t\t您有一条群消息!!!\n");
                //printf("\t\t\t%s--->%s:%s\n",send_id,recv_id,msg);
                P_UNLOCK;
                addgmsg(mid,gid,msg);
            }
            break;
        }
        case CREATEGROUP:{
            char gid[10];
            memset(gid,0,sizeof(gid));
            if(get_arg(recv_buf,gid,sizeof(gid))<0){
                my_err("read",__LINE__);
            }
            if(gid[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t群名重复，请重新创建群\n");
                P_UNLOCK;
                break;
            }
            P_LOCK;
            printf("\t\t\t\t\t您所创建的群ID为%s\n",gid);
            P_UNLOCK;
            break;
        }
        case GROUPLIST:{
            char gid[10];
            memset(gid,0,sizeof(gid));
            if(get_arg(recv_buf,gid,sizeof(gid))<0){
                my_err("read",__LINE__);
            }
            if(gid[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t您没有加入任何群\n");
                P_UNLOCK;
                break;
            }
            char gname[20];
            memset(gname,0,sizeof(gname));
            if(get_arg(recv_buf,gname,sizeof(gname))<0){
                my_err("read",__LINE__);
            }
            char state[3];
            memset(state,0,sizeof(state));
            if(get_arg(recv_buf,state,sizeof(state))<0){
                my_err("read",__LINE__);
            }
            P_LOCK;
            printf("\t\t\t\t\tgroup_id:%s  group_name:%s  state:%s\n",gid,gname,state);
            P_UNLOCK;
            break;
        }
        case ADDGROUP:{
            char mid[10];
            memset(mid,0,sizeof(mid));
            if(get_arg(recv_buf,mid,sizeof(mid))<0){
                my_err("read",__LINE__);
            }
            if(mid[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t此群不存在\n");
                P_UNLOCK;
                break;
            }
            else if(mid[0]=='a'){
                P_LOCK;
                printf("\t\t\t\t\t您已是群成员\n");
                P_UNLOCK;
                break;
            }
            char gid[10];
            memset(gid,0,sizeof(gid));
            if(get_arg(recv_buf,gid,sizeof(gid))<0){
                my_err("read",__LINE__);
            }
            P_LOCK;
            printf("\t\t\t\t\t您收到一条群验证消息\n");
            P_UNLOCK;
            addgnode(mid,gid);
            break;
        }
        case DEALGROUP:{
            char flag[2];
            memset(flag,0,sizeof(flag));
            if(get_arg(recv_buf,flag,sizeof(flag))<0){
                my_err("read",__LINE__);
            }
            if(flag[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t此请求已被其他管理人员处理\n");
                P_UNLOCK;
                break;
            }
            //printf("flag is %c\n",flag[0]);//
            char gid[10];
            memset(gid,0,sizeof(gid));
            if(get_arg(recv_buf,gid,sizeof(gid))<0){
                my_err("read",__LINE__);
            }
            if(flag[0]=='1'){
                P_LOCK;
                printf("\t\t\t\t\t您加入群%s的申请已通过\n",gid);
                P_UNLOCK;
                break;
            }
            else if(flag[0]=='2'){
                P_LOCK;
                printf("\t\t\t\t\t您加入群%s的申请未通过\n",gid);
                P_UNLOCK;
                break;
            }
        }
        case EXITGROUP:{
            char flag[2];
            memset(flag,0,sizeof(flag));
            if(get_arg(recv_buf,flag,sizeof(flag))<0){
                my_err("read",__LINE__);
            }
            //printf("flag is %c\n",flag[0]);//
            if(flag[0]=='1'){
                P_LOCK;
                printf("\t\t\t\t\t您已成功退出该群\n");
                P_UNLOCK;
                break;
            }
            else if(flag[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t您不在该群中\n");
                P_UNLOCK;
                break;
            }
        }
        case SETSTATE:{
            char flag[2];
            memset(flag,0,sizeof(flag));
            if(get_arg(recv_buf,flag,sizeof(flag))<0){
                my_err("read",__LINE__);
            }
            //printf("flag is %c\n",flag[0]);//
            if(flag[0]=='1'){
                P_LOCK;
                printf("\t\t\t\t\t您的权限不够\n");
                P_UNLOCK;
                break;
            }
            else if(flag[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t您不在该群中\n");
                P_UNLOCK;
                break;
            }
            else if(flag[0]=='2'){
                P_LOCK;
                printf("\t\t\t\t\t此成员不在该群中\n");
                P_UNLOCK;
                break;
            }
            else if(flag[0]=='3'){
                P_LOCK;
                printf("\t\t\t\t\t此成员已是管理员\n");
                P_UNLOCK;
                break;
            }
            char gid[10];
            memset(gid,0,sizeof(gid));
            if(get_arg(recv_buf,gid,sizeof(gid))<0){
                my_err("read",__LINE__);
            }
            if(flag[0]=='4'){
                P_LOCK;
                printf("\t\t\t\t\t您已成为群%s的管理员\n",gid);
                P_UNLOCK;
            }
        }
        case DELMEMBER:{
            char flag[2];
            memset(flag,0,sizeof(flag));
            if(get_arg(recv_buf,flag,sizeof(flag))<0){
                my_err("read",__LINE__);
            }
            //printf("flag is %c\n",flag[0]);//
            if(flag[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t此群不存在\n");
                P_UNLOCK;
                break;
            }
            else if(flag[0]=='1'){
                P_LOCK;
                printf("\t\t\t\t\t您不是管理人员\n");
                P_UNLOCK;
                break;
            }
            else if(flag[0]=='2'){
                P_LOCK;
                printf("\t\t\t\t\t该用户不在此群中\n");
                P_UNLOCK;
                break;
            }
            else if(flag[0]=='3'){
                P_LOCK;
                printf("\t\t\t\t\t您的权限不够\n");
                P_UNLOCK;
                break;
            }
            char gid[10];
            memset(gid,0,sizeof(gid));
            if(get_arg(recv_buf,gid,sizeof(gid))<0){
                my_err("read",__LINE__);
            }
            if(flag[0]=='4'){
                P_LOCK;
                printf("\t\t\t\t\t您已成功将其踢出群%s\n",gid);
                P_UNLOCK;
            }
            else if(flag[0]=='5'){
                P_LOCK;
                printf("\t\t\t\t\t您已被管理员踢出群%s\n",gid);
                P_UNLOCK;
            }
            break;
        }
        case DELGROUP:{
            char flag[2];
            memset(flag,0,sizeof(flag));
            if(get_arg(recv_buf,flag,sizeof(flag))<0){
                my_err("read",__LINE__);
            }
            //printf("flag is %c\n",flag[0]);//
            if(flag[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t此群不存在\n");
                P_UNLOCK;
                break;
            }
            else if(flag[0]=='1'){
                P_LOCK;
                printf("\t\t\t\t\t您不是群主\n");
                P_UNLOCK;
                break;
            }
            else if(flag[0]=='2'){
                P_LOCK;
                printf("\t\t\t\t\t您已成功解散该群\n");
                P_UNLOCK;
                break;
            }
        }
        case GROUPMEMBER:{
            char mid[10];
            memset(mid,0,sizeof(mid));
            if(get_arg(recv_buf,mid,sizeof(mid))<0){
                my_err("read",__LINE__);
            }
            if(mid[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t此群不存在或您未加入此群\n");
                P_UNLOCK;
                break;
            }
            char name[32];
            memset(name,0,sizeof(name));
            if(get_arg(recv_buf,name,sizeof(name))<0){
                my_err("read",__LINE__);
            }
            char state[2];
            memset(state,0,sizeof(state));
            if(get_arg(recv_buf,state,sizeof(state))<0){
                my_err("read",__LINE__);
            }
            P_LOCK;
            printf("\t\t\t\t\tid:%s  name:%s  state:%s\n",mid,name,state);
            P_UNLOCK;
            break;
        }
        case FCHATMSG:{
            char send_id[10];
            memset(send_id,0,sizeof(send_id));
            if(get_arg(recv_buf,send_id,sizeof(send_id))<0){
                my_err("read",__LINE__);
            }
            if(send_id[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t好友不存在\n");
                P_UNLOCK;
                break;
            }
            char recv_id[10];
            memset(recv_id,0,sizeof(recv_id));
            if(get_arg(recv_buf,recv_id,sizeof(recv_id))<0){
                my_err("read",__LINE__);
            }
            char msg[500];
            memset(msg,0,sizeof(msg));
            if(get_arg(recv_buf,msg,sizeof(msg))<0){
                my_err("read",__LINE__);
            }
            P_LOCK;
            printf("\t\t\t\t\t%s--->%s:%s\n",send_id,recv_id,msg);
            P_UNLOCK;
            break;
        }
        case GCHATMSG:{
            char send_id[10];
            memset(send_id,0,sizeof(send_id));
            if(get_arg(recv_buf,send_id,sizeof(send_id))<0){
                my_err("read",__LINE__);
            }
            if(send_id[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t此群不存在或您未加入此群\n");
                P_UNLOCK;
                break;
            }
            char msg[500];
            memset(msg,0,sizeof(msg));
            if(get_arg(recv_buf,msg,sizeof(msg))<0){
                my_err("read",__LINE__);
            }
            P_LOCK;
            printf("\t\t\t\t\t%s说:%s\n",send_id,msg);
            P_UNLOCK;
            break;
        }
        case SENDFILE:{
            char send_id[10];
            memset(send_id,0,sizeof(send_id));
            if(get_arg(recv_buf,send_id,sizeof(send_id))<0){
                my_err("read",__LINE__);
            }
            char filename[256];
            memset(filename,0,sizeof(filename));
            if(get_arg(recv_buf,filename,sizeof(filename))<0){
                my_err("read",__LINE__);
            }
            P_LOCK;
            printf("\t\t\t\t\t%s向您发送了文件%s\n",send_id,filename);
            P_UNLOCK;
            break;
        }
        case RECVFILE:{
            //printf("recv_buf is %s",recv_buf);
            char flag[2];
            memset(flag,0,sizeof(flag));
            if(get_arg(recv_buf,flag,sizeof(flag))<0){
                my_err("read",__LINE__);
            }
            //printf("flag is %c\n",flag[0]);//
            if(flag[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t对方未向您发送文件\n");
                P_UNLOCK;
                break;
            }
            if(flag[0]=='1'){
                file_flag=1;
                C_SIGNAL;
                break;
            }
            /*
            //printf("why??");
            char filename[256];
            memset(filename,0,sizeof(filename));
            if(get_arg(recv_buf,filename,sizeof(filename))<0){
                my_err("read",__LINE__);
            }
            char buffer[256];
            memset(buffer,0,sizeof(buffer));
            sprintf(buffer,"%s",recv_buf+read_len);
            if(get_arg(recv_buf,buffer,sizeof(buffer))<0){
                my_err("read",__LINE__);
            }
            //printf("buffer is %s",buffer);
            FILE *fp=fopen(filename,"a");
            if(fwrite(buffer,sizeof(char),atoi(len)-3-strlen(filename),fp)<atoi(len)-3-strlen(filename))
            {
                printf("File:\t%s Write Failed\n", filename);
            }
            fclose(fp);
            break;*/
        }
        case START:{
            /*char flag[2];
            memset(flag,0,sizeof(flag));
            if(get_arg(recv_buf,flag,sizeof(flag))<0){
                my_err("read",__LINE__);
            }
            //printf("flag is %c\n",flag[0]);//
            if(flag[0]=='0'){
                P_LOCK;
                printf("\t\t\t\t\t红色感叹号！\n");
                P_UNLOCK;*/
            //printf("signal coming\n");    
            C_SIGNAL;
            
            break;
            /*}
            else if(flag[0]=='1'){
                chat_flag=1;
                C_SIGNAL;
                break;
            }*/
        }
        }
    }
    printf("msgbox is over\n");
}
