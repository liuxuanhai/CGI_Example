/******************************************    *******************************
      > File Name: AIO.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月20日 星期一 17时31分09秒
      > Modify Time:
 *********************************    ***************************************/

/*
 * Linux中最常用的输入/输出（I/O）模型是同步I/O。在这个模型中，当请求发出之后，应用程序就会
	阻塞，直到请求满足为止。这是一种很好的解决方案，调用应用程序在等待I/O请求完成时不需要占用
CPU。但是在许多应用场景中，I/O请求可能需要与CPU消耗产生交叠，以充分利用CPU和I/O提高吞吐
率。
*/
/*图9.3描绘了异步I/O的时序，应用程序发起I/O动作后，直接开始执行，并不等待I/O结束，它要么过
一段时间来查询之前的I/O请求完成情况，要么I/O请求完成了会自动被调用与I/O完成绑定的回调函数。
Linux的AIO有多种实现，其中一种实现是在用户空间的glibc库中实现的，它本质上是借用了多线程
模型，用开启新的线程以同步的方法来做I/O，新的AIO辅助线程与发起AIO的线程以
pthread_cond_signal（）的形式进行线程间的同步。glibc的AIO主要包括如下函数。
*/
/*
 * 1.aio_read（）
aio_read（）函数请求对一个有效的文件描述符进行异步读操作。这个文件描述符可以表示一个文
件、套接字，甚至管道。aio_read函数的原型如下：
int aio_read( struct aiocb *aiocbp );
aio_read（）函数在请求进行排队之后会立即返回（尽管读操作并未完成）。如果执行成功，返回值
就为0；如果出现错误，返回值就为-1，并设置errno的值。
参数aiocb（AIO I/O Control Block）结构体包含了传输的所有信息，以及为AIO操作准备的用户空间
缓冲区。在产生I/O完成通知时，aiocb结构就被用来唯一标识所完成的I/O操作。
*/
/*2.aio_write（）
aio_write（）函数用来请求一个异步写操作。其函数原型如下：
int aio_write( struct aiocb *aiocbp );
aio_write（）函数会立即返回，并且它的请求已经被排队（成功时返回值为0，失败时返回值为-1，
并相应地设置errno）。
*/
/*3.aio_error（）
aio_error（）函数被用来确定请求的状态。其原型如下：
int aio_error( struct aiocb *aiocbp );
这个函数可以返回以下内容。
EINPROGRESS：说明请求尚未完成。
ECANCELED：说明请求被应用程序取消了。
-1：说明发生了错误，具体错误原因由errno记录。
*/


/*4.aio_return（）
异步I/O和同步阻塞I/O方式之间的一个区别是不能立即访问这个函数的返回状态，因为异步I/O并没有
阻塞在read（）调用上。在标准的同步阻塞read（）调用中，返回状态是在该函数返回时提供的。但是在
异步I/O中，我们要使用aio_return（）函数。这个函数的原型如下：
ssize_t aio_return( struct aiocb *aiocbp );
只有在aio_error（）调用确定请求已经完成（可能成功，也可能发生了错误）之后，才会调用这个函
数。aio_return（）的返回值就等价于同步情况中read（）或write（）系统调用的返回值（所传输的字节数
如果发生错误，返回值为负数）。
代码清单9.12给出了用户空间应用程序进行异步读操作的一个例程，它首先打开文件，然后准备aiocb
结构体，之后调用aio_read（&my_aiocb）进行提出异步读请求，当aio_error（&my_aiocb）
==EINPROGRESS，即操作还在进行中时，一直等待，结束后通过aio_return（&my_aiocb）获得返回值。
代码清单9.12 用户空间异步读例程
*/

#include <aio.h>
...
int fd, ret;
struct aiocb my_aiocb;

fd = open("file.txt"
		, O_RDONLY);
if (fd < 0)
	perror("open");

	/* 清零aiocb结构体 */
	bzero(&my_aiocb, sizeof(struct aiocb));

	/* 为aiocb请求分配数据缓冲区 */
	my_aiocb.aio_buf = malloc(BUFSIZE + 1);
if (!my_aiocb.aio_buf)
	perror("malloc");

	/* 初始化aiocb的成员 */
	my_aiocb.aio_fildes = fd;
	my_aiocb.aio_nbytes = BUFSIZE;
	my_aiocb.aio_offset = 0;

	ret = aio_read(&my_aiocb);
if (ret < 0)
	perror("aio_read");

while (aio_error(&my_aiocb) == EINPROGRESS)
	continue;

if ((ret = aio_return(&my_iocb)) > 0) {
	/* 获得异步读的返回值 */
} else {
		/* 读失败，分析errorno */
}
5.aio_suspend（）
用户可以使用aio_suspend（）函数来阻塞调用进程，直到异步请求完成为止。调用者提供了一个
aiocb引用列表，其中任何一个完成都会导致aio_suspend（）返回。aio_suspend（）的函数原型如下：

int aio_suspend( const struct aiocb *const cblist[],
int n, const struct timespec *timeout );

代码清单9.13给出了用户空间进行异步读操作时使用aio_suspend（）函数的例子。
代码清单9.13 用户空间异步I/O aio_suspend（）函数使用例程

struct aioct *cblist[MAX_LIST]
/* 清零aioct结构体链表 */
bzero( (char *)cblist, sizeof(cblist) );
/* 将一个或更多的aiocb放入aioct的结构体链表中 */
cblist[0] = &my_aiocb;
ret = aio_read(&my_aiocb);
ret = aio_suspend(cblist, MAX_LIST, NULL );



当然，在glibc实现的AIO中，除了上述同步的等待方式以外，也可以使用信号或者回调机制来异步地
标明AIO的完成。

6.aio_cancel（）
aio_cancel（）函数允许用户取消对某个文件描述符执行的一个或所有I/O请求。其原型如下：
int aio_cancel(int fd, struct aiocb *aiocbp);
要取消一个请求，用户需提供文件描述符和aiocb指针。如果这个请求被成功取消了，那么这个函数
就会返回AIO_CANCELED。如果请求完成了，这个函数就会返回AIO_NOTCANCELED。
要取消对某个给定文件描述符的所有请求，用户需要提供这个文件的描述符，并将aiocbp参数设置为
NULL。如果所有的请求都取消了，这个函数就会返回AIO_CANCELED；如果至少有一个请求没有被取
消，那么这个函数就会返回AIO_NOT_CANCELED；如果没有一个请求可以被取消，那么这个函数就会
返回AIO_ALLDONE。然后，可以使用aio_error（）来验证每个AIO请求，如果某请求已经被取消了，那
么aio_error（）就会返回-1，并且errno会被设置为ECANCELED。

7.lio_listio（）
lio_listio（）函数可用于同时发起多个传输。这个函数非常重要，它使得用户可以在一个系统调用中
启动大量的I/O操作。lio_listio API函数的原型如下：
int lio_listio( int mode, struct aiocb *list[], int nent, struct sigevent *sig );
mode参数可以是LIO_WAIT或LIO_NOWAIT。LIO_WAIT会阻塞这个调用，直到所有的I/O都完成为
止。但是若是LIO_NOWAIT模型，在I/O操作进行排队之后，该函数就会返回。list是一个aiocb引用的列
表，最大元素的个数是由nent定义的。如果list的元素为NULL，lio_listio（）会将其忽略。
代码清单9.14给出了用户空间进行异步I/O操作时使用lio_listio（）函数的例子。
代码清单9.14 用户空间异步I/O lio_listio（）函数使用例程
i
struct aiocb aiocb1, aiocb2;
struct aiocb *list[MAX_LIST];
...
/* 准备第一个aiocb */
aiocb1.aio_fildes = fd;
aiocb1.aio_buf = malloc( BUFSIZE+1 );
aiocb1.aio_nbytes = BUFSIZE;
aiocb1.aio_offset = next_offset;
aiocb1.aio_lio_opcode = LIO_READ; /* 异步读操作*/
... /* 准备多个aiocb */
bzero( (char *)list, sizeof(list) );

/* 将aiocb填入链表*/
list[0] = &aiocb1;
list[1] = &aiocb2;
...
ret = lio_listio( LIO_WAIT, list, MAX_LIST, NULL ); /* 发起大量I/O操作*/

上述代码第9行中，因为是进行异步读操作，所以操作码为LIO_READ，对于写操作来说，应该使用
LIO_WRITE作为操作码，而LIO_NOP意味着空操作



Linux AIO也可以由内核空间实现，异步I/O是Linux 2.6以后版本内核的一个标准特性。对于块设备而
言，AIO可以一次性发出大量的read/write调用并且通过通用块层的I/O调度来获得更好的性能，用户程序
也可以减少过多的同步负载，还可以在业务逻辑中更灵活地进行并发控制和负载均衡。相较于glibc的用户
空间多线程同步等实现也减少了线程的负载和上下文切换等。对于网络设备而言，在socket层面上，也可
以使用AIO，让CPU和网卡的收发动作充分交叠以改善吞吐性能

在用户空间中，我们一般要结合libaio来进行内核AIO的系统调用。内核AIO提供的系统调用主要包
括：

int io_setup(int maxevents, io_context_t *ctxp);
int io_destroy(io_context_t ctx);
int io_submit(io_context_t ctx, long nr, struct iocb *ios[]);
int io_cancel(io_context_t ctx, struct iocb *iocb, struct io_event *evt);
int io_getevents(io_context_t ctx_id, long min_nr, long nr, struct io_event *events,
		struct timespec *timeout);
void io_set_callback(struct iocb *iocb, io_callback_t cb);
void io_prep_pwrite(struct iocb *iocb, int fd, void *buf, size_t count, long long offset);
void io_prep_pread(struct iocb *iocb, int fd, void *buf, size_t count, long long offset);
void io_prep_pwritev(struct iocb *iocb, int fd, const struct iovec *iov, int iovcnt,
		long long offset);
void io_prep_preadv(struct iocb *iocb, int fd, const struct iovec *iov, int iovcnt,
		long long offset);

AIO的读写请求都用io_submit（）下发。下发前通过io_prep_pwrite（）和io_prep_pread（）生成iocb
的结构体，作为io_submit（）的参数。这个结构体指定了读写类型、起始地址、长度和设备标志符等信
息。读写请求下发之后，使用io_getevents（）函数等待I/O完成事件。io_set_callback（）则可设置一个
AIO完成的回调函数。
代码清单9.15演示了一个简单的利用libaio向内核发起AIO请求的模版。该程序位于本书源代码
的/kernel/drivers/globalfifo/ch9/aior.c下，使用命令gcc aior.c-o aior–laio编译，运行时带1个文本文件路径作为
参数，该程序会打印该文本文件前4096个字节的内容。
代码清单9.15 使用libaio调用内核AIO的范例

1#define _GNU_SOURCE /* O_DIRECT is not POSIX */
2#include <stdio.h> /* for perror() */
3#include <unistd.h> /* for syscall() */
4#include <fcntl.h> /* O_RDWR */
5#include <string.h> /* memset() */
6#include <inttypes.h> /* uint64_t */
7#include <stdlib.h>
8
9#include <libaio.h>
10
11#define BUF_SIZE 4096
12
13int main(int argc, char **argv)
	14{
		15 io_context_t ctx = 0;
		16 struct iocb cb;
		17 struct iocb *cbs[1];
		18 unsigned char *buf;
		19 struct io_event events[1];
		20 int ret;
		21 int fd;
		22
			23 if (argc < 2) {
				24 printf("the command format: aior [FILE]\n");
				25 exit(1);
				26 }
		27
			28 fd = open(argv[1], O_RDWR | O_DIRECT);
		29 if (fd < 0) {
			30 perror("open error");
			31 goto err;
			32 }
		33
			34 /* Allocate aligned memory */
			35 ret = posix_memalign((void **)&buf, 512, (BUF_SIZE + 1));
		36 if (ret < 0) {
			37 perror("posix_memalign failed");
			38 goto err1;
			39 }
		40 memset(buf, 0, BUF_SIZE + 1);
		41
			42 ret = io_setup(128, &ctx);
		43 if (ret < 0) {
			44 printf("io_setup error:%s"
					, strerror(-ret));
			45 goto err2;
			46 }
		47
			48 /* setup I/O control block */
			49 io_prep_pread(&cb, fd, buf, BUF_SIZE, 0);
		50
			51 cbs[0] = &cb;
		52 ret = io_submit(ctx, 1, cbs);
		53 if (ret != 1) {
			54 if (ret < 0) {
				55 printf("io_submit error:%s"
						, strerror(-ret));
				56 } else {
					57 fprintf(stderr,
							"could not sumbit IOs");
					58 }
			59 goto err3;
			60 }
		61
			62 /* get the reply */
			63 ret = io_getevents(ctx, 1, 1, events, NULL);
		64 if (ret != 1) {
			65 if (ret < 0) {
				66 printf("io_getevents error:%s"
						, strerror(-ret));
				67 } else {
					68 fprintf(stderr,
							"could not get Events");
					69 }
			70 goto err3;
			71 }
		72 if (events[0].res2 == 0) {
			73 printf("%s\n"
					, buf);
			74 } else {
				75 printf("AIO error:%s"
						, strerror(-events[0].res));
				76 goto err3;
				77 }
		78
			79 if ((ret = io_destroy(ctx)) < 0) {
				80 printf("io_destroy error:%s"
						, strerror(-ret));
				81 goto err2;
				82 }
		83
			84 free(buf);
		85 close(fd);
		86 return 0;
		87
			88 err3:
			89 if ((ret = io_destroy(ctx)) < 0)
			90 printf("io_destroy error:%s"
					, strerror(-ret));
		91 err2:
			92 free(buf);
		93 err1:
			94 close(fd);
		95 err:
			96 return -1;
		97}

9.4.3 AIO与设备驱动
用户空间调用io_submit（）后，对应于用户传递的每一个iocb结构，内核会生成一个与之对应的kiocb
结构。file_operations包含3个与AIO相关的成员函数：

ssize_t (*aio_read) (struct kiocb *iocb, const struct iovec *iov, unsigned long
				nr_segs, loff_t pos);
ssize_t (*aio_write) (struct kiocb *iocb, const struct iovec *iov, unsigned
				long nr_segs, loff_t pos);
int (*aio_fsync) (struct kiocb *iocb, int datasync);

		io_submit（）系统调用间接引起了file_operations中的aio_read（）和aio_write（）的调用。
		在早期的Linux内核中，aio_read（）和aio_write（）的原型是：

ssize_t (*aio_read) (struct kiocb *iocb, char __user *buffer,
				size_t size, loff_t pos);
ssize_t (*aio_write) (struct kiocb *iocb, const char *buffer,
		size_t count, loff_t offset);

在这个老的原型里，只含有一个缓冲区指针，而在新的原型中，则可以传递一个向量iovec，它含有
多段缓冲区。详见位于https://lwn.net/Articles/170954/的文档《Asynchronous I/O and vectored operations》。
AIO一般由内核空间的通用代码处理，对于块设备和网络设备而言，一般在Linux核心层的代码已经
解决。字符设备驱动一般不需要实现AIO支持。Linux内核中对字符设备驱动实现AIO的特例包括
drivers/char/mem.c里实现的null、zero等，由于zero这样的虚拟设备其实也不存在在要去读的时候读不到东
西的情况，所以aio_read_zero（）本质上也不包含异步操作，不过从代码清单9.16我们可以一窥iovec的全
貌。
代码清单9.16 zero设备的aio_read实现
static ssize_t aio_read_zero(struct kiocb *iocb, const struct iovec *iov,
		2 unsigned long nr_segs, loff_t pos)
	3{
		4 size_t written = 0;
		5 unsigned long i;
		6 ssize_t ret;
		7
			8 for (i = 0; i < nr_segs; i++) {
				9 ret = read_zero(iocb->ki_filp, iov[i].iov_base, iov[i].iov_len,
						10 &pos);
				11 if (ret < 0)
					12 break;
				13 written += ret;
				14 }
		15
			16 return written written : -EFAULT;
		17}

		9.5 总结
		本章主要讲述了Linux中的异步I/O，异步I/O可以使得应用程序在等待I/O操作的同时进行其他操作。
		使用信号可以实现设备驱动与用户程序之间的异步通知，总体而言，设备驱动和用户空间要分别完成
		3项对应的工作，用户空间设置文件的拥有者、FASYNC标志及捕获信号，内核空间响应对文件的拥有
		者、FASYNC标志的设置并在资源可获得时释放信号。
		Linux 2.6以后的内核包含对AIO的支持，它为用户空间提供了统一的异步I/O接口。另外，glibc也提供
		了一个不依赖于内核的用户空间的AIO支持
