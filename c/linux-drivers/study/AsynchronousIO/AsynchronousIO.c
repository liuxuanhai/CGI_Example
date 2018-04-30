/******************************************    *******************************
      > File Name: AsynchronousIO.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月20日 星期一 16时24分40秒
      > Modify Time:
 *********************************    ***************************************/

/*
 * 在设备驱动中使用异步通知可以使得在进行对设备的访问时，由驱动主动通知应用程序进行访问。这
 * 样，使用非阻塞I/O的应用程序无须轮询设备是否可访问，而阻塞访问也可以被类似“中断”的异步通知所
 * 取代。
 * 除了异步通知以外，应用还可以在发起I/O请求后，立即返回。之后，再查询I/O完成情况，或者I/O完
 * 成后被调回。这个过程叫作异步I/O
 * */

/*
 * 阻塞与非阻塞访问、poll（）函数提供了较好的解决设备访问的机制，但是如果有了异步通知，整套
 * 机制则更加完整了。
 * 异步通知的意思是：一旦设备就绪，则主动通知应用程序，这样应用程序根本就不需要查询设备状
 * 态，这一点非常类似于硬件上“中断”的概念，比较准确的称谓是“信号驱动的异步I/O”
 * 。信号是在软件层
 * 次上对中断机制的一种模拟，在原理上，一个进程收到一个信号与处理器收到一个中断请求可以说是一样
 * 的。信号是异步的，一个进程不必通过任何操作来等待信号的到达，事实上，进程也不知道信号到底什么
 * 时候到达。
 * 阻塞I/O意味着一直等待设备可访问后再访问，非阻塞I/O中使用poll（）意味着查询设备是否可访
 * 问，而异步通知则意味着设备通知用户自身可访问，之后用户再进行I/O处理。由此可见，这几种I/O方式
 * 可以相互补充。
 * 图9.1呈现了阻塞I/O，结合轮询的非阻塞I/O及基于SIGIO的异步通知在时间先后顺序上的不同。
 * */
/*
 * 一共64个信号
 *  1) SIGHUP	 		2) SIGINT	 		3) SIGQUIT	 		4) SIGILL	 		5) SIGTRAP
 *  6) SIGABRT	 		7) SIGBUS	 		8) SIGFPE	 		9) SIGKILL			10) SIGUSR1
 *  11) SIGSEGV			12) SIGUSR2			13) SIGPIPE			14) SIGALRM			15) SIGTERM
 *  16) SIGSTKFLT		17) SIGCHLD			18) SIGCONT			19) SIGSTOP			20) SIGTSTP
 *  21) SIGTTIN			22) SIGTTOU			23) SIGURG			24) SIGXCPU			25) SIGXFSZ
 *  26) SIGVTALRM		27) SIGPROF			28) SIGWINCH		29) SIGIO			30) SIGPWR
 *  31) SIGSYS			34) SIGRTMIN		35) SIGRTMIN+1		36) SIGRTMIN+2		37) SIGRTMIN+3
 *  38) SIGRTMIN+4		39) SIGRTMIN+5		40) SIGRTMIN+6		41) SIGRTMIN+7		42) SIGRTMIN+8
 *  43) SIGRTMIN+9		44) SIGRTMIN+10		45) SIGRTMIN+11		46) SIGRTMIN+12		47) SIGRTMIN+13
 *  48) SIGRTMIN+14		49) SIGRTMIN+15		50) SIGRTMAX-14		51) SIGRTMAX-13		52) SIGRTMAX-12
 *  53) SIGRTMAX-11		54) SIGRTMAX-10		55) SIGRTMAX-9		56) SIGRTMAX-8		57) SIGRTMAX-7
 *  58) SIGRTMAX-6		59) SIGRTMAX-5		60) SIGRTMAX-4		61) SIGRTMAX-3		62) SIGRTMAX-2
 *  63) SIGRTMAX-1		64) SIGRTMAX
 * linux系统中定义得信号可以通过kill -l命令查看，共定义了64个信号。
 * Signal	Description
 * SIGABRT	由调用abort函数产生，进程非正常退出
 * SIGALRM	用alarm函数设置的timer超时或setitimer函数设置的interval timer超时
 * SIGBUS	某种特定的硬件异常，通常由内存访问引起
 * SIGCANCEL	由Solaris Thread Library内部使用，通常不会使用
 * SIGCHLD	进程Terminate或Stop的时候，SIGCHLD会发送给它的父进程。缺省情况下该Signal会被忽略
 * SIGCONT	当被stop的进程恢复运行的时候，自动发送
 * SIGEMT	和实现相关的硬件异常
 * SIGFPE	数学相关的异常，如被0除，浮点溢出，等等
 * SIGFREEZE	Solaris专用，Hiberate或者Suspended时候发送
 * SIGHUP	发送给具有Terminal的Controlling Process，当terminal被disconnect时候发送
 * SIGILL	非法指令异常
 * SIGINFO	BSD signal。由Status Key产生，通常是CTRL+T。发送给所有Foreground Group的进程
 * SIGINT	由Interrupt Key产生，通常是CTRL+C或者DELETE。发送给所有ForeGround Group的进程
 * SIGIO	异步IO事件
 * SIGIOT	实现相关的硬件异常，一般对应SIGABRT
 * SIGKILL	无法处理和忽略。中止某个进程
 * SIGLWP	由Solaris Thread Libray内部使用
 * SIGPIPE	在reader中止之后写Pipe的时候发送
 * SIGPOLL	当某个事件发送给Pollable Device的时候发送
 * SIGPROF	Setitimer指定的Profiling Interval Timer所产生
 * SIGPWR	和系统相关。和UPS相关。
 * SIGQUIT	输入Quit Key的时候（CTRL+\）发送给所有Foreground Group的进程
 * SIGSEGV	非法内存访问
 * SIGSTKFLT	Linux专用，数学协处理器的栈异常
 * SIGSTOP	中止进程。无法处理和忽略。
 * SIGSYS	非法系统调用
 * SIGTERM	请求中止进程，kill命令缺省发送
 * SIGTHAW	Solaris专用，从Suspend恢复时候发送
 * SIGTRAP	实现相关的硬件异常。一般是调试异常
 * SIGTSTP	Suspend Key，一般是Ctrl+Z。发送给所有Foreground Group的进程
 * SIGTTIN	当Background Group的进程尝试读取Terminal的时候发送
 * SIGTTOU	当Background Group的进程尝试写Terminal的时候发送
 * SIGURG	当out-of-band data接收的时候可能发送
 * SIGUSR1	用户自定义signal 1
 * SIGUSR2	用户自定义signal 2
 * SIGVTALRM	setitimer函数设置的Virtual Interval Timer超时的时候
 * SIGWAITING	Solaris Thread Library内部实现专用
 * SIGWINCH	当Terminal的窗口大小改变的时候，发送给Foreground Group的所有进程
 * SIGXCPU	当CPU时间限制超时的时候
 * SIGXFSZ	进程超过文件大小限制
 * SIGXRES	Solaris专用，进程超过资源限制的时候发送
 *
 * 除了SIGSTOP和SIGKILL两个信号外，进程能够忽略或捕获其他的全部信号。一个信号被捕获的意思
 * 是当一个信号到达时有相应的代码处理它。如果一个信号没有被这个进程所捕获，内核将采用默认行为处
 * 理
 * */

/*设置信号对应的处理函数*/
void (*signal(int signum, void (*handler)(int)))(int);	/*一个函数指针指向一个函数，这个函数带有两个参数,返回值为空，
														  一个参数为int型另一个参数也是一个函数指针，
														  这个函数指针指向的函数带有一个int型的参数，返回值为空*/
typedef void (*sighandler_t)(int);						/*函数指针*/
sighandler_t signal(int signum, sighandler_t handler);

/*除了signal（）函数外，sigaction（）函数可用于改变进程接收到特定信号后的行为，它的原型为：
 * */
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
/*
 * 该函数的第一个参数为信号的值，可以是除SIGKILL及SIGSTOP外的任何一个特定有效的信号。第二
 * 个参数是指向结构体sigaction的一个实例的指针，在结构体sigaction的实例中，指定了对特定信号的处理
 * 函数，若为空，则进程会以缺省方式对信号处理；第三个参数oldact指向的对象用来保存原来对相应信号
 * 的处理函数，可指定oldact为NULL。如果把第二、第三个参数都设为NULL，那么该函数可用于检查信号
 * 的有效性。
 * */
/*
 * 先来看一个使用信号实现异步通知的例子，它通过signal（SIGIO，input_handler）对标准输入文件描
 * 述符STDIN_FILENO启动信号机制。用户输入后，应用程序将接收到SIGIO信号，其处理函数
 * input_handler（）将被调用，如代码清单9.2所示
 * */
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#define MAX_LEN 100
void input_handler(int num)
{
	char data[MAX_LEN];
	int len;

	/* 读取并输出STDIN_FILENO上的输入 */
	len = read(STDIN_FILENO, &data, MAX_LEN);
	data[len] = 0;
	printf("input available:%s\n"
			, data);
}

19main()
{
	int oflags;

	/* 启动信号驱动机制 */
	signal(SIGIO, input_handler);
	/*这个地方设置本进程为STDIN_FILENO文件的拥有者，没有这一步，内核不会知道应该将信号发给哪个进程*/
	fcntl(STDIN_FILENO, F_SETOWN, getpid());
	oflags = fcntl(STDIN_FILENO, F_GETFL);
	/*而为了启用异步通知机制，还需对设备设置FASYNC标志*/
	fcntl(STDIN_FILENO, F_SETFL, oflags | FASYNC);

	/* 最后进入一个死循环，仅为保持进程不终止，如果程序中
	   没有这个死循会立即执行完毕 */
	while (1);
}
/*
 * 由此可见，为了能在用户空间中处理一个设备释放的信号，它必须完成3项工作。
 * 1）通过F_SETOWN IO控制命令设置设备文件的拥有者为本进程，这样从设备驱动发出的信号才能被
 * 本进程接收到。
 * 2）通过F_SETFL IO控制命令设置设备文件以支持FASYNC，即异步通知模式。
 * 3）通过signal（）函数连接信号和信号处理函数
 *
 *
 *
 * 为了使设备支持异步通知机制，驱动程序中涉及3项工作。
 * 1）支持F_SETOWN命令，能在这个控制命令处理中设置filp->f_owner为对应进程ID。不过此项工作
 * 已由内核完成，设备驱动无须处理。
 * 2）支持F_SETFL命令的处理，每当FASYNC标志改变时，驱动程序中的fasync（）函数将得以执行。
 * 因此，驱动中应该实现fasync（）函数。
 * 3）在设备资源可获得时，调用kill_fasync（）函数激发相应的信号。
 * 驱动中的上述3项工作和应用程序中的3项工作是一一对应的
 * */


/*设备驱动中异步通知编程比较简单，主要用到一项数据结构和两个函数, 将fasync_struct结构体指针放在设备结构体中仍然是最佳选择*/
/*1）处理FASYNC标志变更的函数。*/

int fasync_helper(int fd, struct file *filp, int mode, struct fasync_struct **fa);

/*2）释放信号用的函数*/

void kill_fasync(struct fasync_struct **fa, int sig, int band);

/*范例*/
struct xxx_dev {
	struct cdev cdev; /* cdev结构体*/
	...
	struct fasync_struct *async_queue; /* 异步结构体指针 */
};

static int xxx_fasync(int fd, struct file *filp, int mode)
{
	struct xxx_dev *dev = filp->private_data;
	return fasync_helper(fd, filp, mode, &dev->async_queue);
}

/*
 * 在设备资源可以获得时，应该调用kill_fasync（）释放SIGIO信号。在可读时，第3个参数设置为
 * POLL_IN，在可写时，第3个参数设置为POLL_OUT
 * */

//支持异步通知的设备驱动信号释放范例
static ssize_t xxx_write(struct file *filp, const char __user *buf, size_t count,
		loff_t *f_pos)
{
	struct xxx_dev *dev = filp->private_data;
	...
		/* 产生异步读信号 */
		if (dev->async_queue)
			kill_fasync(&dev->async_queue, SIGIO, POLL_IN);
	...
}

/*最后，在文件关闭时，即在设备驱动的release（）函数中，应调用设备驱动的fasync（）函数将文件
 * 从异步通知的列表中删除。*/

//支持异步通知的设备驱动release（）函数模板
static int xxx_release(struct inode *inode, struct file *filp)
{
	/* 将文件从异步通知列表中删除 */
	xxx_fasync(-1, filp, 0);
	...
		return 0;
}
