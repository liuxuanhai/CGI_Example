/******************************************    *******************************
  > File Name: test.c
  > Author: Ywl
  > Descripsion:
  > Created Time:     2017年11月21日 星期二 17时07分16秒
  > Modify Time:
 *********************************    ***************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


/* third_test
 *  */
int main(int argc ,char *argv[])

{
	int fd;
	unsigned char key_val;

	fd = open("/dev/buttons",O_RDWR);
	if (fd < 0)
	{
		printf("open error\n");
	}

	while(1)
	{
		read(fd,&key_val,1);
		printf("key_val = 0x%x\n",key_val);

	}
	return 0;
}
/*
 * 一、
 * [WJ2440]# ls
 * Qt             etc            mnt            second_drv.ko  udisk
 * TQLedtest      first_drv.ko   opt            second_test    usr
 * app_test       first_test     proc           sys            var
 * bin            home           root           third_drv.ko   web
 * dev            lib            sbin           third_test
 * driver_test    linuxrc        sddisk         tmp
 * [WJ2440]# ls /dev/buttons -l
 * ls: /dev/buttons: No such file or directory
 * [WJ2440]# insmod third_drv.ko
 * [WJ2440]# lsmod
 * third_drv 3016 0 - Live 0xbf003000
 * [WJ2440]# ls /dev/buttons -l
 * crw-rw----    1 root     root      252,   0 Jan  2 02:12 /dev/buttons
 * [WJ2440]# ./third_test
 * key_val = 0x1
 * key_val = 0x81
 * key_val = 0x2
 * key_val = 0x82
 * key_val = 0x3
 * key_val = 0x83
 * key_val = 0x4
 * key_val = 0x84
 * key_val = 0x2
 * key_val = 0x2
 * key_val = 0x82
 * key_val = 0x1
 * key_val = 0x81
 * key_val = 0x2
 * key_val = 0x82
 * key_val = 0x2
 * key_val = 0x82
 * key_val = 0x4
 * key_val = 0x4
 * key_val = 0x4
 * key_val = 0x84
 *
 * 键值: 按下时, 0x01, 0x02, 0x03, 0x04
 * 键值: 松开时, 0x81, 0x82, 0x83, 0x84
 *
 * [WJ2440]# ./third_test &
 * [WJ2440]# top
 * Mem: 10912K used, 49252K free, 0K shrd, 0K buff, 8104K cached
 * CPU:  0.0% usr  0.7% sys  0.0% nic 99.0% idle  0.0% io  0.1% irq  0.0% sirq
 * Load average: 0.00 0.05 0.03 1/23 627
 * PID  PPID USER     STAT   VSZ %MEM CPU %CPU COMMAND
 *   627   589 root     R     2092  3.4   0  0.7 top
 *   589     1 root     S     2092  3.4   0  0.0 -/bin/sh
 *       1     0 root     S     2088  3.4   0  0.0 init
 *         590     1 root     S     2088  3.4   0  0.0 /usr/sbin/telnetd -l /bin/login
 *           587     1 root     S     1508  2.5   0  0.0 EmbedSky_wdg
 *             626   589 root     S     1428  2.3   0  0.0 ./third_test
 *               573     2 root     SW<      0  0.0   0  0.0 [rpciod/0]
 *                   5     2 root     SW<      0  0.0   0  0.0 [khelper]
 *                     329     2 root     SW<      0  0.0   0  0.0 [nfsiod]
 *                      2     0 root     SW<      0  0.0   0  0.0 [kthreadd]
 *                          3     2 root     SW<      0  0.0   0  0.0 [ksoftirqd/0]
 *                              4     2 root     SW<      0  0.0   0  0.0 [events/0]
 *                                 11     2 root     SW<      0  0.0   0  0.0 [async/mgr]
 *                                   237     2 root     SW<      0  0.0   0  0.0 [kblockd/0]
 *                                     247     2 root     SW<      0  0.0   0  0.0 [khubd]
 *                                       254     2 root     SW<      0  0.0   0  0.0 [kmmcd]
 *                                         278     2 root     SW       0  0.0   0  0.0 [pdflush]
 *                                           279     2 root     SW       0  0.0   0  0.0 [pdflush]
 *                                             280     2 root     SW<      0  0.0   0  0.0 [kswapd0]
 *                                               325     2 root     SW<      0  0.0   0  0.0 [aio/0]
 *
 * 可发现，按键没有被按下时，third_test进程是处于睡眠状态的，并且几乎不占用CPU的利用率
 * [WJ2440]# lsmod
* third_drv 3016 2 - Live 0xbf003000
* [WJ2440]# rmmod third_drv
* rmmod: remove 'third_drv': Resource temporarily unavailable
* [WJ2440]# kill -9 626
* [1]+  Killed                     ./third_test
* [WJ2440]# rmmod  third_drv
* rmmod: module 'third_drv' not found
* [WJ2440]# lsmod
* [WJ2440]#
 * */
