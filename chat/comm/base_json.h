
#pragma once

#include<iostream>
#include<string>
#include<json/json.h>

//将Value转化为字符串
void val_to_string(Json::Value& v,std::string& s);
//将字符串转化为Value
void string_to_val(std::string& s,Json::Value& v);
