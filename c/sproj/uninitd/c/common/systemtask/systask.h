/******************************************    *******************************
      > File Name: systask.h
      > Author: Ywl
      > Descripsion:
      > Created Time:     Thu 30 Nov 2017 06:25:51 PM CST
      > Modify Time: 
 *********************************    ***************************************/

#ifndef _SYSTASK_H
#define _SYSTASK_H


#define TASK_ID_MAX			64				/* 最大任务号 */

#define TASK_DIE_LEN		8				/* 任务状态字节数 */
#define TASK_CNT_MAX		5				/* 任务最大超时次数 */

/* 任务ID号 */
enum {
	TASK_MAIN_ID = 0,							/* 主任务id */
	TASK_ALM_ID = 1,							/* 告警任务 */
	TASK_EMAIL_ID = 2,							/* 邮件任务 */
	TASK_LAST_ID
};

/* 任务注册 */
void c_task_register(unsigned char id, const char *name, unsigned int life);
/* 任务更新 */
void c_task_update(unsigned char id);
/* 任务池检测 */
void c_check_task_queue(void);

unsigned char c_system_initialize(void);
void c_system_main_task(void);
void c_task_exit();

#endif

