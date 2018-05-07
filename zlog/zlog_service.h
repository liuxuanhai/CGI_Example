/******************************************    *******************************

      > File Name: zlog_service.h

      > Author: Ywl

      > Descripsion:

      > Created Time:     Mon 07 May 2018 01:44:54 AM PDT

      > Modify Time: 

 *********************************    ***************************************/

#ifndef _ZLOG_SERVICE_H
#define _ZLOG_SERVICE_H

int zlog_printf(char level, const char *category, const char *fmt, ...);

char m_zlog_initial(const char *fmt_conf);
char m_zlog_output(const char *, char *);

#endif
