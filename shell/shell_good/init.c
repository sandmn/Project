#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "init.h"
#include "extern.h"

void handler(int s)
{
	printf("\n[sxshell]: ");
	fflush(stdout);
}

void sig_init()
{
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
}

// 初始化全局变量
void init()
{
	memset(cmdline, 0x00, sizeof(cmdline));
	memset(avline, 0x00, sizeof(avline));
	for (int k=0; k<MAXPIPE; k++) {
		for (int i=0; i<ARGS; i++)
			cmd[k].args[i] = NULL;
		cmd[k].infd = 0;
		cmd[k].outfd = 1;
	}
	pipenum = 0;
}

