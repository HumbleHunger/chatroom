/*************************************************************************
	> File Name: print_meau.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月30日 星期四 10时37分19秒
 ************************************************************************/

#include"client.h"
int print_meau()
{
    int chose;
    while(1){
        chose=-1;
        P_LOCK;
        printf("\n主菜单\n");
        printf("1.好友管理\n");
        printf("2.群管理\n");
        printf("3.文件传输\n");
        printf("0.注销\n");
        printf("请输入您的选择\n");
        P_UNLOCK;
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
            print_friend();
            break;
            case 2:
            print_group();
            break;
            case 3:
            print_file();
            break;
            case 0:{
                char data[11];
                sprintf(data,"%s\n",user_id);
                send_pack(connfd,OVER,strlen(data),data);
                delgnode();
                delfrnode();
                delgmsgnode();
                delnode();
                return 0;
                break;
            }
            default:
            P_LOCK;
            printf("\t\t\t\t\t请勿非法输入!\n");
            P_UNLOCK;
            while((getchar())!='\n');
            break;
        }
    }
}
