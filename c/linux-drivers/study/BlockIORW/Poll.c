/******************************************    *******************************
      > File Name: Poll.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月20日 星期一 16时00分48秒
      > Modify Time:
 *********************************    ***************************************/

unsigned int(*poll)(struct file * filp, struct poll_table* wait);

/*
 * 第1个参数为file结构体指针，第2个参数为轮询表指针。这个函数应该进行两项工作。
 * 1）对可能引起设备文件状态变化的等待队列调用poll_wait（）函数，将对应的等待队列头部添加到
 * poll_table中。
 * 2）返回表示是否能对设备进行无阻塞读、写访问的掩码。
 * 用于向poll_table注册等待队列的关键poll_wait（）函数的原型如下：
 * */

void poll_wait(struct file *filp, wait_queue_heat_t *queue, poll_table * wait);
/*
 * poll_wait（）函数的名称非常容易让人产生误会，以为它和wait_event（）等一样，会阻塞地等待某
 * 事件的发生，其实这个函数并不会引起阻塞。poll_wait（）函数所做的工作是把当前进程添加到wait参数
 * 指定的等待列表（poll_table）中，实际作用是让唤醒参数queue对应的等待队列可以唤醒因select（）而睡
 * 眠的进程。
 * 驱动程序poll（）函数应该返回设备资源的可获取状态，即POLLIN、POLLOUT、POLLPRI、
 * POLLERR、POLLNVAL等宏的位“或”结果。每个宏的含义都表明设备的一种状态，如POLLIN（定义为
 * 0x0001）意味着设备可以无阻塞地读，POLLOUT（定义为0x0004）意味着设备可以无阻塞地写。
 *
 * 在数量很多都时使用epoll()更合理，不会影响效率
 *
 * */

static unsigned int xxx_poll(struct file *filp, poll_table *wait)
{
	unsigned int mask = 0;
	struct xxx_dev *dev = filp->private_data; /* 获得设备结构体指针*/

	...
		poll_wait(filp, &dev->r_wait, wait); /* 加入读等待队列 */
	poll_wait(filp, &dev->w_wait, wait); /* 加入写等待队列 */

	if (...) /* 可读 */
		mask |= POLLIN | POLLRDNORM; /* 标示数据可获得（对用户可读）*/

	if (...) /* 可写 */
		mask |= POLLOUT | POLLWRNORM; /* 标示数据可写入*/
	...
	return mask;
}
