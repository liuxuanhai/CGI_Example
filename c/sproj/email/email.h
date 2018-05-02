/******************************************    *******************************
      > File Name: Email.h
      > Author: Ywl
      > Descripsion:
      > Created Time:     Wed 13 Dec 2017 10:19:40 AM CST
      > Modify Time: 
 *********************************    ***************************************/

#ifndef _EMAIL_H
#define _EMAIL_H

#define EMAIL_UDP_SERVER_PORT				11316				/* UDP邮箱数据端口号 */
#define EMAIL_UDP_CLIENT_PORT				11317				/* python邮箱UDP端口 */

#define EMAIL_HEART_PACK					0x01				/* 心跳包 */
#define EMAIL_NORMAL_PACK					0x02				/* 正常回复包 */

typedef struct{
	uint8_t type;						/* 内部类型 */
	uint8_t mailheadType;				/* 头部标志 */
	uint16_t HeadLen;			/* 头部数据长度, 从type到头部结尾*/
	/*如果是心跳包则type不同而已，pdu存放主机名,时间
	 :<hostName:xxx:emailTime:xxx>:*/
	/*一下字段以类型:内容格式组包
:<sendtime:xxx:mailhost:xxx:mailuser:xxx:mailpass:xxx:sender:xxx:senderNickName>:
	 */
#if 0
	uint32_t sendtime;					/* 发送时间，单位为秒 */
	/*写个长度好了,以特殊字符区分数据mailhost:*/
	uint8_t mailhost[25];				/* 邮箱服务器 */
	uint8_t mailuser[25];				/* 发送邮箱 */
	uint8_t mailpass[16];				/* 邮箱口令 */
	uint8_t sender[64];				/* 发送人 */
	uint8_t senderNickName[64];		/* 发送人昵称 */
#endif
	uint8_t pdu[0];					/* 下个字段 */
}PACKED S_EMAIL_HEAD;

typedef struct{
	uint8_t recversType;				/* 接收人字段 */
	uint16_t recvsLen;				/* 收件人字段长度,recversType到字段末尾 */
	uint8_t recvsNum;					/* 收件人数量 */
	/*收件人有多个使用如下打包方式:
	 :<mailrecvers:xxx:recvers:xxx:recversNickName:xxx>:<...>:*/
#if 0
	uint8_t mailrecvers[25];			/* 接收邮箱 */
	uint8_t recvers[64];				/* 接收人 */
	uint8_t recversNickName[64];		/* 接收人昵称 */
#endif
	uint8_t pdu[0];					/* 下个字段 */
}PACKED S_EMAIL_RECVS;
/*抄送字段*/
/*附件名*/
typedef struct{
	uint8_t annexType;				/* 附件字段 */
	uint16_t annexLen;				/* 附件名长度 */
	uint8_t annexNum;					/* 附件数量 */
/*格式如下:
	:<annex:xxx>:<annex:xxx>:<...>:*/
//	uint8_t annex[128];				/* 附件名 */
	uint8_t pdu[0];					/* 下个字段 */
}PACKED S_EMAIL_ANNEX;
/*正文加图片*/
typedef struct{
	uint8_t imageType;				/* 图片字段 */
	uint16_t imageLen;
	uint8_t imageNum;					/* 图片数量 */
	/*格式如下:
	 :<imageId:xxx:imageName:xxx>:<...>:*/
//	uint8_t imageId;					/* 图片编号,相同编号的图片位于文字上方 */
	uint8_t pdu[0];					/* 下个字段 */
}PACKED S_EMAIL_IMAGES;
typedef struct{
	uint8_t textType;					/* 文字字段 */
	uint16_t textLen;
	uint8_t textNum;					/* 文字数量 */
	/*格式如下:
	 :<textId:xxx:textText:xxx>:<...>:*/
//	uint8_t textId;					/* 文字编号,相同编号的文字位于图片下方 */
	uint8_t pdu[0];					/* 下个字段 */
}PACKED S_EMAIL_TEXT;
typedef struct {
	uint8_t heartType;
	uint16_t heartLen;
	/*
	 * :<hostname:xxx:time>:
	 * */
	uint8_t pdu[0];
}PACKED S_EMAIL_HEART;
/*解析email UDP数据*/
void EmailUdpRecv(int8_t *data, int32_t len, int8_t *ip, short port);
void EmailTaskInit(void);
void PythonEmailUdpRecv(S_COMMON_DATA *packet);

#endif

