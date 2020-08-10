/*************************************************************************
	> File Name: print_file.c
	> Author: 
	> Mail: 
	> Created Time: 2020年08月06日 星期四 20时31分45秒
 ************************************************************************/

#include"client.h"
int print_file()
{
    int chose=0;
    while(1){
        P_LOCK;
        printf("1.发送文件\n");
        printf("2.接收文件\n");
        printf("0.返回\n");
        printf("请输入您的选择\n");
        P_UNLOCK;
        scanf("%d",&chose);
        switch(chose){
            case 1:
            send_file();
            break;
            case 2:
            recv_file();
            break;
            case 0:
            return 0;
            break;
        }
    }
}
