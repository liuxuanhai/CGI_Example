/******************************************    *******************************

      > File Name: network_detection.c

      > Author: Ywl

      > Descripsion:

      > Created Time:     Mon 07 May 2018 09:54:29 PM CST

      > Modify Time: 

 *********************************    ***************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include "../../module/module.h"
#include "../common.h"

static int s_socket = -1;

static char udp_buf[1024];

struct class_poll{
    char *data;
    unsigned int len;
};

static struct class_poll test_send = {
    "daemon heart",
    strlen("daemon heart")
};

void send_udp_data(char *data, unsigned int len)
{
    m_udpsock_send(s_socket, NULL, VER_CENTER_PORT, data, len);
}

int recv_udp_data(struct sockaddr_in *sin)
{
    int len = sizeof(struct sockaddr);
    memset(sin, 0, sizeof(struct sockaddr_in));
    len = recvfrom(s_socket, udp_buf, sizeof(udp_buf), 0, (struct sockaddr *)sin, (socklen_t *)&len);
    return len;
}

static int create_udp_simple(void)
{
    return m_udpsock_create(NULL, NULL, DAEMON_SERVER_PORT, 2, 5);
}

static char check_recv_udp_data(int len)
{
    if(len > 0)
    {
        return true;
    }
    else if(len == -1)
    {
        if(errno != EAGAIN)
        {
            close(s_socket);
            s_socket = -1;
            m_ostime_delay(OS_SEC(2));
        }
    }
    return false;
}

static void deal_udp_data(int len, struct sockaddr_in sin)
{
    dbg_print("udp-recv : ip = %s port = %d | %s", &sin.sin_addr.s_addr, ntohs(sin.sin_port), udp_buf);
}

static void network_task(void)
{
    int len;
    struct sockaddr_in sin;
    for(;;)
    {
        if(s_socket < 0)
        {
            if((s_socket = create_udp_simple()) < 0){
                m_ostime_delay(OS_SEC(2));
                continue;
            }
        }
        send_udp_data(test_send.data, test_send.len);
        len = recv_udp_data(&sin);
        if(check_recv_udp_data(len) == true)
        {
            deal_udp_data(len, sin);
        }
        m_ostime_delay(OS_SEC(5));
    }
}

void network_start(void)
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, (void *)network_task, NULL);
}
