/******************************************    *******************************
      > File Name: systask.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     Thu 30 Nov 2017 06:29:36 PM CST
      > Modify Time: 
 *********************************    ***************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <inttypes.h>

#include "../../module/module.h"
#include "systask.h"

typedef struct {
	unsigned char id;
	char name[128];
	pid_t tid;
	unsigned int life;
	unsigned int now_time;
}s_class_taskinfo;


/* 任务池 */
static s_class_taskinfo s_system_task_queue[TASK_LAST_ID];
static void s_system_task_queue_init(void)
{
	int i = 0;
	for(i = 0; i < ARRAY_SIZE(s_system_task_queue); i++)
	{
		s_system_task_queue[i].id = 0xFF;
		memset(s_system_task_queue[i].name, 0, sizeof(s_system_task_queue[i].name));
		s_system_task_queue[i].tid = -1;
		s_system_task_queue[i].life = 0;
		s_system_task_queue[i].now_time = 0;
	}
}

void c_task_exit(void)
{
	char *ptr = "UbuntuServer Sys Exit";
    printf("%s\n", ptr);
	return;
}

/* 任务注册 */
void c_task_register(unsigned char id, const char *name, unsigned int life)
{
	int index = 0;
	for(index = 0; index < ARRAY_SIZE(s_system_task_queue); index++)
	{
		if(s_system_task_queue[index].id != 0xFF)
		{
			s_system_task_queue[index].id = id;
			memcpy(s_system_task_queue[index].name, name, strlen((char *)name));
			s_system_task_queue[index].tid = (pid_t)syscall(224);				//gettid();
			s_system_task_queue[index].life = life;
			s_system_task_queue[index].now_time = m_getostime();
			break;
		}
	}
}
/* 任务更新 */
void c_task_update(unsigned char id)
{
	int index = 0;
	for(index = 0; index < ARRAY_SIZE(s_system_task_queue); index++)
	{
		if(s_system_task_queue[index].id == id)
		{
			s_system_task_queue[index].now_time = m_getostime();
			return;
		}
	}
}
static void s_write_fail_task(char *fail, char fail_len)
{
    int index = 0;
	for(index = 0; index < fail_len; index++)
	{
		if(fail[index])
		{
			//读配置文件buf
			//比较fail buf
			//不一样写文件记录
			//break;
		}
	}
}
static char s_ckeck_dead_task(unsigned char *fail_task_count, char lasttask_id)
{
    int index = 0;
	char fail_log[255];
	char rebootflag = false;
	for(index = 0; index < lasttask_id; index++)
	{
		if(fail_task_count[index] >= TASK_CNT_MAX)
		{
			rebootflag = true;
			memset(fail_log, 0, sizeof(fail_log));
			sprintf((char *)fail_log, "[Task :id =  %d; name = %s, tid = %d] timeout reset!\n", \
					s_system_task_queue[index].id, s_system_task_queue[index].name, s_system_task_queue[index].tid);
			printf("%s", fail_log);
		}
	}
    return rebootflag;
}
static void s_check_fail_task(unsigned char *task_count, char *fail)
{
	int index = 0;
	unsigned int time;
	for(index = 0; index < ARRAY_SIZE(s_system_task_queue); index++)
	{
		if(s_system_task_queue[index].id != 0xFF)
		{
			time = m_getostime();
			if((time - s_system_task_queue[index].now_time) >= OS_SEC(s_system_task_queue[index].life))
			{
				//记录并重启
				fail[s_system_task_queue[index].id / 8] |= 1 << (s_system_task_queue[index].id % 8);
				task_count[index]++;
			}
			else
			{
				s_system_task_queue[index].now_time = time;
				task_count[index] = 0;
			}
		}
	}
}
/* 任务池检测 */
void c_ckeck_task_queue(void)
{
	static unsigned char fail_task_count[TASK_LAST_ID];					/* 超时任务计数 */
	char fail[TASK_DIE_LEN], buf[TASK_DIE_LEN];				/* 记录任务状态 */
	memset(fail, 0, sizeof(fail));
    s_check_fail_task(fail_task_count, fail);
    s_write_fail_task(fail, sizeof(fail));
	if(s_ckeck_dead_task(fail_task_count, sizeof(fail)) == true)
	{
		/* 邮件通知 */
		printf("System reboot by UbuntuServer, by task over time!\r\n");
		m_system_reboot();
	}
}

void c_system_main_task(void)
{
	const char *ptr = __FUNCTION__;
	c_task_register(TASK_MAIN_ID, (char *)ptr, 1800);
	while(1)
	{
		c_task_update(TASK_MAIN_ID);

        m_print_ostime();

		m_ostime_delay(OS_SEC(20));

        c_ckeck_task_queue();
	}
	return;
}

unsigned char  c_system_initialize(void)
{
	char *ptr = "LinuxPlatServer System Initialize";
	s_system_task_queue_init();

    printf("%s\n", ptr);

	m_print_ostime();

    c_system_main_task();

	return true;
}
