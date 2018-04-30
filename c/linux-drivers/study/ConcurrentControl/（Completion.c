/******************************************    *******************************
      > File Name: （Completion.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月17日 星期五 17时24分15秒
      > Modify Time:
 *********************************    ***************************************/

Linux提供了完成量（Completion，关于这个名词，至今没有好的翻译，笔者将其译为“完成量”），它
用于一个执行单元等待另一个执行单元执行完某事。
Linux中与完成量相关的操作主要有以下4种。
1.定义完成量
下列代码定义名为my_completion的完成量：
struct completion my_completion;
2.初始化完成量
下列代码初始化或者重新初始化my_completion这个完成量的值为0（即没有完成的状态）：
init_completion(&my_completion);
reinit_completion(&my_completion)
	3.等待完成量
	下列函数用于等待一个完成量被唤醒：
	void wait_for_completion(struct completion *c);
	4.唤醒完成量
	下面两个函数用于唤醒完成量：
	void complete(struct completion *c);
	void complete_all(struct completion *c);
	前者只唤醒一个等待的执行单元，后者释放所有等待同一完成量的执行单元。
	完成量用于同步的流程一般如下：
	7.9 增加并发控制后的globalmem的设备驱动
	在globalmem（）的读写函数中，由于要调用copy_from_user（）、copy_to_user（）这些可能导致阻
	塞的函数，因此不能使用自旋锁，宜使用互斥体。
	驱动工程师习惯将某设备所使用的自旋锁、互斥体等辅助手段也放在设备结构中，因此，可如代码清
	单7.4那样修改globalmem_dev结构体的定义，并在模块初始化函数中初始化这个信号量，如代码清单7.5所
	示。
	代码清单7.4 增加并发控制后的globalmem设备结构体
	1struct globalmem_dev {
		2 struct cdev cdev;
		3 unsigned char mem[GLOBALMEM_SIZE];
		4 struct mutex mutex;
		5};
代码清单7.5 增加并发控制后的globalmem设备驱动模块加载函数
1static int __init globalmem_init(void)
	2{
		3 int ret;
		4 dev_t devno = MKDEV(globalmem_major, 0);
		5
			6 if (globalmem_major)
			7 ret = register_chrdev_region(devno, 1,
					"globalmem");
		8 else {
			9 ret = alloc_chrdev_region(&devno, 0, 1,
					"globalmem");
			10 globalmem_major = MAJOR(devno);
			11 }
		12 if (ret < 0)
			13 return ret;
		14
			15 globalmem_devp = kzalloc(sizeof(struct globalmem_dev), GFP_KERNEL);
		16 if (!globalmem_devp) {
			17 ret = -ENOMEM;
			18 goto fail_malloc;
			19 }
		20
			21 mutex_init(&globalmem_devp->mutex);
		22 globalmem_setup_cdev(globalmem_devp, 0);
		23 return 0;
		24
			25 fail_malloc:
			26 unregister_chrdev_region(devno, 1);
		27 return ret;
		28}
		29module_init(globalmem_init);
		在访问globalmem_dev中的共享资源时，需先获取这个互斥体，访问完成后，随即释放这个互斥体。
		驱动中新的globalmem读、写操作如代码清单7.6所示。
		代码清单7.6 增加并发控制后的globalmem读、写操作
		1static ssize_t globalmem_read(struct file *filp, char __user * buf, size_t size,
				2 loff_t * ppos)
	3{
		4 unsigned long p = *ppos;
		5 unsigned int count = size;
		6 int ret = 0;
		7 struct globalmem_dev *dev = filp->private_data;
		8
			9 if (p >= GLOBALMEM_SIZE)
			10 return 0;
		11 if (count > GLOBALMEM_SIZE - p)
			12 count = GLOBALMEM_SIZE - p;
		13
			14 mutex_lock(&dev->mutex);
		15
			16 if (copy_to_user(buf, dev->mem + p, count)) {
				17 ret = -EFAULT;
				18 } else {
					19 *ppos += count;
					20 ret = count;
					21
						22 printk(KERN_INFO "read %u bytes(s) from %lu\n"
								, count, p);
					23 }
		24
			25 mutex_unlock(&dev->mutex);
		26
			27 return ret;
		28}
		29
		30static ssize_t globalmem_write(struct file *filp, const char __user * buf,
				31 size_t size, loff_t * ppos)
	32{
		33 unsigned long p = *ppos;
		34 unsigned int count = size;
		35 int ret = 0;
		36 struct globalmem_dev *dev = filp->private_data;
		37
			38 if (p >= GLOBALMEM_SIZE)
			39 return 0;
		40 if (count > GLOBALMEM_SIZE - p)
			41 count = GLOBALMEM_SIZE - p;
		42
			43 mutex_lock(&dev->mutex);
		44
			45 if (copy_from_user(dev->mem + p, buf, count))
			46 ret = -EFAULT;
		47 else {
			48 *ppos += count;
			49 ret = count;
			50
				51 printk(KERN_INFO "written %u bytes(s) from %lu\n"
						, count, p);
			52 }
		53
			54 mutex_unlock(&dev->mutex);
		55
			56 return ret;
		57}
		代码第14行和第43行用于获取互斥体，代码第25和54行用于在对临界资源访问结束后释放信号量。
		除了globalmem的读、写操作之外，如果在读、写的同时，另一个执行单元执行MEM_CLEAR IO控制
		命令，也会导致全局内存的混乱，因此，globalmem_ioctl（）函数也需被重写，如代码清单7.7所示。
		代码清单7.7 增加并发控制后的globalmem设备驱动ioctl（）函数
		1static long globalmem_ioctl(struct file *filp, unsigned int cmd,
				2 unsigned long arg)
	3{
		4 struct globalmem_dev *dev = filp->private_data; /* μ éè± á11ì */
		5
			6 switch (cmd) {
				7 case MEM_CLEAR:
					8 mutex_lock(&dev->mutex);
				9 memset(dev->mem, 0, GLOBALMEM_SIZE);
				10 mutex_unlock(&dev->mutex);
				11
					12 printk(KERN_INFO "globalmem is set to zero\n");
				13 break;
				14
					15 default:
					16 return -EINVAL;
				17 }
		18
			19 return 0;
		20}
		增加并发控制后globalmem的完整驱动位于本书虚拟机的例子/kernel/drivers/globalmem/ch7目录下，其
		使用方法与第6章globalmem驱动在用户空间的验证一致。
