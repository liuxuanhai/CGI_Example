/******************************************    *******************************
      > File Name: fileoperation.h
      > Author: Ywl
      > Descripsion: 各种文件操作
      > Created Time:     Fri 01 Dec 2017 05:02:00 PM CST
      > Modify Time: 
 *********************************    ***************************************/

#ifndef _FILEOPERATION_H
#define _FILEOPERATION_H

/* 追加字符串到文件末尾 */
unsigned char c_appendstr_tofile(int fd, char *str, pthread_mutex_t *lock);

#endif

