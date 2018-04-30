/******************************************    *******************************
      > File Name: interrupt.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月21日 星期二 08时45分22秒
      > Modify Time:
 *********************************    ***************************************/

/*
 * 所谓中断是指CPU在执行程序的过程中，出现了某些突发事件急待处理，CPU必须暂停当前程序的执
 * 行，转去处理突发事件，处理完毕后又返回原程序被中断的位置继续执行。
 * 根据中断的来源，中断可分为内部中断和外部中断，内部中断的中断源来自CPU内部（软件中断指
 * 令、溢出、除法错误等，例如，操作系统从用户态切换到内核态需借助CPU内部的软件中断），外部中断
 * 的中断源来自CPU外部，由外设提出请求。
 *
 * 根据中断是否可以屏蔽，中断可分为可屏蔽中断与不可屏蔽中断（NMI），
 * 可屏蔽中断可以通过设置中断控制器寄存器等方法被屏蔽，屏蔽后，该中断不再得到响应，
 * 而不可屏蔽中断不能被屏蔽。
 *
 * 根据中断入口跳转方法的不同，中断可分为向量中断和非向量中断。
 *
 * 采用向量中断的CPU通常为不同的中断分配不同的中断号，
 * 当检测到某中断号的中断到来后，就自动跳转到与该中断号对应的地址执行。
 * 不同中断号的中断有不同的入口地址。
 *
 * 非向量中断的多个中断共享一个入口地址，进入该入口地址后，再通过软件判断中断标志来识别具体是哪个中断。
 *
 * 也就是说，向量中断由硬件提供中断服务程序入口地址，非向量中断由软件提供中断服务程序入口地址
 * */
//非向量中断的一个典型应用程序
irq_handler()
{
	...
		int int_src = read_int_status(); /* 读硬件的中断相关寄存器 */
	switch (int_src) { /* 判断中断源 */
		case DEV_A:
			dev_a_handler();
			break;
		case DEV_B:
			dev_b_handler();
			break;
			...
		default:
				break;
	}
	...
}

/*
 * 嵌入式系统以及x86PC中大多包含可编程中断控制器（PIC），许多MCU内部就集成了PIC。如在
 * 80386中，PIC是两片i8259A芯片的级联。
 *
 * 通过读写PIC的寄存器，程序员可以屏蔽/使能某中断及获得中断
 * 状态，前者一般通过中断MASK寄存器完成，后者一般通过中断PEND寄存器完成
 *
 *
 * 在ARM多核处理器里最常用的中断控制器是GIC（Generic Interrupt Controller）
 *
 * SGI（Software Generated Interrupt）：软件产生的中断，可以用于多核的核间通信，一个CPU可以通过
 * 写GIC的寄存器给另外一个CPU产生中断。多核调度用的IPI_WAKEUP、IPI_TIMER、
 * IPI_RESCHEDULE、IPI_CALL_FUNC、IPI_CALL_FUNC_SINGLE、IPI_CPU_STOP、IPI_IRQ_WORK、
 * IPI_COMPLETION都是由SGI产生的。
 * PPI（Private Peripheral Interrupt）：某个CPU私有外设的中断，这类外设的中断只能发给绑定的那个
 * CPU。
 * SPI（Shared Peripheral Interrupt）：共享外设的中断，这类外设的中断可以路由到任何一个CPU。
 *
 * */

/*对于SPI类型的中断，内核可以通过如下API设定中断触发的CPU核：*/

extern int irq_set_affinity (unsigned int irq, const struct cpumask *m);

/*在ARM Linux默认情况下，中断都是在CPU0上产生的，比如，我们可以通过如下代码把中断irq设定
 * 到CPU i上去：*/

irq_set_affinity(irq, cpumask_of(i));

/*
 * 设备的中断会打断内核进程中的正常调度和运行，系统对更高吞吐率的追求势必要求中断服务程序尽
 * 量短小精悍。
 * 但是，这个良好的愿望往往与现实并不吻合。在大多数真实的系统中，当中断到来时，要完
 * 成的工作往往并不会是短小的，它可能要进行较大量的耗时处理。
 *
 ***********************************************************************************************************************
 * 为了在中断执行时间尽量短和中断处理需完成的工作尽量大之间找到一个平衡点，
 * Linux将中断处理程序分解为两个半部：顶半部（Top Half）和底半部（BottomHalf）。
 *
 *
 * 顶半部用于完成尽量少的比较紧急的功能，它往往只是简单地读取寄存器中的中断状态，并在清除中
 * 断标志后就进行“登记中断”的工作。
 * “登记中断”意味着将底半部处理程序挂到该设备的底半部执行队列中
 * 去。这样，顶半部执行的速度就会很快，从而可以服务更多的中断请求
 *
 *
 * 现在，中断处理工作的重心就落在了底半部的头上，需用它来完成中断事件的绝大多数任务。底半部
 * 几乎做了中断处理程序所有的事情，而且可以被新的中断打断，这也是底半部和顶半部的最大不同，因为
 * 顶半部往往被设计成不可中断。底半部相对来说并不是非常紧急的，而且相对比较耗时，不在硬件中断服
 * 务程序中执行
 *
 * 尽管顶半部、底半部的结合能够改善系统的响应能力，但是，僵化地认为Linux设备驱动中的中断处
 * 理一定要分两个半部则是不对的。如果中断要处理的工作本身很少，则完全可以直接在顶半部全部完成
 *
 * 许多操作系统都提供了中断上下文和非中断上下文相结合的机制，将中断的耗时工作保留
 * 到非中断上下文去执行
 *
 * 在Linux中，查看/proc/interrupts文件可以获得系统中中断的统计信息，并能统计出每一个中断号上的
 * 中断在每个CPU上发生的次数
 *
 *
 * **********************************************************************************************************************/


/* Linux中断编程 */

/*
 * 在Linux设备驱动中，使用中断的设备需要申请和释放对应的中断，并分别使用内核提供的
 * request_irq（）和free_irq（）函数
 * */

/*1.申请irq*/

int request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,
		const char *name, void *dev);

/*
 * irq是要申请的硬件中断号。
 * handler是向系统登记的中断处理函数（顶半部），是一个回调函数，中断发生时，系统调用这个函
 * 数，dev参数将被传递给它。
 * irqflags是中断处理的属性，可以指定中断的触发方式以及处理方式。在触发方式方面，可以是
 * IRQF_TRIGGER_RISING、IRQF_TRIGGER_FALLING、IRQF_TRIGGER_HIGH、IRQF_TRIGGER_LOW
 * 等。在处理方式方面，若设置了IRQF_SHARED，则表示多个设备共享中断，dev是要传递给中断服务程
 * 序的私有数据，一般设置为这个设备的设备结构体或者NULL。
 *
 * request_irq（）返回0表示成功，返回-EINVAL表示中断号无效或处理函数指针为NULL，返回-
 * EBUSY表示中断已经被占用且不能共享。
 *
 * */

int devm_request_irq(struct device *dev, unsigned int irq, irq_handler_t handler,
		unsigned long irqflags, const char *devname, void *dev_id);

/*
 * 此函数与request_irq（）的区别是devm_开头的API申请的是内核“managed”的资源，一般不需要在出
 * 错处理和remove（）接口里再显式的释放。有点类似Java的垃圾回收机制。比如，对于at86rf230驱动，如
 * 下的补丁中改用devm_request_irq（）后就删除了free_irq（），该补丁对应的内核commit ID是652355c5。
 * */

/*顶半部handler的类型irq_handler_t定义为：*/

typedef irqreturn_t (*irq_handler_t)(int, void *);
typedef int irqreturn_t;

/*2.释放irq*/

/*与request_irq（）相对应的函数为free_irq（），free_irq（）的原型为*/
void free_irq(unsigned int irq,void *dev_id);
/*free_irq（）中参数的定义与request_irq（）相同。*/

/*使能和屏蔽中断*/

/*下列3个函数用于屏蔽一个中断源*/
void disable_irq(int irq);
void disable_irq_nosync(int irq);
void enable_irq(int irq);
/*
 * disable_irq_nosync（）与disable_irq（）的区别在于前者立即返回，而后者等待目前的中断处理完
 * 成。由于disable_irq（）会等待指定的中断被处理完，因此如果在n号中断的顶半部调用disable_irq（n），
 * 会引起系统的死锁，这种情况下，只能调用disable_irq_nosync（n）。
 * */

/*下列两个函数（或宏，具体实现依赖于CPU的体系结构）将屏蔽本CPU内的所有中断*/

#define local_irq_save(flags) ...
void local_irq_disable(void);
/*
 * 前者会将目前的中断状态保留在flags中（注意flags为unsigned long类型，被直接传递，而不是通过指
 * 针），后者直接禁止中断而不保存状态
 * */

/*与上述两个禁止中断对应的恢复中断的函数（或宏）是：*/

#define local_irq_restore(flags) ...
void local_irq_enable(void);

/*以上各以local_开头的方法的作用范围是本CPU内*/

/* ****************************************************************************************************
 * 底半部机制
 * ****************************************************************************************************/

/*Linux实现底半部的机制主要有tasklet、工作队列、软中断和线程化irq。*/

/*1.taskle*/
/*
 * tasklet的使用较简单，它的执行上下文是软中断，执行时机通常是顶半部返回的时候。我们只需要定
 * 义tasklet及其处理函数，并将两者关联则可，例如：
 * */
void my_tasklet_func(unsigned long); /*定义一个处理函数*/
DECLARE_TASKLET(my_tasklet, my_tasklet_func, data);
/*定义一个tasklet结构my_tasklet，与my_tasklet_func(data)函数相关联*/

/*
 * 代码DECLARE_TASKLET（my_tasklet，my_tasklet_func，data）实现了定义名称为my_tasklet的
 * tasklet，并将其与my_tasklet_func（）这个函数绑定，而传入这个函数的参数为data。
 * */

/*在需要调度tasklet的时候引用一个tasklet_schedule（）函数就能使系统在适当的时候进行调度运行*/

tasklet_schedule(&my_tasklet);

//tasklet使用模板
/* 定义tasklet和底半部函数并将它们关联 */
void xxx_do_tasklet(unsigned long);
DECLARE_TASKLET(xxx_tasklet, xxx_do_tasklet, 0);

/* 中断处理底半部 */
void xxx_do_tasklet(unsigned long)
{
	...
}

/* 中断处理顶半部 */
irqreturn_t xxx_interrupt(int irq, void *dev_id)
{
	...
		tasklet_schedule(&xxx_tasklet);
	...
}

/* 设备驱动模块加载函数 */
int __init xxx_init(void)
{
	...
		/* 申请中断 */
		result = request_irq(xxx_irq, xxx_interrupt,
				0,
				"xxx"
				, NULL);
	...
		return IRQ_HANDLED;
}

/* 设备驱动模块卸载函数 */
void __exit xxx_exit(void)
{
	...
		/* 释放中断 */
		free_irq(xxx_irq, xxx_interrupt);
	...
}


/*2.工作队列*/

/*
 * 工作队列的使用方法和tasklet非常相似，但是工作队列的执行上下文是内核线程，因此可以调度和睡
 * 眠。下面的代码用于定义一个工作队列和一个底半部执行函数：
 * */

struct work_struct my_wq; /* 定义一个工作队列 */
void my_wq_func(struct work_struct *work); /* 定义一个处理函数 */

/*通过INIT_WORK（）可以初始化这个工作队列并将工作队列与处理函数绑定*/

INIT_WORK(&my_wq, my_wq_func);
/* 初始化工作队列并将其与处理函数绑定 */

/*与tasklet_schedule（）对应的用于调度工作队列执行的函数为schedule_work（），如*/

schedule_work(&my_wq); /* 调度工作队列执行 */

//工作队列使用模板
/* 定义工作队列和关联函数 */
struct work_struct xxx_wq;
void xxx_do_work(struct work_struct *work);

/* 中断处理底半部 */
void xxx_do_work(struct work_struct *work)
{
	...
}

/*中断处理顶半部*/
irqreturn_t xxx_interrupt(int irq, void *dev_id)
{
	...
		schedule_work(&xxx_wq);
	...
		return IRQ_HANDLED;
}

/* 设备驱动模块加载函数 */
int xxx_init(void)
{
	...
		/* 申请中断 */
		result = request_irq(xxx_irq, xxx_interrupt,
				0,
				"xxx"
				, NULL);
	...
		/* 初始化工作队列 */
		INIT_WORK(&xxx_wq, xxx_do_work);
	...
}

/* 设备驱动模块卸载函数 */
void xxx_exit(void)
{
	...
		/* 释放中断 */
		free_irq(xxx_irq, xxx_interrupt);
	...
}
/*
 * 上述程序在设计驱动模块加载函数中增加了初始化工作队列的代码
 * 工作队列早期的实现是在每个CPU核上创建一个worker内核线程，所有在这个核上调度的工作都在该
 * worker线程中执行，其并发性显然差强人意。在Linux 2.6.36以后，转而实现了“Concurrency-managed
 * workqueues”
 * ，简称cmwq，cmwq会自动维护工作队列的线程池以提高并发性，同时保持了API的向后兼
 * 容。
 * */

/*3.软中断*/

/*
 * 软中断（Softirq）也是一种传统的底半部处理机制，它的执行时机通常是顶半部返回的时候，tasklet
 * 是基于软中断实现的，因此也运行于软中断上下文。
 * 在Linux内核中，用softirq_action结构体表征一个软中断，这个结构体包含软中断处理函数指针和传递
 * 给该函数的参数。使用open_softirq（）函数可以注册软中断对应的处理函数，而raise_softirq（）函数可以
 * 触发一个软中断。
 * 软中断和tasklet运行于软中断上下文，仍然属于原子上下文的一种，而工作队列则运行于进程上下
 * 文。因此，在软中断和tasklet处理函数中不允许睡眠，而在工作队列处理函数中允许睡眠。
 * */

/*
 * local_bh_disable（）和local_bh_enable（）是内核中用于禁止和使能软中断及tasklet底半部机制的函
 * 数。
 * 内核中采用softirq的地方包括HI_SOFTIRQ、TIMER_SOFTIRQ、NET_TX_SOFTIRQ、
 * NET_RX_SOFTIRQ、SCSI_SOFTIRQ、TASKLET_SOFTIRQ等，一般来说，驱动的编写者不会也不宜直
 * 接使用softirq
 * */

/*
 * 总结一下硬中断、软中断和信号的区别：
 * 硬中断是外部设备对CPU的中断，
 * 软中断是中断底半部的一种处理机制，
 * 而信号则是由内核（或其他进程）对某个进程的中断。
 * 在涉及系统调用的场合，人们也常说通过软中断（例如ARM为swi）陷入内核，
 * 此时软中断的概念是指由软件指令引发的中断，
 * 和我们这个地方说的softirq是两个完全不同的概念，
 * 一个是software，一个是soft。
 *
 *
 * 需要特别说明的是，
 * 软中断以及基于软中断的tasklet如果在某段时间内大量出现的话，
 * 内核会把后续软中断放入ksoftirqd内核线程中执行。
 * 总的来说，中断优先级高于软中断，软中断又高于任何一个线程。
 * 软中断适度线程化，可以缓解高负载情况下系统的响应
 * */

/*4.threaded_irq*/

/*
 * 在内核中，除了可以通过request_irq（）、devm_request_irq（）申请中断以外，还可以通过
 * request_threaded_irq（）和devm_request_threaded_irq（）申请。这两个函数的原型为：
 *
 * */

int request_threaded_irq(unsigned int irq, irq_handler_t handler,
		irq_handler_t thread_fn,
		unsigned long flags, const char *name, void *dev);
int devm_request_threaded_irq(struct device *dev, unsigned int irq,
		irq_handler_t handler, irq_handler_t thread_fn,
		unsigned long irqflags, const char *devname,
		void *dev_id);

/*
 * 由此可见，它们比request_irq（）、devm_request_irq（）多了一个参数thread_fn。用这两个API申请中
 * 断的时候，内核会为相应的中断号分配一个对应的内核线程。注意这个线程只针对这个中断号，如果其他
 * 中断也通过request_threaded_irq（）申请，自然会得到新的内核线程。
 * 参数handler对应的函数执行于中断上下文，thread_fn参数对应的函数则执行于内核线程。如果handler
 * 结束的时候，返回值是IRQ_WAKE_THREAD，内核会调度对应线程执行thread_fn对应的函数。
 * request_threaded_irq（）和devm_request_threaded_irq（）支持在irqflags中设置IRQF_ONESHOT标记，
 * 这样内核会自动帮助我们在中断上下文中屏蔽对应的中断号，而在内核调度thread_fn执行后，重新使能该
 * 中断号。对于我们无法在上半部清除中断的情况，IRQF_ONESHOT特别有用，避免了中断服务程序一退
 * 出，中断就洪泛的情况。
 * handler参数可以设置为NULL，这种情况下，内核会用默认的irq_default_primary_handler（）代替
 * handler，并会使用IRQF_ONESHOT标记。irq_default_primary_handler（）定义为：
 * */

/*
 * * Default primary interrupt handler for threaded interrupts. Is
 * * assigned as primary handler when request_threaded_irq is called
 * * with handler == NULL. Useful for oneshot interrupts.
 * */
static irqreturn_t irq_default_primary_handler(int irq, void *dev_id)
{
	return IRQ_WAKE_THREAD;
}


/*中断共享*/
/* 
 多个设备共享一根硬件中断线的情况在实际的硬件系统中广泛存在，Linux支持这种中断共享。下面
 是中断共享的使用方法。

 1）共享中断的多个设备在申请中断时，都应该使用IRQF_SHARED标志，而且一个设备以
 IRQF_SHARED申请某中断成功的前提是该中断未被申请，或该中断虽然被申请了，但是之前申请该中断
 的所有设备也都以IRQF_SHARED标志申请该中断。

 2）尽管内核模块可访问的全局地址都可以作为request_irq（…，void*dev_id）的最后一个参数
 dev_id，但是设备结构体指针显然是可传入的最佳参数。

 3）在中断到来时，会遍历执行共享此中断的所有中断处理程序，直到某一个函数返回
 IRQ_HANDLED。在中断处理程序顶半部中，应根据硬件寄存器中的信息比照传入的dev_id参数迅速地判
 断是否为本设备的中断，若不是，应迅速返回IRQ_NONE
 */

//共享中断编程模板
/* 中断处理顶半部 */
irqreturn_t xxx_interrupt(int irq, void *dev_id)
{
	...
		int status = read_int_status(); /* 获知中断源 */
	if(!is_myint(dev_id,status)) /* 判断是否为本设备中断 */
		return IRQ_NONE; /* 不是本设备中断，立即返回 */

	/* 是本设备中断，进行处理 */
	...
		return IRQ_HANDLED; /* 返回IRQ_HANDLED表明中断已被处理 */
}

/* 设备驱动模块加载函数 */
int xxx_init(void)
{
	...
		/* 申请共享中断 */
		result = request_irq(sh_irq, xxx_interrupt,
				IRQF_SHARED, "xxx", xxx_dev);
	...
}

/* 设备驱动模块卸载函数 */
void xxx_exit(void)
{
	...
		/* 释放中断 */
		free_irq(xxx_irq, xxx_interrupt);
	...
}



