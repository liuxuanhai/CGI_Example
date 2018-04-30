/******************************************    *******************************
      > File Name: led.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年10月23日 星期一 11时15分20秒
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


struct light_dev{
	struct cdev cdev;		/*字符设备cdev结构体*/
	unsigned char value;	/*亮为1,灭为0,用户可读写*/
};

#define LIGHT_ON	1
#define LIGHT_OFF	0


#define LIGHT_MAJOR		234		/*主设备号，别与其他的冲突*/

struct light_dev *light_devp;
int light_major = LIGHT_MAJOR;

void light_gpio_init(void)
{
}
void light_on(void)
{
}
void light_off(void)
{
}

/*打开和关闭函数*/
int light_open(struct inode *inode, struct file *filp)
{
	struct light_dev *dev;
	/*获得设备结构体指针*/
	dev = container_of(inode->i_cdev, struct light_dev, cdev);
	/*让设备结构体作为设备的私有信息*/
	filp->private_data = dev;
	return 0;
}

int light_release(struct inode *inode, struct file *filp)
{
	return 0;
}

/*读写设备*/
static ssize_t light_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	struct light_dev *dev = filp->private_data;	/*获得设备结构体*/
	//貌似copy_to_user()会报错，难道是丢弃了?
	if(raw_copy_to_user(buf, &(dev->value), 1))
	{
		return -EFAULT;
	}
	return 1;	/*返回读取成功的长度*/
}
/*读写设备*/
static ssize_t light_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	struct light_dev *dev = filp->private_data;	/*获得设备结构体*/
	if(raw_copy_from_user(&(dev->value), buf, 1))
	{
		return -EFAULT;
	}
	/*根据写入的值点亮或者熄灭*/
	if(dev->value == 1)
	{
		light_on();
	}
	else
	{
		light_off();
	}
	return 1;	/*返回读取成功的长度*/
}

/*ioctl, 旧的ioctl已经被淘汰了, 但是要自己枷锁*/
long light_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
//int light_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct light_dev *dev = filp->private_data;
	switch(cmd)
	{
		case LIGHT_ON:
			dev->value = 1;
			light_on();
			break;
		case LIGHT_OFF:
			dev->value = 0;
			light_off();
			break;
		default:
			/*不能支持的命令*/
			return -ENOTTY;
	}
	return 0;
}
/*初始化设备操作函数指针（文件指针）*/
struct file_operations light_fops = {
	.owner = THIS_MODULE,
	.read = light_read,
	.write = light_write,
//	.ioctl = light_ioctl,
	.unlocked_ioctl = light_ioctl,
	.release = light_release,
};

/*设置字符设备cdev结构体*/
static void light_setup_cdev(struct light_dev *dev, int index)
{
	int err, devno = MKDEV(light_major, index);
	cdev_init(&dev->cdev, &light_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &light_fops;
	err = cdev_add(&dev->cdev, devno, 1);
	if(err)
	{
		printk(KERN_NOTICE "Error %d adding LED%d", err, index);
	}
}

/*模块加载函数*/
int light_init(void)
{
	int result;
	dev_t dev = MKDEV(light_major, 0);
	/*申请设备字符号*/
	if(light_major)
	{
		result = register_chrdev_region(dev, 1, "LED");
	}
	else
	{
		result = alloc_chrdev_region(&dev, 0, 1, "LED");	
		light_major = MAJOR(dev);
	}
	if(result < 0)
		return result;
	/*分配设备结构体的内存*/
	light_devp = kmalloc(sizeof(struct light_dev), GFP_KERNEL);
	if(!light_devp)
	{
		result = -ENOMEM;
		goto fail_malloc;
	}
	memset(light_devp, 0, sizeof(struct light_dev));
	light_setup_cdev(light_devp, 0);
	light_gpio_init();
	return 0;
fail_malloc:
	unregister_chrdev_region(dev, 1);
	return result;
}

/*模块卸载函数*/
void light_cleanup(void)
{
	cdev_del(&light_devp->cdev);	/*删除字符设备结构体*/
	kfree(light_devp);				/*释放内存*/
	unregister_chrdev_region(MKDEV(light_major, 0), 1);		/*删除字符设备*/
}

MODULE_AUTHOR("ywl <astralrovers@outlook.com>");
MODULE_LICENSE("Dual BSD/GPL");

module_init(light_init);		/*驱动程序入口函数*/
module_exit(light_cleanup);		/*卸载函数*/
