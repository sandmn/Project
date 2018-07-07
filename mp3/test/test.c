#include"list.h"

//打印菜单
static int menu()
{
    printf("*************************\n");
    printf("1.play/pause\n");
    printf("2.next\n");
    printf("3.prev\n");
    printf("4.stop\n");//
    printf("0.exit\n");
    printf("*************************\n");
    //显示歌曲名
    list_show();
    //定义选择变量
    int choose = 5;
    do
    {
        printf(">");
        //scanf只会读取缓冲区中的内容，而不会刷新
        scanf("%d",&choose);
        if(choose >= 0 && choose <= 4)
        {
            break;
        }
        //如果输入不在0~4范围内，则选择非法，重新进行选择
        printf("choose invalid\n");
        //while(getchar() != '\n');//有问题
    }while(1);
    return choose;
}

int main(int argc,char* argv[])
{
    //初始化双向链表
    list_init();
    //加载歌曲，将歌曲插入到双向链表中
    load_music("/home/admin/music");
    //如果双向链表不为空，则当前歌曲的指针指向实际的第一首歌曲
    if(head->next != head)
    {
        cur = head->next;
    }
    //打印菜单进行循环输入
    do
    {
        int choose = menu();
        switch(choose)
        {
            case 1:
                playPause();
                break;
            case 2:
                next();
                break;
            case 3:
                prev();
                break;
            case 4:
                stop();
                break;
            case 0:
                printf("谢谢观赏\n");
                system("killall -SIGKILL aplay");
                exit(0);
                break;
            default:
                break;
        }
    }while(1);

    return 0;
}
