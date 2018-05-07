/******************************************    *******************************
      > File Name: tools.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     Thu 30 Nov 2017 10:30:32 PM CST
      > Modify Time: 
 *********************************    ***************************************/

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>

#include <linux/types.h>

#include "../bool.h"
#include "tools.h"

/* 申请内存 */
/*
size:申请的内存大小
return:内存首地址
 */
void *m_memory_alloc(unsigned int size)
{
	void *addr;
    int memcount = 200*1000;    /* 1秒钟申请不到返回 */
	while((addr = malloc(size)) == NULL)
	{
        if(!memcount)
            break;
		printf("申请不到内存\r\n");
        usleep(500*1000);
        memcount--;
	}
	memset(addr, 0, size);
	return addr;
}
/* 重新分配内存，增加内存 */
/*
new_size:重新分配的内存大小
old:释放的内存首地址
addr:新内存首地址
len:需要保存的内存内容长度
return:新内存首地址
 */
void *m_memory_realloc(unsigned int new_size, void *old, void *addr, unsigned int len)
{
	void *new_ptr;
	if(NULL == (new_ptr = m_memory_alloc(new_size)))
        return new_ptr;
	if(len)
	{
		memcpy(new_ptr, addr, (new_size < len) ? new_size : len);
	}
	free(old);
	return new_ptr;
}

#define dbg		0
/*执行系统命令:
cmd:命令名称
args:命令参数
return:执行结果返回
 */
unsigned char  m_system_cmd(const char *cmd, const char *args)
{
	if(cmd == NULL)
		return false;
	pid_t status;
	char cmd_or_output[255];
	memset(cmd_or_output, 0, sizeof(cmd_or_output));
	if(NULL != args)
		sprintf(cmd_or_output, "%s %s", cmd, args);
	else
		sprintf(cmd_or_output, "%s", cmd);
	memset(cmd_or_output, 0, sizeof(cmd_or_output));
	if((status = system(cmd_or_output)) == -1)
	{
		sprintf(cmd_or_output, "system:%s\r\n", strerror(errno));
#if dbg
		printf("%s", cmd_or_output);
#endif
	}
	else
	{
		if(WIFEXITED(status))
		{
			if(0 == WEXITSTATUS(status))
			{
				sprintf(cmd_or_output, "脚本执行成功[%d,%d,0x%X]\r\n", errno, WEXITSTATUS(status), status);
#if dbg
				printf("%s", cmd_or_output);
#endif
				return true;
			}
			sprintf(cmd_or_output, "脚本执行失败[%d,%d,0x%X]\r\n", errno, WEXITSTATUS(status), status);
#if dbg
			printf("%s", cmd_or_output);
#endif
		}
		else
		{
			sprintf(cmd_or_output, "错误退出[%d,%d,0x%X]\r\n", errno, WEXITSTATUS(status), status);
#if dbg
			printf("%s", cmd_or_output);
#endif
		}
	}
	return false;
}
#undef dbg
/*系统重启*/
void m_system_reboot(void)
{
	while(1)
	{
        sleep(2);
		m_system_cmd("reboot now", NULL);
	}
}
static const unsigned short s_crc16_table[] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};
/*CRC16校验
 * crc:上次校验码
 * ptr:校验数据指针
 * len:校验数据长度
 * return:校验码
 * */
unsigned short m_calculate_crc16(unsigned short crc, unsigned char *ptr, unsigned int len)
{
	unsigned char temp;

    if(len <= 0 || NULL == ptr)
    {
        return false;
    }
	for( ; len--; ptr++)
	{
		temp = crc / 256;
		crc <<= 8;								/* 左移8位 */
		crc ^= s_crc16_table[temp ^ *ptr];			/* 高8位和当前字节相加后再查表求CRC ，再加上以前的CRC */
	}
	return crc;
}
/* 多个字节转换成无符号整型数 */
unsigned int s_byte_to_int(const unsigned char *byte, unsigned int len)
{
	unsigned int tmp;

	if(len > 4)
	{
		len = 4;
	}
	for(tmp = 0 ; len--; )
	{
		tmp = (tmp << 8) + *(byte + len);
	}
	return tmp;
}

/* 多个字节转换成有符号整型数 */
int s_byte_to_sint(const unsigned char *byte, unsigned int len)
{
	unsigned int tmp;

	if(len > 4)
	{
		len = 4;
	}
	for(tmp = 0 ; len--; )
	{
		tmp = (tmp << 8) + *(byte + len);
	}
	return tmp;
}

static const char HexAscii[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

unsigned int m_ascii_to_hex(unsigned char *hex, const char *ascii, unsigned int len)
{
    unsigned int num;
    unsigned int ch;
    unsigned int flag;

    if(len == 0)
    {
        while(isxdigit(*(ascii + len)))
        {
            len++;
        }
    }
    if((num = len & 0x01) == 1)
    {
        len++;
        flag = true;
    }
    else
    {
        flag = false;
    }
    for(; num < len; num++)
    {
        ch = *ascii++;
        if((ch >= '0') && (ch <= '9'))
        {
            ch -= '0';
        }
        else if((ch >= 'A') && (ch <= 'F'))
        {
            ch -= 'A' - 0x0A;
        }
        else if((ch >= 'a') && (ch <= 'f'))
        {
            ch -= 'a' - 0x0A;
        }
        else
        {
            break;      // 遇到非法字符,转换提前结束
        }
        if(num & 0x01)
        {
            if(flag)
            {
                flag = false;
                *hex = 0;
            }
            *hex |= ch;         //低半字节
            hex++;
        }
        else
        {
            *hex = ch << 4;     //高半字节
        }
    }
    return num / 2;
}

char *m_digit_to_ascii(char *ascii, const unsigned char *digit, unsigned int len)
{
    for( ; len--; digit++)
    {
            *ascii++ = HexAscii[*digit >> 4];
            *ascii++ = HexAscii[*digit & 0x0F];
        }
    return ascii;
}
