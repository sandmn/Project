#pragma once
#include<iostream>
#include<string>
#include"base_json.h"
//定义一个数据类
class data
{
    public:
        //将各个独立的数据序列化为一个字符串
        void serilize(std::string& s);
        //将字符串反序列化为各个独立的数据
        void deserilize(std::string& s);
        //服务器端和客户端发送数据的公共信息
        std::string nickname;//昵称
        std::string school;//学校
        std::string msg;//消息
        std::string cmd;//命令
};
