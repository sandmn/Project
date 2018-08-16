#pragma once

#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<string>
#include<vector>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define MAX 1024

//定义一个用于客户端服务的类
class udp_client
{
    public:
        //构造函数，与外部的接口
        udp_client(const std::string& ip,int port);
        //初始化函数：创建一个文件描述符
        void Init();
        //客户端向服务器发送数据:此时的数据已经是序列化之后的数据（即字符流）
        void sendData_to_server(const std::string& s);
        //客户端从服务器接收数据：此时接受到的数据为字符流
        void recvData_from_server(std::string& s);
        //添加用户
        void AddUser(const std::string& s);
        //删除用户
        void DelUser(const std::string& s);
        //维护一个用户在线列表用于在窗口上显示
        //因为在类的外部要通过该在线用户列表将用户显示在窗口上，所以需要将其定义为公有，以便在外部访问
        std::vector<std::string> online;
        //析构函数
        ~udp_client();
    private:
        //客户端只需要根据服务器的ip和端口号创建一个文件描述符即可收发信息
        int sock;
        struct sockaddr_in server;
};
