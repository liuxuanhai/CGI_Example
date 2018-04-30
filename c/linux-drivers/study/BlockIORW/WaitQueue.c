/******************************************    *******************************
      > File Name: WaitQueue.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月20日 星期一 10时41分42秒
      > Modify Time:
 *********************************    ***************************************/

/*
 * 在Linux驱动程序中，可以使用等待队列（Wait Queue）来实现阻塞进程的唤醒。等待队列很早就作为
 * 一个基本的功能单位出现在Linux内核里了，它以队列为基础数据结构，与进程调度机制紧密结合，可以
 * 用来同步对系统资源的访问，第7章中所讲述的信号量在内核中也依赖等待队列来实现。
 *
 * */

/*1.定义“等待队列头部”*/
wait_queue_head_t my_queue;

/*2.初始化“等待队列头部”*/

init_waitqueue_head(&my_queue);
/*而下面的DECLARE_WAIT_QUEUE_HEAD（）宏可以作为定义并初始化等待队列头部的“快捷方式”
 * */

DECLARE_WAIT_QUEUE_HEAD (name);

#define DECLARE_WAIT_QUEUE_HEAD(name) \
	    wait_queue_head_t name = __WAIT_QUEUE_HEAD_INITIALIZER(name)
#define __WAIT_QUEUE_HEAD_INITIALIZER(name) {                \
	    .lock        = __SPIN_LOCK_UNLOCKED(name.lock),        \
	    .task_list    = { &(name).task_list, &(name).task_list } }

/*3.定义等待队列元素*/

DECLARE_WAITQUEUE(name, tsk);
//tsk一般为当前进行current. 这个宏定义并初始化一个名为name的等待队列.
#define DECLARE_WAITQUEUE(name, tsk)                    \
	    wait_queue_t name = __WAITQUEUE_INITIALIZER(name, tsk)
#define __WAITQUEUE_INITIALIZER(name, tsk) {                \
	    .private    = tsk,                        \
	    .func        = default_wake_function,            \
	    .task_list    = { NULL, NULL } }

/*
 * 该宏用于定义并初始化一个名为name的等待队列元素。
 * */

/*4.添加/移除等待队列*/

void add_wait_queue(wait_queue_head_t *q, wait_queue_t *wait);
void remove_wait_queue(wait_queue_head_t *q, wait_queue_t *wait);

/*
 * add_wait_queue（）用于将等待队列元素wait添加到等待队列头部q指向的双向链表中，而
 * remove_wait_queue（）用于将等待队列元素wait从由q头部指向的链表中移除
 * */

/*5.等待事件*/

wait_event(queue, condition);
wait_event_interruptible(queue, condition);
wait_event_timeout(queue, condition, timeout);
wait_event_interruptible_timeout(queue, condition, timeout);


/*
 * 等待第1个参数queue作为等待队列头部的队列被唤醒，而且第2个参数condition必须满足，否则继续
 * 阻塞。wait_event（）和wait_event_interruptible（）的区别在于后者可以被信号打断，而前者不能。加上
 * _timeout后的宏意味着阻塞等待的超时时间，以jiffy为单位，在第3个参数的timeout到达时，不论condition
 * 是否满足，均返回。
 * */

/*6.唤醒队列*/

void wake_up(wait_queue_head_t *queue);
void wake_up_interruptible(wait_queue_head_t *queue);

/*
 * 上述操作会唤醒以queue作为等待队列头部的队列中所有的进程。
 * wake_up（）应该与wait_event（）或wait_event_timeout（）成对使用，而wake_up_interruptible（）则
 * 应与wait_event_interruptible（）或wait_event_interruptible_timeout（）成对使用。wake_up（）可唤醒处于
 * TASK_INTERRUPTIBLE和TASK_UNINTERRUPTIBLE的进程，而wake_up_interruptible（）只能唤醒处于
 * TASK_INTERRUPTIBLE的进程。
 * */

/*7.在等待队列上睡眠*/

sleep_on(wait_queue_head_t *q );
interruptible_sleep_on(wait_queue_head_t *q );


/*
 * sleep_on（）函数的作用就是将目前进程的状态置成TASK_UNINTERRUPTIBLE，并定义一个等待队
 * 列元素，之后把它挂到等待队列头部q指向的双向链表，直到资源可获得，q队列指向链接的进程被唤
 * 醒。
 * interruptible_sleep_on（）与sleep_on（）函数类似，其作用是将目前进程的状态置成
 * TASK_INTERRUPTIBLE，并定义一个等待队列元素，之后把它附属到q指向的队列，直到资源可获得（q
 * 指引的等待队列被唤醒）或者进程收到信号。
 * sleep_on（）函数应该与wake_up（）成对使用，interruptible_sleep_on（）应该与
 * wake_up_interruptible（）成对使用。
 * */
