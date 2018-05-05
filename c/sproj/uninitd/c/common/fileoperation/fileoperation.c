/******************************************    *******************************
      > File Name: fileoperation.c
      > Author: Ywl
      > Descripsion: 各种文件操作
      > Created Time:     Fri 01 Dec 2017 05:00:54 PM CST
      > Modify Time: 
 *********************************    ***************************************/


#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "../../module/bool.h"
#include "../../module/timer/timer.h"
#include "fileoperation.h"


unsigned char c_appendstr_tofile(int fd, char *str, pthread_mutex_t *lock)
{
	if((fd < 0) \
		|| (str == NULL) \
		|| (lock == NULL))
	{
		return false;
	}
	uint time;
	int ret = 0;
	uint total = 0;
	uint len = strlen((char *)str);
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


