/******************************************    *******************************
      > File Name: atomic_op.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月17日 星期五 16时08分36秒
      > Modify Time:
 *********************************    ***************************************/


/*1.设置原子变量的值*/

void atomic_set(atomic_t *v, int i); /* 设置原子变量的值为i */
atomic_t v = ATOMIC_INIT(0); /* 定义原子变量v并初始化为0 */

/*2.获取原子变量的值*/

atomic_read(atomic_t *v); /* 返回原子变量的值*/

/*3.原子变量加/减*/

void atomic_add(int i, atomic_t *v);		/*原子变量增加i*/
void atomic_sub(int i, atomic_t *v);		/*原子变量减少i*/

/*4.原子变量自增/自减*/

void atomic_inc(atomic_t *v); /* 原子变量增加1 */
void atomic_dec(atomic_t *v); /* 原子变量减少1 */

/*5.操作并测试*/

int atomic_inc_and_test(atomic_t *v);
int atomic_dec_and_test(atomic_t *v);
int atomic_sub_and_test(int i, atomic_t *v);

/*
 * 上述操作对原子变量执行自增、自减和减操作后（注意没有加），测试其是否为0，为0返回true，否
 * 则返回false。
 * */

/*6.操作并返回*/

int atomic_add_return(int i, atomic_t *v);
int atomic_sub_return(int i, atomic_t *v);
int atomic_inc_return(atomic_t *v);
int atomic_dec_return(atomic_t *v);

/*上述操作对原子变量进行加/减和自增/自减操作，并返回新的值。*/

/*1.位原子操作*/

void set_bit(nr, void *addr);

/*上述操作设置addr地址的第nr位，所谓设置位即是将位写为1。*/

/*2.清除位*/

void clear_bit(nr, void *addr);

/*上述操作清除addr地址的第nr位，所谓清除位即是将位写为0。*/

/*3.改变位*/

void change_bit(nr, void *addr);

/*上述操作对addr地址的第nr位进行反置。
 * */

/*4.测试位
 * */

test_bit(nr, void *addr);

/*上述操作返回addr地址的第nr位。*/

/*5.测试并操作位*/

int test_and_set_bit(nr, void *addr);
int test_and_clear_bit(nr, void *addr);
int test_and_change_bit(nr, void *addr);

/*上述test_and_xxx_bit（nr，void*addr）操作等同于执行test_bit（nr，void*addr）后再执行
 * xxx_bit（nr，void*addr）。*/
