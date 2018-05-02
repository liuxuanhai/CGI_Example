/******************************************    *******************************
      > File Name: syslog.h
      > Author: Ywl
      > Descripsion:
      > Created Time:     Thu 30 Nov 2017 03:48:25 PM CST
      > Modify Time: 
 *********************************    ***************************************/

#ifndef _SYSLOG_H
#define _SYSLOG_H


/* 调试宏 */
#define SYS_PROGRAM_DEBUG(para)				{printf("[Debug]:>>>>>>file:%s, fun:%s, line%d, para:%s>>>>>>>>[End_Debug]\n", \
											__FILE__, __FUNCTION__, __LINE__, (char *)para);}
#define SYS_LOG_FILE_MAX_SIZE				(10*1024*1024)							/* 系统日志文件最大大小10M */

#define SYS_LOG_PATH                        "./system_log/"
#define CLEAN_SYSTEM_LOG                    "./system_log/clean_system_log.sh"
#define SYS_PROG_LOG_ERR_NO_TIME            "./system_log/programming_log_err_no_time.log"
#define SYS_PROG_LOG_ERR_TIME               "./system_log/programming_log_err_time.log"
#define SYS_LOG_HEART                       "./system_log/system_heart.log"


/* 系统错误日志的标准格式 */
int m_system_error_standard(char *str, char *file, const char *fun, unsigned int line);
/* 记录系统日志，不带时间 */
void m_system_log_notime(char *errStr);
/* 记录系统日志，带时间 */
void m_system_log_time(char *errStr);
/* 记录系统心跳日志 */
void m_system_heart_log(void);
/* 系统日志初始化 */
unsigned char m_system_log_initialize(void);
/* 系统日志退出 */
void m_close_system_log(void);
/* 日志大小,日期检测，并清理日志 */
void m_check_system_log(void);

#endif

