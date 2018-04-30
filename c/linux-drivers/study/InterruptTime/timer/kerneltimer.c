/******************************************    *******************************
      > File Name: kerneltimer.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     Mon 27 Nov 2017 03:37:31 PM CST
      > Modify Time:
 *********************************    ***************************************/

/*
 内核定时器编程
 软件意义上的定时器最终依赖硬件定时器来实现，内核在时钟中断发生后检测各定时器是否到期，到
 期后的定时器处理函数将作为软中断在底半部执行。实质上，时钟中断处理程序会唤起TIMER_SOFTIRQ
 软中断，运行当前处理器上到期的所有定时器。
 在Linux设备驱动编程中，可以利用Linux内核中提供的一组函数和数据结构来完成定时触发工作或者
 完成某周期性的事务。这组函数和数据结构使得驱动工程师在多数情况下不用关心具体的软件定时器究竟
 对应着怎样的内核和硬件行为。
 */

//timer_list结构体
struct timer_list {
	/*
	   3 * All fields that change during normal runtime grouped to the
	   4 * same cacheline
	   5 */
	struct list_head entry;
	unsigned long expires;
	struct tvec_base *base;

	void (*function)(unsigned long);
	unsigned long data;

	int slack;

#ifdef CONFIG_TIMER_STATS
	int start_pid;
	void *start_site;
	char start_comm[16];
#endif
#ifdef CONFIG_LOCKDEP
	struct lockdep_map lockdep_map;
#endif
};
/*
 * 当定时器期满后，其中第10行的function（）成员将被执行，而第11行的data成员则是传入其中的参
 * 数，第7行的expires则是定时器到期的时间（jiffies）
 * */
/*2.初始化定时器*/
//init_timer是一个宏，它的原型等价于：
void init_timer(struct timer_list * timer);
/*
 * 上述init_timer（）函数初始化timer_list的entry的next为NULL，并给base指针赋值。
 * TIMER_INITIALIZER（_function，_expires，_data）宏用于赋值定时器结构体的function、expires、
 * data和base成员，这个宏等价于：
 * */

#define TIMER_INITIALIZER(_function, _expires, _data) { \
	.entry = { .prev = TIMER_ENTRY_STATIC }, \
	.function = (_function), \
	.expires = (_expires), \
	.data = (_data), \
	.base = &boot_tvec_bases, \
}

/*
 * DEFINE_TIMER（_name，_function，_expires，_data）宏是定义并初始化定时器成员的“快捷方式”，
 * 这个宏定义为：
 * */

#define DEFINE_TIMER(_name, _function, _expires, _data)\
	struct timer_list _name =\
		TIMER_INITIALIZER(_function, _expires, _data)
//此外，setup_timer（）也可用于初始化定时器并赋值其成员，其源代码为：
#define __setup_timer(_timer, _fn, _data, _flags) \
	do { \
		__init_timer((_timer), (_flags)); \
		(_timer)->function = (_fn); \
		(_timer)->data = (_data); \
	} while (0)

/*3.增加定时器*/
void add_timer(struct timer_list * timer);
/*上述函数用于注册内核定时器，将定时器加入到内核动态定时器链表中。*/

/*4.删除定时器*/
int del_timer(struct timer_list * timer);

/*
 * 上述函数用于删除定时器。
 * del_timer_sync（）是del_timer（）的同步版，在删除一个定时器时需等待其被处理完，因此该函数的
 * 调用不能发生在中断上下文中。
 * */

/*5.修改定时器的expire*/

int mod_timer(struct timer_list *timer, unsigned long expires);
/*
 * 上述函数用于修改定时器的到期时间，在新的被传入的expires到来后才会执行定时器函数
 * */

/*内核定时器使用模板*/
/* xxx设备结构体 */
struct xxx_dev {
	struct cdev cdev;
	...
	timer_list xxx_timer; /* 设备要使用的定时器 */
};

/* xxx驱动中的某函数 */
xxx_func1(…)
{
	struct xxx_dev *dev = filp->private_data;
	...
		/* 初始化定时器 */
	init_timer(&dev->xxx_timer);
	dev->xxx_timer.function = &xxx_do_timer;
	dev->xxx_timer.data = (unsigned long)dev;
	/* 设备结构体指针作为定时器处理函数参数 */
	dev->xxx_timer.expires = jiffies + delay;
	/* 添加（注册）定时器 */
	add_timer(&dev->xxx_timer);
	...
}

/* xxx驱动中的某函数 */
xxx_func2(…)
{
	...
		/* 删除定时器 */
		del_timer (&dev->xxx_timer);
	...
}

/* 定时器处理函数 */
static void xxx_do_timer(unsigned long arg)
{
	struct xxx_device *dev = (struct xxx_device *)(arg);
	...
		/* 调度定时器再执行 */
		dev->xxx_timer.expires = jiffies + delay;
	add_timer(&dev->xxx_timer);
	...
}


/*
 * 定时器的到期时间往往是在目前jiffies的基础上添加一个时延，若
 * 为Hz，则表示延迟1s。
 * 在定时器处理函数中，在完成相应的工作后，往往会延后expires并将定时器再次添加到内核定时器链
 * 表中，以便定时器能再次被触发。
 * 此外，Linux内核支持tickless和NO_HZ模式后，内核也包含对hrtimer（高精度定时器）的支持，它可
 * 以支持到微秒级别的精度。内核也定义了hrtimer结构体，hrtimer_set_expires（）、
 * hrtimer_start_expires（）、hrtimer_forward_now（）、hrtimer_restart（）等类似的API来完成hrtimer的设
 * 置、时间推移以及到期回调。我们可以从sound/soc/fsl/imx-pcm-fiq.c中提取出一个使用范例
 * */
/*内核高精度定时器（hrtimer）使用模板*/
static enum hrtimer_restart snd_hrtimer_callback(struct hrtimer *hrt)
{
	...

		hrtimer_forward_now(hrt, ns_to_ktime(iprtd->poll_time_ns));

	return HRTIMER_RESTART;
}

static int snd_imx_pcm_trigger(struct snd_pcm_substream *substream, int cmd)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct imx_pcm_runtime_data *iprtd = runtime->private_data;

	switch (cmd) {
		case SNDRV_PCM_TRIGGER_START:
		case SNDRV_PCM_TRIGGER_RESUME:
		case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
			...
				hrtimer_start(&iprtd->hrt, ns_to_ktime(iprtd->poll_time_ns),
						HRTIMER_MODE_REL);
			...
	}
}
static int snd_imx_open(struct snd_pcm_substream *substream)
{
	...
		hrtimer_init(&iprtd->hrt, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	iprtd->hrt.function = snd_hrtimer_callback;

	...
		return 0;
}
static int snd_imx_close(struct snd_pcm_substream *substream)
{
	...
		hrtimer_cancel(&iprtd->hrt);
	...
}
/*
 * 第28~29行在声卡打开的时候通过hrtimer_init（）初始化了hrtimer，并指定回调函数为
 * snd_hrtimer_callback（）；在启动播放（第15~21行SNDRV_PCM_TRIGGER_START）等时刻通过
 * hrtimer_start（）启动了hrtimer；iprtd->poll_time_ns纳秒后，时间到snd_hrtimer_callback（）函数在中断上
 * 下文被执行，它紧接着又通过hrtimer_forward_now（）把hrtimer的时间前移了iprtd->poll_time_ns纳秒，这
 * 样周而复始；直到声卡被关闭，第37行又调用了hrtimer_cancel（）取消在open时初始化的hrtimer
 * */


/*内核中延迟的工作delayed_work*/

/*
 * 对于周期性的任务，除了定时器以外，在Linux内核中还可以利用一套封装得很好的快捷机制，其本
 * 质是利用工作队列和定时器实现，这套快捷机制就是delayed_work，delayed_work结构体的定义如代码
 * */

struct delayed_work {
	struct work_struct work;
	struct timer_list timer;

	/* target workqueue and CPU ->timer uses to queue ->work */
	struct workqueue_struct *wq;
	int cpu;
};
/*我们可以通过如下函数调度一个delayed_work在指定的延时后执行*/

int schedule_delayed_work(struct delayed_work *work, unsigned long delay);
/*
 * 当指定的delay到来时，delayed_work结构体中的work成员work_func_t类型成员func（）会被执行。
 * work_func_t类型定义为：
 * */

typedef void (*work_func_t)(struct work_struct *work);
/*其中，delay参数的单位是jiffies，因此一种常见的用法如下：*/

schedule_delayed_work(&work, msecs_to_jiffies(poll_interval));
/*
 * msecs_to_jiffies（）用于将毫秒转化为jiffies。
 * 如果要周期性地执行任务，通常会在delayed_work的工作函数中再次调用schedule_delayed_work（），
 * 周而复始
 * */

/*如下函数用来取消delayed_work：*/
int cancel_delayed_work(struct delayed_work *work);
int cancel_delayed_work_sync(struct delayed_work *work);

/*内核延时*/

//Linux内核中提供了下列3个函数以分别进行纳秒、微秒和毫秒延迟：
void ndelay(unsigned long nsecs);
void udelay(unsigned long usecs);
void mdelay(unsigned long msecs);

/*
 * 上述延迟的实现原理本质上是忙等待，它根据CPU频率进行一定次数的循环。有时候，人们在软件中
 * 进行下面的延迟：
 * */
void delay(unsigned int time)
{
	while(time--);
}
/*
 * ndelay（）、udelay（）和mdelay（）函数的实现方式原理与此类似。内核在启动时，会运行一个延迟
 * 循环校准（Delay Loop Calibration），计算出lpj（Loops Per Jiffy），内核启动时会打印如下类似信息：
 *
 * Calibrating delay loop... 530.84 BogoMIPS (lpj=1327104)
 * */

/*
 * 如果我们直接在bootloader传递给内核的bootargs中设置lpj=1327104，则可以省掉这个校准的过程，节
 * 省约百毫秒级的开机时间。
 * 毫秒时延（以及更大的秒时延）已经比较大了，在内核中，最好不要直接使用mdelay（）函数，这将
 * 耗费CPU资源，对于毫秒级以上的时延，内核提供了下述函数
 * */

void msleep(unsigned int millisecs);
unsigned long msleep_interruptible(unsigned int millisecs);
void ssleep(unsigned int seconds);
/*
 * 上述函数将使得调用它的进程睡眠参数指定的时间为millisecs，msleep（）、ssleep（）不能被打断，
 * 而msleep_interruptible（）则可以被打断。
 * */

/*长延迟*/

/*
 * 在内核中进行延迟的一个很直观的方法是比较当前的jiffies和目标jiffies（设置为当前jiffies加上时间间
 * 隔的jiffies），直到未来的jiffies达到目标jiffies。
 * */
//忙等待时延实例
/* 延迟100个jiffies */
unsigned long delay = jiffies + 100;
while(time_before(jiffies, delay));

/* 再延迟2s */
unsigned long delay = jiffies + 2*Hz;
while(time_before(jiffies, delay));
/*
 * 与time_before（）对应的还有一个time_after（），它们在内核中定义为（实际上只是将传入的未来时
 * 间jiffies和被调用时的jiffies进行一个简单的比较）：
 * */
#define time_after(a,b) \
	(typecheck(unsigned long, a) && \
	 typecheck(unsigned long, b) && \
	 ((long)(b) - (long)(a) < 0))
#define time_before(a,b) time_after(b,a)
/*
 * 为了防止在time_before（）和time_after（）的比较过程中编译器对jiffies的优化，内核将其定义为
 * volatile变量，这将保证每次都会重新读取这个变量。因此volatile更多的作用还是避免这种读合并。
 * */

/*睡着延迟*/
/*
 * 睡着延迟无疑是比忙等待更好的方式，睡着延迟是在等待的时间到来之前进程处于睡眠状态，CPU资
 * 源被其他进程使用。schedule_timeout（）可以使当前任务休眠至指定的jiffies之后再重新被调度执行，
 * msleep（）和msleep_interruptible（）在本质上都是依靠包含了schedule_timeout（）的
 * schedule_timeout_uninterruptible（）和schedule_timeout_interruptible（）来实现的
 * */

//schedule_timeout（）的使用
void msleep(unsigned int msecs)
{
	unsigned long timeout = msecs_to_jiffies(msecs) + 1;

	while (timeout)
		timeout = schedule_timeout_uninterruptible(timeout);
}

unsigned long msleep_interruptible(unsigned int msecs)
{
	unsigned long timeout = msecs_to_jiffies(msecs) + 1;

	while (timeout && !signal_pending(current))
		timeout = schedule_timeout_interruptible(timeout);
	1return jiffies_to_msecs(timeout);
}

/*
 * 实际上，schedule_timeout（）的实现原理是向系统添加一个定时器，在定时器处理函数中唤醒与参数
 * 对应的进程
 *
 * 第6行和第14行分别调用schedule_timeout_uninterruptible（）和
 * schedule_timeout_interruptible（），这两个函数的区别在于前者在调用schedule_timeout（）之前置进程状
 * 态为TASK_INTERRUPTIBLE，后者置进程状态为TASK_UNINTERRUPTIBLE
 *
 * */
//schedule_timeout_interruptible（）和schedule_timeout_interruptible（）
signed long __sched schedule_timeout_interruptible(signed long timeout)
{
	__set_current_state(TASK_INTERRUPTIBLE);
	return schedule_timeout(timeout);
}

signed long __sched schedule_timeout_uninterruptible(signed long timeout)
{
	__set_current_state(TASK_UNINTERRUPTIBLE);
	return schedule_timeout(timeout);
}

/*另外，下面两个函数可以将当前进程添加到等待队列中，从而在等待队列上睡眠。当超时发生时，进
 * 程将被唤醒（后者可以在超时前被打断）：*/
sleep_on_timeout(wait_queue_head_t *q, unsigned long timeout);
interruptible_sleep_on_timeout(wait_queue_head_t*q, unsigned long timeout);
/*
 * Linux的中断处理分为两个半部，顶半部处理紧急的硬件操作，底半部处理不紧急的耗时操作。tasklet
 * 和工作队列都是调度中断底半部的良好机制，tasklet基于软中断实现。内核定时器也依靠软中断实现。
 * 内核中的延时可以采用忙等待或睡眠等待，为了充分利用CPU资源，使系统有更好的吞吐性能，在对
 * 延迟时间的要求并不是很精确的情况下，睡眠等待通常是值得推荐的，而ndelay（）、udelay（）忙等待
 * 机制在驱动中通常是为了配合硬件上的短时延迟要求。
 * */
