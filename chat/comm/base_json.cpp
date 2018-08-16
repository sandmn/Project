#include"base_json.h"

//将value转换为字符串
void val_to_string(Json::Value& v,std::string& s)
{
    Json::StyledWriter w;
    s = w.write(v);
}
//将字符串转化为Value
void string_to_val(std::string& s,Json::Value& v)
{
    Json::Reader r;
    r.parse(s,v,false);
}
