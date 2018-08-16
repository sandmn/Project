
#include"window.h"
//构造函数
window::window()
{
    //对屏幕进行初始化
    initscr();
    curs_set(0);//有问题
}
//构建标题窗口
void window::show_title()
{
    //x,y是窗口的起始坐标
    int x = 0;
    int y = 0;
    //high,wide为窗口的高和宽
    int high = LINES/5;//标题窗口高为总窗口的五分之一
    int wide = COLS;//标题窗口宽与总窗口的宽相同
    //构建一个标题窗口
    title = newwin(high,wide,y,x);
    //利用方框绘制窗口
    box(title,0,0);
    //刷新窗口进行显示
    wrefresh(title);
}
//构建输入窗口
void window::show_input()
{
    int x = 0;
    int y = LINES*4/5;
    int high = LINES/ 5;
    int wide = COLS;
    input = newwin(high,wide,y,x);
    box(input,0,0);
    wrefresh(input);
}
//构建输出窗口
void window::show_output()
{
    int x = 0;
    int y = LINES/5;
    int high = LINES*3/5;
    int wide = COLS*3/4;
    output = newwin(high,wide,y,x);
    box(output,0,0);
    wrefresh(output);
}
//构建用户列表窗口
void window::show_list()
{
    int x = COLS*3/4;
    int y = LINES/5;
    int high = LINES*3/5;
    int wide = COLS/4;
    userlist = newwin(high,wide,y,x);
    box(userlist,0,0);
    wrefresh(userlist);
}
//从屏幕上获取字符串:对屏幕上进行字符串操作时都是C语言类型的字符串
void window::getstr_from_win(WINDOW* win,std::string& s)
{
    char buf[1024];
    wgetnstr(win,buf,sizeof(buf) - 1);
    s = buf;
}
//向屏幕上写字符串
void window::putstr_to_win(WINDOW* win,int y,int x,const std::string& s)
{
    mvwaddstr(win,y,x,s.c_str());
    wrefresh(win);
}
//清空指定屏幕:
//其中win为要指定清空的屏幕，begin为清空的起始行，num为要清空的行数
void window::clearwin(WINDOW* win,int begin,int num)
{
    int i = 0;
    for(;i < num;i++)
    {
        //先将窗口指针移动到指定位置
        wmove(win,begin++,0);
        //然后依次清空一行
        wclrtoeol(win);
    }
    //在清空一行内容的同时，可能会将屏幕的边框也会清除
    //所以，清空完毕之后，再绘制屏幕
    box(win,0,0);
    //然后进行刷新
    wrefresh(win);
}
//移动标题
void window::MoveTitle()
{
    //在标题栏中显示一行字符串
    const std::string& s = "welcome yanxuechun's chating system";
    int y;
    int x;//x,y用于表示窗口的大小
    int i = 1;//i用于表示窗口指针的纵坐标
    //使字符串循环在标题窗口上滚动
    int dir = 0;//定义一个方向标志，表示正向移还是反向移
    while(1)
    {
        //首先获得窗口的大小
        getmaxyx(title,y,x);
        //将字符串显示在窗口上
        putstr_to_win(title,y/2,i,s);
        //有问题
        int len = x - s.size();
        len = len > 0 ? len:x;
        if(i == len - 1)
        {
            dir = 1;//此时字符串到头了，需要反向移
        }
        if(i == 1)
        {
            dir = 0;
        }
        if(dir == 1)
        {
            i--;
        }
        if(dir == 0)
        {
            i++;
        }
        //此时刷新一下窗口（有问题）
        //每隔一段时间，清空一次窗口
        usleep(1000000);
        clearwin(title,y/2,1);
    }
}

//在输入窗口中放置提示输入字符串，
void window::inputgetstr(std::string& s)
{
    //首先获取窗口的宽和高
    int x;
    int y;
    getmaxyx(input,y,x);
    //在窗口中放置提示输入的字符串
    putstr_to_win(input,y/2,1,"please enter#");
    //然后涮新窗口
    //wrefresh(input);
    //此时指针指向#后面
    //然后需要用户进行输入
    getstr_from_win(input,s);
    //获取字符串之后对该字符串进行清空
    clearwin(input,y/2,1);
}
//析构函数：销毁各个窗口
window::~window()
{
    delwin(input);
    delwin(output);
    delwin(title);
    delwin(userlist);
    curs_set(1);//有问题
    endwin();
}
