/******************************************    *******************************

      > File Name: init_config_file.h

      > Author: Ywl

      > Descripsion:

      > Created Time:     Sat 05 May 2018 01:43:30 AM PDT

      > Modify Time: 

 *********************************    ***************************************/

#ifndef _INIT_CONFIG_FILE_H
#define _INIT_CONFIG_FILE_H

#define INI_LINE_MAX                250

unsigned char c_init_module_parametr(FILE *file, c_class_parameter_map *module);

/* 初始化配置文件 */
FILE *c_init_confini(char *path);
FILE *c_open_confini(char *path);
/* 关闭配置文件 */
void c_close_confini(FILE *file);
/* 读取配置文件 */
unsigned char c_get_confini(FILE *file, char *section, char *keyId, unsigned char *buf, unsigned int len, unsigned char type);
/* 写入配置文件 */
unsigned char c_set_confini(FILE *file, char *section, char *keyId, unsigned char *buf, unsigned int len, unsigned char type);

#endif
