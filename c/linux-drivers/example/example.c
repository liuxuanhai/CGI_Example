/******************************************    *******************************
      > File Name: example.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年10月24日 星期二 17时42分04秒
      > Modify Time: 
 *********************************    ***************************************/

#include <linux/init.h>
#include <linux/module.h>

static int example_init(void)
{
	printk(KERN_INFO "Example driver init\n");
	return 0;
}

static void example_exit(void)
{
	printk(KERN_INFO "Example driver exit\n");
}

//加载与卸载函数
module_init(example_init);
module_exit(example_exit);

//作者
MODULE_AUTHOR("Ywl <astralrovers@outlook.com>");
//模块许可声明，不声明编译不过的
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("An example driver program");
MODULE_ALIAS("a simple module");
