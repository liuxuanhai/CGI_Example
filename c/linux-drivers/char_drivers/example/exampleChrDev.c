/******************************************    *******************************
      > File Name: exampleChrDev.c
      > Author: Ywl
      > Descripsion: 相当于一个字符设备驱动的模板
      > Created Time:     2017年10月25日 星期三 15时57分43秒
      > Modify Time: 
 *********************************    ***************************************/

#include <linux/module.h>   /*最基本的文件，支持动态添加和卸载模块*/
#include <linux/types.h>  /*对一些特殊类型的定义，例如dev_t, off_t, pid_t.
							其实这些类型大部分都是unsigned int型通过一连串的typedef变过来的，只是为了方便阅读。*/
#include <linux/fs.h>  /*文件系统头文件，定义文件表结构（file,buffer_head,m_inode等）*/
#include <linux/errno.h>  /*错误号头文件，包含系统中各种出错号。(Linus从minix中引进的)*/
#include <linux/mm.h>	/*内存管理头文件，含有页面大小定义和一些页面释放函数原型。*/
#include <linux/sched.h>  /*调度程序头文件，定义了任务结构task_struct、初始任务0的数据，
							以及一些有关描述符参数设置和获取的嵌入式汇编函数宏语句。*/
#include <linux/init.h>  /*//初始化头文件*/
#include <linux/cdev.h>  /*对字符设备结构cdev以及一系列的操作函数的定义。*/
#include <asm/io.h>		/*I/O头文件，以宏的嵌入汇编程序形式定义对I/O端口操作的函数。*/
#include <asm/uaccess.h>  /*与处理器相关的入口*/
#include <linux/uaccess.h>           //包含了copy_to_user、copy_from_user等内核访问用
#include <linux/timer.h>  /*内核定时器*/
#include <asm/atomic.h>  /*原子锁*/
#include <linux/slab.h>  /*包含了kcalloc、kzalloc内存分配函数的定义。*/
#include <linux/device.h> /*包含了device、class 等结构的定义*/

//#include <stdio.h>

#define XXX_MAJOR	255		/*预定义一个主设备号, 设备号唯一标识一个设备，主设备号标识与设备相关联的驱动程序，反应设备类型*/
#define DEV_NUM		2		/*设备数量*/

/*组成部分：
 * 驱动模块的加载与卸载
*/

#if 0
<include/linux/cdev.h>  

struct cdev {   
	struct kobject kobj;                  //内嵌的内核对象.  
	struct module *owner;                 //该字符设备所在的内核模块的对象指针.  
	const struct file_operations *ops;    //该结构描述了字符设备所能实现的方法，是极为关键的一个结构体.  
	struct list_head list;                //用来将已经向内核注册的所有字符设备形成链表.  
	dev_t dev;                            //字符设备的设备号，由主设备号和次设备号构成.  
	unsigned int count;                   //隶属于同一主设备号的次设备号的个数.  
};  
#endif


/*设备结构体*/
struct xxx_dev_t{
	struct cdev cdev;				/*设备结构体*/
	unsigned char value[4];				/*自定义的值*/
};

static struct xxx_dev_t *xxx_dev;			/*创建一个实例*/
static int xxx_major = XXX_MAJOR;

static struct class *xxx_class;				/*设备类*/
//static struct device *xxx_device;			/*设备*/

#if 0
struct file {
	union {
		struct llist_node	fu_llist;
		struct rcu_head 	fu_rcuhead;
	} f_u;
	struct path		f_path;
	struct inode		*f_inode;	/* cached value */
	const struct file_operations	*f_op;

	/*
	 * Protects f_ep_links, f_flags.
	 * Must not be taken from IRQ context.
	 */
	spinlock_t		f_lock;
	enum rw_hint		f_write_hint;
	atomic_long_t		f_count;
	unsigned int 		f_flags;
	fmode_t			f_mode;
	struct mutex		f_pos_lock;
	loff_t			f_pos;
	struct fown_struct	f_owner;
	const struct cred	*f_cred;
	struct file_ra_state	f_ra;

	u64			f_version;
#ifdef CONFIG_SECURITY
	void			*f_security;
#endif
	/* needed for tty driver, and maybe others */
	void			*private_data;

#ifdef CONFIG_EPOLL
	/* Used by fs/eventpoll.c to link all the hooks to this file */
	struct list_head	f_ep_links;
	struct list_head	f_tfile_llink;
#endif /* #ifdef CONFIG_EPOLL */
	struct address_space	*f_mapping;
	errseq_t		f_wb_err;
} __randomize_layout
  __attribute__((aligned(4)));	/* lest something weird decides that 2 is OK */
#endif
/*打开和关闭函数*/
static int xxx_open(struct inode *inode, struct file *filp)
{
	struct xxx_dev_t *dev;
	/*获得设备结构体指针*/
	/*container_of宏
	 * 1> Container_of在Linux内核中是一个常用的宏，用于从包含在某个结构中的指针获得结构本身的指针，通俗地讲就是通过结构体变量中某个成员的首地址进而获得整个结构体变量的首地址。
	 * 2>接口：
	 * container_of(ptr, type, member) 
	 * 	ptr:表示结构体中member的地址
	 * 	type:表示结构体类型
	 * 	member:表示结构体中的成员
	 * 通过ptr的地址可以返回结构体的首地址
	 * */
	dev = container_of(inode->i_cdev, struct xxx_dev_t, cdev);
	/*让设备结构体作为设备的私有信息*/
	filp->private_data = dev;
	return 0;
}

static int xxx_release(struct inode *inode, struct file *filp)
{
	return 0;
}

/*读取
 *文件操作指针，用户空间内存指针，要读取的长度，相对开始的偏移位置
 *内核空间与用户空间的内存不能互访,使用raw_copy_to/from_user()
 * */
static ssize_t xxx_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int len = 0;
#if 0
	/*获取设备操作*/
	struct xxx_dev_t *dev = filp->private_data;
	/*传回用户空间,count是用户请求的长度,应该与实际的长度做个比较*/
	/*to, from, count*/
	/*copy_to_user(),高版本中这个函数被下面的函数替换了*/
	if((len = raw_copy_to_user(buf, dev->value, 4)) < 0)
	{
		return -EFAULT;
	}
	*f_pos += len;						/*读取了做个偏移*/
#endif
	return len;
}

static ssize_t xxx_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	int len = 0;
#if 0
	struct xxx_dev_t *dev = filp->private_data;	/*获得设备结构体*/
	if((len = raw_copy_from_user(dev->value, buf, 4)) < 0)
	{
		return -EFAULT;
	}
	*f_pos += len;						/*做个偏移*/
#endif
	return len;
}

/*
 *I/O控制函数
 *cmd是事先定义好的I/O控制命令,
 *arg为对应于该命令的参数
 * */
static long xxx_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	/*
	struct xxx_dev_t *dev = filp->private_data;
	 switch(cmd)
	 {
		case CMD_1:
		case CMD_2:
			break;
		default:
		//未识别的命令
			return -ENOTTY;
	 }
	 * */
	return 0;
}

/*设备操作函数组，留给用户操作设备的接口*/
struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = xxx_open,
	.release = xxx_release,
	.read = xxx_read,
	.write = xxx_write,
	.unlocked_ioctl = xxx_ioctl,
};

/*传参参考struct_des.txt*/
/*设备注册*/
static void xxx_setup(struct xxx_dev_t *dev, int index)
{
	//...
	int err = 0;
	dev_t devno = MKDEV(xxx_major, index);		/*取得设备号*/
	//初始化cdev
	cdev_init(&dev->cdev, &fops);				/*fops 与 cdev建立链接*/
	dev->cdev.owner = THIS_MODULE;
	/*是可以一次性添加的，第一个设备号 DEV_NUM*/
	err = cdev_add(&dev->cdev, devno, 1);				/*注册设备，向系统添加一个cdev以完成注册,后面的是从设备数量*/
	if(err)
	{
		printk(KERN_INFO "cdev_add Register device err\n");
	}
}

/*驱动加载函数,返回值为0则是成功*/
static int __init xxx_init(void)
{
	int result;
	int index;		/*当前从设备号*/
	char dev_name[DEV_NUM][20];			/*设备节点名*/
	dev_t devno = MKDEV(xxx_major, 0);		/*从设备号为0，第一个设备*/
	/*申请设备号*/
	if(xxx_major){
		/*静态方式，使用自己定义的主设备号*/
		result = register_chrdev_region(devno, DEV_NUM, "example_driver");	/*所有设备数量中的第一个编号(其起始位置),
																		  必须包含主设备号,
																			设备数量,
																			设备名称*/
	} else {
		/*动态申请,其实是从0开始去查找的,都是调用的同一个函数去注册*/
		result = alloc_chrdev_region(&devno, 0, DEV_NUM, "example_driver");	/*,第一个从设备号，
																			设备数量，
																			设备名称*/
		xxx_major = MAJOR(devno);
	}
	if(result < 0)
	{
		return result;
	}

	xxx_class = class_create(THIS_MODULE, "example_driver");	/*创建设备类*/
	if(IS_ERR(xxx_class))
	{
		printk(KERN_INFO "Err:failed to create class\n");
		unregister_chrdev_region(devno, DEV_NUM);		/*注销设备号,第一个编号和数量*/
		return -EBUSY;
	}


	//其他操作
	xxx_dev = kmalloc(sizeof(struct xxx_dev_t)*DEV_NUM, GFP_KERNEL);	/*开辟空间*/
	if(NULL == xxx_dev)
	{
		result = -ENOMEM;
		goto fail_mem;
	}
	memset(xxx_dev, 0, sizeof(struct xxx_dev_t)*DEV_NUM);		/*清零*/
	
	for(index = 0; index < DEV_NUM; index++)
	{
		sprintf(dev_name[index], "exampledev%d", index);
		device_create(xxx_class, NULL, MKDEV(xxx_major, index), NULL, dev_name[index]);		/*mknod /dev/example0 c 255 0*/
		xxx_setup(xxx_dev+index, index);
	}
	printk(KERN_INFO "Ywl example char device init\n");
	return 0;
fail_mem:
	class_destroy(xxx_class);
	unregister_chrdev_region(devno, DEV_NUM);		/*注销设备号,第一个编号和数量*/
	return result;
}

/*注销驱动*/
static void __exit xxx_exit(void)
{
	int index;
	for(index = 0; index < DEV_NUM; index++)
	{
		device_destroy(xxx_class, MKDEV(xxx_major, index));		/*必须先删节点再删class*/	
		cdev_del(&((xxx_dev+index)->cdev));						/*删除设备*/
	}
	class_destroy(xxx_class);
	kfree(xxx_dev);												/*释放内存*/
	unregister_chrdev_region(MKDEV(xxx_major, 0), DEV_NUM);		/*注销设备号,第一个编号和数量*/
	printk(KERN_INFO "Ywl example char device exit\n");
}


MODULE_AUTHOR("ywl <astralrovers@outlook.com>");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("An example driver program");                                                                  
MODULE_ALIAS("a simple module");

module_init(xxx_init);
module_exit(xxx_exit);

