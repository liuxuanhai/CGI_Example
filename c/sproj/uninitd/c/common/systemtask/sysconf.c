/******************************************    *******************************

      > File Name: sysconf.c

      > Author: Ywl

      > Descripsion:

      > Created Time:     Sat 28 Apr 2018 12:41:42 AM PDT

      > Modify Time: 

 *********************************    ***************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>

#include "../../module/module.h"
#include "../usrlib/usrlib.h"
#include "../fileoperation/init_config_file.h"
#include "sysconf.h"

static FILE *ssys_confini_fd = NULL;

/* 关闭配置文件 */
void c_closesys_confini(void)
{
    c_close_confini(ssys_confini_fd);
}

/* 打开系统配置文件 */
static unsigned char s_opensys_confini(void)
{
    if(NULL == (ssys_confini_fd = c_init_confini(SYS_CONFIG_INI)))
    {
        return false;
    }
    return true;
}
/* 初始化系统配置文件 */
unsigned char s_initsys_confini(void)
{
    return s_opensys_confini();
}
