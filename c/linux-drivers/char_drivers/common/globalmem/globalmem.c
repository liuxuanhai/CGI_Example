/******************************************    *******************************
      > File Name: globalmem.c
      > Author: Ywl
      > Descripsion:		虚拟内存字符设备，没啥用，用来进程间通信倒是可以
      > Created Time:     2017年10月25日 星期三 19时18分29秒
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

#define barrier() __asm__ __volatile__("": : :"memory")				/*barrier前后的代码不会颠倒。乱序
																	 因为多核、多线程可能导致程序被分到不同位置执行，*/

#define GLB_MEM_SIZE	0x1000		/*4k*/
/*命令码格式可以查看文档*/
#define GLOBALMEM_MAGIC	1			/*设备类型*/
//#define MEM_CLEAR		_IO(GLOBALMEM_MAGIC, 0)			/*清零全局内存*/
#define MEM_CLEAR		0x1
#define GLBMEM_MAJOR		0			/*主设备号,动态分配算了*/
#define DEV_NUM			2				/*设备数量2个*/


struct globalmem_dev_t {
	struct cdev cdev;
	unsigned char mem[GLB_MEM_SIZE];			/*设备内存*/
};

static int globalmem_major = GLBMEM_MAJOR;
static struct globalmem_dev_t *globalmem_dev;		/*创建实例*/
static struct class *globalmem_class;				/*创建类*/
//static struct device *globalmem_device;				/*设备实例*/

/*
 *打开设备文件
 * */
static int glbmem_open(struct inode *inode, struct file *filp)
{
	struct globalmem_dev_t *dev = NULL;
	dev = container_of(inode->i_cdev, struct globalmem_dev_t, cdev);
	if(NULL == dev)
	{
		return -ENODEV;
	}
	filp->private_data = dev;
	return 0;
}

/*释放*/
static int glbmem_release(struct inode *inode, struct file *filp)
{
	return 0;
}

/*读取内存数据*/
static ssize_t glbmem_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	loff_t pos = *f_pos;
	size_t size = count;
	int ret = 0;
	struct globalmem_dev_t *dev = filp->private_data;

	if(pos >= GLB_MEM_SIZE)
	{
		return 0;				/*到文件末尾了，没有数据可读*/
	}
	if(size > GLB_MEM_SIZE - pos)
	{
		size = GLB_MEM_SIZE;		/*要读取的长度超过了剩余长度*/
	}
	if(raw_copy_to_user(buf, dev->mem+pos, size))
	//if(copy_to_user(buf, dev->mem+pos, size))
	{
		ret = -EFAULT;
	}
	else
	{
		*f_pos += size;
		ret = size;

		printk(KERN_INFO "Have read %lu bytes(s) form %llu\n", size, pos);
	}
	return ret;
}

/*写入内存数据*/
static ssize_t glbmem_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	loff_t pos = *f_pos;
	size_t size = count;
	int ret = 0;
	struct globalmem_dev_t *dev = filp->private_data;

	if(pos >= GLB_MEM_SIZE)
	{
		return 0;				/*到文件末尾了，写不进去了*/
	}
	if(size > GLB_MEM_SIZE - pos)
	{
		size = GLB_MEM_SIZE;		/*要写入的长度超过了内存剩余大小*/
	}
	if(raw_copy_from_user(dev->mem+pos, buf, size))
	//if(copy_from_user(dev->mem+pos, buf, size))
	{
		ret = -EFAULT;
	}
	else
	{
		*f_pos += size;
		ret = size;

		printk(KERN_INFO "Have write %lu bytes(s) to %llu\n", size, pos);
	}
	return ret;
}

/*定位*/
static loff_t glbmem_llseek(struct file *filp, loff_t offset, int orig)
{
	loff_t ret = 0;
	switch(orig)
	{
		/*相对文件开始位置 SEEK_SET*/
		case 0:
			if((offset < 0) || (offset > GLB_MEM_SIZE))
			{
				ret = -EINVAL;
				break;
			}
			filp->f_pos = offset;
			ret = filp->f_pos;
			break;
		/*相对当前位置,SEEK_CUR*/
		/*相对文件末尾,SEEK_END,文件末尾的话offset值是负值*/
		case 1:
		case 2:
			if(((filp->f_pos + offset) > GLB_MEM_SIZE) || ((filp->f_pos + offset) < 0))
			{
				ret = -EINVAL;
				break;
			}
			filp->f_pos += offset;
			ret = filp->f_pos;
			break;
		default:
			ret = -EINVAL;
			break;
	}
	return ret;
}

/*ioctl*/
static long glbmem_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct globalmem_dev_t *dev = filp->private_data;
	switch(cmd)
	{
		case MEM_CLEAR:
			/*全局清零*/
			memset(dev->mem, 0, GLB_MEM_SIZE);

			printk(KERN_INFO "globalmem is set to zero\n");
			break;
		default:
			return -EINVAL;
	}
	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = glbmem_open,
	.release = glbmem_release,
	.llseek = glbmem_llseek,
	.read = glbmem_read,
	.write = glbmem_write,
	.unlocked_ioctl = glbmem_unlocked_ioctl,
};

static void globalmem_setup_cdev(struct globalmem_dev_t *dev, int index)
{
	int err;
	dev_t devno = MKDEV(globalmem_major, index);
	cdev_init(&dev->cdev, &fops);
	dev->cdev.owner = THIS_MODULE;
	err = cdev_add(&dev->cdev, devno, 1);
	if(err)
	{
		printk(KERN_INFO "Globalmem err : %d", err);
	}
}

static int __init global_mem_init(void)
{
	int result;
	char dev_name[DEV_NUM][20];
	int index = 0;
	dev_t devno = MKDEV(globalmem_major, 0);

	if(globalmem_major)
	{
		result = register_chrdev_region(devno, DEV_NUM, "globalmem");
	}
	else
	{
		result = alloc_chrdev_region(&devno, 0, DEV_NUM, "globalmem");
		globalmem_major = MAJOR(devno);
	}
	if(result < 0)
		return result;

	globalmem_class = class_create(THIS_MODULE, "globalmem");
	
	if(IS_ERR(globalmem_class))
	{
		printk(KERN_INFO "Globalmem create class err\n");
		unregister_chrdev_region(MKDEV(globalmem_major, 0), DEV_NUM);
		return -EBUSY;
	}

#if 0
	if(IS_ERR(globalmem_device))
	{
		printk(KERN_INFO "Globalmem create device err\n");
		class_destroy(globalmem_class);
		unregister_chrdev_region(MKDEV(globalmem_major, 0), 1);
		return -EBUSY;
	}
#endif

	globalmem_dev = kmalloc(sizeof(struct globalmem_dev_t)*DEV_NUM, GFP_KERNEL);
	if(NULL == globalmem_dev)
	{
		result = -ENOMEM;
		goto fail_mem;
	}
	memset(globalmem_dev, 0, sizeof(struct globalmem_dev_t)*DEV_NUM);
	for(index = 0; index < DEV_NUM; index++)
	{
		sprintf(dev_name[index], "globalmem%d", index);
		device_create(globalmem_class, NULL, MKDEV(globalmem_major, index), NULL, dev_name[index]);
		globalmem_setup_cdev(globalmem_dev+index, index);
	}
	printk(KERN_INFO "globalmem init\n");
	return 0;
fail_mem:
	unregister_chrdev_region(devno, DEV_NUM);
	return result;
}

static void __exit global_mem_exit(void)
{
	int index = 0;
	for(index = 0; index < DEV_NUM; index++)
	{
		device_destroy(globalmem_class, MKDEV(globalmem_major, index));
		cdev_del(&((globalmem_dev+index)->cdev));
	}
	class_destroy(globalmem_class);
	kfree(globalmem_dev);
	unregister_chrdev_region(MKDEV(globalmem_major, 0), DEV_NUM);
	printk(KERN_INFO "globalmem exit\n");
}

module_init(global_mem_init);
module_exit(global_mem_exit);

MODULE_AUTHOR("Ywl <astralrovers@outlook.com>");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("globalmem char drivers");                                                                  
MODULE_ALIAS("globalmem module");


/*验证：
 *可以在加载驱动创建设备节点后
 * echo "globlamem test i/o" > /dev/globalmem
 * cat /dev/gloablmem
 * 或者写程序去读写
 * */
