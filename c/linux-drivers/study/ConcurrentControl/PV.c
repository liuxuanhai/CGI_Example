/******************************************    *******************************
      > File Name: PV.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月17日 星期五 17时19分17秒
      > Modify Time:
 *********************************    ***************************************/

/*
 * 信号量（Semaphore）是操作系统中最典型的用于同步和互斥的手段，信号量的值可以是0、1或者n。
 * 信号量与操作系统中的经典概念PV操作对应。
 * P（S）：①将信号量S的值减1，即S=S-1；
 * ②如果S≥0，则该进程继续执行；否则该进程置为等待状态，排入等待队列。
 * V（S）：①将信号量S的值加1，即S=S+1；
 * ②如果S>0，唤醒队列中等待信号量的进程。
 * Linux中与信号量相关的操作主要有下面几种。
 * */


/*1.定义信号量*/

struct semaphore sem;

/*2.初始化信号量
 * */

void sema_init(struct semaphore *sem, int val);

/*3.获得信号量*/

void down(struct semaphore * sem);

/*该函数用于获得信号量sem，它会导致睡眠，因此不能在中断上下文中使用。
 * */

int down_interruptible(struct semaphore * sem);

/*
 * 该函数功能与down类似，不同之处为，因为down（）进入睡眠状态的进程不能被信号打断，但因为
 * down_interruptible（）进入睡眠状态的进程能被信号打断，信号也会导致该函数返回，这时候函数的返回
 * 值非0。
 *
 * */

int down_trylock(struct semaphore * sem);

/*
 * 该函数尝试获得信号量sem，如果能够立刻获得，它就获得该信号量并返回0，否则，返回非0值。它
 * 不会导致调用者睡眠，可以在中断上下文中使用。
 * 在使用down_interruptible（）获取信号量时，对返回值一般会进行检查，如果非0，通常立即返回-
 * ERESTARTSYS，如：
 *
 * */

if (down_interruptible(&sem))
	return -ERESTARTSYS;


/*4.释放信号量*/

void up(struct semaphore * sem);


