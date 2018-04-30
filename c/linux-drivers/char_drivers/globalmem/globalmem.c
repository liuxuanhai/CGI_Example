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
#include <linux/init.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/timer.h>
#include <asm/atomic.h>
#include <linux/slab.h>
#include <linux/device.h>

#include <linux/sched.h>
#include <linux/sched/signal.h>										/*要这样加?这是从那个版本开始的，*/
#include <linux/wait.h>												/* waitqueuee */
#include <linux/poll.h>												/*poll*/


#define GLB_MEM_SIZE	0x1000		/*4k*/
/*命令码格式可以查看文档*/
#define GLOBALMEM_MAGIC	1			/*设备类型*/
//#define MEM_CLEAR		_IO(GLOBALMEM_MAGIC, 0)			/*清零全局内存*/
#define MEM_CLEAR		0x1
#define GLBMEM_MAJOR		0			/*主设备号,动态分配算了*/
#define DEV_NUM			2				/*设备数量2个*/

#define FILE_DRIVER_EN			0			/*作为文件驱动*/
#define POP_MEM_DRIVER_EN		1			/*作为流式驱动，读了就没了*/


//static atomic_t glbmem_v = ATOMIC_INIT(1);			/*定义原子变量并初始化为1*/


struct globalmem_dev_t {
	struct cdev cdev;
	unsigned char mem[GLB_MEM_SIZE];			/*设备内存*/
	unsigned int data_len;						/*数据长度*/
	atomic_t glbmem_v;							/*原子锁*/
#if FILE_DRIVER_EN
	rwlock_t rwock								/*读写锁*/
#elif POP_MEM_DRIVER_EN
	struct mutex mutex;							/*互斥锁，在读写时只允许一个进程操作*/
#endif
	wait_queue_head_t write_queue;				/*写队列*/
	wait_queue_head_t read_queue;				/*读队列*/
	struct fasync_struct *fasync_queue;			/*异步队列*/
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
	if(!atomic_dec_and_test(&dev->glbmem_v))			/*减一之后如果是0则返回真，否则返回否*/
	{
		atomic_inc(&dev->glbmem_v);			/*测试之后要加回去*/
		return -EBUSY;
	}
	atomic_dec(&dev->glbmem_v);				/*占用设备*/
	filp->private_data = dev;
	return 0;
}


/*支持F_SETFL命令的处理，每当FASYNC标志改变时，驱动程序中的fasync（）函数将得以执行。
 因此，驱动中应该实现fasync（）函数*/
static int glbmem_fasync(int fd, struct file *filp, int mode)
{
	struct globalmem_dev_t *dev = filp->private_data;
	return fasync_helper(fd, filp, mode, &dev->fasync_queue);
}
/*释放*/
static int glbmem_release(struct inode *inode, struct file *filp)
{
	struct globalmem_dev_t *dev = filp->private_data;
	atomic_inc(&dev->glbmem_v);
	glbmem_fasync(-1, filp, 0);
	return 0;
}

/*读取内存数据,读了就没了*/
static ssize_t glbmem_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	loff_t pos = *f_pos;
	size_t size = count;
	int ret = 0;
	struct globalmem_dev_t *dev = filp->private_data;
#if POP_MEM_DRIVER_EN
	DECLARE_WAITQUEUE(wait, current);			/*定义并初始化队列元素*/
	mutex_lock(&dev->mutex);					/*加锁*/
#endif

#if FILE_DRIVER_EN
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
#elif POP_MEM_DRIVER_EN
	add_wait_queue(&dev->read_queue, &wait);		/*加入队列*/
	while(!dev->data_len)							/*没有数据可读*/
	{
		/*非阻塞*/
		if(filp->f_flags & O_NONBLOCK)
		{
			ret = -EAGAIN;
			goto out_driver;
		}
		/*阻塞模式*/
		__set_current_state(TASK_INTERRUPTIBLE);			/*改变进程状态为睡眠*/
		mutex_unlock(&dev->mutex);							/*在睡眠状态需要释放锁，让其他进程能正常运行*/
		schedule();											/*切换其他进程执行*/
		if(signal_pending(current))							/*为嘛编译不过? 加了<linux/sched.h>还是不行?*/
		{
			ret = -ERESTARTSYS;								/*如果是由于信号产生的 伪信号 唤醒的，则晚会错误*/
			goto out_driver2;
		}
		mutex_lock(&dev->mutex);					/*加锁*/
	}		/*如果程序在可中断的情况下被唤醒，但是数据仍然为空，进行数据操作会导致出错，
			  还有一个原因是如果多个进程在读，可能某些进程先响应，然后读完了，导致出错，因此在唤醒后需要再次判断
			  因此这里不用if而用while*/

	if(size >= dev->data_len)
	{
		size = dev->data_len;				/*长度过大，则将流里面的数据读完*/
	}
	if(raw_copy_to_user(buf, dev->mem, size))
	//if(copy_to_user(buf, dev->mem, size))
	{
		ret = -EFAULT;
		goto out_driver;
	}
	else
	{
		dev->data_len -= size;						/*数据前移*/
		memcpy(dev->mem, dev->mem+size, dev->data_len);
		ret = size;
		wake_up_interruptible(&dev->write_queue);				/*唤醒写进程，主要是在内存写满时唤醒可以继续写*/
		if(dev->fasync_queue)
		{
			kill_fasync(&dev->fasync_queue, SIGIO, POLL_OUT);		/*发出可读信号*/
		}
	}
out_driver:
	mutex_unlock(&dev->mutex);
out_driver2:
	remove_wait_queue(&dev->read_queue, &wait);						/*从队列从删除*/
	set_current_state(TASK_RUNNING);							/*恢复进程状态为正常运行*/
#endif
	return ret;
}

/*写入内存数据*/
static ssize_t glbmem_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	loff_t pos = *f_pos;
	size_t size = count;
	int ret = 0;
	struct globalmem_dev_t *dev = filp->private_data;
#if POP_MEM_DRIVER_EN
	DECLARE_WAITQUEUE(wait, current);			/*定义并初始化队列元素*/
	mutex_lock(&dev->mutex);					/*加锁*/
#endif

#if FILE_DRIVER_EN
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
#elif POP_MEM_DRIVER_EN
	add_wait_queue(&dev->write_queue, &wait);
	while(dev->data_len == GLB_MEM_SIZE)			/*内存已经写满了*/
	{
		if(filp->f_flags & O_NONBLOCK)
		{
			ret = -EAGAIN;
			goto out_driver;
		}
		__set_current_state(TASK_INTERRUPTIBLE);
		mutex_unlock(&dev->mutex);
		schedule();
		if(signal_pending(current))
		{
			ret = -ERESTARTSYS;
			goto out_driver2;
		}
		mutex_lock(&dev->mutex);
	}
	if(size > (GLB_MEM_SIZE - dev->data_len))
	{
		size = GLB_MEM_SIZE - dev->data_len;
	}
	if(raw_copy_from_user(dev->mem + dev->data_len, buf, size))
	//if(copy_from_user(dev->mem+pos, buf, size))
	{
		ret = -EFAULT;
		goto out_driver;
	}
	else
	{
		dev->data_len += size;
		ret = size;
		wake_up_interruptible(&dev->read_queue);
		if(dev->fasync_queue)
		{
			kill_fasync(&dev->fasync_queue, SIGIO, POLL_IN);			/*POLL_IN，可写信号*/
		}
	}

out_driver:
	mutex_unlock(&dev->mutex);
out_driver2:
	remove_wait_queue(&dev->write_queue, &wait);
	set_current_state(TASK_RUNNING);
#endif
	return ret;
}
static unsigned int glbmem_poll(struct file *filp, poll_table *wait)
{
	unsigned int mask = 0;
	struct globalmem_dev_t *dev = filp->private_data;

	mutex_lock(&dev->mutex);

	poll_wait(filp, &dev->read_queue, wait);
	poll_wait(filp, &dev->write_queue, wait);			/*这里只是把wait添加到等待列表,并不会睡眠*/
	if(dev->data_len != 0)
	{
		mask |= POLLIN | POLLRDNORM;
	}
	if(dev->data_len != GLB_MEM_SIZE)
	{
		mask |= POLLOUT | POLLWRNORM;
	}
	mutex_unlock(&dev->mutex);
	return mask;
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
#if POP_MEM_DRIVER_EN
	mutex_lock(&dev->mutex);
#endif
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
#if POP_MEM_DRIVER_EN
	mutex_unlock(&dev->mutex);
#endif
	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = glbmem_open,
	.release = glbmem_release,
#if FILE_DRIVER_EN
	.llseek = glbmem_llseek,
#endif
	.read = glbmem_read,
	.write = glbmem_write,
	.poll = glbmem_poll,
	.fasync = glbmem_fasync,
	.unlocked_ioctl = glbmem_unlocked_ioctl,
};

static void globalmem_setup_cdev(struct globalmem_dev_t *dev, int index)
{
	int err;
	dev_t devno = MKDEV(globalmem_major, index);
	/*初始化原子锁*/
	atomic_set(&dev->glbmem_v, 10);			//允许打开10次
#if POP_MEM_DRIVER_EN
	/*初始化互斥锁*/
	mutex_init(&dev->mutex);
	/*初始化队列*/
	init_waitqueue_head(&dev->write_queue);
	init_waitqueue_head(&dev->read_queue);
#endif

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
