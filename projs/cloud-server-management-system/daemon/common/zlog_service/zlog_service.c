/******************************************    *******************************

      > File Name: zlog_service.c

      > Author: Ywl

      > Descripsion:

      > Created Time:     Mon 07 May 2018 02:45:55 AM PDT

      > Modify Time: 

 *********************************    ***************************************/
#include <ctype.h>
#include <string.h>
#include <pthread.h>

#include "../../module/module.h"
#include "zlog_service.h"

static pthread_mutex_t zlog_lock = PTHREAD_MUTEX_INITIALIZER;

static char dzlog_initial(const char *fmt_conf, const char *category)
{
    int rc;
    zlog_reload(fmt_conf);
    rc = dzlog_init(fmt_conf, category);
    if (rc) 
    {
        printf("dzlog init failed\n");
        return false;
    }
    return true;
}

static void dzlog_out_stream(const char *log_path, const char *category, char lv, const char *fmt, va_list args)
{
    pthread_mutex_lock(&zlog_lock);
    if(dzlog_initial(log_path, category) == false){
        pthread_mutex_unlock(&zlog_lock);
        return;
    }
    switch(lv){
        case LV_DEBUG:
            vdzlog_debug(fmt, args);
            break;
        case LV_INFO:
            vdzlog_info(fmt, args);
            break;
        case LV_NOTICE:
            vdzlog_notice(fmt, args);
            break;
        case LV_WARN:
            vdzlog_warn(fmt, args);
            break;
        case LV_ERROR:
            vdzlog_error(fmt, args);
            break;
        case LV_FATAL:
            vdzlog_fatal(fmt, args);
            break;
        default:
            vdzlog_debug(fmt, args);
            break;
    }
    zlog_fini();
    pthread_mutex_unlock(&zlog_lock);
}
void s_system_log(char lv, const char *format, ...)
{
	va_list args;

	va_start(args, format);
    dzlog_out_stream(SYSTEM_LOG_CONF, SYSTEM_LOG, lv, format, args);
	va_end(args);
}
void s_user_log(char lv, const char *format, ...)
{
	va_list args;

	va_start(args, format);
    dzlog_out_stream(SYSTEM_LOG_CONF, USER_LOG, lv, format, args);
	va_end(args);
}
void s_dbg_log(char lv, const char *format, ...)
{
	va_list args;

	va_start(args, format);
    dzlog_out_stream(SYSTEM_LOG_CONF, PROGRAM_DBG, lv, format, args);
	va_end(args);
}

/*
int main()
{
    printf("------------------------ zlog_version : %s ----------------------------\r\n", zlog_version());
    dbg_print("%s", "test-dbg_print");
    dbg_error("%s", "test-dbg_error");
    dbg_log("%s", "test-dbg_log");
    system_log(LV_INFO, "test-system_log");
    user_log(LV_INFO, "test-user_log");
    return 0;
}
*/
