/******************************************    *******************************

      > File Name: main.c

      > Author: Ywl

      > Descripsion:

      > Created Time:     Sun 06 May 2018 05:53:55 PM CST

      > Modify Time: 

 *********************************    ***************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "../module/module.h"
#include "../common/common.h"

#define DAEMON_TEST 1

#if DAEMON_TEST

#define DIR_MODE   1
#define STD_MODE   0

#else

#define DIR_MODE    0
#define STD_MODE    1

#endif

void create_daemon(void)
{
    if(-1 == daemon(DIR_MODE, STD_MODE))
    {
        system_info("daemon initial ending:error");
        exit(0);
    }
}

int main(int args, char **argv)
{
    //create_daemon();
    system_info("..........................system start..................................");
    system_start();
    return 0;
}
