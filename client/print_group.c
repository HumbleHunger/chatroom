/*************************************************************************
	> File Name: print_group.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月03日 星期一 21时01分35秒
 ************************************************************************/

#include"client.h"
int print_group()
{
    int chose;
    while(1){ 
        chose=-1;
        P_LOCK;
        printf("\n群管理\n");
        printf("1.查看群列表\n");
        printf("2.查看群成员\n");
        printf("3.创建群\n");
        printf("4.添加群\n");
        printf("5.退出群\n");
        printf("6.设置管理员\n");
        printf("7.踢人\n");
        printf("8.解散群\n");
        printf("9.群聊\n");
        printf("10.处理群请求\n");
        printf("11.查看群消息\n");
        printf("12.查看群消息记录\n");
        printf("0.返回\n");
        P_UNLOCK;
        int chose;
        scanf("%d",&chose);
        if(getchar()!='\n'){
            P_LOCK;
            printf("\t\t\t\t\t请勿非法输入!\n");
            P_UNLOCK;
            while((getchar())!='\n');
            continue;
        }
        switch(chose){
            case 1:
            grouplist();
            break;
            case 2:
            groupmember();
            break;
            case 3:
            creategroup();
            break;
            case 4:
            addgroup();
            break;
            case 5:
            exitgroup();
            break;
            case 6:
            gsetstate();
            break;
            case 7:
            delmember();
            break;
            case 8:
            delgroup();
            break;
            case 9:
            gchat();
            break;
            case 10:
            dealgroup();
            break;
            case 11:
            printgnode();
            break;
            case 12:
            gchatmsg();
            break;
            case 0:
            return 0;
            break;
            default:
            P_LOCK;
            printf("\t\t\t\t\t请勿非法输入!\n");
            P_UNLOCK;
            while((getchar())!='\n');
            break;
        }
    }
}

