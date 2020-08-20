/*************************************************************************
	> File Name: print_main.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月28日 星期二 16时39分46秒
 ************************************************************************/

#include"client.h"
int print_main()
{
    int chose;
    system("clear");
    while(1){
        chose=-1;
        printf("\n\t主界面\n");
        printf("请选择您所要执行的操作\n");
        printf("\t1.登录\n");
        printf("\t2.注册\n");
        printf("\t3.找回密码\n");
        printf("\t0.爪巴\n");
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
            login();
            break;
            case 2:
            uregister();
            break;
            case 3:
            findpsw();
            break;
            case 0:
            printf("再见\n");
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

