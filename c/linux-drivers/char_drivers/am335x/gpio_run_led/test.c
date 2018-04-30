/******************************************    *******************************
      > File Name: test.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     Sat 28 Oct 2017 03:34:09 PM CST
      > Modify Time: 
 *********************************    ***************************************/

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "../Include"

#define LED_DIR		"/dev/pri_gpio_driver"

#define INPUT	1
#define OUTPUT	0

#define OFF		1
#define ON		0

int main()
{
	char on = ON;
	char off = OFF;
	/*打开权限必须是可读可写*/
	int fd = open(LED_DIR, O_RDWR);
	ioctl(fd, OUTPUT, GPIO2_30);
	ioctl(fd, OUTPUT, GPIO2_31);
/* 
 *off : 1, 1
 *red : 0, 1
 *green : 1, 0
 *orange : 0, 0
 * */
	write(fd, &off, GPIO2_30);
	write(fd, &on, GPIO2_31);
	close(fd);
	return 0;
}
