
#include"udp_client.h"
//构造函数，与外部的接口
udp_client::udp_client(const std::string& ip,int port)
{
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_port = htons(port);
}
//初始化函数：创建一个文件描述符
void udp_client::Init()
{
    sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock < 0)
    {
        std::cerr<<"socket error"<<std::endl;
        exit(0);
    }
    return;
}
//客户端向服务器发送数据:此时的数据已经是序列化之后的数据（即字符流）
void udp_client::sendData_to_server(const std::string& s)
{
    //           ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
    //                                      const struct sockaddr *dest_addr, socklen_t addrlen);
    ssize_t ss = sendto(sock,s.c_str(),s.size(),0,(struct sockaddr*)&server,sizeof(server));
}
//客户端从服务器接收数据：此时接受到的数据为字符流
void udp_client::recvData_from_server(std::string& s)
{
    //  ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
    //                                struct sockaddr *src_addr, socklen_t *addrlen);
    char buf[MAX];
    ssize_t ss = recvfrom(sock,buf,sizeof(buf) - 1,0,NULL,NULL);
    if(ss > 0)
    {
        buf[ss] = 0;
        s = buf;
    }
}
//添加用户
void udp_client::AddUser(const std::string& s)
{
    //使用迭代器遍历用户在线列表
    std::vector<std::string>::iterator it = online.begin();
    while(it != online.end())
    {
        if(*it == s)
        {
            return;//此时该用户已经在列表中，不用在添加进去
        }
        it++;
    }
    //否则该用户不在用户在线列表中，将该用户添加进去
    online.push_back(s);
    return;
}
//删除用户
void udp_client::DelUser(const std::string& s)
{
    //利用迭代器遍历在线用户列表，查找要删除的用户
    std::vector<std::string>::iterator it = online.begin();
    while(it != online.end())
    {
        if(*it == s)//找到要删除的用户了
        {
            it = online.erase(it);//删除该用户
            break;
        }
        it++;
    }
    return;
}
//析构函数：关闭文件描述符
udp_client::~udp_client()
{
    if(sock > 0)
    {
        close(sock);
        sock= -1;
    }
}
