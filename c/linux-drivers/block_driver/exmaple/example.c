/******************************************    *******************************
      > File Name: example.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     Wed 06 Dec 2017 10:52:26 AM CST
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

#define XXX_MAJOR				0

static struct request_queue_t *request_queue;				/*请求队列*/
static spinlock_t lock;										/*控制访问队列权限的自旋锁*/
static gendisk *xxx_disk;									/*磁盘*/

static request_fn_proc (*xxx_rfn)(struct request_queue *q);						/*请求回调函数*/

 struct block_device_operations fops ={
	.owner = THIS_MODULE,
	.open = xxx_open,
	.release = xxx_release,
 };
/*
 块设备初始化
 */
static int xxx_init(void)
{
	int err;
	/*块设备驱动注册*/
	if(register_blkdev(XXX_MAJOR, "blk_example"))
	{
		err = -EIO;
		goto out;
	}

	/*初始化请求队列*/
	request_queue = blk_init_queue(xxx_rfn, &lock);
	if(!request_queue)
	{
		goto out_queue;
	}
	/*用于通知通用块层和I/O调度器该请求队列支持的每个请求中能够包含的最大扇区数*/
	blk_queue_max_hw_sectors(request_queue, 255);
	/*用于告知该请求队列的逻辑块大小*/
	blk_queue_logical_block_size(request_queue, 512);

	/*gendisk通用磁盘初始化*/
	xxx_disk->major = XXX_MAJOR;
	xxx_disk->first_minor = 0;
	xxx_disks->fops = &fops;
	xxx_disks->queue = request_queue;
	sprintf(xxx_disks->disk_name, "xxx%d", i);
	set_capacity(xxx_disks, xxx_size * 2);
	add_disk(xxx_disks); /* 添加gendisk */
	return 0;
out_queue:
	/*注销块设备驱动*/
	unregister_blkdev(XXX_MAJOR, "blk_example");
out:
	/*删除gendisk引用*/
	put_disk(xxx_disk);
	/*清除请求队列*/
	blk_cleanup_queue(xxx_queue);
	return -ENOMEM;
}
/*块设备的打开与释放*/
static int xxx_open(strutc block_device *bdev, fmode_t mode)
{
	struct xxx_dev *dev = bdev->bd_disk->private_data;
	return 0;
}
static void release(struct gendisk *disk, fmode_t mode)
{
	struct xxx_dev *dev = disk->private_data;

}


