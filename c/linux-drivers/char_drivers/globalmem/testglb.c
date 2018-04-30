/******************************************    *******************************
      > File Name: test.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年10月26日 星期四 10时45分09秒
      > Modify Time: 
 *********************************    ***************************************/

#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define DEV_GLB		"/dev/globalmem0"

int main()
{
	int fd = open(DEV_GLB, O_RDWR);
	ioctl(fd, 0x1, 0);
	close(fd);
	return 0;
}
