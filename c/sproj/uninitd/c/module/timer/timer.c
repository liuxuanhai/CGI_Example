/******************************************    *******************************
      > File Name: timer.c
      > Author: Ywl
      > Descripsion: 关于时间的接口
      > Created Time:     Thu 30 Nov 2017 02:03:07 PM CST
      > Modify Time: 
 *********************************    ***************************************/

#include <stdio.h>
#include <inttypes.h>   /* stdint.h */
#include <time.h>
#include <pthread.h>    /* -lpthread */

#include <sys/types.h>
#include <sys/time.h>

#include "timer.h"

static pthread_mutex_t s_time_lock = PTHREAD_MUTEX_INITIALIZER;			/* 时间读取锁 */

static  const int8_t *const wday_zh[] = {"星期天", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};
static  const int8_t *const wday_en[] = {"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};

/*
 获取的是时间,微秒级别
 返回值:当前秒数
 */
uint32_t m_getostime(void)
{
	uint32_t tick;
	struct timeval tv;

	pthread_mutex_lock(&s_time_lock);
	gettimeofday(&tv, NULL);
	tick = OS_SEC(tv.tv_sec) + OS_MS(tv.tv_usec / 1000);
	pthread_mutex_unlock(&s_time_lock);
	return tick;
}

/*
 指定延时
tick:延时时间(一个tick 5微秒)
 */
void m_ostime_delay(uint32_t tick)
{
	struct timespec tv;

	tv.tv_sec = tick / OS_SEC(1);
	tv.tv_nsec = tick % OS_SEC(1) * TICK_NANOSEC;
	nanosleep(&tv, NULL);
}

/*
 获取一个格式化时间
 */
struct tm *m_getostime_format(struct tm *tm)
{
	if(tm == NULL)
		return NULL;
	time_t sec;
	if((sec = time(NULL)) < 0)
	{
		return NULL;
	}
	tm = localtime(&sec);
	return tm;
}

static int8_t *s_time_strformat(struct tm *tm, int8_t *str)
{
    if((NULL == tm) || (NULL == str))
    {
        return NULL;
    }
#if CUR_LANG_ZH
	sprintf(str, "%d年 %d月 %d日 %s %d:%d:%d\n", \
			tm->tm_year + 1900, \
			tm->tm_mon + 1, \
			tm->tm_mday, \
			wday_zh[tm->tm_wday], \
			tm->tm_hour, \
			tm->tm_min, \
			tm->tm_sec);
#else
	sprintf(str, "%d %d %d %s %d:%d:%d\n", \
			tm->tm_year + 1900, \
			tm->tm_mon + 1, \
			tm->tm_mday, \
			wday_en[tm->tm_wday], \
			tm->tm_hour, \
			tm->tm_min, \
			tm->tm_sec);
#endif
    return str;
}


/*
 返回时间字符串
 */
int8_t *m_getostimestr(int8_t *str)
{
	if(str == NULL)
		return NULL;
	struct tm *tm;
    return s_time_strformat(m_getostime_format(tm), str);
}

/*
打印时间: 
 */
void m_print_ostime(void)
{
    int8_t str[1024];
    printf("%s", m_getostimestr(str));
}

/* 打印一个月的月历 */
void m_print_monthly(uint8_t month)
{
    return;
}
/* 打印一年的年历 */
void m_print_calendar(uint16_t year)
{
    return;
}


#define __main__    0

/*
 * gcc timer.c -o timer -lpthread
 *
 * */

#if __main__

int32_t main()
{
    m_print_ostime();
    return 0;
}
#endif

#undef __main__
