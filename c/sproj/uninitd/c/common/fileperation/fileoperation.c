/******************************************    *******************************
      > File Name: fileoperation.c
      > Author: Ywl
      > Descripsion: 各种文件操作
      > Created Time:     Fri 01 Dec 2017 05:00:54 PM CST
      > Modify Time: 
 *********************************    ***************************************/


#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "../../module/bool.h"
#include "../../module/timer/timer.h"
#include "fileoperation.h"

static int32_t ssys_confini_fd = -1;

uint8_t c_appendstr_tofile(int32_t fd, int8_t *str, pthread_mutex_t *lock)
{
	if((fd < 0) \
		|| (str == NULL) \
		|| (lock == NULL))
	{
		return false;
	}
	uint32_t time;
	int32_t ret = 0;
	uint32_t total = 0;
	uint32_t len = strlen(str);
	pthread_mutex_lock(lock);
	time = m_getostime();
	do{
		if(total >= len)
			break;
		ret = write(fd, str+total, len-total);
		if(ret < 0)
		{
			pthread_mutex_unlock(lock);
			return false;
		}
		total += ret;
	}while((m_getostime() - time) < OS_SEC(3));
	pthread_mutex_unlock(lock);
	if((m_getostime() - time) >= OS_SEC(3))
	{
		return false;
	}
	return true;
}
/* 读取配置文件 */
uint8_t c_getsys_confini(int8_t *section, int8_t *keyId, int8_t *buf, uint32_t len, uint8_t type)
{
	return true;
}
/* 写入配置文件 */
uint8_t c_setsys_confini(int8_t *section, int8_t *keyId, int8_t *buf, uint32_t len, uint8_t type)
{
	return true;
}


