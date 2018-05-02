/******************************************    *******************************
      > File Name: syslog.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     Thu 30 Nov 2017 04:13:04 PM CST
      > Modify Time: 
 *********************************    ***************************************/

#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "../../module/module.h"
#include "fileoperation.h"
#include "syslog.h"

static int s_syslog_program_notime_fd = -1;
static int s_syslog_program_time_fd = -1;
static int s_syslog_heart_fd = -1;

static pthread_mutex_t s_syslog_program_notime_lock = PTHREAD_MUTEX_INITIALIZER;			/* 锁 */
static pthread_mutex_t s_syslog_program_time_lock = PTHREAD_MUTEX_INITIALIZER;				/* 锁 */
static pthread_mutex_t s_syslog_heart_lock = PTHREAD_MUTEX_INITIALIZER;				/* 锁 */

/*
static int *s_system_logfd[] = {
	&s_syslog_program_notime_fd, 
	&s_syslog_program_time_fd,
	&s_syslog_heart_fd,
	NULL
};
static const char *s_system_logfile[] = {
	SYS_PROG_LOG_ERR_NO_TIME,  
	SYS_PROG_LOG_ERR_TIME,  
	SYS_LOG_HEART,
	NULL
};
static pthread_mutex_t *s_system_loglock[] = {
	&s_syslog_program_notime_lock, 
	&s_syslog_program_time_lock,
	&s_syslog_heart_lock,
	NULL
};
*/
typedef struct {
    int *fd;
    pthread_mutex_t *lock;
    const char *filename;
}s_class_system_loghandle;

static s_class_system_loghandle s_system_logfile_handle[] = {
    { &s_syslog_program_notime_fd,      &s_syslog_program_notime_lock,      SYS_PROG_LOG_ERR_NO_TIME },
    { &s_syslog_program_time_fd,        &s_syslog_program_time_lock,        SYS_PROG_LOG_ERR_TIME },
    { &s_syslog_heart_fd,               &s_syslog_heart_lock,               SYS_LOG_HEART },
    {NULL}
};

/*
 系统错误日志的标准格式
 输入值:
	字符缓存，所在文件，所在函数，所在行数
返回值:
	正确=字符缓存的长度
	错误=FALSE
 */
int m_system_error_standard(char *str, char  *file, const char *fun, unsigned int line)
{
	if((NULL == str) \
		|| (NULL == file) \
		|| (NULL == fun) \
		|| (line < 0))
	{
		return false;
	}
	return sprintf((char *)str, "File:%s; Function:%s; Line:%d; Error:", file, fun, line);
}

/*不带时间的日志*/
void m_system_log_notime(char *errStr)
{
	if(c_appendstr_tofile(s_syslog_program_notime_fd, "\n", &s_syslog_program_notime_lock) == false)
		return;
	c_appendstr_tofile(s_syslog_program_notime_fd, errStr, &s_syslog_program_notime_lock);
}
/*带有时间的日志*/
void m_system_log_time(char *errStr)
{
	char timeStr[SYS_TIME_STR_LEN];
	/*获取时间*/
	if(NULL == m_getostimestr(timeStr))
	{
		return;
	}

	//写时间
	if(c_appendstr_tofile(s_syslog_program_time_fd, "\n", &s_syslog_program_time_lock) == false)
		return;
	if(c_appendstr_tofile(s_syslog_program_time_fd, timeStr, &s_syslog_program_time_lock) == false)
		return;
	//写错误
	if(c_appendstr_tofile(s_syslog_program_time_fd, "\n", &s_syslog_program_time_lock) == false)
		return;
	c_appendstr_tofile(s_syslog_program_time_fd, errStr, &s_syslog_program_time_lock);
}
/*
 系统心跳日志
 */
void m_system_heart_log(void)
{
	char *ptr = "\n[m_system_heart_log]: <Normal> ";
	char timeStr[SYS_TIME_STR_LEN];
	/*获取时间*/
	if(NULL == m_getostimestr(timeStr))
	{
		return;
	}
	//写头部
	if(c_appendstr_tofile(s_syslog_heart_fd, ptr, &s_syslog_heart_lock) == false)
		return;
	printf("%s", ptr);
	//写时间
	c_appendstr_tofile(s_syslog_heart_fd, timeStr, &s_syslog_heart_lock);
	printf("%s\n", timeStr);
}

/*
 检查日志文件，超过10M删除前10万行
 */

void m_check_system_log(void)
{
	struct stat st;
	int i = 0;
	for(i = 0; NULL != s_system_logfile_handle[i].fd; i++)
	{
		memset(&st, 0, sizeof(struct stat));
		fstat(*s_system_logfile_handle[i].fd, &st);
		if(st.st_size > SYS_LOG_FILE_MAX_SIZE)
		{
			pthread_mutex_lock(s_system_logfile_handle[i].lock);
			printf("SysLog Clean : %s\n", s_system_logfile_handle[i].filename);
			m_system_cmd(CLEAN_SYSTEM_LOG, s_system_logfile_handle[i].filename);
			pthread_mutex_unlock(s_system_logfile_handle[i].lock);
		}
	}
}

unsigned char m_system_log_initialize(void)
{
    int index = 0;
    for(; NULL != s_system_logfile_handle[index].fd; index++)
    {
        if((*s_system_logfile_handle[index].fd = open(s_system_logfile_handle[index].filename, O_CREAT|O_WRONLY|O_APPEND)) < 0)
        {
            printf("Open %s Failed\n", s_system_logfile_handle[index].filename);
            return false;
        }
    }
	return true;
}

void m_close_system_log(void)
{
    int index = 0;
    for(; NULL != s_system_logfile_handle[index].fd; index++)
    {
	    close(*s_system_logfile_handle[index].fd);
    }
}
