#ifndef __DEF_H__
#define __DEF_H__

#define MAXLINE 1024   // 一行的最大值
#define ARGS    8      // 命令行参数个数
#define MAXPIPE 8      // 系统最多能够解析8个管道

typedef struct command_ {
	char *args[ARGS];
	int infd;
	int outfd;
}command_t;


#endif //__DEF_H__

