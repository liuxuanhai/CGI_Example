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
uint8_t c_appendstr_tofile(int32_t fd, int8_t *str, pthread_mutex_t *lock);
/* 打开配置文件 */
uint8_t c_opensys_confini(void);
/* 关闭配置文件 */
uint8_t c_closesys_confini(void);
/* 初始化配置文件 */
uint8_t c_initsys_confini(void);
/* 读取配置文件 */
uint8_t c_getsys_confini(int8_t *section, int8_t *keyId, int8_t *buf, uint32_t len, uint8_t type);
/* 写入配置文件 */
uint8_t c_setsys_confini(int8_t *section, int8_t *keyId, int8_t *buf, uint32_t len, uint8_t type);

#endif

