/******************************************    *******************************
      > File Name: platform_device.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月29日 星期三 15时02分22秒
      > Modify Time:
 *********************************    ***************************************/

/*platform总线、设备与驱动*/

/*
 * 设备驱动模型中，需关心总线、设备和驱动这3个实体，总线将设备和驱动绑定。
 *
 * 在系统每注册一个设备的时候，会寻找与之匹配的驱动；
 * 相反的，在系统每注册一个驱动的时候，会寻找与之匹配的设备，而匹配由总线完成。
 *
 * 一个现实的Linux设备和驱动通常都需要挂接在一种总线上，对于本身依附于PCI、USB、I
 * 2C、SPI等的设备而言，这自然不是问题，
 * 但是在嵌入式系统里面，在SoC系统中集成的独立外设控制器、挂接在
 * SoC内存空间的外设等却不依附于此类总线。
 *
 * 基于这一背景，Linux发明了一种虚拟的总线，称为platform总线，相应的设备称为platform_device，而驱动成为platform_driver。
 *
 * 注意：所谓的platform_device并不是与字符设备、块设备和网络设备并列的概念，而是Linux系统提供
 * 的一种附加手段，例如，我们通常把在SoC内部集成的I2C、RTC、LCD、看门狗等控制器都归纳为
 * platform_device，而它们本身就是字符设备
 * */
/*platform_device结构体
 * */
struct platform_device {
	const char *name;
	int id;
	boo id_auto;
	struct device dev;				/*这个属性才是真正的设备一员*/
	u32 num_resources;
	struct resource *resource;

	const struct platform_device_id *id_entry;
	char *driver_override; /* Driver name to force a match */

	/* MFD cell pointer */
	struct mfd_cell *mfd_cell;

	/* arch specific additions */
	struct pdev_archdata archdata;
};

struct device {
	struct device           *parent;
	struct device_private   *p;
	struct kobject          kobj;
	const char              *init_name;     /* initial name of the device 这个就是传统的bus_id，具体到每一个设备之后当做默认值 */
	struct device_type      *type;
	......
	struct bus_type         *bus;           /* type of bus device is on */
	struct device_driver    *driver;        /* which driver has allocated this device */
	void                    *driver_data;   /* data private to the driver */
	void                    *platform_data; /* Platform specific data, device core doesn't touch it,驱动和设备间传数据用的 */
	......
	void                    (*release)(struct device *dev);
};
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
 * flags 指资源类型，我们常用的是 IORESOURCE_MEM、IORESOURCE_IRQ  这两种。start 和 end 的含义会随着 flags而变更，如
 * a -- flags为IORESOURCE_MEM 时，start 、end 分别表示该platform_device占据的内存的开始地址和结束值；
 * b -- flags为 IORESOURCE_IRQ   时，start 、end 分别表示该platform_device使用的中断号的开始地址和结束值；
 * */

/*
 * platform_driver这个结构体中包含probe（）、remove（）、一个device_driver实例、电源管理函数
 * suspend（）、resume（）	platform_driver结构体
 *
 * */
struct platform_driver {
	int (*probe)(struct platform_device *);
	int (*remove)(struct platform_device *);
	void (*shutdown)(struct platform_device *);
	int (*suspend)(struct platform_device *
			, pm_message_t state);
	int (*resume)(struct platform_device *);
	struct device_driver driver;					/*真正的驱动一员*/
	const struct platform_device_id *id_table;
	bool prevent_deferred_probe;
};

/*
 * 直接填充platform_driver的suspend（）、resume（）做电源管理回调的方法目前已经过时，较好的做
 * 法是实现platform_driver的device_driver中的dev_pm_ops结构体成员（后续的Linux电源管理章节会对此进行
 * 更细致的介绍）
 * */

/*device_driver结构体*/
struct device_driver {
	const char *name;
	struct bus_type *bus;

	struct module *owner;
	const char *mod_name; /* used for built-in modules */

	bool suppress_bind_attrs; /* disables bind/unbind via sysfs */

	const struct of_device_id *of_match_table;
	const struct acpi_device_id *acpi_match_table;

	int (*probe) (struct device *dev);
	int (*remove) (struct device *dev);
	void (*shutdown) (struct device *dev);
	int (*suspend) (struct device *dev, pm_message_t state);
	int (*resume) (struct device *dev);
	const struct attribute_group **groups;

	const struct dev_pm_ops *pm;

	struct driver_private *p;
};

/*
 * 与platform_driver地位对等的i2c_driver、spi_driver、usb_driver、pci_driver中都包含了device_driver结构
 * 体实例成员。它其实描述了各种xxx_driver（xxx是总线名）在驱动意义上的一些共性
 *
 * 系统为platform总线定义了一个bus_type的实例platform_bus_type，其定义位于drivers/base/platform.c
 * 下
 * */
/*platform总线的bus_type实例platform_bus_type*/
struct bus_type platform_bus_type = {
	.name = "platform",
	.dev_groups = platform_dev_groups,
	.match = platform_match,
	.uevent = platform_uevent,
	.pm = &platform_dev_pm_ops,
};

struct bus_type {
	const char      *name;
	struct bus_attribute    *bus_attrs;
	struct device_attribute *dev_attrs;
	struct driver_attribute *drv_attrs;
	int (*match)(struct device *dev, struct device_driver *drv); //#####
	int (*uevent)(struct device *dev, struct kobj_uevent_env *env);
	int (*probe)(struct device *dev);
	int (*remove)(struct device *dev);
	void (*shutdown)(struct device *dev);
	int (*suspend)(struct device *dev, pm_message_t state);
	int (*suspend_late)(struct device *dev, pm_message_t state);
	int (*resume_early)(struct device *dev);
	int (*resume)(struct device *dev);
	struct dev_pm_ops *pm;
	struct bus_type_private *p; //看到这个private就有点C++类中的限定域关键字的意思，这个类的私有成员
};

/*这里要重点关注其match（）成员函数，正是此成员函数确定了platform_device和platform_driver之间是
 * 如何进行匹配*/

/*platform_bus_type的match（）成员函数*/
static int platform_match(struct device *dev, struct device_driver *drv)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct platform_driver *pdrv = to_platform_driver(drv);

	/* Attempt an OF style match first */
	if (of_driver_match_device(dev, drv))
		return 1;

	/* Then try ACPI style match */
	if (acpi_driver_match_device(dev, drv))
		return 1;

	/* Then try to match against the id table */
	if (pdrv->id_table)
		return platform_match_id(pdrv->id_table, pdev) != NULL;

	/* fall-back to driver name match */
	return (strcmp(pdev->name, drv->name) == 0);
}
/*
 * 匹配platform_device和platform_driver有4种可能性，
 * 一是基于设备树风格的匹配；
 * 二是基于ACPI风格的匹配；
 * 三是匹配ID表（即platform_device设备名是否出现在platform_driver的ID表内）；
 * 第四种是匹配platform_device设备名和驱动的名字
 * */
/*
 * 对platform_device的定义通常在BSP的板文件中实现，在板文件中，将
 * platform_device归纳为一个数组，最终通过platform_add_devices（）函数统一注册。
 * platform_add_devices（）函数可以将平台设备添加到系统中，这个函数的原型为：
 *
 * */
int platform_add_devices(struct platform_device **devs, int num);
/*
 * 该函数的第一个参数为平台设备数组的指针，第二个参数为平台设备的数量，它内部调用了
 * platform_device_register（）函数以注册单个的平台设备。
 * Linux 3.x之后，ARM Linux不太喜欢人们以编码的形式去填写platform_device和注册，而倾向于根据
 * 设备树中的内容自动展开platform_device
 * */

/*
 * platform设备资源和数据
 * */

/*
 * resource结构体定义
 * */
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
 * flags 指资源类型，我们常用的是 IORESOURCE_MEM、IORESOURCE_IRQ  这两种。start 和 end 的含义会随着 flags而变更，如
 * a -- flags为IORESOURCE_MEM 时，start 、end 分别表示该platform_device占据的内存的开始地址和结束值；
 * b -- flags为 IORESOURCE_IRQ   时，start 、end 分别表示该platform_device使用的中断号的开始地址和结束值；
 * */
/*
 * flags可以
 * 为IORESOURCE_IO、IORESOURCE_MEM、IORESOURCE_IRQ、IORE-SOURCE_DMA等。
 * start、end的含义会随着flags而变更，
 * 如当flags为IORESOURCE_MEM时，start、end分别表示该platform_device占据的内存的开始地址和结束地址；
 * 当flags为IORESOURCE_IRQ时，start、end分别表示该platform_device使用的中断号的开始值和结束值，
 * 如果只使用了1个中断号，开始和结束值相同。
 * 对于同种类型的资源而言，可以有多份，
 * 例如说某设备占据了两个内存区域，则可以定义两个IORESOURCE_MEM资源
 * */

/*
 * 对resource的定义也通常在BSP的板文件中进行，而在具体的设备驱动中通过platform_get_resource（）
 * 这样的API来获取，
 * */
struct resource *platform_get_resource(struct platform_device *, unsigned int, unsigned int);
/*例如在arch/arm/mach-at91/board-sam9261ek.c板文件中为DM9000网卡定义了如下resouce：*/
static struct resource dm9000_resource[] = {
	[0] = {
		.start = AT91_CHIPSELECT_2,
		.end = AT91_CHIPSELECT_2 + 3,
		.flags = IORESOURCE_MEM
	},
	[1] = {
		.start = AT91_CHIPSELECT_2 + 0x44,
		.end = AT91_CHIPSELECT_2 + 0xFF,
		.flags = IORESOURCE_MEM
	},
	[2] = {
		.flags = IORESOURCE_IRQ
			| IORESOURCE_IRQ_LOWEDGE | IORESOURCE_IRQ_HIGHEDGE,
	}
};

/*在DM9000网卡的驱动中则是通过如下办法拿到这3份资源*/
db->addr_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
db->data_res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
db->irq_res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
/*
 * 对于IRQ而言，platform_get_resource（）还有一个进行了封装的变体platform_get_irq（），其原型
 * 为：
 * */
int platform_get_irq(struct platform_device *dev, unsigned int num);
/*它实际上调用了“platform_get_resource（dev，IORESOURCE_IRQ，num）；”*/


/*
 * 设备除了可以在BSP中定义资源以外，还可以附加一些数据信息，因为对设备的硬件描述除了中断、
 * 内存等标准资源以外，可能还会有一些配置信息，而这些配置信息也依赖于板，不适宜直接放置在设备驱
 * 动上。因此，platform也提供了platform_data的支持，platform_data的形式是由每个驱动自定义的，如对于
 * DM9000网卡而言，platform_data为一个dm9000_plat_data结构体，完成定义后，就可以将MAC地址、总线
 * 宽度、板上有无EEPROM信息等放入platform_data中
 * */

static struct dm9000_plat_data dm9000_platdata = {
	.flags = DM9000_PLATF_16BITONLY | DM9000_PLATF_NO_EEPROM,
};

static struct platform_device dm9000_device = {
	.name = "dm9000"
		,
	.id = 0,
	.num_resources = ARRAY_SIZE(dm9000_resource),
	.resource = dm9000_resource,
	.dev = {
		.platform_data = &dm9000_platdata,
	}
};
/*
 * 而在DM9000网卡的驱动drivers/net/ethernet/davicom/dm9000.c的probe（）中，通过如下方式就拿到了
 * platform_data
 * */
struct dm9000_plat_data *pdata = dev_get_platdata(&pdev->dev);
/*
 * 其中，pdev为platform_device的指针。
 * 由以上分析可知，在设备驱动中引入platform的概念至少有如下好处。
 * 1）使得设备被挂接在一个总线上，符合Linux 2.6以后内核的设备模型。其结果是使配套的sysfs节
 * 点、设备电源管理都成为可能。
 * 2）隔离BSP和驱动。在BSP中定义platform设备和设备使用的资源、设备的具体配置信息，而在驱动
 * 中，只需要通过通用API去获取资源和数据，做到了板相关代码和驱动代码的分离，使得驱动具有更好的
 * 可扩展性和跨平台性。
 * 3）让一个驱动支持多个设备实例。譬如DM9000的驱动只有一份，但是我们可以在板级添加多份
 * DM9000的platform_device，它们都可以与唯一的驱动匹配。
 * */

