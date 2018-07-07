
#include"comm.h"

//客户端进入游戏模式
void client_work(int client_sock)
{
    int ret = 0;
    char arr[ROW][COL] = { 0 };
    Init(arr, ROW, COL);//初始化数组
    print(arr, ROW, COL);//首先打印九宫格
    while (1)
    {
        //loc保存客户端发送的位置信息
        location loc;
        printf("客户端走，客户端请选择：");
        //将客户端选择的坐标保存在结构体变量loc中
        move(arr,ROW,COL,&loc,'*');
        //将坐标信息发送给服务器端
        write(client_sock,&loc,sizeof(loc));
        //选择坐标进行移动之后打印棋盘
        print(arr, ROW, COL);
        //判断是否有人赢
        ret = iswin(arr, ROW, COL);
        int r = who_win(ret);//判断是谁赢
        if(r == 1)
        {
            //有人赢
            break;
        }
        //客户端移动之后，没有人赢，则进入等待，
        //接收来自服务器端的坐标信息
        printf("wait ...\n");

        //client_loc用于保存来自服务器端发送的坐标信息
        location client_loc;
        //从网络中读取坐标信息
        ssize_t s = read(client_sock,&client_loc,sizeof(client_loc));

        //在客户端的数组中将接收到的坐标处改变为对应的字符表示服务器在此处落棋
        arr[client_loc.x - 1][client_loc.y -1] ='Y';

        //打印棋盘
        print(arr,ROW,COL);
        //判断是否有人赢
        ret = iswin(arr,ROW,COL);
        r = who_win(ret);//判断是谁赢
        if(r == 1)
        {
            //有人赢
            break;
        }
    } 
}
int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        printf("Usage:%s [ip][port]\n",argv[0]);
        return 1;
    }
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0)
    {
        printf("socket error\n");
        return 2;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(atoi(argv[2]));
    socklen_t len = sizeof(server);

    //客户端向服务器发起连接请求
    if(connect(sock,(struct sockaddr*)&server,len) < 0)
    {

        printf("connect failed\n");
        return 3;
    }

    printf("connect success\n");


    int input = 0;
    //客户端对菜单的选择
    Response res;
    do
    {
        //打印菜单
        menu();
        printf("请选择：");
        scanf("%d", &input);
        switch (input)
        {
            case 1:
                printf("开始游戏\n");
                res.res = input;
                //将客户端的选择发送给服务器
                write(sock,&res,sizeof(res));
                //如果选择开始游戏，则客户端进入游戏模式
                client_work(sock);
                printf("\n");
                break;
            case 0:
                res.res = input;
                //将客户端的选择发送给服务器
                write(sock,&res,sizeof(res));
                //如果选择退出游戏，则客户端退出游戏
                printf("退出游戏\n");
                break;
            default:
                printf("选择错误，请重新选择！\n");
                printf("\n");
                break;
        }

    } while (input);
    close(sock);
    return 0;
}
