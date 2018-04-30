/******************************************    *******************************
      > File Name: SequenceLock.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月17日 星期五 16时49分31秒
      > Modify Time:
 *********************************    ***************************************/

/*
 * 顺序锁（seqlock）是对读写锁的一种优化，若使用顺序锁，读执行单元不会被写执行单元阻塞，也就
 * 是说，读执行单元在写执行单元对被顺序锁保护的共享资源进行写操作时仍然可以继续读，而不必等待写
 * 执行单元完成写操作，写执行单元也不需要等待所有读执行单元完成读操作才去进行写操作。但是，写执
 * 行单元与写执行单元之间仍然是互斥的，即如果有写执行单元在进行写操作，其他写执行单元必须自旋在
 * 那里，直到写执行单元释放了顺序锁。
 * 对于顺序锁而言，尽管读写之间不互相排斥，但是如果读执行单元在读操作期间，写执行单元已经发
 * 生了写操作，那么，读执行单元必须重新读取数据，以便确保得到的数据是完整的。所以，在这种情况
 * 下，读端可能反复读多次同样的区域才能读到有效的数据。
 * */

/*1.获得顺序锁*/

void write_seqlock(seqlock_t *sl);
int write_tryseqlock(seqlock_t *sl);
write_seqlock_irqsave(lock, flags);
write_seqlock_irq(lock);
write_seqlock_bh(lock);

/*
 * 其中，
 * write_seqlock_irqsave() = loal_irq_save() + write_seqlock()
 * write_seqlock_irq() = local_irq_disable() + write_seqlock()
 * write_seqlock_bh() = local_bh_disable() + write_seqlock()
 *
 * */

/*2.释放顺序锁*/

void write_sequnlock(seqlock_t *sl);
write_sequnlock_irqrestore(lock, flags);
write_sequnlock_irq(lock);
write_sequnlock_bh(lock);


/*
 * 其中，
 * write_sequnlock_irqrestore() = write_sequnlock() + local_irq_restore()
 * write_sequnlock_irq() = write_sequnlock() + local_irq_enable()
 * write_sequnlock_bh() = write_sequnlock() + local_bh_enable()
 *
 * */

write_seqlock(&seqlock_a);
.../* 写操作代码块 */
write_sequnlock(&seqlock_a);


/*1.读开始*/


unsigned read_seqbegin(const seqlock_t *sl);
read_seqbegin_irqsave(lock, flags);

/*
 * 读执行单元在对被顺序锁s1保护的共享资源进行访问前需要调用该函数，该函数返回顺序锁s1的当前
 * 顺序号。其中，
 * read_seqbegin_irqsave() = local_irq_save() + read_seqbegin()
 * */

/*2.重读
 * */

int read_seqretry(const seqlock_t *sl, unsigned iv);
read_seqretry_irqrestore(lock, iv, flags);

/*
 * 读执行单元在访问完被顺序锁s1保护的共享资源后需要调用该函数来检查，在读访问期间是否有写操
 * 作。如果有写操作，读执行单元就需要重新进行读操作。其中，
 * read_seqretry_irqrestore() = read_seqretry() + local_irq_restore()
 *
 * */

do {
	seqnum = read_seqbegin(&seqlock_a);
	/* 读操作代码块 */
	...
} while (read_seqretry(&seqlock_a, seqnum));



