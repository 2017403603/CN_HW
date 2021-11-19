#include <syslog.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include "common.hpp"
#define MAXFD 64

int daemon_init(const char *program_name,int facility) {
	int i;

	pid_t pid;

	//终止父进程，pid < 0 出错，pid = 0 子进程, pid > 0 父进程
	if((pid = fork()) < 0)
		return -1;
	else if(pid)
		exit(0);

	//重启新会话，并设置子进程为新会话的头进程（无控制终端）
	if(setsid() < 0)
		return -1;

	//再次fork确保即使打开一个终端设备也不会自动获取控制终端
	signal(SIGHUP,SIG_IGN);

	if((pid = fork()) < 0)
		return -1;
	else if(pid)
		exit(0);  //产生SIGHUP信号

	//使用err_xxx 时候用syslog() 而不是使用错误处理函数
	// daemon_proc = 1;

	//程序所在目录文件系统不能卸载
	chdir("/");

	//关闭继承来的文件描述符
	for(i=0;i<MAXFD;i++)
		close(i);

	// 将标准输入，标准输出，标准错误重定向
	open("/dev/null",O_RDONLY);
	open("/dev/null",O_RDWR);
	open("/dev/null",O_RDWR);

	//使用syslog处理错误
	//注意,第一个参数是程序名,会在rsyslog.conf配置中捕获
	//如果program_name是'./http_server',rsyslog将匹配不到'./http_server'huozhe'http_server'
	openlog(program_name,LOG_PID|LOG_CONS,facility);
	return 0;
}
