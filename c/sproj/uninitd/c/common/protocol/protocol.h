/******************************************    *******************************
  > File Name: Protocol.h
  > Author: Ywl
  > Descripsion:
  > Created Time:     Mon 04 Dec 2017 01:54:05 PM CST
  > Modify Time: 
 *********************************    ***************************************/

#ifndef _PROTOCOL_H
#define _PROTOCOL_H

/*******************************************协议包类型******************************************************/
/*系统级*/
#define SYS_LINUX_ID			0x01					/* 通用Linux系统数据 */
#define SYS_UBUNTUOS_ID			0x02					/* Ubuntu系统数据 */
#define SYS_CENTOS_ID			0x03					/* CentOS系统数据 */
#define SYS_WINDOWS_ID			0x04					/* Windows系统数据 */
#define SYS_ANDROID_ID			0x05					/* Android系统数据 */
#define SYS_IOS_ID				0x06					/* IOS系统数据 */

/*语言级*/
#define LANG_C_ID				0x01					/* C语言数据 */
#define LANG_CPP_ID				0x02					/* C++语言数据 */
#define LANG_JAVA_ID			0x03					/* JAVA语言数据 */
#define LANG_SHELL_ID			0x04					/* Shell语言数据 */
#define LANG_PYTHON_ID			0x05					/* Python语言数据 */
#define LANG_JS_ID				0x06					/* JavaScript语言数据 */
#define LANG_QT_ID				0x07					/* QT语言数据 */
#define LANG_GO_ID				0x08					/* GO语言数据 */
#define LANG_PHP_ID				0x09					/* PHP语言数据 */
#define LANG_KOTLIN_ID			0x0A					/* Kotlin语言数据 */

/*模块级*/
#define MODULE_CGI_ID			0x01					/* CGI数据 */
#define MODULE_EMAIL_ID			0x02					/* 邮件数据 */
#define MODULE_MSCOM_ID			0x03					/* 主从通信 */

#define TEL_LEN_MIN					4			// 有效电话号码最小长度
#define TEL_MAX_LEN					20			// 电话号码最长20 位

enum {
	PACKET_ONLY,		// 没有头部
	HEAD_INCLUDE,		// 移动包中包含头部
};

/*********************************************************移动数据协议*****************************************************************************************/
#define CMD_HEAD_SIZE			13				// 从apId 到cmdAck 的字节数

#define SMS_MO_MAX				(140 - 32) / 2	// 短信监控对象允许的最大长度
#define MCPA_MO_MAX				255				// MCPA 监控对象允许的最大长度

/*****************************命令标志***************************************************/
#define CMD_ACK						0xFF		/*初始包*/
#define CMD_OK						0x01		/*命令正常*/
/*****************************读写标志***************************************************/
#define READ_RW						0x01		/*读数据*/
#define WRTIE_RW					0x02		/*写数据*/


typedef struct{
	uint16_t packLen;					/* 包长 */
	uint8_t ip[4];					/* ip地址 */
	uint16_t port;					/* 端口 */
	uint8_t sysType;					/* 系统类型 */
	uint8_t langType;					/* 语言类型 */
	uint8_t moduleType;				/* 模块类型 */
	uint8_t src;						/* 来源端口 */
	uint8_t original;					/* 最初的来源端口 */
	uint8_t dest;						/* 目标端口 */
	uint16_t pktSn;					/* 包序号 */
	uint16_t cmdId;					/* 命令id,查询设置 */
	uint8_t cmdAck;					/* 命令标志,错误类型 */
	uint16_t dataLen;					/* 数据长度 */
	uint8_t pdu[0];					/* 数据段 */
}PACKED c_class_common_net_protocol;

#define PACKET_MAX_LEN				4096				/* 一个包最大长度 */
#define c_class_common_net_protocol_LEN			(sizeof(c_class_common_net_protocol))
/*监控对象*/
typedef struct {
	uint8_t len;				/* 监控对象长度 */
	uint16_t id;				/* 监控对象标号 */
	uint8_t pdu[1];			/* 监控对象内容(变长) */
}PACKED c_class_mcpa_mo;

typedef struct
{
	uint32_t len;				/* 数据长度 */
	uint8_t *data;			/* 指向数据首地址 */
}c_class_unpack;

typedef struct
{
	uint32_t len;				/* 数据长度 */
	uint8_t data[1];			/* 数据首地址 */
}c_class_buf;



/*通用组包*/
c_class_common_net_protocol *c_common_packet(uint32_t len);
/*通用解包*/
c_class_common_net_protocol *c_common_unpacket(int8_t *data, uint32_t len);
/*底层数据打包，加头尾，加校验*/
c_class_buf *c_base_packet(c_class_common_net_protocol *packet);
/*底层数据解包，去头尾，去校验*/
c_class_common_net_protocol *c_base_unpacket(c_class_unpack *unpack);
/*计算转义长度*/
uint32_t c_ap_escape_len(uint8_t *ptr, uint32_t len);
/*数据转义*/
uint8_t *c_ap_escape(uint8_t *dest, uint8_t *src, uint32_t len);
/*系统平台区分*/
uint8_t c_ckeck_system_type(c_class_common_net_protocol *packet);

#endif

