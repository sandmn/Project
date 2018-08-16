#pragma once

#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<string>
#include<map>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"data_pool.h"
#define MAX 1024

//该文件中只写类的声明和成员函数
class udp_server
{
    public:
        //构造函数
        udp_server(int _port);
        //初始化函数:用于创建文件描述符和绑定端口号
        int Init();
        //从客户端接收数据，然后放入数据池中
        void recv_from_client();
        //（从数据池中取数据）广播发送
        void boardcase_to_client();
        //添加用户至在线用户列表中
        void AddUser(struct sockaddr_in& c);
        //从在线用户列表中删除用户
        void DelUser(struct sockaddr_in& c);
        //析构函数:关闭文件描述符
        ~udp_server();


    private:
        int sock;//服务器端用于接收信息的文件描述符
        int port;//服务器端绑定的端口号
        std::map<int,struct sockaddr_in> online;//定义一个客户端的在线用户列表：以整型IP地址作为key，套接字序列作为value
        data_pool pool;//定义一个数据池
};

