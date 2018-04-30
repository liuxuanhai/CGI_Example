/******************************************    *******************************
      > File Name: （Read-Copy-Update.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月17日 星期五 16时54分13秒
      > Modify Time:
 *********************************    ***************************************/

/*
 * 不同于自旋锁，使用RCU的读端没有锁、内存屏障、原子指令类的开销，几乎可以认为是直接读
 * （只是简单地标明读开始和读结束），而RCU的写执行单元在访问它的共享资源前首先复制一个副本，
 * 然后对副本进行修改，最后使用一个回调机制在适当的时机把指向原来数据的指针重新指向新的被修改的
 * 数据，这个时机就是所有引用该数据的CPU都退出对共享数据读操作的时候。等待适当时机的这一时期称
 * 为宽限期（Grace Period）。
 *
 * */
//比如，有下面的一个由struct foo结构体组成的链表：
struct foo {
	struct list_head list;
	int a;
	int b;
	int c;
};
/*假设进程A要修改链表中某个节点N的成员a、b。自旋锁的思路是排他性地访问这个链表，等所有其
他持有自旋锁的进程或者中断把自旋锁释放后，进程A再拿到自旋锁访问链表并找到N节点，之后修改它
的a、b两个成员，完成后解锁。而RCU的思路则不同，它直接制造一个新的节点M，把N的内容复制给
M，之后在M上修改a、b，并用M来代替N原本在链表的位置。之后进程A等待在链表前期已经存在的所
有读端结束后（即宽限期，通过下文说的synchronize_rcu（）API完成），再释放原来的N。用代码来描述
这个逻辑就是：*/
struct foo {
	struct list_head list;
	int a;
	int b;
	int c;
};
LIST_HEAD(head);
/* . . . */
p = search(head, key);
if (p == NULL) {
	/* Take appropriate action, unlock, and return. */
}
q = kmalloc(sizeof(*p), GFP_KERNEL);
*q = *p;
q->b = 2;
q->c = 3;
list_replace_rcu(&p->list, &q->list);
synchronize_rcu();
kfree(p);
/*RCU可以看作读写锁的高性能版本，相比读写锁，RCU的优点在于既允许多个读执行单元同时访问
被保护的数据，又允许多个读执行单元和多个写执行单元同时访问被保护的数据。但是，RCU不能替代
读写锁，因为如果写比较多时，对读执行单元的性能提高不能弥补写执行单元同步导致的损失。因为使用
RCU时，写执行单元之间的同步开销会比较大，它需要延迟数据结构的释放，复制被修改的数据结构，
它也必须使用某种锁机制来同步并发的其他写执行单元的修改操作
*/

/*1.读锁定
 * */

rcu_read_lock();
rcu_read_lock_bh();

/*2.读解锁
 * */

rcu_read_unlock();
rcu_read_unlock_bh();

//使用RCU进行读的模式如下：
rcu_read_lock();
	.../* 读临界区*/
rcu_read_unlock();

/*3.同步RCU*/

synchronize_rcu();

/*
 *
该函数由RCU写执行单元调用，它将阻塞写执行单元，直到当前CPU上所有的已经存在（Ongoing）
的读执行单元完成读临界区，写执行单元才可以继续下一步操作。synchronize_rcu（）并不需要等待后续
（Subsequent）读临界区的完成
 * */

/*4.挂接回调*/
void call_rcu(struct rcu_head *head,
		void (*func)(struct rcu_head *rcu));
函数call_rcu（）也由RCU写执行单元调用，与synchronize_rcu（）不同的是，它不会使写执行单元阻
塞，因而可以在中断上下文或软中断中使用。该函数把函数func挂接到RCU回调函数链上，然后立即返
回。挂接的回调函数会在一个宽限期结束（即所有已经存在的RCU读临界区完成）后被执行。
rcu_assign_pointer(p, v)
	给RCU保护的指针赋一个新的值。
	rcu_dereference(p)
	读端使用rcu_dereference（）获取一个RCU保护的指针，之后既可以安全地引用它（访问它指向的区
	域）。一般需要在rcu_read_lock（）/rcu_read_unlock（）保护的区间引用这个指针，例如：
	rcu_read_lock();
	irq_rt = rcu_dereference(kvm->irq_routing);
	if (irq < irq_rt->nr_rt_entries)
	hlist_for_each_entry(e, &irq_rt->map[irq], link) {
		if (likely(e->type == KVM_IRQ_ROUTING_MSI))
			ret = kvm_set_msi_inatomic(e, kvm);
		else
			ret = -EWOULDBLOCK;
		break;
	}
rcu_read_unlock();
上述代码取自virt/kvm/irq_comm.c的kvm_set_irq_inatomic（）函数。
rcu_access_pointer(p)
	读端使用rcu_access_pointer（）获取一个RCU保护的指针，之后并不引用它。这种情况下，我们只关
	心指针本身的值，而不关心指针指向的内容。比如我们可以使用该API来判断指针是否为NULL。
	把rcu_assign_pointer（）和rcu_dereference（）结合起来使用，写端分配一个新的struct foo内存，并初
	始化其中的成员，之后把该结构体的地址赋值给全局的gp指针：
	struct foo {
		int a;
		int b;
		int c;
	};
struct foo *gp = NULL;
/* . . . */
p = kmalloc(sizeof(*p), GFP_KERNEL);
p->a = 1;
p->b = 2;
p->c = 3;
rcu_assign_pointer(gp, p);
读端访问该片区域：
rcu_read_lock();
p = rcu_dereference(gp);
if (p != NULL) {
	do_something_with(p->a, p->b, p->c);
}
rcu_read_unlock();
在上述代码中，我们可以把写端rcu_assign_pointer（）看成发布（Publish）了gp，而读端
rcu_dereference（）看成订阅（Subscribe）了gp。它保证读端可以看到rcu_assign_pointer（）之前所有内存
被设置的情况（即gp->a，gp->b，gp->c等于1、2、3对于读端可见）。由此可见，与RCU相关的原语已经
内嵌了相关的编译屏障或内存屏障。
对于链表数据结构而言，Linux内核增加了专门的RCU保护的链表操作API：
static inline void list_add_rcu(struct list_head *new, struct list_head *head);
该函数把链表元素new插入RCU保护的链表head的开头。
static inline void list_add_tail_rcu(struct list_head *new,
		struct list_head *head);
该函数类似于list_add_rcu（），它将把新的链表元素new添加到被RCU保护的链表的末尾。
static inline void list_del_rcu(struct list_head *entry);
该函数从RCU保护的链表中删除指定的链表元素entry。
static inline void list_replace_rcu(struct list_head *old, struct list_head *new);
它使用新的链表元素new取代旧的链表元素old。
list_for_each_entry_rcu(pos, head)
	该宏用于遍历由RCU保护的链表head，只要在读执行单元临界区使用该函数，它就可以安全地和其他
	RCU保护的链表操作函数（如list_add_rcu（））并发运行。
	链表的写端代码模型如下：
	struct foo {
		struct list_head list;
		int a;
		int b;
		int c;
	};
LIST_HEAD(head);
/* . . . */
p = kmalloc(sizeof(*p), GFP_KERNEL);
p->a = 1;
p->b = 2;
p->c = 3;
list_add_rcu(&p->list, &head);
链表的读端代码则形如：
rcu_read_lock();
list_for_each_entry_rcu(p, head, list) {
	do_something_with(p->a, p->b, p->c);
}
rcu_read_unlock();
前面已经看到了对RCU保护链表中节点进行修改以及添加新节点的动作，下面我们看一下RCU保护
的链表删除节点N的工作。写端分为两个步骤，第1步是从链表中删除N，之后等一个宽限期结束，再释放
N的内存。下面的代码分别用读写锁和RCU两种不同的方法来描述这一过程：


/*
 *
 * */
1 struct el { 														1 struct el {
	2 struct list_head lp; 											2 	struct list_head lp;
	3 long key; 													3 	long key;
	4 spinlock_t mutex; 											4 	spinlock_t mutex;
	5 int data; 													5 	int data;
	6 /* Other data fields */ 										6 /* Other data fields */
		7}; 														7};
8 DEFINE_RWLOCK(listmutex); 										8 DEFINE_SPINLOCK(listmutex);
9 LIST_HEAD(head); 													9 LIST_HEAD(head);
1 int search(long key, int *result) 								1 int search(long key, int *result)
	2{ 																2{
		3 struct el *p; 											3 	struct el *p;
		4 															4
			5 read_lock(&listmutex); 								5 rcu_read_lock();
		6 list_for_each_entry(p, &head, lp) { 						6 list_for_each_entry_rcu(p, &head, lp) {
			7 if (p->key == key) { 									7 if (p->key == key) {
				8 *result = p->data; 								8 *result = p->data;
				9 read_unlock(&listmutex); 							9 rcu_read_unlock();
				10 return 1; 										10 return 1;
				11 } 												11 }
			12 } 													12 }
		13 read_unlock(&listmutex); 								13 rcu_read_unlock();
		14 return 0; 												14 return 0;
		15} 														15 }
		1 int delete(long key) 										1 int delete(long key)
	2{ 																2{
		3 struct el *p; 											3 struct el *p;
		4 															4
			5 write_lock(&listmutex); 5 spin_lock(&listmutex);
		6 list_for_each_entry(p, &head, lp) { 6 list_for_each_entry(p, &head, lp) {
			7 if (p->key == key) { 7 if (p->key == key) {
				8 list_del(&p->lp); 8 list_del_rcu(&p->lp);
				9 write_unlock(&listmutex); 9 spin_unlock(&listmutex);
				10 synchronize_rcu();
				10 kfree(p); 11 kfree(p);
				11 return 1; 12 return 1;
				12 } 13 }
			13 } 14 }
		14 write_unlock(&listmutex); 15 spin_unlock(&listmutex);
		15 return 0; 16 return 0;
		16} 17}
