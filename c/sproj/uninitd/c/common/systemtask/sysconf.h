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

/* 系统配置 */
unsigned char c_initsys_confini(void);
void c_closesys_confini(void);
unsigned char c_setsys_confini(char *section, char *keyId, unsigned char *buf, unsigned int len, unsigned char type);
unsigned char c_getsys_confini(char *section, char *keyId, unsigned char *buf, unsigned int len, unsigned char type);

#endif
