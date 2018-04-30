/******************************************    *******************************
  > File Name: platform_device_sum.c
  > Author: Ywl
  > Descripsion:
  > Created Time:     2017年11月29日 星期三 16时08分15秒
  > Modify Time:
 *********************************    ***************************************/

/*Linux 设备驱动开发 —— platform设备驱动应用实例解析*/

/*
一、platform 驱动的工作过程
platform模型驱动编程，需要实现platform_device(设备)与platform_driver（驱动）在platform(虚拟总线)上的注册、匹配，相互绑定，然后再做为一个普通的字符设备进行相应的应用，总之如果编写的是基于字符设备的platform驱动，在遵循并实现platform总线上驱动与设备的特定接口的情况下，最核心的还是字符设备的核心结构：cdev、 file_operations（他包含的操作函数接口）、dev_t(设备号)、设备文件（/dev）等，因为用platform机制编写的字符驱动，它的本质是字符驱动。
我们要记住，platform 驱动只是在字符设备驱动外套一层platform_driver 的外壳。
在一般情况下，2.6内核中已经初始化并挂载了一条platform总线在sysfs文件系统中。那么我们编写platform模型驱动时，需要完成两个工作：
a -- 实现platform驱动
b -- 实现platform设备
然而在实现这两个工作的过程中还需要实现其他的很多小工作，在后面介绍。platform模型驱动的实现过程核心架构就很简单，如下所示：
	定义platform_device
			|
			|
			V
	注册义platform_device
			|
			|
			V
	定义platform_driver
			|
			|
			V
	注册义platform_driver

	platform驱动模型三个对象：platform总线、platform设备、platform驱动。
	platform总线对应的内核结构：struct bus_type-->它包含的最关键的函数：match() (要注意的是，这块由内核完成，我们不参与)
	platform设备对应的内核结构：struct platform_device-->注册：platform_device_register(unregister)
	platform驱动对应的内核结构：struct platform_driver-->注册：platform_driver_register(unregister)

	那具体platform驱动的工作过程是什么呢：
	设备(或驱动)注册的时候，都会引发总线调用自己的match函数来寻找目前platform总线是否挂载有与该设备(或驱动)名字匹配的驱动（或设备），如果存在则将双方绑定；
	如果先注册设备，驱动还没有注册，那么设备在被注册到总线上时，将不会匹配到与自己同名的驱动，然后在驱动注册到总线上时，因为设备已注册，那么总线会立即匹配与绑定这时的同名的设备与驱动，再调用驱动中的probe函数等；
	如果是驱动先注册，同设备驱动一样先会匹配失败，匹配失败将导致它的probe函数暂不调用，而是要等到设备注册成功并与自己匹配绑定后才会调用。

*/
/*
	二、实现platform 驱动与设备的详细过程
	1、思考问题？
	在分析platform 之前，可以先思考一下下面的问题：
	a -- 为什么要用 platform 驱动？不用platform驱动可以吗？
	b -- 设备驱动中引入platform 概念有什么好处？
	现在先不回答，看完下面的分析就明白了，后面会附上总结。

	2、platform_device 结构体 VS platform_driver 结构体
	这两个结构体分别描述了设备和驱动，二者有什么关系呢？先看一下具体结构体对比
	设备（硬件部分）：中断号，寄存器，DMA等
	*/
/*
	platform_device 结构体
	驱动（软件部分）
	platform_driver 结构体
	*/
struct platform_device {										struct platform_driver {
	const char    *name;       /*名字*/								int (*probe)(struct platform_device *);
	int        id;													//	硬件和软件匹配成功之后调用该函数
	bool        id_auto;											int (*remove)(struct platform_device *);
	struct device    dev;   /*硬件模块必须包含该结构体*/			//	硬件卸载了调用该函数
	u32        num_resources;        /*资源个数*/					void (*shutdown)(struct platform_device *);
	struct resource    *resource;         /*资源  人脉*/			int (*suspend)(struct platform_device *, pm_message_t state);
	const struct platform_device_id    *id_entry;					int (*resume)(struct platform_device *);
	/* arch specific additions */									struct device_driver driver;
	struct pdev_archdata    archdata;								//内核里所有的驱动程序必须包含该结构体
};																	const struct platform_device_id *id_table;
																	/*对于一个驱动匹配多个设备的情况*/
																};

/*设备实例：*/
static struct platform_device hello_device=
{
	.name = "bigbang",
	.id = -1,
	.dev.release = hello_release,
};
/*驱动实例：*/
static struct platform_driver hello_driver=
{
	.driver.name = "bigbang",
	.probe = hello_probe,
	.remove = hello_remove,
};

/*
前面提到，实现platform模型的过程就是总线对设备和驱动的匹配过程 。打个比方，就好比相亲，总线是红娘，设备是男方，驱动是女方：
a -- 红娘（总线）负责男方（设备）和女方（驱动）的撮合；
b -- 男方（女方）找到红娘，说我来登记一下，看有没有合适的姑娘（汉子）—— 设备或驱动的注册；
c -- 红娘这时候就需要看看有没有八字（二者的name 字段）匹配的姑娘（汉子）——match 函数进行匹配，看name是否相同；
d -- 如果八字不合，就告诉男方（女方）没有合适的对象，先等着，别急着乱做事 —— 设备和驱动会等待，直到匹配成功；
e -- 终于遇到八字匹配的了，那就结婚呗！接完婚，男方就向女方交代，我有多少存款，我的房子在哪，钱放在哪等等（ struct resource    *resource），女方说好啊，于是去房子里拿钱，去给男方买菜啦，给自己买衣服、化妆品、首饰啊等等（int (*probe)(struct platform_device *) 匹配成功后驱动执行的第一个函数），当然如果男的跟小三跑了（设备卸载），女方也不会继续待下去的（  int (*remove)(struct platform_device *)）。
*/

/*
3、设备资源结构体
在struct platform_device 结构体中有一重要成员 struct resource *resource
*/
struct resource {
	resource_size_t start;  /*资源起始地址*/
	resource_size_t end;   /*资源结束地址*/
	const char *name;
	unsigned long flags;   /*区分是资源什么类型的*/
	struct resource *parent, *sibling, *child;
};

#define IORESOURCE_MEM        0x00000200
#define IORESOURCE_IRQ        0x00000400
/*
flags 指资源类型，我们常用的是 IORESOURCE_MEM、IORESOURCE_IRQ  这两种。start 和 end 的含义会随着 flags而变更，如
a -- flags为IORESOURCE_MEM 时，start 、end 分别表示该platform_device占据的内存的开始地址和结束值；
b -- flags为 IORESOURCE_IRQ   时，start 、end 分别表示该platform_device使用的中断号的开始地址和结束值；
*/
/*下面看一个实例：*/
static struct  resource beep_resource[] =
{
	[0] = {
		.start = 0x114000a0,
		.end = 0x114000a0+0x4,
		.flags = IORESOURCE_MEM,
	},

	[1] = {
		.start = 0x139D0000,
		.end = 0x139D0000+0x14,
		.flags = IORESOURCE_MEM,
	},
};
/*
4、将字符设备添加到 platform的driver中
前面我们提到platform 驱动只是在字符设备驱动外套一层platform_driver 的外壳，下面我们看一下添加的过程：
*/
static struct file_operations hello_ops=
{
	.open = hello_open,
	.release = hello_release,
	.unlocked_ioctl = hello_ioctl,
};

static int hello_remove(struct platform_device *pdev)
{
//	注销分配的各种资源
}

static int hello_probe(struct platform_device *pdev)
{
/*
	1.申请设备号
		2.cdev初始化注册,&hello_ops
		3.从pdev读出硬件资源
		4.对硬件资源初始化，ioremap，request_irq( )
		*/
}

static int hello_init(void)
{
//	只注册 platform_driver
}

static void hello_exit(void)
{
//	只注销 platform_driver
}
/*
可以看到，模块加载和卸载函数仅仅通过paltform_driver_register()、paltform_driver_unregister() 函数进行 platform_driver 的注册和注销，而原先注册和注销字符设备的工作已经被移交到 platform_driver 的 probe() 和 remove() 成员函数中。
*/
/*
5、platform是如何匹配device和driver
这时就该总线出场了，系统为platform总线定义了一个bus_type 的实例platform_bus_type，其定义如下：
*/
struct bus_type platform_bus_type = {
	.name        = "platform",
	.dev_groups    = platform_dev_groups,
	.match        = platform_match,
	.uevent        = platform_uevent,
	.pm        = &platform_dev_pm_ops,
};

/*
其又是怎样工作的呢？在platform.c (e:\linux-3.14-fs4412\drivers\base)    31577    2014/3/31 中可以看到
*/
__platform_driver_register（）
{
	drv->driver.bus = &platform_bus_type;     //536行
}
/*在 platform_bus_type 中调用 了platform_match：*/
static int platform_match(struct device *dev, struct device_driver *drv)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct platform_driver *pdrv = to_platform_driver(drv);

	//匹配设备树信息，如果有设备树，就调用 of_driver_match_device() 函数进行匹配, 一对多
		if (of_driver_match_device(dev, drv))
			return 1;

	/* Then try ACPI style match */
	if (acpi_driver_match_device(dev, drv))
		return 1;

	//匹配id_table,一对多
		if (pdrv->id_table)
			return platform_match_id(pdrv->id_table, pdev) != NULL;

	//最基本匹配规则,name 一对一
		return (strcmp(pdev->name, drv->name) == 0);
}


/*
6、解决问题

现在可以回答这两个问题了
a -- 为什么要用 platform 驱动？不用platform驱动可以吗？
b -- 设备驱动中引入platform 概念有什么好处？
引入platform模型符合Linux 设备模型 —— 总线、设备、驱动，设备模型中配套的sysfs节点都可以用，方便我们的开发；当然你也可以选择不用，不过就失去了一些platform带来的便利；
设备驱动中引入platform 概念，隔离BSP和驱动。在BSP中定义platform设备和设备使用的资源、设备的具体匹配信息，而在驱动中，只需要通过API去获取资源和数据，做到了板相关代码和驱动代码的分离，使得驱动具有更好的可扩展性和跨平台性。
*/

/*
三、实例
这是一个蜂鸣器的驱动，其实前面已经有解析 Linux 字符设备驱动开发基础（二）—— 编写简单 PWM 设备驱动， 下面来看一下，套上platform 外壳后的程序：
1、device.c
*/
#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>

static struct resource beep_resource[] =
{
	[0] ={
		.start = 0x114000a0,
		.end =  0x114000a0 + 0x4,
		.flags = IORESOURCE_MEM,
	},

	[1] ={
		.start = 0x139D0000,
		.end =  0x139D0000 + 0x14,
		.flags = IORESOURCE_MEM,
	}
};

static void hello_release(struct device *dev)
{
	printk("hello_release\n");
	return ;
}



static struct platform_device hello_device=
{
	.name = "bigbang",
	.id = -1,
	.dev.release = hello_release,
	.num_resources = ARRAY_SIZE(beep_resource),
	.resource = beep_resource,
};

static int hello_init(void)
{
	printk("hello_init");
	return platform_device_register(&hello_device);
}

static void hello_exit(void)
{
	printk("hello_exit");
	platform_device_unregister(&hello_device);
	return;
}

MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);

/*2、driver.c*/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <asm/io.h>

static int major = 250;
static int minor=0;
static dev_t devno;
static struct class *cls;
static struct device *test_device;

#define TCFG0         0x0000
#define TCFG1         0x0004
#define TCON          0x0008
#define TCNTB0        0x000C
#define TCMPB0        0x0010

static unsigned int *gpd0con;
static void *timer_base;

#define  MAGIC_NUMBER    'k'
#define  BEEP_ON    _IO(MAGIC_NUMBER    ,0)
#define  BEEP_OFF   _IO(MAGIC_NUMBER    ,1)
#define  BEEP_FREQ   _IO(MAGIC_NUMBER   ,2)

static void fs4412_beep_init(void)
{
	writel ((readl(gpd0con)&~(0xf<<0)) | (0x2<<0),gpd0con);
	writel ((readl(timer_base +TCFG0  )&~(0xff<<0)) | (0xff <<0),timer_base +TCFG0);
	writel ((readl(timer_base +TCFG1 )&~(0xf<<0)) | (0x2 <<0),timer_base +TCFG1 );

	writel (500, timer_base +TCNTB0  );
	writel (250, timer_base +TCMPB0 );
	writel ((readl(timer_base +TCON )&~(0xf<<0)) | (0x2 <<0),timer_base +TCON );
}

void fs4412_beep_on(void)
{
	writel ((readl(timer_base +TCON )&~(0xf<<0)) | (0x9 <<0),timer_base +TCON );
}

void fs4412_beep_off(void)
{
	writel ((readl(timer_base +TCON )&~(0xf<<0)) | (0x0 <<0),timer_base +TCON );
}

static void beep_unmap(void)
{
	iounmap(gpd0con);
	iounmap(timer_base);
}

static int beep_open (struct inode *inode, struct file *filep)
{
	fs4412_beep_on();
	return 0;
}

static int beep_release(struct inode *inode, struct file *filep)
{
	fs4412_beep_off();
	return 0;
}

#define BEPP_IN_FREQ 100000
static void beep_freq(unsigned long arg)
{
	writel(BEPP_IN_FREQ/arg, timer_base +TCNTB0  );
	writel(BEPP_IN_FREQ/(2*arg), timer_base +TCMPB0 );

}

static long beep_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
	switch(cmd)
	{
		case BEEP_ON:
			fs4412_beep_on();
			break;
		case BEEP_OFF:
			fs4412_beep_off();
			break;
		case BEEP_FREQ:
			beep_freq( arg );
			break;
		default :
			return -EINVAL;
	}
	return 0;
}

static struct file_operations beep_ops=
{
	.open     = beep_open,
	.release = beep_release,
	.unlocked_ioctl      = beep_ioctl,
};

static int beep_probe(struct platform_device *pdev)
{
	int ret;
	printk("match ok!");

	gpd0con = ioremap(pdev->resource[0].start,pdev->resource[0].end - pdev->resource[0].start);
	timer_base = ioremap(pdev->resource[1].start, pdev->resource[1].end - pdev->resource[1].start);

	devno = MKDEV(major,minor);
	ret = register_chrdev(major,"beep",&beep_ops);

	cls = class_create(THIS_MODULE, "myclass");
	if(IS_ERR(cls))
	{
		unregister_chrdev(major,"beep");
		return -EBUSY;
	}

	test_device = device_create(cls,NULL,devno,NULL,"beep");//mknod /dev/hello
	if(IS_ERR(test_device))
	{
		class_destroy(cls);
		unregister_chrdev(major,"beep");
		return -EBUSY;
	}

	fs4412_beep_init();

	return 0;
}

static int beep_remove(struct platform_device *pdev)
{
	beep_unmap();
	device_destroy(cls,devno);
	class_destroy(cls);
	unregister_chrdev(major,"beep");

	return 0;
}


static struct platform_driver beep_driver=
{
	.driver.name = "bigbang",
	.probe = beep_probe,
	.remove = beep_remove,
};


static int beep_init(void)
{
	printk("beep_init");

	return platform_driver_register(&beep_driver);
}

static void beep_exit(void)
{
	printk("beep_exit");
	platform_driver_unregister(&beep_driver);

	return;
}


MODULE_LICENSE("GPL");
module_init(beep_init);
module_exit(beep_exit);

/*3、makefile*/
ifneq  ($(KERNELRELEASE),)
	obj-m:=device.o driver.o
	$(info "2nd")
	else
#KDIR := /lib/modules/$(shell uname -r)/build
	KDIR := /home/fs/linux/linux-3.14-fs4412
	PWD:=$(shell pwd)
	all:
	$(info "1st")
	make -C $(KDIR) M=$(PWD) modules
	clean:
	rm -f *.ko *.o *.symvers *.mod.c *.mod.o *.order
	endif

/*	4、test.c*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
	int fd,i,lednum;

	fd = open("/dev/beep",O_RDWR);
	if(fd<0)
	{
		perror("open fail \n");
		return ;
	}

	sleep(10);
	close(fd);
}
