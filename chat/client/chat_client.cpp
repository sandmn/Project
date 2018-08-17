
#include<stdio.h>
#include<pthread.h>
#include<signal.h>
#include"udp_client.h"
#include"window.h"
#include"data.h"

//将窗口对象和客户端的服务对象集成在一个结构体中
typedef struct client
{
    window* wp;
    udp_client* ucp;
}client;

//创建一个包含窗口类和网络数据收发的类的结构体对象
//定义为全局变量，这样在信号捕捉函数和各线程处理函数中均可以使用
client for_cli;

//定义一个包含用户姓名和学校的结构体
struct
{
    std::string nickname;
    std::string school;
}usr_msg;//定义一个全局变量,这样在信号捕捉函数和各线程处理函数中均可以使用

void* handler_title(void* arg)
{
    //在标题线程中，该线程只用创建一个窗口，然后在窗口中显示字符串即可
    window* win = ((client*)arg)->wp;
    win->show_title();
    //然后在在窗口中构建移动的标题字符串
    win->MoveTitle();
}
//如果客户端ctl+c退出，若不对该信号进行捕捉
//在下一次该客户端再次连接服务器时，就会因为服务器端没有删除该用户，而连接不上
//此时，客户端还会在次收到该用户发送的消息，但是不能发送给客户端
void handler_SIGINT(int num)
{
    udp_client* uc = for_cli.ucp;
    //主线程处理SIGINT的捕捉
    std::string sendstring;
    data d;
    d.nickname = usr_msg.nickname;
    d.school = usr_msg.school;
    d.msg = "";
    d.cmd = "QUIT";
    d.serilize(sendstring);
    uc->sendData_to_server(sendstring);   
    delete for_cli.wp;
    exit(1);
}

//该线程用于循环的从输入窗口中获取消息
void* handler_input(void* arg)
{
    window* win = ((client*)arg)->wp;
    udp_client* uc = ((client*)arg)->ucp;
    //该线程首先需要创建一个用于输入的窗口
    win->show_input();
    //然后在输入窗口中获取字符串
    std::string s;
    std::string sendstring;
    while(1)
    {
        win->inputgetstr(s);
        //获取到要发送的消息后将昵称，学校，消息，命令进行序列化
        data d;
        d.nickname = usr_msg.nickname;
        d.school = usr_msg.school;
        d.msg = s;
        d.cmd = "None";
        d.serilize(sendstring);
        //将序列化之后的字符串通过udp_client的对象将其发送到网络中
        uc->sendData_to_server(sendstring);
    }
}

//该线程用于循环的从服务器端接受信息
void* handler_output(void* arg)
{
    window* win = ( (client*)arg )->wp;
    udp_client* uc = ( (client*)arg )->ucp;
    //构造一个输出框和列表框
    win->show_output();
    win->show_list();
    //在输出线程：
    //首先要通过udp_client类对象从网络中获取到服务器发送的消息
    std::string recvstring;
    int i = 1;
    while(1)
    {
        uc->recvData_from_server(recvstring);
        //对收到的信息进行反序列化
        data d;
        d.deserilize(recvstring);
        std::string s(d.nickname);
        s = s + "-";
        s = s + d.school;
        int y,x;
        //然后对该消息进行解析，如果该消息是某个客户端退出的命令，则将该客户端从在线用户列表中删除
        //然后进行在线用户显示
        if(d.cmd == "QUIT")
        {
            uc->DelUser(s);
        }
        //否则将该用户添加进在线用户列表中（在添加函数中会判断该用户是否已经存在）
        //然后进行在线用户显示
        else
        {
            uc->AddUser(s);
            //然后将收到的消息中学校和昵称以及消息显示在输出框中
            s = s + "#";
            s = s + d.msg;
            //计算输出框的长度和宽度
            getmaxyx(win->output,y,x);
            //然后将消息显示在输出框中
            win->putstr_to_win(win->output,i++,2,s);
            //如果消息占满屏幕了，则进行刷新
            if(i >= y - 1)
            {
                i = 1;
                sleep(1);
                win->clearwin(win->output,1,y - 1);
                }
            }
        //最后重新显示在线用户列表
        //先将之前的列表进行清空
        win->clearwin(win->userlist,1,y);

        //根据online将用户显示在框中
        int i = 0;
        int j = 1;
        for(;i < uc->online.size();i++)
        {
            win->putstr_to_win(win->userlist,j,1,uc->online[i]);
        }
    }
}


int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        printf("Usage:%s [ip] [port]\n",argv[0]);
        return -1;
    }
    //创建一个udp_client类型的对象来创建套接字文件进行数据的收发
    udp_client netdata(argv[1],atoi(argv[2]));
    netdata.Init();

    //然后提醒用户进行输入
    //用户输入的数据保存在全局变量中
    std::cout<<"enter nickname:";
    std::cin>>usr_msg.nickname;
    std::cout<<"enter school:";
    std::cin>>usr_msg.school;

    //创建一个窗口类
    window* win = new window;
    for_cli.wp = win;
    for_cli.ucp = &netdata;
    //设置ctl+c信号的捕捉
    signal(SIGINT,handler_SIGINT);

    //然后创建一个三个线程用于标题框，输入框，输出框的显示
    //int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
      //                                    void *(*start_routine) (void *), void *arg);
    pthread_t title,input,output;
    pthread_create(&title,NULL,handler_title,(void*)&for_cli);
    pthread_create(&input,NULL,handler_input,(void*)&for_cli);
    pthread_create(&output,NULL,handler_output,(void*)&for_cli);
    
    //主线程等待三个新线程
    //int pthread_join(pthread_t thread, void **retval);
    pthread_join(title,NULL);
    pthread_join(input,NULL);
    pthread_join(output,NULL);
    return 0;


}
