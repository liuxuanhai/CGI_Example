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

#include "sysconf.h"
#include "../../module/module.h"

pthread_mutex_t s_ini_lock = PTHREAD_MUTEX_INITIALIZER;

static FILE *ssys_confini_fd = NULL;

#define INI_LINE_MAX                250

static char s_file_buf[INI_LINE_MAX + 1];


/* 关闭配置文件 */
void c_close_confini(FILE *file)
{
    if(file)
    {
        fclose(file);
    }
    file = NULL;
}
FILE *c_open_confini(char *path)
{
    FILE *fd = NULL;
    if((fd = fopen(path, "r+")) == NULL)
    {
        if((fd = fopen(path, "w+")) == NULL)
        {
            return NULL;
        }
    }
    return fd;
}
/* 初始化配置文件 */
FILE *c_init_confini(char *path)
{
    return c_open_confini(path);
}

/* 读取配置文件 */
unsigned char c_get_confini(FILE *file, char *section, char *keyId, char *buf, unsigned int len, unsigned char type)
{
	return true;
}
int IniFindSection(FILE* file, char *section)
{
    unsigned int len;
    char *ptr;
    int pos;

    if(file == NULL)
    {
        return -1;
    }
    fseek(file, 0, SEEK_SET);
    len = strlen(section);
    pos = ftell(file);
    while((ptr = fgets(s_file_buf, INI_LINE_MAX, file)) != NULL)
    {
        if((*ptr == '[') && (strlen((char *)ptr) >= len + 2))
        {
            ptr++;
            if((*(ptr + len) == ']') && (memcmp(ptr, section, len) == 0))
            {
                return pos;
            }
        }
        pos = ftell(file);
    }
    return -1;  //找不到
}
int FindKey(FILE* file, char *key, unsigned int pos)
{
    int len;
    char *ptr;

    if(file == NULL)
    {
        return -1;
    }
    len = strlen(key);
    while((ptr = fgets(s_file_buf, INI_LINE_MAX, file)) != NULL)
    {
        if(*ptr == '[')     //遇到下一段
        {
            break;
        }
        if((strlen(ptr) >= len) && (*ptr != '\r') && (*ptr != '\n') && (*ptr != ';'))
        {
            if(memcmp(ptr, key, len) == 0)
            {
                if(*(ptr + len) == '=')
                {
                    return pos;     //返回文件位置
                }
            }
        }
        pos = ftell(file);
    }
    return -1;
}
/* 写入配置文件 */
unsigned char c_set_confini(FILE *file, char *section, char *keyId, char *buf, unsigned int len, unsigned char type)
{
    int pos;
    int fd;
    int size, cur;
    char *ptr;

    pthread_mutex_lock(&s_ini_lock);
    if(IniFindSection(file, section) != -1)
    {
        pos = ftell(file);
        if((pos = FindKey(file, keyId, pos)) != -1)
        {
            cur = ftell(file);
            fseek(file, 0, SEEK_END);      //文件尾
            size = ftell(file);        //剩余文件大小
            if(size > cur)     //暂存剩余文件
            {
                ptr = m_memory_alloc(size - cur);
                fseek(file, cur, SEEK_SET);
                fread(ptr, 1, size - cur, file);
            }
            else
            {
                ptr = NULL;
            }
            sprintf(s_file_buf, "%s=", keyId);
            Val2Str(s_file_buf + strlen(s_file_buf), buf, len, type);
            strcat(s_file_buf, "\r\n");
            fseek(file, pos, SEEK_SET);
            fputs(s_file_buf, file);
            if(ptr)
            {
                fwrite(ptr, 1, size - cur, file);
                free(ptr); 
            }
            size = ftell(file);        //新的文件大小
            fflush(file);
            fd = fileno(file);
            ftruncate(fd, size);
            fsync(fd);
            printf("修改%s的%s", section, s_file_buf);
            pthread_mutex_unlock(&s_ini_lock);
            return 0;
        }
        else
        {
            printf("找不到关键字%s\r\n", keyId);
        }
    }
    else
    {
        printf("找不到段%s\r\n", section);
    }

    pthread_mutex_unlock(&s_ini_lock);
    return -1;
    return true;
}


/* 关闭配置文件 */
void c_closesys_confini(void)
{
    c_close_confini(ssys_confini_fd);
}

/* 打开配置文件 */
static unsigned char s_opensys_confini(void)
{
    if(NULL == (ssys_confini_fd = c_init_confini(SYS_CONFIG_INI)))
    {
        return false;
    }
    return true;
}
/* 初始化配置文件 */
unsigned char s_initsys_confini(void)
{
    return s_opensys_confini();
}
