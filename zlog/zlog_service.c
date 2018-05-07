/******************************************    *******************************

      > File Name: zlog_service.c

      > Author: Ywl

      > Descripsion:

      > Created Time:     Mon 07 May 2018 02:45:55 AM PDT

      > Modify Time: 

 *********************************    ***************************************/
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <zlog.h>
#include "zlog_service.h"

int zlog_printf(char level, const char *category, const char *fmt, ...)
{
	char printf_buf[1024];
	va_list args;
	int printed;
	va_start(args, fmt);
	printed = vsprintf(printf_buf, fmt, args);
	va_end(args);
    if(level)
    {
	    puts(printf_buf);
    }
	return printed;
}

char m_zlog_initial(const char *fmt_conf)
{
    int rc;
    rc = zlog_init(fmt_conf);
    if (rc) 
    {
        printf("init failed\n");
        return 1;
    }
    return 0;
}
char m_zlog_output(const char *category, char *output)
{
    zlog_category_t *c;
    c = zlog_get_category(category);

    if (!c) {
        printf("get cat fail\n");
        zlog_fini();
        return -2;
    }

    zlog_info(c, output);

    zlog_fini();
    return 0;
}
