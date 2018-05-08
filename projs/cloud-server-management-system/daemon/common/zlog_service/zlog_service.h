/******************************************    *******************************

      > File Name: zlog_service.h

      > Author: Ywl

      > Descripsion:

      > Created Time:     Mon 07 May 2018 01:44:54 AM PDT

      > Modify Time: 

 *********************************    ***************************************/

#ifndef _ZLOG_SERVICE_H
#define _ZLOG_SERVICE_H


#include <zlog.h>

#define LV_DEBUG    ZLOG_LEVEL_DEBUG
#define LV_INFO     ZLOG_LEVEL_INFO
#define LV_NOTICE   ZLOG_LEVEL_NOTICE
#define LV_WARN     ZLOG_LEVEL_WARN
#define LV_ERROR    ZLOG_LEVEL_ERROR
#define LV_FATAL    ZLOG_LEVEL_FATAL

#define SYSTEM_LOG_CONF     "./system/etc/system_conf/system_log.conf"
#define PROGRAM_DBG         "program"
#define USER_LOG            "user"
#define SYSTEM_LOG          "system"

void s_system_log(char lv, const char *format, ...);
void s_user_log(char lv, const char *format, ...);
void s_dbg_log(char lv, const char *format, ...);

#define dbg_print(fmt, args...)     s_dbg_log(LV_DEBUG, fmt, ##args)
#define dbg_error(fmt, args...)     s_dbg_log(LV_ERROR, "file: %s fun: %s line : %d] "fmt, __FILE__, __func__,  __LINE__, ##args)
#define dbg_log(fmt, args...)       s_dbg_log(LV_FATAL, "file: %s fun: %s line : %d] "fmt, __FILE__, __func__,  __LINE__, ##args)

#define system_log(lv, fmt, args...) s_system_log(lv, "file: %s fun: %s line : %d] "fmt, __FILE__, __func__,  __LINE__, ##args)
#define user_log(lv, fmt, args...)   s_user_log(lv, "file: %s fun: %s line : %d] "fmt, __FILE__, __func__,  __LINE__, ##args)
#define system_info(fmt, args...)   s_system_log(LV_INFO, fmt, ##args)

#endif

