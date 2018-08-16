
#include"data.h"


//将各个独立的数据序列化为一个字符串
void data::serilize(std::string& s)
{
    Json::Value v;
    v["nickname"] = nickname;
    v["school"] = school;
    v["msg"] = msg;
    v["cmd"] = cmd;
    val_to_string(v,s);

}
//将字符串反序列化为各个独立的数据
void data::deserilize(std::string& s)
{
    Json::Value v;
    string_to_val(s,v);
    nickname = v["nickname"].asString();
    school = v["school"].asString();
    msg = v["msg"].asString();
    cmd = v["cmd"].asString();
}
