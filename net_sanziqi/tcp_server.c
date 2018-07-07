#include "comm.h"

//获取监听套接字
int server_sock(char* ip,int port)
{
    //打开网卡文件，获取文件描述符
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0)
    {
        printf("socker error\n");
        exit(1);
    }

    int opt = 1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(port);

    //绑定套接字
    socklen_t len = sizeof(server);
    if(bind(sock,(struct sockaddr*)&server,len) < 0)
    {
        printf("bind error\n");
        exit(2);
    }

    //监听套接字
    if(listen(sock,10) < 0)
    {
        printf("listen error\n");
        exit(3);
    }

    return sock;//得到监听套接字
}


//服务器端进入游戏模式
void server_work(int client_sock,char* ip,int port)
{
    int ret = 0;
    //在服务器端定义一个数组
    char arr[ROW][COL] = { 0 };
    Init(arr, ROW, COL);//初始化数组
    //根据数组打印棋盘
    print(arr,ROW,COL);
    while (1)
    {
        //客户端先走，服务器端进行等待
        printf("please wait...\n");
        //client_loc保存客户端发送来的坐标信息
        location client_loc;
        //服务器从网络中读取客户端发送来的坐标信息
        read(client_sock,&client_loc,sizeof(client_loc));//客户端先走
        //服务器根据客户端发送来的坐标信息将数组对应位置添加相应的标记，表示客户端在此落棋
        arr[client_loc.x - 1][client_loc.y - 1] ='*';
        print(arr,ROW,COL);//打印棋盘
        ret = iswin(arr,ROW,COL);//判断是否有人胜利
        //判断是谁赢
        int r = who_win(ret); 
        if(r == 1)
        {
            //有人赢
            break;
        }

        //loc保存服务器移动的坐标信息
        location loc;
        printf("玩家1走，玩家1请选择：");        
        move(arr,ROW,COL,&loc,'Y');//服务器端移动
        print(arr, ROW, COL);//打印棋盘
        //将服务器端移动的坐标发送到客户端
        write(client_sock,&loc,sizeof(loc));
        ret = iswin(arr,ROW,COL);//判断是否有人胜利
        //判断是谁胜利
        r = who_win(ret); 
        if(r == 1)
        {
            //有人赢
            break;
        }
    } 
}

//服务器创建子进程，子进程创建孙子进程与客户端进行下棋
void process_work(int client_sock,char* ip_buf,int port)
{
    pid_t pid = fork();
    if(pid < 0)
    {
        printf("fork error\n");
        return;
    }
    else if(pid == 0)//child
    {
        pid_t pid1 = fork();
        if(pid1 < 0)
        {
            printf("fork error\n");
            return;
        }
        else if(pid1 == 0)//grandchild
        {
            while(1)
            {
                //res保存客户端对菜单的选择信息
                Response res;
                //服务器从网络中读取客户端发来的选择
                read(client_sock,&res,sizeof(res));
                //如果选择退出游戏，则退出
                if(res.res == 0)
                {
                    break;
                }
                //否则进入游戏，开始与客户端进行下棋
                printf("开始游戏\n");
                server_work(client_sock,ip_buf,port);
            }
            printf("client quit\n");
            exit(0);
        }
        //子进程退出，孙子进程变成孤儿进程，被1号进程领养，
        //此时父进程立即将子进程回收，接着处理来自其它客户端的连接，而不用阻塞等待回收孙子进程
        exit(0);
    }
    else//father
    {
        //父进程等待子进程
        waitpid(pid,NULL,0);
    }
}
int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        printf("Usage:%s [ip][port]\n",argv[0]);
        return 1;
    }
    
    int listen_sock = server_sock(argv[1],atoi(argv[2]));//得到监听套接字
    printf("bind and listen success,wait accept...\n");


    struct sockaddr_in client;

    while(1)
    {
        socklen_t len = sizeof(client);
        //接收来自客户端的连接请求
        int client_sock = accept(listen_sock,(struct sockaddr*)&client,&len);
        if(client_sock < 0)
        {
            printf("accept error\n");
            continue;
        }

        //将来自网络的ip地址转化为字符串
        //将来自网络的端口号转化为主机格式
        char ip_buf[24];
        ip_buf[0] = 0;
        inet_ntop(AF_INET,&client.sin_addr,ip_buf,sizeof(ip_buf));
        int port = ntohs(client.sin_port);
        printf("connect success, ip:[%s],port:[%d]\n",ip_buf,port);

        process_work(client_sock,ip_buf,port);
    }

    return 0;
}
