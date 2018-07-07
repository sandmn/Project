#pragma once

#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

typedef struct node_ node_t;

//定义链表的节点，用于保存歌曲相关的信息
struct node_
{
    char* name;//歌名,因为歌名的长度不取定，所以需要动态申请
    node_t* next;//指向下一首歌曲
    node_t* prev;//指向上一首歌曲
};

node_t* head = NULL;//定义带头节点的双向链表，将头节点定义为全局变量
node_t* cur = NULL;//定义当前歌曲的指针变量
int first = 1;//first表示是否是第一次播放，初始表示第一次播放，从第一首开始播放

//定义枚举常量，表示当前的状态
enum
{
    STOP,//停止状态
    PLAY,//播放状态
    PAUSE//暂停状态
};
int status = STOP;

//初始化双向链表
void list_init();
//对链表进行插入
void list_insert(const char* name);
//打印链表
void list_show();
//将可取从文件中提取出来加载到链表中
//参数为歌曲所在的路径
void load_music(const char* path);
//播放函数
void playPause();
//停止播放，下一次从头开始播放
//杀死当前播放歌曲的进程
void stop();
//播放下一首歌曲
void next();
//播放上一首歌曲
void prev();
