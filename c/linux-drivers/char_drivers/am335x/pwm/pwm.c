/******************************************    *******************************
      > File Name: pwm.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年10月30日 星期一 09时52分48秒
      > Modify Time: 
 *********************************    ***************************************/


#include <linux/module.h>  
#include <linux/types.h>  
#include <linux/fs.h>  
#include <linux/errno.h>  
#include <linux/mm.h>  
#include <linux/sched.h>  
#include <linux/init.h>  
#include <linux/cdev.h>  
#include <asm/io.h>  
#include <asm/uaccess.h>  
#include <linux/timer.h>  
#include <asm/atomic.h>  
#include <linux/slab.h>  
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <asm-generic/gpio.h>

#define AM335X_EN		1
#if AM335X_EN
#include <asm/gpio.h>
#endif


