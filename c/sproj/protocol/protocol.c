/******************************************    *******************************
      > File Name: Protocol.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     Mon 04 Dec 2017 01:52:26 PM CST
      > Modify Time: 
 *********************************    ***************************************/

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

#include "../../module/module.h"
#include "protocol.h"
/*通用组包*/
/*
len:需要申请的包长度
return:初始协议包
 */
c_class_common_net_protocol *c_common_packet(uint32_t len)
{
	if((len + c_class_common_net_protocol_LEN + 2 + 2) >= PACKET_MAX_LEN)					/*两个校验位,两个头尾标志*/
	{
		return NULL;
	}
	c_class_common_net_protocol *packet = (c_class_common_net_protocol *)m_memory_alloc(c_class_common_net_protocol_LEN + len);
	packet->cmdAck = CMD_ACK;
	packet->packLen = len + c_class_common_net_protocol_LEN;
	packet->dataLen = len;
	return packet;
}
/*通用解包*/
/*
data:数据
len:数据长度
 */
c_class_common_net_protocol *c_common_unpacket(int8_t *data, uint32_t len)
{
	return NULL;
}
/*计算转义长度
 * 0x5E 0x7E多站一个字节
 * ptr:数据头指针
 * len:数据长度
 * return:转义后的长度
 * */
uint32_t c_ap_escape_len(uint8_t *ptr, uint32_t len)
{
	uint32_t num;

	for(num = 0; len--; num++, ptr++)
	{
		if((*ptr == 0x5E) || (*ptr == 0x7E))
		{
			num++;
		}
	}
	return num;
}
/*
 * 数据转义
 * dest:目标数据
 * src:源数据
 * len:长度
 * return:转义后的数据
 * */
uint8_t *c_ap_escape(uint8_t *dest, uint8_t *src, uint32_t len)
{
	for(; len--; src++)
	{
		if (*src == 0x5E)
		{
			*dest++ = 0x5E;
			*dest++ = 0x5D;
		}
		else if(*src == 0x7E)
		{
			*dest++ = 0x5E;
			*dest++ = 0x7D;
		}
		else
		{
			*dest++ = *src;
		}
	}
	return dest;
}
/*底层数据打包，加头尾，加校验*/
c_class_buf *c_base_packet(c_class_common_net_protocol *packet)
{
	uint16_t crc;
	uint32_t len;
	uint8_t *ptr;
	c_class_buf *s_buf;

	/*计算转义后的长度以便申请合适大小内存*/
	len = 2 + c_ap_escape_len((uint8_t *)&packet->packLen, packet->packLen);		/* 加上头尾标志2字节 */
	crc = m_calculate_crc16(0, (uint8_t *)&packet->packLen, packet->packLen);			/* 计算CRC */
	len += c_ap_escape_len((uint8_t *)&crc, 2);							/* 校验码的转义 */
	s_buf = (c_class_buf *)m_memory_alloc(len + sizeof(c_class_buf));
	s_buf->len = len;
	s_buf->data[0] = '~';
	ptr = &s_buf->data[1];
	ptr = c_ap_escape(ptr, (uint8_t *)&packet->packLen, packet->packLen);
	ptr = c_ap_escape(ptr, (uint8_t *)&crc, 2);
	*ptr = '~';
	return s_buf;
}
/*底层数据解包，去头尾，去校验*/
c_class_common_net_protocol *c_base_unpacket(c_class_unpack *unpack)
{
	uint8_t *dest;
	uint8_t *start, *tail, *end;
	uint32_t esc;
	uint32_t min_len;
	c_class_common_net_protocol *packet;

	/*最小长度 通用数据+头尾+校验*/
	min_len = c_class_common_net_protocol_LEN + 2 + 2;
	/*数据尾部指针*/
	end = unpack->data + unpack->len;
	/*数据头部指针*/
	start = unpack->data;
	while(end - unpack->data >= min_len)
	{
		/*从第一个查找到倒数第二个*/
		if((start = (uint8_t *)memchr(unpack->data, '~', end - unpack->data - 1)) == NULL)		// 没有起始标志
		{
			break;
		}
		start++;
		/*从第二个查找到最后一个*/
		if((tail = (uint8_t *)memchr(start, '~', end - start)) == NULL)		// 没有结束标志
		{
			break;
		}
		unpack->data = tail;
		unpack->len = end - unpack->data;
		if(tail - start >= min_len - 2)		// 最小长度
		{
			packet = (c_class_common_net_protocol *)m_memory_alloc(tail - start);
			dest = (uint8_t *)&packet->packLen;
			for(esc = false; start < tail; start++)
			{
				if(esc == false)
				{
					if(*start == 0x5E)		// 需要转义
					{
						esc = true;
					}
					else
					{
						*dest++ = *start;
					}
				}
				else
				{
					if((*start != 0x5D) && (*start != 0x7D))		// 0x5E 后面必须有0x5D 或0x7D
					{
						break;
					}
					esc = false;
					*dest++ = *start + 1;
				}
			}
			if(esc == false)		// 转义正确
			{
				{
					if(packet->packLen + 2 != dest - (uint8_t *)&packet->packLen)	//长度不对
					{
//						printf("包头长度%d, %d\r\n", packet->packLen, dest - (uint8_t *)&packet->packLen);
						free(packet);
						continue;
					}
				}
				printf("包长度%u\r\n", packet->packLen);
				if((packet->packLen >= min_len)		// 去掉前后的7E 和CRC 校验的长度
						&& (m_byte_to_word((uint8_t *)(&packet->packLen + packet->packLen)) \
							== m_calculate_crc16(0, (uint8_t *)&packet->packLen, packet->packLen)))
				{
					printf("CRC16校验正确\r\n");
					unpack->len--;
					unpack->data++;
					return packet;
				}
			}
			free(packet);
		}
	}
	return NULL;
}

static uint8_t LinuxOSCLangData(c_class_common_net_protocol *packet)
{
	return true;
}
static uint8_t LinuxOSCppLangData(c_class_common_net_protocol *packet)
{
	return true;
}
static uint8_t LinuxOSJavaLangData(c_class_common_net_protocol *packet)
{
	return true;
}
static uint8_t LinuxOSShellLangData(c_class_common_net_protocol *packet)
{
	return true;
}
static uint8_t LinuxOSPythonLangData(c_class_common_net_protocol *packet)
{
	switch(packet->moduleType)
	{
		case MODULE_EMAIL_ID:
			//PythonEmailUdpRecv(packet);
		default:
			return false;
	}
	return true;
}

/*Linux系统数据来源*/
static uint8_t LinuxOSLangData(c_class_common_net_protocol *packet)
{
	switch(packet->langType)
	{
		case LANG_C_ID:
			LinuxOSCLangData(packet);
		case LANG_CPP_ID:
			LinuxOSCppLangData(packet);
		case LANG_JAVA_ID:
			LinuxOSJavaLangData(packet);
		case LANG_SHELL_ID:
			LinuxOSShellLangData(packet);
		case LANG_PYTHON_ID:
			LinuxOSPythonLangData(packet);
		default:
			return false;
	}
	return true;
	
}
static uint8_t WindowsOSLangData(c_class_common_net_protocol *packet)
{
	return true;
}
static uint8_t AndroidOSLangData(c_class_common_net_protocol *packet)
{
	return true;
}
static uint8_t IosOSLangData(c_class_common_net_protocol *packet)
{
	return true;
}
/*系统平台数据来源*/
uint8_t c_ckeck_system_type(c_class_common_net_protocol *packet)
{
	switch(packet->sysType)
	{
		case SYS_LINUX_ID:
		case SYS_UBUNTUOS_ID:
		case SYS_CENTOS_ID:
			LinuxOSLangData(packet);
		case SYS_WINDOWS_ID:
			WindowsOSLangData(packet);
		case SYS_ANDROID_ID:
			AndroidOSLangData(packet);
		case SYS_IOS_ID:
			IosOSLangData(packet);
		default:
			return false;
	}
	return true;
}
