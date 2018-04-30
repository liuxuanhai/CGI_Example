/******************************************    *******************************
      > File Name: private_timer.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月15日 星期三 15时02分24秒
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
#define LINUX_OS_EN		0

#if AM335X_EN
#include <asm/gpio.h>
#endif

//私有头文件
#include "../Include/SystemDef.h"
#include "../Include/TimerDef.h"

/*命令规则:
 * 32位，
 * 高4位表示定时器编号
 * 低28位表示命令编号
 * */
#define PRIVATE_TIMER0			0				/*定时器0*/
#define PRIVATE_TIMER1			1				/*定时器1*/
#define PRIVATE_TIMER2			2				/*定时器2*/
#define PRIVATE_TIMER3			3				/*定时器3*/
#define PRIVATE_TIMER4			4				/*定时器4*/
#define PRIVATE_TIMER5			5				/*定时器5*/
#define PRIVATE_TIMER6			6				/*定时器6*/
#define PRIVATE_TIMER7			7				/*定时器7*/

#define TIMCMD_CLKSOC			0				/*时钟源选择命令*/
#define TIMCMD_DISENABLE		1				/*定时器开关命令*/
#define TIMCMD_OCPCFG			2				/*OCP配置*/
#define TIMCMD_TCLR_ST			3				/*控制*/
#define TIMCMD_TLDR				4				/*装载值*/
#define TIMCMD_PR				5				/*预分频*/
#define TIMCMD_DUTY				6				/*周期*/

#define DMTIMER_NUM				6				/*2~7*/

volatile unsigned long *CM_PER_TIMER_CLKCTRL[DMTIMER_NUM] = {NULL};
volatile unsigned long *CLKSEL_TIMER_CLK[DMTIMER_NUM] = {NULL};

volatile unsigned long *TIOCP_CFG[DMTIMER_NUM] = {NULL};
volatile unsigned long *TCLR[DMTIMER_NUM] = {NULL};
volatile unsigned long *TLDR[DMTIMER_NUM] = {NULL};
volatile unsigned long *TCRR[DMTIMER_NUM] = {NULL};

unsigned long DMTIMER_AddrBuf[] = {
	DMTIMER2_ADDR,
	DMTIMER3_ADDR,
	DMTIMER4_ADDR,
	DMTIMER5_ADDR,
	DMTIMER6_ADDR,
	DMTIMER7_ADDR
};

unsigned long CM_PER_TIMER_CLKCTRL_AddrBuf[] = {
	CM_PER_TIMER2_CLKCTRL_ADDR_OFFSET,
	CM_PER_TIMER3_CLKCTRL_ADDR_OFFSET,
	CM_PER_TIMER4_CLKCTRL_ADDR_OFFSET,
	CM_PER_TIMER5_CLKCTRL_ADDR_OFFSET,
	CM_PER_TIMER6_CLKCTRL_ADDR_OFFSET,
	CM_PER_TIMER7_CLKCTRL_ADDR_OFFSET
};

unsigned long CLKSEL_TIMER_CLK_AddrBuf[] = {
	CLKSEL_TIMER2_CLK_ADDR_OFFSET,
	CLKSEL_TIMER3_CLK_ADDR_OFFSET,
	CLKSEL_TIMER4_CLK_ADDR_OFFSET,
	CLKSEL_TIMER5_CLK_ADDR_OFFSET,
	CLKSEL_TIMER6_CLK_ADDR_OFFSET,
	CLKSEL_TIMER7_CLK_ADDR_OFFSET,
};

#define PRI_TIMER_MAJOR				0

static int pri_timer_major = PRI_TIMER_MAJOR;
static struct class *class;
static struct cdev cdev;

static char CheckTimerNo(char no)
{
	if((no < 2) || (no > 7))
	{
		return -1;
	}
	return 0;
}

static int pri_timer_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "Open pri_timer_driver\n");
	return 0;
}

static int pri_timer_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "Close pri_timer_driver\n");
	return 0;
}

static ssize_t pri_timer_read(struct file *filp, char __user *val, size_t timer_no, loff_t *f_pos)
{
	unsigned long index_counter = 0;
	unsigned long ret;
	if(CheckTimerNo((char)timer_no) < 0)
	{
		return -EFAULT;
	}
	index_counter = *TCRR[timer_no];
#if AM335X_EN
	if((ret = copy_to_user(val, (char *)&index_counter, sizeof(unsigned long))) < 0)
#elif LINUX_OS_EN
	if((ret = raw_copy_to_user(val, (char *)&index_counter, sizeof(unsigned long))) < 0)
#endif
	{
		return -EFAULT;
	}
	return 0;
}

static long pri_timer_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int timer_no = (cmd >> 28) & 0xF;
	int timer_cmd = cmd & ~(0xF << 28);
	if(CheckTimerNo(timer_no) < 0)			/*目前设置定时器2~7*/
	{
		return -EFAULT;
	}
	switch(timer_cmd)
	{
		/*设置时钟源*/
		case TIMCMD_CLKSOC:
			*CLKSEL_TIMER_CLK[timer_no] = 0x3 & arg;
			break;
		/*时钟模块开启*/
		case TIMCMD_DISENABLE:
			*CM_PER_TIMER_CLKCTRL[timer_no] = (*CM_PER_TIMER_CLKCTRL[timer_no] & ~0x3) | arg;
			break;
		/*OCP配置*/
		case TIMCMD_OCPCFG:
			*TIOCP_CFG[timer_no] = (*TIOCP_CFG[timer_no] & ~(0x1<<1)) | arg;
			break;
		/*停止/开始计时*/
		case TIMCMD_TCLR_ST:
			*TCLR[timer_no] = (*TCLR[timer_no] & ~0x1) | arg;
			break;
		/*装载值*/
		case TIMCMD_TLDR:
			*TLDR[timer_no] = arg;
			break;
		case TIMCMD_PR:
		case TIMCMD_DUTY:
		default:
			return -EFAULT;
	}
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = pri_timer_open,
	.release = pri_timer_release,
	.read = pri_timer_read,
	.unlocked_ioctl = pri_timer_unlocked_ioctl,
};


static void pri_timer_setup(void)
{
	cdev_init(&cdev, &fops);
	cdev.owner = THIS_MODULE;
	if(cdev_add(&cdev, MKDEV(pri_timer_major, 0), 1) < 0)
	{
		printk(KERN_INFO "Cdev pri_timer_driver err\n");
	}
}

static int pri_timer_init(void)
{
	int index = 0;
	int result;
	dev_t devno = MKDEV(pri_timer_major, 0);

	for(index = 0; index < DMTIMER_NUM; index++)
	{
		CM_PER_TIMER_CLKCTRL[index] = (volatile unsigned long *)ioremap(CM_PER_ADDR + CM_PER_TIMER_CLKCTRL_AddrBuf[index], 4);
		CLKSEL_TIMER_CLK[index] = (volatile unsigned long *)ioremap(CM_DPLL_ADDR + CLKSEL_TIMER_CLK_AddrBuf[index], 4);
		TIOCP_CFG[index] = (volatile unsigned long *)ioremap((DMTIMER_AddrBuf[index] + TIOCP_CFG_ADDR_OFFSET), 4);
		TCLR[index] = (volatile unsigned long *)ioremap((DMTIMER_AddrBuf[index] + TCLR_ADDR_OFFSET), 4);
		TLDR[index] = (volatile unsigned long *)ioremap((DMTIMER_AddrBuf[index] + TLDR_ADDR_OFFSET), 4);
		TCRR[index] = (volatile unsigned long *)ioremap((DMTIMER_AddrBuf[index] + TCRR_ADDR_OFFSET), 4);
	}

	if(pri_timer_major)
	{
		result = register_chrdev_region(devno, 1, "pri_timer_dev");
	}
	else
	{
		result = alloc_chrdev_region(&devno, 0, 1, "pri_timer_dev");
		pri_timer_major = MAJOR(devno);
	}
	if(result < 0)
	{
		goto devno_fail;
	}

	class = class_create(THIS_MODULE, "pri_timer_driver");
	if(IS_ERR(class))
	{
		printk(KERN_INFO "Err:failed to create class\n");
		result = -EBUSY;
		goto class_fail;
	}
	device_create(class, NULL, MKDEV(pri_timer_major, 0), NULL, "pri_timer_driver");

	pri_timer_setup();
	printk(KERN_INFO "Init pri_timer_driver ok\n");
	return 0;
class_fail:
	unregister_chrdev_region(devno, 1);		/*注销设备号,第一个编号和数量*/
devno_fail:
	for(index = 0; index < DMTIMER_NUM; index++)
	{
		iounmap(CM_PER_TIMER_CLKCTRL[index]);
		iounmap(CLKSEL_TIMER_CLK[index]);
		iounmap(TIOCP_CFG[index]);
		iounmap(TCLR[index]);
		iounmap(TLDR[index]);
		iounmap(TCRR[index]);
	}
	return result;
}

static void pri_timer_exit(void)
{
	int index = 0;
	for(index = 0; index < DMTIMER_NUM; index++)
	{
		iounmap(CM_PER_TIMER_CLKCTRL[index]);
		iounmap(CLKSEL_TIMER_CLK[index]);
		iounmap(TIOCP_CFG[index]);
		iounmap(TCLR[index]);
		iounmap(TLDR[index]);
		iounmap(TCRR[index]);
	}
	device_destroy(class, MKDEV(pri_timer_major, 0));
	class_destroy(class);
	cdev_del(&cdev);
	unregister_chrdev_region(MKDEV(pri_timer_major, 0), 1);		/*注销设备号,第一个编号和数量*/
	printk(KERN_INFO "Ywl pri_timer_driver exit\n");
}


module_init(pri_timer_init);
module_exit(pri_timer_exit);

MODULE_LICENSE("Dual BSD/GPL");
