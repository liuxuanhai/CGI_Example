/******************************************    *******************************
      > File Name: led_dev.c
      > Author: Ywl
      > Descripsion:	run灯 platform ti-am335x
      > Created Time:     2017年10月26日 星期四 17时20分34秒
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
#elif LINUX_OS_EN
#include <linux/gpio.h>
#endif

#include "../Include/GpioDef.h"

#define MODE_1_EN		1		/*两种方式去控制*/	



/*两个IO控制一个灯的颜色*/

/*
 *控制寄存器单个控制GPIO的每个管脚模式(每个管脚单独使用一个寄存器设置),控制寄存器是哪个？
 *方向控制寄存器(GPIO_OE)控制每个管脚的输入输出模式(32位每位控制一个管脚为输入或者输出)
 *数据寄存器
 *		AM335x有4个数据寄存器:
 *			GPIO_DATAIN,为输入时读取数据
 *			GPIO_DATAOUT,数据输出可写1,0
 *			GPIO_SETDATAOUT,某位写1时将GPIO_DATAOUT的对应位写1,写0没用
 *			GPIO_CLEARDATAOUT,某位写1时将GPIO_DATAOUT的对应位写0,写0没有
 * */
#define INPUT	1	/*输入*/
#define OUTPUT	0	/*输出*/

#define PRI_GPIO_MAJOR		0

static int pri_gpio_major = PRI_GPIO_MAJOR;
static struct cdev cdev;
static struct class *class;

#if MODE_1_EN

static int pri_gpio_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "Open gpio\n");
	return 0;
}

static int pri_gpio_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "Close gpio\n");
	return 0;
}

static ssize_t pri_gpio_read(struct file *filp, char __user *val, size_t pin, loff_t *f_pos)
{
	//先检查pin是否合法0~127
	int temp;
	long ret;
	if((pin < GPIO_MIN_PIN) || (pin > GPIO_MAX_PIN))
	{
		printk(KERN_INFO "Read fail:gpio_id over\n");
		return -EFAULT;
	}
	temp = gpio_get_value(pin);
	if((0 != temp) && (1 != temp))
	{
		return -EFAULT;
	}
	if((ret = copy_to_user(val, (char *)&temp, 1)) < 0)
	{
		return -EFAULT;
	}
	return 1;
}

static ssize_t pri_gpio_write(struct file *filp, const char __user *val, size_t pin, loff_t *fpos)
{
	char temp;
	long ret;	
	if((pin < GPIO_MIN_PIN) || (pin > GPIO_MAX_PIN))
	{
		printk(KERN_INFO "Write fail:gpio_id over\n");
		return -EFAULT;
	}
	if((ret = copy_from_user(&temp, val, 1)) < 0)
	{
		printk(KERN_INFO "Write fail:%ld\n", ret);
		return -EFAULT;
	}
	switch(temp)
	{
		case 0:
		case 1:
			gpio_set_value(pin, temp);
			break;
		default:
			return -EFAULT;
	}
	return 1;
}

static long pri_gpio_unlocked_ioctl(struct file *filp, unsigned int in_out, unsigned long pin)
{
	if((pin < GPIO_MIN_PIN) || (pin > GPIO_MAX_PIN))
	{
		printk(KERN_INFO "Ioctl fail:gpio_id over\n");
		return -EFAULT;
	}
	switch(in_out)
	{
		case INPUT:
			if(gpio_direction_input(pin) < 0)
			{
				return -EFAULT;
			}
		case OUTPUT:
			printk(KERN_INFO "ioctl pin:%ld, in_out:%d\n", pin, in_out);
			if(gpio_direction_output(pin, 1) < 0)
			{
				return -EFAULT;
			}
		default:
			return -EFAULT;
	}
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = pri_gpio_open,
	.release = pri_gpio_release,
	.read = pri_gpio_read,
	.write = pri_gpio_write,
	.unlocked_ioctl = pri_gpio_unlocked_ioctl,
};

static void pri_gpio_setup(void)
{
	cdev_init(&cdev, &fops);
	cdev.owner = THIS_MODULE;
	if(cdev_add(&cdev, MKDEV(pri_gpio_major, 0), 1))
	{
		printk(KERN_INFO "Cdev pri_gpio_driver err\n");
	}
}

static int pri_gpio_init(void)
{
	int result;
	dev_t devno = MKDEV(pri_gpio_major, 0);

	/*no driver, who is using them?*/
#if 0
	gpio_free(GPIO2_30);
	gpio_free(GPIO2_31);

	result = gpio_request(GPIO2_30, "pri_gpio_driver");
	if(result < 0)
	{
		printk(KERN_INFO "gpio2_30 request failed : %d\n", result);
		return result;
	}
	result = gpio_request(GPIO2_31, "pri_gpio_driver");
	if(result < 0)
	{
		gpio_free(GPIO2_30);
		printk(KERN_INFO "gpio2_31 request failed\n");
		return result;
	}
#endif
	/*通用GPIO初始化*/
	char pin_id = 0;
	char pin_index = 0;
	for(pin_id = GPIO_MIN_PIN; pin_id <= GPIO_MAX_PIN; pin_id++)
	{
		gpio_free(pin_id);
	}
	for(pin_id = GPIO_MIN_PIN; pin_id <= GPIO_MAX_PIN; pin_id++)
	{
		result = gpio_request(pin_id, "pri_gpio_driver");
		if(result < 0)
		{
			printk(KERN_INFO "gpio2_30 request failed : gpio=%d, err=%d\n", pin_id, result);
			for(pin_index = GPIO_MIN_PIN; pin_index < pin_id; pin_index++)
			{
				gpio_free(pin_index);
				return result;
			}
		}

	}

	if(pri_gpio_major)
	{
		result = register_chrdev_region(devno, 1, "pri_gpio_driver");
	}
	else
	{
		result = alloc_chrdev_region(&devno, 0, 1, "pri_gpio_driver");
		pri_gpio_major = MAJOR(devno);
	}
	if(result < 0)
	{
		goto devno_fail; 
	}

	class = class_create(THIS_MODULE, "pri_gpio_driver");
	if(IS_ERR(class))
	{
		printk(KERN_INFO "Err:failed to create class\n");
		result = -EBUSY;
		goto class_fail;
	}
	device_create(class, NULL, MKDEV(pri_gpio_major, 0), NULL, "pri_gpio_driver");
	pri_gpio_setup();

	printk(KERN_INFO "Ywl pri_gpio_driver device init\n");
	return 0;
class_fail:
	unregister_chrdev_region(devno, 1);		/*注销设备号,第一个编号和数量*/
devno_fail:
	for(pin_id = GPIO_MIN_PIN; pin_id <= GPIO_MAX_PIN; pin_id++)
	{
		gpio_free(pin_id);
	}
	//gpio_free(GPIO2_30);
	//gpio_free(GPIO2_31);
	return result;
}

static void pri_gpio_exit(void)
{
	char pin_id = 0;
	for(pin_id = GPIO_MIN_PIN; pin_id <= GPIO_MAX_PIN; pin_id++)
	{
		gpio_free(pin_id);
	}
	//gpio_free(GPIO2_30);
	//gpio_free(GPIO2_31);
	device_destroy(class, MKDEV(pri_gpio_major, 0));
	class_destroy(class);
	cdev_del(&cdev);
	unregister_chrdev_region(MKDEV(pri_gpio_major, 0), 1);		/*注销设备号,第一个编号和数量*/
	printk(KERN_INFO "Ywl pri_gpio_driver exit\n");
}

#else
/*注意这个地方的类型*/
volatile unsigned long *GPIO2_OE = NULL;
volatile unsigned long *GPIO2_DATAIN = NULL;
volatile unsigned long *GPIO2_DATAOUT = NULL;

static int pri_gpio_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "Open gpio\n");
	return 0;
}

static int pri_gpio_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "Close gpio\n");
	return 0;
}


static ssize_t pri_gpio_read(struct file *filp, char __user *val, size_t pin, loff_t *f_pos)
{
	int temp;
	unsigned long ret;
	switch(pin)
	{
		//只能是常量
		case GPIO2_30:
			temp = (*GPIO2_DATAIN >> 30) & 0x1;
			break;
		case GPIO2_31:
			temp = (*GPIO2_DATAIN >> 30) & 0x1;
			break;
		default:
			return -EFAULT;
	}
	if((ret = copy_to_user(val, (char *)&temp, 1)) < 0)
	{
		return -EFAULT;
	}
	return 1;
}

static ssize_t pri_gpio_write(struct file *filp, const char __user *val, size_t pin, loff_t *fpos)
{
	char temp;
	unsigned long ret;
	unsigned long gpio_pin = pin - (2 << 5);	/*pin = 2 << 5 + 30 or 31,so here result is 30 or 31*/
	if((ret = copy_from_user(&temp, val, 1)) < 0)
	{
		return -EFAULT;
	}
	switch(pin)
	{
		case GPIO2_30:
		case GPIO2_31:
			switch(temp)
			{
				case INPUT:
				case OUTPUT:
					/*先将该位清零，在将改位写入*/
					*GPIO2_DATAOUT = (*GPIO2_DATAOUT & (~(0x1 << gpio_pin))) | (temp << gpio_pin);	
					break;
				default:
					return -EFAULT;
			}
			break;
		default:
			return -EFAULT;
	}
	return 1;
}

static long pri_gpio_unlocked_ioctl(struct file *filp, unsigned int in_out, unsigned long pin)
{
	unsigned long gpio_pin = pin - (2 << 5);	/*pin = 2 << 5 + 30 or 31,so here result is 30 or 31*/
	switch(pin)
	{
		case GPIO2_30:
		case GPIO2_31:
			switch(in_out)
			{
				case INPUT:
				case OUTPUT:
					/*先将该位清零，在将改位写入*/
					*GPIO2_OE = (*GPIO2_OE & (~(0x1 << gpio_pin))) | (in_out << gpio_pin);	
					break;
				default:
					return -EFAULT;
			}
			break;
		default:
			return -EFAULT;
	}
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = pri_gpio_open,
	.release = pri_gpio_release,
	.read = pri_gpio_read,
	.write = pri_gpio_write,
	.unlocked_ioctl = pri_gpio_unlocked_ioctl,
};

static void pri_gpio_setup(void)
{
	cdev_init(&cdev, &fops);
	cdev.owner = THIS_MODULE;
	if(cdev_add(&cdev, MKDEV(pri_gpio_major, 0), 1))
	{
		printk(KERN_INFO "Cdev pri_gpio_driver err\n");
	}
}

static int pri_gpio_init(void)
{
	int result;
	dev_t devno = MKDEV(pri_gpio_major, 0);

	/*地址映射,也可以直接使用写内存的方式:writel(),readl(),甚至可以直接读写地址(裸机驱动的方式)*/
	GPIO2_OE = (volatile unsigned long *)ioremap(GPIO2_BASE_ADDR + GPIO_OE_OFFSET, 4);
	GPIO2_DATAIN = (volatile unsigned long *)ioremap(GPIO2_BASE_ADDR + GPIO_DATAIN_OFFSET, 4);
	GPIO2_DATAOUT = (volatile unsigned long *)ioremap(GPIO2_BASE_ADDR + GPIO_DATAOUT_OFFSET, 4);

	if(pri_gpio_major)
	{
		result = register_chrdev_region(devno, 1, "pri_gpio_driver");
	}
	else
	{
		result = alloc_chrdev_region(&devno, 0, 1, "pri_gpio_driver");
		pri_gpio_major = MAJOR(devno);
	}
	if(result < 0)
	{
		goto devno_fail; 
	}

	class = class_create(THIS_MODULE, "pri_gpio_driver");
	if(IS_ERR(class))
	{
		printk(KERN_INFO "Err:failed to create class\n");
		result = -EBUSY;
		goto class_fail;
	}
	device_create(class, NULL, MKDEV(pri_gpio_major, 0), NULL, "pri_gpio_driver");
	pri_gpio_setup();

	printk(KERN_INFO "Ywl pri_gpio_driver device init\n");
	return 0;
class_fail:
	unregister_chrdev_region(devno, 1);		/*注销设备号,第一个编号和数量*/
devno_fail:
	iounmap(GPIO2_OE);
	iounmap(GPIO2_DATAOUT);
	return result;
}

static void pri_gpio_exit(void)
{
	iounmap(GPIO2_OE);
	iounmap(GPIO2_DATAOUT);
	device_destroy(class, MKDEV(pri_gpio_major, 0));
	class_destroy(class);
	cdev_del(&cdev);
	unregister_chrdev_region(MKDEV(pri_gpio_major, 0), 1);		/*注销设备号,第一个编号和数量*/
	printk(KERN_INFO "Ywl pri_gpio_driver exit\n");
}


#endif
module_init(pri_gpio_init);
module_exit(pri_gpio_exit);

MODULE_LICENSE("Dual BSD/GPL");
