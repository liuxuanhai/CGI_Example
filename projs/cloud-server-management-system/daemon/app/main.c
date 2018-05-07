/******************************************    *******************************

      > File Name: main.c

      > Author: Ywl

      > Descripsion:

      > Created Time:     Sun 06 May 2018 05:53:55 PM CST

      > Modify Time: 

 *********************************    ***************************************/
#include <stdio.h>

#include "../module/module.h"

int main(int args, char **argv)
{
    int rc;
    zlog_category_t *c;
    rc = zlog_init("system.conf");
    if(rc){
        printf("init failed\n");
        return -1;
    }
    c = zlog_get_category("system");
    if(!c){
        printf("get system fail\n");
        zlog_fini();
        return -2;
    }
    zlog_info(c, "hello system");
    zlog_fini();
    return 0;
}
