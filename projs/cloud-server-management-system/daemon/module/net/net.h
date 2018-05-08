/******************************************    *******************************
  > File Name: net.h
  > Author: Ywl
  > Descripsion: 网络通信部分的配置
  > Created Time:     Thu 30 Nov 2017 02:16:36 PM CST
  > Modify Time: 
 *********************************    ***************************************/

#ifndef _NET_H
#define _NET_H

#include <arpa/inet.h>
#include <net/if.h>
#include <sys/socket.h>

/* 将网络二进制的数字转换成网络地址 */
#define INET_NTOA(ip)		ip & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, (ip >> 24) & 0xFF

#define NET_UDP_MODULE_EN			1					/* UDP网络通信 */
#define NET_TCP_MODULE_EN			1					/* TCP网络通信 */

#define UDP_DATA_TYPE				0x1
#define TCP_DATA_TYPE				0x2

#define UBUNTU_UDP_SERVER_EN				1
#define MASTER_UDP_SERVER_PORT				111314					/* 主服务器UDP端口号 */
#define CGI_UDP_CLIENT_PORT					111315					/* 主服务器UDP端口号 */
/* UDP数据解析 */
void m_unpack_udpdata(char *data, int len, char *ip, unsigned short port);
/* UDP 套接字创建 */
int m_udpsock_create(char *eth, char *ip, unsigned short port, unsigned int sendtime, unsigned int recvtime);
/* UDP发送 */
int m_udpsock_send(int fd, char *ip, unsigned short port, char *data, unsigned int len);

/* UDP数据解析 */
void m_unpack_tcpdata(char *data, int len, char *ip, unsigned short port);

/* 数据类型解析 */
void m_unpack_netdata(char *data, int len, char *ip, unsigned short port);

#endif

