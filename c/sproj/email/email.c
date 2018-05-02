/******************************************    *******************************
      > File Name: Email.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     Wed 13 Dec 2017 10:18:34 AM CST
      > Modify Time: 
 *********************************    ***************************************/

#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../../module/module.h"
#include "../systemtask/systask.h"
#include "../protocol/protocol.h"
#include "email.h"

typedef struct {
	uint8_t type_id;
	uint16_t len;
	uint8_t pdu[0];
}PACKED S_EMAIL_TEST;
const int8_t *CstEmHeartData[] = {
	"hostname",
	"time",
	NULL
};

int32_t EmailUdpFd = -1;
int8_t EmailBuf[PACKET_MAX_LEN];

/*查找字符并填充结构体*/
static uint8_t emailStrSearch(int8_t *start, uint16_t len, uint8_t num, const int8_t **keyword, uint8_t ***beload)
{
	int8_t buf[30];
	int8_t *ptr = start;
	uint8_t offset = 0;
	uint8_t inoffset = 0;
	int8_t *head, *tail;
	uint8_t inlen;
	uint8_t index = 0;
	uint8_t ptrindex = 0;
	for(index = 0; index < num; index++)
	{
		for(ptrindex = 0; keyword[ptrindex] != NULL; ptrindex++)
		{
			if((ptr = strstr(start+offset, keyword[ptrindex])) != NULL)			/* 找到字符串 */
			{
				offset = ptr - start + strlen(keyword[ptrindex]);				/*偏移位置,':'的地址*/
				/*字符串的末尾应该是':'，否则认为是内容部分*/
				if(*(ptr+strlen(keyword[ptrindex])) != ':')
				{
					/*内容部分则重新查找本字符串*/
					ptrindex--;
					continue;
				}
				head = ptr;												/*记录查找位置*/
				if(keyword[ptrindex+1] != NULL)							/*下个字符串关键字不为空*/
				{
					for(inoffset = 0;;)
					{
						/*上次位置之后开始查找，避免只有一个关键字的情况*/
						if((ptr = strstr(start+offset+inoffset, keyword[ptrindex+1])) != NULL)		/* 找到本段下个字符串 */
						{
							inoffset = ptr - start + strlen(keyword[ptrindex+1]);
							if(*(ptr+strlen(keyword[ptrindex+1])) != ':')
							{
								continue;
							}
							tail = ptr;								/*记录下个字符串查找位置*/
							offset = ptr - start;					/*偏移为下个字符串起始位置*/
							inlen = strlen(keyword[ptrindex]) + 1;	/*内容相对起始位置*/
							memset(buf, 0, sizeof(buf));
							memcpy(buf, (head + inlen), tail - (head + inlen));		/* 差值为内容长度 */
							printf("buf:%s\n", buf);
			//				memcpy(beload[index][ptrindex], (head + inlen), tail - (head + inlen));		/* 差值为内容长度 */
							break;
						}
						else
						{
							break;
						}
					}
				}
				else if(keyword[ptrindex+1] == NULL)				/*查完一组*/
				{
					for(inoffset = 0;;)
					{
						if((ptr = strstr(start+offset+inoffset, keyword[0])) != NULL)			/* 存在下个段 */
						{
							/*下个段起始判断*/
							if((*(ptr - 1) != '<') || (*(ptr - 2) != ':') || (*(ptr - 3) != '>'))
							{
								inoffset = ptr - start + strlen(keyword[0]);
								continue;
							}
							tail = ptr - 3;							/*上个段末尾*/
							offset = ptr - start;
							inlen = strlen(keyword[ptrindex]) + 1;	/*内容相对起始位置*/
				//			memcpy(beload[index][ptrindex], (head + inlen), tail - (head + inlen));		/* 差值为内容长度 */
							memset(buf, 0, sizeof(buf));
							memcpy(buf, (head + inlen), tail - (head + inlen));		/* 差值为内容长度 */
							printf("buf:%s\n", buf);
							break;
						}
						else
						{
							break;
						}
					}
				}
				else if((index + 1) == num)													/* 已经是最后一段最后一个字符串 */
				{
					tail = start + len - 1;														/*数据末尾'>'*/
					inlen = strlen(keyword[ptrindex]) + 1;										/*内容相对起始位置*/
				//	memcpy(beload[index][ptrindex], (head + inlen), tail - (head + inlen));		/* 差值为内容长度 */
							memset(buf, 0, sizeof(buf));
							memcpy(buf, (head + inlen), tail - (head + inlen));		/* 差值为内容长度 */
							printf("buf:%s\n", buf);
				}
			}
		}
	}
	if(index != num)
	{
		return false;
	}
	return true;
}
static void emailHeartInfoUnpack(c_class_buf *c_class_buf)
{
	if(c_class_buf->data[0] != '<')
	{
		return;
	}
}
static uint8_t emailParaNumCheck(uint8_t *data, uint16_t datalen, uint8_t num)
{
	uint8_t *start, *tail;
	uint8_t realNum = 0;
	start = data;
	tail = data;
	for(;(tail - data + 1) < datalen;)
	{
		if(((start = (uint8_t *)memchr(data, '<', datalen - (tail - data))) != NULL) \
				&& ((start - data + 1) < datalen))		/* 段落起始 */
		{
			if(((tail = (uint8_t *)memchr(data, '>', datalen - (start - data))) == NULL) \
					&& ((tail - data + 1) <= datalen) && ((tail - start) > 0))		/* 段落结束标志 */
			{
				realNum++;
			}
		}
	}
	if(realNum != num)
	{
		return false;
	}
	return true;
}
/*心跳上报包*/
static void emailHeartUnpack(c_class_common_net_protocol *packet, S_EMAIL_HEAD *email_pack)
{
	uint16_t heartlen;
	S_EMAIL_HEART *heartpack = NULL;
	c_class_buf *s_buf = NULL;
	heartpack= (S_EMAIL_HEART *)email_pack->pdu;
	if(heartpack->heartType != EMAIL_HEART_PACK)
	{
		//数据错误，报错
	}
	heartlen = heartpack->heartLen;
	if((heartpack->pdu[0] != ':') || (heartpack->pdu[heartlen-1] != ':'))
	{
		//数据错误，报错
	}
	if(emailParaNumCheck(heartpack->pdu, heartlen, 1) == false)
	{
		return;
	}
	s_buf = (c_class_buf *)m_memory_alloc(sizeof(c_class_buf)+(heartlen-2));
	memcpy(s_buf->data, &heartpack->pdu[1], heartlen-2);
	//先匹配字段数目
	emailHeartInfoUnpack(s_buf);
	free(s_buf);
}
/*邮件回复包*/
static void emailResultUnpack(S_EMAIL_HEAD *packet)
{

}
/*python邮件处理*/
void PythonEmailUdpRecv(c_class_common_net_protocol *packet)
{
	S_EMAIL_HEAD *email_pack = NULL;
	//uint16_t len = packet->dataLen;
	switch(packet->cmdAck)
	{
		case CMD_OK:
			email_pack = (S_EMAIL_HEAD *)packet->pdu;
			switch(email_pack->type)
			{
				case EMAIL_HEART_PACK:
					emailHeartUnpack(packet, email_pack);
				case EMAIL_NORMAL_PACK:
				default:
					break;
			}
		default:
			break;
	}
}

void EmailUdpRecv(int8_t *data, int32_t len, int8_t *ip, short port)
{
	c_class_unpack *unpack = NULL;
	c_class_common_net_protocol *packet = NULL;
	unpack->len = len;
	unpack->data = (uint8_t *)data;
	if((packet = c_base_unpacket(unpack)))
	{
		c_ckeck_system_type(packet);
	}
	free(packet);
}
static void EmailSyncPacket(void)
{
	/*同步协议包*/
}
static void EmailTaskStart(void)
{
	const int8_t *ptr = __FUNCTION__;
	int32_t len;
	struct sockaddr_in sin;
	S_EMAIL_TEST *email_test;
	int8_t buf[100] = "<hostname:CentOS:time:2017>";
	emailStrSearch(buf, strlen(buf), 1, CstEmHeartData, NULL);
	for(c_task_register(TASK_EMAIL_ID, ptr, 120); ; )
	{
		c_task_update(TASK_EMAIL_ID);
		if(EmailUdpFd == -1)
		{
			if((EmailUdpFd = m_udpsock_create(NULL, NULL, EMAIL_UDP_SERVER_PORT, 2, 5)) < 0)
			{
				m_ostime_delay(OS_SEC(2));
				continue;
			}
		}
		EmailSyncPacket();
		memset(EmailBuf, 0, sizeof(EmailBuf));
		len = sizeof(struct sockaddr);
		if((len = recvfrom(EmailUdpFd, EmailBuf, sizeof(EmailBuf), 0, (struct sockaddr *)&sin, (socklen_t *)&len)) > 0)
		{
			printf("Recv Email:%s\n", EmailBuf);
			email_test = (S_EMAIL_TEST *)malloc(sizeof(S_EMAIL_TEST) + len);
			memset(email_test, 0, (sizeof(S_EMAIL_TEST) + len));
			email_test->type_id = 11;
			email_test->len = len;
			memcpy(email_test->pdu, EmailBuf, len);
		//	MasterUdpRev(MasterBuf, len, (INT8U *)&sin.sin_addr.s_addr, ntohs(sin.sin_port));
			m_udpsock_send(EmailUdpFd, (int8_t *)&sin.sin_addr.s_addr, ntohs(sin.sin_port), (int8_t *)email_test, \
								email_test->len+sizeof(S_EMAIL_TEST));
		}
		else if(len == -1)
		{
			if(errno != EAGAIN)
			{
				printf("[Email]recvfrom:%s\r\n", strerror(errno));
				close(EmailUdpFd);
				EmailUdpFd = -1;
				m_ostime_delay(OS_SEC(2));
				continue;
			}
		}
		//TaskPollCheck();
		//OSTimeDelay(OS_SEC(1));
	}
}

void EmailTaskInit(void)
{
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, (void *)EmailTaskStart, NULL);
}

