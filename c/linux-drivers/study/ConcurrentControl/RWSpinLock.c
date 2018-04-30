/******************************************    *******************************
      > File Name: RWSpinLock.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月17日 星期五 16时44分05秒
      > Modify Time:
 *********************************    ***************************************/


/*
 * 自旋锁不关心锁定的临界区究竟在进行什么操作，不管是读还是写，它都一视同仁。即便多个执行单
 * 元同时读取临界资源也会被锁住。实际上，对共享资源并发访问时，多个执行单元同时读取它是不会有问
 * 题的，自旋锁的衍生锁读写自旋锁（rwlock）可允许读的并发。读写自旋锁是一种比自旋锁粒度更小的锁
 * 机制，它保留了“自旋”的概念，但是在写操作方面，只能最多有1个写进程，在读操作方面，同时可以有
 * 多个读执行单元。当然，读和写也不能同时进行
 * */

/*1.定义和初始化读写自旋锁*/

rwlock_t my_rwlock;
rwlock_init(&my_rwlock); /* 动态初始化 */

/*2.读锁定*/

void read_lock(rwlock_t *lock);
void read_lock_irqsave(rwlock_t *lock, unsigned long flags);
void read_lock_irq(rwlock_t *lock);
void read_lock_bh(rwlock_t *lock);


/*3.读解锁*/

void read_unlock(rwlock_t *lock);
void read_unlock_irqrestore(rwlock_t *lock, unsigned long flags);
void read_unlock_irq(rwlock_t *lock);
void read_unlock_bh(rwlock_t *lock);

/*
 * 在对共享资源进行读取之前，应该先调用读锁定函数，完成之后应调用读解锁函数。
 * read_lock_irqsave（）、read_lock_irq（）和read_lock_bh（）也分别是read_lock（）分别与
 * local_irq_save（）、local_irq_disable（）和local_bh_disable（）的组合，读解锁函数
 * read_unlock_irqrestore（）、read_unlock_irq（）、read_unlock_bh（）的情况与此类似。
 *
 * */

/*4.写锁定
 * */

void write_lock(rwlock_t *lock);
void write_lock_irqsave(rwlock_t *lock, unsigned long flags);
void write_lock_irq(rwlock_t *lock);
void write_lock_bh(rwlock_t *lock);
int write_trylock(rwlock_t *lock);

/*5.写解锁*/

void write_unlock(rwlock_t *lock);
void write_unlock_irqrestore(rwlock_t *lock, unsigned long flags);
void write_unlock_irq(rwlock_t *lock);
void write_unlock_bh(rwlock_t *lock);

/*
 * write_lock_irqsave（）、write_lock_irq（）、write_lock_bh（）分别是write_lock（）与
 * local_irq_save（）、local_irq_disable（）和local_bh_disable（）的组合，写解锁函数
 * write_unlock_irqrestore（）、write_unlock_irq（）、write_unlock_bh（）的情况与此类似。
 * 在对共享资源进行写之前，应该先调用写锁定函数，完成之后应调用写解锁函数。和spin_trylock（）
 * 一样，write_trylock（）也只是尝试获取读写自旋锁，不管成功失败，都会立即返回
 * */



rwlock_t lock; /* 定义rwlock */
rwlock_init(&lock); /* 初始化rwlock */
/* 读时获取锁*/
read_lock(&lock);
... /* 临界资源 */
read_unlock(&lock);
/* 写时获取锁*/
write_lock_irqsave(&lock, flags);
... /* 临界资源 */
write_unlock_irqrestore(&lock, flags);
