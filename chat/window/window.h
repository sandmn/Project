#pragma once

#include<iostream>
#include<string>
#include<ncurses.h>

//定义一个窗口类，其中包含客户端的几个窗口以及对窗口的操作
class window
{
    public:
        //构造函数
        window();
        //构建标题窗口
        void show_title();
        //构建输入窗口
        void show_input();
        //构建输出窗口
        void show_output();
        //构建用户列表窗口
        void show_list();
        //从屏幕上获取字符串:对屏幕上进行字符串操作时都是C语言类型的字符串
        void getstr_from_win(WINDOW* win,std::string& s);
        //向屏幕上写字符串
        void putstr_to_win(WINDOW* win,int y,int x,const std::string& s);
        //清空指定屏幕:
        //其中win为要指定清空的屏幕，begin为清空的起始行，num为要清空的行数
        void clearwin(WINDOW* win,int begin,int num);
            //移动标题
        void MoveTitle();
        //在输入窗口中放置提示输入字符串，
        void inputgetstr(std::string& s);
        //析构函数：销毁各个窗口
        ~window();
    public:
        WINDOW* title;//用于显示标题的窗口
        WINDOW* input;//用于显示输入的窗口
        WINDOW* output;//用于显示输出的窗口
        WINDOW* userlist;//用于显示在线用户的窗口
};
