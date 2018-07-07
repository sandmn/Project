#include "parse.h"
#include "def.h"
#include "init.h"

command_t cmd[MAXPIPE];  // 命令行参数

char cmdline[MAXLINE+1]; // 读取命令行数据
char avline[MAXLINE+1];  // 处理后的数据
int pipenum;             // 当前用了几个管道符
int lastpid;

int main( void )
{
	sig_init();
	shell_loop();
	return 0;
}

