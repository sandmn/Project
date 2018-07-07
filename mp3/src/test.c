#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

//初始化双向链表
void list_init()
{
    //动态申请头节点
    head = malloc(sizeof(node_t));

    memset(head,0x00,sizeof(node_t));
    //头节点的两个指针分别指向自己
    head->next = head->prev = head;
}

//对链表进行插入
void list_insert(const char* name)
{
    //动态申请存放歌曲的节点
    node_t* p = malloc(sizeof(node_t));
    memset(p,0x00,sizeof(node_t));
    //为歌名动态申请空间进行存放
    p->name = malloc(strlen(name) + 1);
    //将歌名存放在链表的节点中
    strcpy(p->name,name);
   // 将动态申请的节点进行头插，此时需要改变四个指针的指向
    p->next = head->next;
    head->next->prev = p;
    p->prev = head;
    head->next = p;
}

//打印链表
void list_show()
{
    //从链表的实际第一个节点开始遍历打印
    node_t* p = head->next;

    //当链表没有遍历结束时，一直往后遍历，进行打印
    while(p != head)
    {
        printf("%s",p->name);
        //显示当前歌曲的位置
        if(p == cur)
        {
            printf("<=====cur");
        }
        printf("\n");
        p = p->next;
    }
}

//将可取从文件中提取出来加载到链表中
//参数为歌曲所在的路径
void load_music(const char* path)
{
    //打开歌曲所在目录
    DIR* pdir = opendir(path);
    //打开失败
    if(pdir == NULL)
    {
        perror("opendir");
        exit(1);
    }
    //该结构体用于保存歌曲文件中的一条记录
    struct dirent* p = NULL;
    //对每个歌曲文件进行循环的提取
    while((p = readdir(pdir))!= NULL)
    {
        //去除隐藏文件
        if(p->d_name[0] == '.')
        {
            continue;
        }
        //将提取的歌曲插入到双向链表中
        list_insert(p->d_name);
    }
    //将所有歌曲提取完毕之后，关闭文件
    closedir(pdir);
}

//打印菜单
int menu()
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

//播放函数
void playPause()
{
    //如果是第一次播放，则播放第一首歌曲，此时cur指向第一首歌曲
    if(first == 1)
    {
        char buf[1024] = {};
        //将标准错误重定向到/dev/null，有问题
        sprintf(buf,"madplay -o wav:- /home/admin/music/%s  2> /dev/null | aplay  2> /dev/null&",cur->name);
        //指向buf
        system(buf);
        //执行之后，就不是第一次播放了，此时first不为1
        first = 0;
        //此时的状态为播放状态
        status = PLAY;
    }
    else//如果不是第一次播放
    {
        //如果当前是播放状态，则使歌曲暂停
        if(status == PLAY)
        {
            system("killall -SIGSTOP aplay");
            status = PAUSE;
        }
        //如果是暂停状态，则使歌曲开始播放
        else if(status == PAUSE)
        {
            system("killall -SIGCONT aplay");
            status = PLAY;
        }
    }
}

//停止播放，下一次从头开始播放
//杀死当前播放歌曲的进程
void stop()
{
    system("killall -SIGKILL aplay");
    first = 1;
}

//播放下一首歌曲
void next()
{
    stop();//先将当前播放歌曲的进程杀死
    cur = cur->next;//使当前歌曲的指针指向下一首歌曲
    //如果下一首歌曲是头节点，即上一首歌曲是最后一首歌曲，使下一首歌曲指向实际的第一首歌曲
    if(cur == head)
    {
        cur = cur->next;
    }
    //开始播放歌曲
    playPause();
}

//播放上一首歌曲
void prev()
{
    stop();//将当前播放歌曲的进程杀死
    cur = cur->prev;//使当前歌曲的指针指向上一首歌曲
    //如果上一首歌曲指针指向了头节点
    //即当前歌曲为实际的第一首歌曲
    //则使上一首歌曲为最后一首歌曲
    if(cur == head)
    {
        cur = cur->prev;
    }
    //开始播放歌曲
    playPause();
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
