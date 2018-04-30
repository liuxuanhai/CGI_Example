/******************************************    *******************************
  > File Name: example.c
  > Author: Ywl
  > Descripsion:
  > Created Time:     2017年11月21日 星期二 17时05分44秒
  > Modify Time:
 *********************************    ***************************************/

#include <linux/delay.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/module.h>
#include <linux/device.h>         //class_create
#include <mach/regs-gpio.h>       //S3C2410_GPF1
//#include <asm/arch/regs-gpio.h>
#include <mach/hardware.h>
//#include <asm/hardware.h>
#include <linux/interrupt.h>  //wait_event_interruptible
/* 定义并初始化等待队列头 */
static DECLARE_WAIT_QUEUE_HEAD(button_waitq);


static struct class *thirddrv_class;
static struct device *thirddrv_device;

static struct pin_desc{
	unsigned int pin;
	unsigned int key_val;
};

static struct pin_desc pins_desc[4] = {
	{S3C2410_GPF1,0x01},
	{S3C2410_GPF4,0x02},
	{S3C2410_GPF2,0x03},
	{S3C2410_GPF0,0x04},
};

static int ev_press = 0;

/* 键值: 按下时, 0x01, 0x02, 0x03, 0x04 */
/* 键值: 松开时, 0x81, 0x82, 0x83, 0x84 */
static unsigned char key_val;
int major;

/* 用户中断处理函数 */
static irqreturn_t buttons_irq(int irq, void *dev_id)
{
	struct pin_desc *pindesc = (struct pin_desc *)dev_id;
	unsigned int pinval;
	pinval = s3c2410_gpio_getpin(pindesc->pin);

	if(pinval)
	{
		/* 松开 */
		key_val = 0x80 | (pindesc->key_val);
	}
	else
	{
		/* 按下 */
		key_val = pindesc->key_val;
	}

	ev_press = 1;                            /* 表示中断已经发生 */
	wake_up_interruptible(&button_waitq);   /* 唤醒休眠的进程 */
	return IRQ_HANDLED;
}
static int third_drv_open(struct inode * inode, struct file * filp)
{
	/*  K1 ---- EINT1,K2 ---- EINT4,K3 ---- EINT2,K4 ---- EINT0
	 *       *  配置GPF1、GPF4、GPF2、GPF0为相应的外部中断引脚
	 *            *  IRQT_BOTHEDGE应该改为IRQ_TYPE_EDGE_BOTH
	 *                 */
	request_irq(IRQ_EINT1, buttons_irq, IRQ_TYPE_EDGE_BOTH, "K1",&pins_desc[0]);
	request_irq(IRQ_EINT4, buttons_irq, IRQ_TYPE_EDGE_BOTH, "K2",&pins_desc[1]);
	request_irq(IRQ_EINT2, buttons_irq, IRQ_TYPE_EDGE_BOTH, "K3",&pins_desc[2]);
	request_irq(IRQ_EINT0, buttons_irq, IRQ_TYPE_EDGE_BOTH, "K4",&pins_desc[3]);
	return 0;
}

static ssize_t third_drv_read(struct file *file, char __user *user, size_t size,loff_t *ppos)
{
	if (size != 1)
		return -EINVAL;

	/* 当没有按键按下时，休眠。
	 *      * 即ev_press = 0;
	 *           * 当有按键按下时，发生中断，在中断处理函数会唤醒
	 *                * 即ev_press = 1;
	 *                     * 唤醒后，接着继续将数据通过copy_to_user函数传递给应用程序
	 *                          */
	wait_event_interruptible(button_waitq, ev_press);
	copy_to_user(user, &key_val, 1);

	/* 将ev_press清零 */
	ev_press = 0;
	return 1;
}

static int third_drv_close(struct inode *inode, struct file *file)
{
	free_irq(IRQ_EINT1,&pins_desc[0]);
	free_irq(IRQ_EINT4,&pins_desc[1]);
	free_irq(IRQ_EINT2,&pins_desc[2]);
	free_irq(IRQ_EINT0,&pins_desc[3]);
	return 0;
}

/* File operations struct for character device */
static const struct file_operations third_drv_fops = {
	.owner      = THIS_MODULE,
	.open       = third_drv_open,
	.read       = third_drv_read,
	.release    = third_drv_close,
};


/* 驱动入口函数 */
static int third_drv_init(void)
{
	/* 主设备号设置为0表示由系统自动分配主设备号 */
	major = register_chrdev(0, "third_drv", &third_drv_fops);

	/* 创建thirddrv类 */
	thirddrv_class = class_create(THIS_MODULE, "thirddrv");

	/* 在thirddrv类下创建buttons设备，供应用程序打开设备*/
	thirddrv_device = device_create(thirddrv_class, NULL, MKDEV(major, 0), NULL, "buttons");

	return 0;
}

/* 驱动出口函数 */
static void third_drv_exit(void)
{
	unregister_chrdev(major, "third_drv");
	device_unregister(thirddrv_device);  //卸载类下的设备
	class_destroy(thirddrv_class);      //卸载类
}

module_init(third_drv_init);  //用于修饰入口函数
module_exit(third_drv_exit);  //用于修饰出口函数

MODULE_AUTHOR("LWJ");
MODULE_DESCRIPTION("Just for Demon");
MODULE_LICENSE("GPL");  //遵循GPL协议
