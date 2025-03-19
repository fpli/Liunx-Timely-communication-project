#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <memory.h>
#include <stdio.h>
#include <signal.h>
#include "fun.h"

using namespace std;

int fd = -1;
void fun(int sig)
{
	do_exit(fd);
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		cerr << "cmd is not enough;errno:" << errno << endl;
		return 0;
	}
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == fd)
	{
		cerr << "sockfd create fail;errno:" << errno << endl;
		return 0;
	}
	// 绑定信号处理函数（do_exit）;
	signal(SIGINT, fun);

	// 分离ip地址和端口号
	char *p1 = argv[1];
	char *p2 = argv[2];

	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(p2));
	saddr.sin_addr.s_addr = inet_addr(p1);
	// 链接服务器

	int res = connect(fd, (struct sockaddr *)&saddr, sizeof(saddr));
	if (-1 == res)
	{
		cerr << "connect fail;errno:" << errno << endl;
		return 0;
	}

	while (1)
	{
		// 让用户选择服务
		cout << "1.register  2.login   6.exit" << endl;

		int m = getchar();
		switch (m)
		{
		case 1:
		{
			do_register(fd);
		}
		break;
		case 2:
		{
			do_login(fd);
		}
		break;
		case 3:
		{
			do_exit(fd);
		}
		break;
		default:
			cout << "error" << endl;
			break;
		}
	}

	return 0;
}
