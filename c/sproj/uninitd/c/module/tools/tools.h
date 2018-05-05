/******************************************    *******************************
      > File Name: tools.h
      > Author: Ywl
      > Descripsion: 一些工具
      > Created Time:     Thu 30 Nov 2017 02:41:55 PM CST
      > Modify Time: 
 *********************************    ***************************************/

#ifndef _TOOLS_H
#define _TOOLS_H

#define PACKED								__attribute__ ((packed))					/* 结构体对齐 */

/*常用宏函数定义*/
#define ARRAY_SIZE(a)						(sizeof((a)) / sizeof((a)[0]))				/* 数组总个数 */
#define GROUP_NUM(g, s)						(((g) + (s) - 1) / (s))						/* g:总个数, s:每组大小, 返回:分组数 */
#define MAXNUM(x, y)						(((x) > (y)) ? (x) : (y))					/* 最大值 */
#define MINNUM(x, y)						(((x) < (y)) ? (x) : (y))					/* 最小值 */
#define OFF_SET_OF(TYPE, MEMBER)			((unsigned int)&((TYPE *)0)->MEMBER)			/* 返回结构体中一个成员
																						   相对第一个成员的位置
																						   返回值为一个INT32U类型*/
#define STACK_TOP(stack)					(&stack[ARRAY_SIZE(stack)-1])				/* 数组中最后一个元素 */
#define BIT(n)								(1ul << (n))								/* 取bit0~31 中的某位 */



unsigned char m_system_cmd(const char *cmd, const char *args);
void m_system_reboot(void);
void *m_memory_alloc(unsigned int size);
void *m_memory_realloc(unsigned int now_size, void *old, void *addr, unsigned int len);
unsigned short m_calculate_crc16(unsigned short crc, unsigned char *ptr, unsigned int len);
/* 多个字节转换成无符号整型数 */
unsigned int s_byte_to_int(const unsigned char *byte, unsigned int len);
/* 多个字节转换成有符号整型数 */
int s_byte_to_sint(const unsigned char *byte, unsigned int len);
char *m_digit_to_ascii(char *ascii, const unsigned char *digit, unsigned int len);
unsigned int m_ascii_to_hex(unsigned char *hex, const char *ascii, unsigned int len);

#define m_byte_to_word(byte)			s_byte_to_int(byte, 2)			/* 两个字节的数组转换成整型数 */
#define m_byte_to_int24(byte)	    	s_byte_to_int(byte, 3)			/* 三个字节的数组转换成整型数 */
#define m_byte_to_long(byte)			s_byte_to_int(byte, 4)			/* 四个字节的数组转换成整型数 */

#endif

