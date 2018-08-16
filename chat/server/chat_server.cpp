

#include<pthread.h>
#include<stdio.h>
#include"udp_server.h"


void* recvData_from_client(void* arg)
{
    udp_server* server = (udp_server*)arg;
    //该线程用于循环的从客户端接收数据
    while(1)
    {
        server->recv_from_client();
    }
}
void* sendData_to_client(void* arg)
{
    udp_server* server = (udp_server*)arg;
    //该线程用于循环的从数据池获得数据然后广播发送给各个客户端
    while(1)
    {
        server->boardcase_to_client();
    }
}

int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        printf("Usage:%s [port]\n",argv[1]);
        exit(1);
    }

    //主线程定义一个类udp_server的对象
    udp_server server(atoi(argv[1]));//将端口传入构造函数中
    //通过对象调用类的初始化函数用于生成一个套接字，并将其进行绑定
    server.Init();

    //然后主线程创建两个新线程，
    //一个用于从客户端循环的接收数据放入数据池中
    //一个用于从数据池中获取数据广播发送给各个客户端
    pthread_t precv;//从客户端获取数据的线程
    pthread_t psend;//向客户端发送数据的线程

    pthread_create(&precv,NULL,recvData_from_client,(void*)&server);
    pthread_create(&psend,NULL,sendData_to_client,(void*)&server);

    //然后主线程来等待两个新线程
    pthread_join(precv,NULL);
    pthread_join(psend,NULL);
    //int pthread_join(pthread_t thread, void **retval);
    return 0;
}
