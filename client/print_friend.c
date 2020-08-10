/*************************************************************************
	> File Name: print_friend.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月30日 星期四 10时47分18秒
 ************************************************************************/

#include"client.h"
int print_friend()
{
    while(1){
        P_LOCK;
        printf("1.查看好友列表\n");
        printf("2.添加好友\n");
        printf("3.删除好友\n");
        printf("4.查找好友\n");
        printf("5.拉黑好友\n");
        printf("6.聊天\n");
        printf("7.查看好友消息\n");
        printf("8.查看好友聊天记录\n");
        printf("0.返回\n");
        P_UNLOCK;
        int chose;
        //S_LOCK;
        scanf("%d",&chose);
        //S_UNLOCK;
        switch(chose){
            case 1:
            friendlist();
            break;
            case 2:
            addfriend();
            break;
            case 3:
            delfriend();
            break;
            case 4:
            findfriend();
            break;
            case 5:
            blockfriend();
            break;
            case 6:
            fchat();
            break;
            case 7:
            printnode();
            break;
            case 8:
            fchatmsg();
            break;
            case 0:
            return 0;
            break;
        }
    }
}
