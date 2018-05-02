/******************************************    *******************************

      > File Name: sysconf.h

      > Author: Ywl

      > Descripsion:

      > Created Time:     Sat 28 Apr 2018 12:42:21 AM PDT

      > Modify Time: 

 *********************************    ***************************************/

#ifndef _SYSCONF_H
#define _SYSCONF_H

#define SYS_CONFIG_INI      "./system_config/system_config.ini"

#define INI_LINE_MAX                250

/* 初始化配置文件 */
FILE *c_init_confini(char *path);
FILE *c_open_confini(char *path);
/* 关闭配置文件 */
void c_close_confini(FILE *file);
/* 读取配置文件 */
unsigned char c_get_confini(FILE *file, char *section, char *keyId, char *buf, unsigned int len, unsigned char type);
/* 写入配置文件 */
unsigned char c_set_confini(FILE *file, char *section, char *keyId, char *buf, unsigned int len, unsigned char type);

/* 系统配置 */
unsigned char c_initsys_confini(void);
void c_closesys_confini(void);
unsigned char c_setsys_confini(char *section, char *keyId, char *buf, unsigned int len, unsigned char type);
unsigned char c_getsys_confini(char *section, char *keyId, char *buf, unsigned int len, unsigned char type);

#endif
