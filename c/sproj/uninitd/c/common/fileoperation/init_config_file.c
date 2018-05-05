/******************************************    *******************************

      > File Name: init_config_file.c

      > Author: Ywl

      > Descripsion:

      > Created Time:     Sat 05 May 2018 01:43:36 AM PDT

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
#include "../protocol/protocol.h"
#include "init_config_file.h"

pthread_mutex_t s_ini_lock = PTHREAD_MUTEX_INITIALIZER;
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

static int s_ini_find_section(FILE* file, char *section)
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
static int s_find_key(FILE* file, char *key, unsigned int pos)
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
unsigned char c_init_module_parametr(FILE *file, c_class_parameter_map *module)
{

}
/* 读取配置文件 */
unsigned char c_get_confini(FILE *file, char *section, char *keyId, unsigned char *buf, unsigned int len, unsigned char type)
{
    char *str;
    int pos;

    if(file == NULL)
    {
        return false;
    }
    pthread_mutex_lock(&s_ini_lock);
    if(s_ini_find_section(file, section) != -1)
    {
        printf("找到关键字%s\r\n", section);
        pos = ftell(file);
        if((pos = s_find_key(file, keyId, pos)) != -1)
        {
            fseek(file, pos, SEEK_SET);
            str = fgets(s_file_buf, INI_LINE_MAX, file);
            pos = strlen(str);
            if((str[pos - 1] == '\r') || (str[pos - 1] == '\n'))
            {
                str[pos - 1] = 0;   
            }
            if((str[pos - 2] == '\r') || (str[pos - 2] == '\n'))
            {
                str[pos - 2] = 0;   
            }
            str += strlen(keyId);
            if(*str == '=')
            {
                str++;
                if(c_str_to_val(buf, len, type, str) == 0)
                {
                    pthread_mutex_unlock(&s_ini_lock);
                    return false;
                }
            }
        }
    }
    pthread_mutex_unlock(&s_ini_lock);
    if(type == TYPE_STR)    // 读取失败,字符串长度设为0
    {
        *buf = 0;
    }
    return false;
}
/* 写入配置文件 */
unsigned char c_set_confini(FILE *file, char *section, char *keyId, unsigned char *buf, unsigned int len, unsigned char type)
{
    int pos;
    int fd;
    int size, cur;
    char *ptr;

    pthread_mutex_lock(&s_ini_lock);
    if(s_ini_find_section(file, section) != -1)
    {
        pos = ftell(file);
        if((pos = s_find_key(file, keyId, pos)) != -1)
        {
            cur = ftell(file);
            fseek(file, 0, SEEK_END);
            size = ftell(file); 
            if(size > cur)  
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
            c_val_to_str(s_file_buf + strlen(s_file_buf), buf, len, type);
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
            return true;
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
    return false;
}


