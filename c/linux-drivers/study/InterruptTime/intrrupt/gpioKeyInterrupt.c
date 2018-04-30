/******************************************    *******************************
      > File Name: gpioKeyInterrupt.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月21日 星期二 11时29分19秒
      > Modify Time:
 *********************************    ***************************************/

/*
 * ARM架构linux内核中，有5种常见的异常，其中中断异常是其一，
 * Linux内核将所有中断统一编号，使用一个irq_desc结构体来描述这些中断，
 * 里面记录了中断名称、中断状态、中断标记、并提供了中断的底层硬件访问函数（如：清除、屏蔽、使能中断），
 * 提供了这个中断的处理函数入口，通过它还可以调用用户注册的的中断处理函数
 *
 * 驱动工程师需要调用request_irq向内核注册中断，其原型为
 * */
request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags, const char * devname, void * dev_id);
/*
 * 第一个参数irq：中断号，在irqs.h中定义，与架构相关；
 * 第二个参数handler:  用户中断处理函数；
 * 第三个参数flags：中断标记；
 * 第四个参数devname：中断名字；可以通过cat proc/interrupts查看；
 * 第五个参数dev_id:  在free_irq中有用，也用作区分中断处理函数；
 * */

/*
 * 问：request_irq函数有什么作用？
 * 答：
 * 1）、irq_desc[irq]结构体中的action链表中已经链入了用户注册的中断处理函数。
 * 2）、中断触发方式已经被设置好。
 * 3）、中断已经被使能。
 * */
/*
 * 问：如何卸载中断处理函数？
 * 答：中断是一种很稀缺的资源，当不再使用一个设备时，应该释放它占据的中断。驱动工程师通过free_irq函数来实现，其原型为：
 * */
void free_irq(unsigned int irq, void *dev_id);
/*
 * 第一个参数irq：中断号，与request_irq中的irq一致，用于定位action链表；
 * 第二个参数dev_id：用于在action链表中找到要卸载的表项；同一个中断的不同中断处理函数必须使用不同的dev_id来区分，这就要求在注册共享中断时参数dev_id必须唯一。
 * 问：free_irq有什么作用？
 * 答：
 * 1）、根据中断号irq、dev_id从action链表中找到表项，将它移除。
 * 2）、如果它是唯一的表项，还要调用IRQ_DESC[IRQ].CHIP->SHUTDOWN或者IRQ_DESC[IRQ].CHIP->DISABLE来关闭中断。
 * 前面说了那么多中断，暂且放下中断，来谈谈linux是如何睡觉的吧，睡觉？linux也会睡觉？没错，只不过在术语上不叫睡觉，人家那叫休眠。
 * 问：linux内核如何进行休眠？
 * 答：使用wait_event函数，其扩展型常用的函数为wait_event_interruptible(wq, condition)，即可被中断打断的休眠。
 * wq是一个等待队列，condition是条件，如果condition = 0，则将会进行休眠，直到condition = 1，并且有唤醒函数唤醒它。
 * 问：linux内核如果唤醒进程？
 * 答：使用wait_up函数，其扩展型常用的函数为wake_up_interruptible(wq)，wq与wait_event_interruptible的wq是一致的。
 * 问：能直接使用wait_event_interruptible和wake_up_interruptible函数吗？
 * 答：不能，必须先事先使用static DECLARE_WAIT_QUEUE_HEAD(wq)定义并初始化一个等待队列头，并设置condition条件变量。
 * */

//实例程序参考:example.c
//测试程序:test.c




/**
 *  * struct irq_desc - interrupt descriptor
 *  * @irq:        interrupt number for this descriptor
 *  * @handle_irq:     highlevel irq-events handler [if NULL, __do_IRQ()]
 *  * @chip:       low level interrupt hardware access
 *  * @action:     the irq action chain
 *  * @status:     status information
 *  * @irq_count:      stats field to detect stalled irqs
 *  * @name:       flow handler name for /proc/interrupts output
 *  */
struct irq_desc {
	unsigned int        irq;
	......
	irq_flow_handler_t  handle_irq;
	struct irq_chip     *chip;
	......
	struct irqaction    *action;    /* IRQ action list */
	unsigned int        status;     /* IRQ status */
	......
	unsigned int        irq_count;  /* For detecting broken IRQs */
	......
	const char      *name;
} ____cacheline_internodealigned_in_smp;


/*
 * drivers/input/keyboard/gpio_keys.c是一个放之四海皆准的GPIO按键驱动，为了让该驱动在特定的电路
 * 板上工作，通常只需要修改arch/arm/mach-xxx下的板文件或者修改device tree对应的dts。该驱动会为每个
 * GPIO申请中断，在gpio_keys_setup_key（）函数中进行。注意最后一个参数bdata，会被传入中断服务程
 * 序
 * */

/*GPIO按键驱动中断申请*/
static int gpio_keys_setup_key(struct platform_device *pdev,
		struct input_dev *input,
		struct gpio_button_data *bdata,
		const struct gpio_keys_button *button)
{
	...

	/*
	 * request_any_context_irq（）会根据GPIO控制器本身的“上级”中断是否为threaded_irq
	 * 来决定采用request_irq（）还是request_threaded_irq（）
	 * */
	error = request_any_context_irq(bdata->irq, isr, irqflags, desc, bdata);
	/*
	 * 一组GPIO（如32个GPIO）虽然每个都提供一个中断，并且都有中断号，
	 * 但是在硬件上一组GPIO通常是嵌套在上一级的中断控制器上的一个中断。
	 * request_any_context_irq（）也有一个变体是devm_request_any_context_irq（）。
	 * */
	if (error < 0) {
		dev_err(dev,
				"Unable to claim irq %d; error %d\n"
				,
				bdata->irq, error);
		goto fail;
	}
	...
}

/*GPIO按键驱动中断释放*/
static void gpio_remove_key(struct gpio_button_data *bdata)
{
	free_irq(bdata->irq, bdata);
	if (bdata->timer_debounce)
		del_timer_sync(&bdata->timer);
	cancel_work_sync(&bdata->work);
	if (gpio_is_valid(bdata->button->gpio))
		gpio_free(bdata->button->gpio);
}


/*
 * GPIO按键驱动的中断处理比较简单，没有明确地分为上下两个半部，而只存在顶半部，
 * GPIO按键驱动中断处理程序
 * */

static irqreturn_t gpio_keys_gpio_isr(int irq, void *dev_id)
{
	/*
	 * 直接从dev_id取出了bdata，这就是对应的那个GPIO键的数据结构，之后根据情况启动timer以进
	 * 行debounce或者直接调度工作队列去汇报按键事件
	 * */
	struct gpio_button_data *bdata = dev_id;

	BUG_ON(irq != bdata->irq);

	if (bdata->button->wakeup)
		pm_stay_awake(bdata->input->dev.parent);
	if (bdata->timer_debounce)
		mod_timer(&bdata->timer,
				jiffies + msecs_to_jiffies(bdata->timer_debounce));
	else
		schedule_work(&bdata->work);

	return IRQ_HANDLED;
}


/*
 * 在GPIO按键驱动初始化的时候，通过
 * INIT_WORK（&bdata->work，gpio_keys_gpio_work_func）初始化了bdata->work，对应的处理函数是
 * gpio_keys_gpio_work_func（）
 *
 * GPIO按键驱动的工作队列底半部
 * */
static void gpio_keys_gpio_work_func(struct work_struct *work)
{
	/*
	 * 它通过container_of（）再次从work_struct反向解析出了bdata。原因是
	 * work_struct本身在定义时，就嵌入在gpio_button_data结构体内。读者朋友们应该掌握Linux的这种可以到
	 * 处获取一个结构体指针的技巧，它实际上类似于面向对象里面的“this”指针
	 * */
	struct gpio_button_data *bdata =
		container_of(work, struct gpio_button_data, work);

	gpio_keys_gpio_report_event(bdata);

	if (bdata->button->wakeup)
		pm_relax(bdata->input->dev.parent);
}

struct gpio_button_data {
	const struct gpio_keys_button *button;
	struct input_dev *input;
	struct timer_list timer;
	struct work_struct work;
	unsigned int timer_debounce; /* in msecs */
	unsigned int irq;
	spinlock_t lock;
	bool disabled;
	bool key_pressed;
};
