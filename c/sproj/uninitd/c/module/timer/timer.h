/******************************************    *******************************
      > File Name: timer.h
      > Author: Ywl
      > Descripsion:
      > Created Time:     Thu 30 Nov 2017 02:32:47 PM CST
      > Modify Time: 
 *********************************    ***************************************/

#ifndef _TIMER_H
#define _TIMER_H

#define OS_TICKS_PER_SEC			200ul													/* 5毫秒为一个tick */
/*********************************************************************************************************
    操作系统的tick
	*********************************************************************************************************/
#define OS_MS(ms)					(uint32_t)(((ms) * OS_TICKS_PER_SEC + 999) / 1000L)		/* ms非零时保证至少一个tick */
#define OS_SEC(sec)					(uint32_t)((sec) * OS_TICKS_PER_SEC)						/* 一秒钟 */
#define OS_MIN(min)					(uint32_t)((min) * 60L * OS_TICKS_PER_SEC)				/* 一分钟 */
#define OS_HOUR(hr)					(uint32_t)((hr) * 60L * 60L * OS_TICKS_PER_SEC)			/* 一小时 */
#define TICK_NANOSEC				((1000 / OS_TICKS_PER_SEC) * 1000 * 1000)				/* 一个tick 对应的纳秒数 */

#define SYS_TIME_STR_LEN			255				/* 系统时间长度 */


/* 获取系统时间,返回秒数 */
uint32_t m_getostime(void);
/* 指定系统延时 */
void m_ostime_delay(uint32_t tick);
/* 打印时间 */
void m_print_ostime(void);
/* 获取时间字符串 */
int8_t *m_getostimestr(int8_t *str);
/* 获取格式化时间 */
struct tm *m_getostime_format(struct tm *tm);

/* 打印一个月的月历 */
void m_print_monthly(uint8_t month);
/* 打印一年的年历 */
void m_print_calendar(uint16_t year);

#endif

