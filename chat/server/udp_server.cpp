
#include"udp_server.h"
#include"data.h"

//类udp_server中的函数实现

//构造函数的实现：接收外部的端口号 
udp_server::udp_server(int _port)
    :port(_port)
{}

//初始化函数的实现：创建文件描述符并绑定端口号
//因为是基于UDP协议实现的，所以不用进行监听
int udp_server::Init()
{
    //int socket(int domain, int type, int protocol);
    //创建文件描述符
    sock = socket(AF_INET,SOCK_DGRAM,0);
    //setsockopt
    if(sock < 0)
    {
        std::cerr<<"socket error"<<std::endl;
        return -1;
    }

    //绑定端口号
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(port);

    //int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
    {
        std::cerr<<"bind error"<<std::endl;
        return -2;
    }
    return 0;
}


//从客户端接收数据，然后放入数据池中
void udp_server::recv_from_client()
{
    data d;//定义一个用于序列化的变量
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    char buf[MAX];
    //ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,struct sockaddr *src_addr, socklen_t *addrlen);
    ssize_t s = recvfrom(sock,buf,sizeof(buf) - 1,0,(struct sockaddr*)&client,&len);
    if(s > 0)
    {
        buf[s] = '\0';
        //有问题
        std:: string instring(buf);//instring为一字符串，调用序列化函数将该字符串转化为中间类型变量data
        //将接受到的字符流进行反序列化
        d.deserilize(instring);
        //如果客户端退出，将该客户端从在线用户列表中删除
        if(d.cmd == "QUIT")
        {
            DelUser(client);
        }
        else
        {
            //否则将客户端添加进在线用户列表中，因为在线用户列表是基于map的
            //在插入相同ip的用户时，就会插入失败
            AddUser(client);
        }
        //将从客户端收到的数据添加到数据池中
        pool.putData(instring);
    }
}
//（从数据池中取数据）广播发送
void udp_server::boardcase_to_client()
{
    std::string str;
    //从数据池中获取数据
    pool.getData(str);
    //将获取的数据发送给在线用户列表中的每个用户
    //当一个客户端退出后，该客户端再次连接时，如果不删除之前的记录
    //会默认发给之前用户保存的套接字信息，但是之前的用户已经退出，此时就会导致发送不到新的客户端
    //可能端口号不同
    //所以，当一个客户端退出后，要清除其在服务器端的记录，再次连接时就会添加成功
    std::map<int,struct sockaddr_in>::iterator it = online.begin();
    while(it != online.end())
    {
        sendto(sock,str.c_str(),str.size(),0,(struct sockaddr*)&(it->second),sizeof(it->second));
        it++;
    }
}
//添加用户至在线用户列表中
void udp_server::AddUser(struct sockaddr_in& c)
{
    //map在插入数据时，如果key相等，就不会重复添加了
    online.insert(std::pair<int,struct sockaddr_in>(c.sin_addr.s_addr,c));
}
//从在线用户列表中删除用户
void udp_server::DelUser(struct sockaddr_in& c)
{
    online.erase(c.sin_addr.s_addr);
}

//析构函数:关闭文件描述符
udp_server::~udp_server()
{
    if(sock > 0)
    {
        close(sock);
        sock = -1;
    }
}
