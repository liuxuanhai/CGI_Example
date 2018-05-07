/******************************************    *******************************
      > File Name: net.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     Thu 14 Dec 2017 09:20:33 AM CST
      > Modify Time: 
 *********************************    ***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <inttypes.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <net/if.h>
#include <sys/socket.h>

#include "../bool.h"
#include "net.h"

/* UDP 套接字创建 */
int m_udpsock_create(char *eth, char *ip, unsigned short port, unsigned int sendtime, unsigned int recvtime)
{
	int sock;
	int tmps;
	struct timeval tv_out;
	struct sockaddr_in sin;
	struct ifreq ifr;

	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		printf("[UDP]socket:%s\r\n", strerror(errno));
		return false;
	}
	if(eth)		//socket绑定网卡
	{
		memset(&ifr, 0, sizeof(ifr));
		strcpy(ifr.ifr_name, (char *)eth);
		if(setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifr, sizeof(struct ifreq)) == -1)
		{
			printf("[UDP]setsockopt:%s\r\n", strerror(errno));
			close(sock);
			return false;
		}
	}
	tmps = 1;		// 允许发送广播数据
	if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &tmps, sizeof(tmps)) == -1)
	{
		printf("[UDP广播]:setsockopt:%s\r\n", strerror(errno));
		close(sock);
		return false;
	}
	tmps = 1;		// 允许端口重用
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &tmps, sizeof(tmps));
	sin.sin_family = AF_INET;
	if(ip)
	{
		memcpy(&sin.sin_addr.s_addr, ip, 4);
	}
	else
	{
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	sin.sin_port = htons(port);		// 网络字节顺序
	memset(&sin.sin_zero, 0, 8);
	if(bind(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1)		//绑定socket
	{
		printf("[UDP]bind:%s\r\n", strerror(errno));
		close(sock);
		return false;
	}
	tv_out.tv_sec = sendtime;			//超时秒数
	tv_out.tv_usec = 0;
	setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv_out, sizeof(tv_out));		// 设置发送超时
	tv_out.tv_sec = recvtime;			//超时秒数
	tv_out.tv_usec = 0;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));		// 设置接收超时
	printf("[UDP]服务器建立成功%u.%u.%u.%u:%d\r\n", INET_NTOA(sin.sin_addr.s_addr), ntohs(sin.sin_port));
	return sock;
}
/* UDP发送 */
int m_udpsock_send(int fd, char *ip, unsigned short port, char *data, unsigned int len)
{
    if(port <= 0 || fd <= 0)
    {
        return false;
    }
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	if(ip)
	{
		memcpy(&addr.sin_addr.s_addr, ip, 4);
	}
	else
	{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	addr.sin_port = htons(port);
	memset(&addr.sin_zero, 0, 8);
	return sendto(fd, data, len, 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
}

#if NET_TCP_MODULE_EN					/* TCP网络通信 */
#endif

#ifndef __main__
#define __main__ 0
#if __main__
int main()
{
    int sock = m_udpsock_create(NULL, NULL, 8994, 3, 3);
    int sock2 = m_udpsock_create(NULL, NULL, 8995, 3, 3);
    printf("send len is :%d\n", m_udpsock_send(sock, "127.0.0.1", 8995, "hello", strlen("hello")));
    close(sock);
    close(sock2);
    return 0;
}
#endif
#undef __main__
#endif
