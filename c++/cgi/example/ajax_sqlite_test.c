/******************************************    *******************************
      > File Name: ajax_sqlite_test.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     Fri 01 Sep 2017 03:15:24 PM CST
      > Modify Time: 
 *********************************    ***************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	printf("Content-type:text/html;charset=utf-8\r\n\r\n");
	char buf[250] = {'0'};
	if(strcmp(getenv("REQUEST_METHOD"), "POST"))
	{
		memcpy(buf, getenv("QUERY_STRING"), atoi(getenv("CONTENT_LENGTH")));
		int fd = open("./swap.txt", O_WRONLY|O_APPEND);
		write(fd, buf, 250);
		close(fd);
		printf("select err:%s %d", getenv("REQUEST_METHOD"), atoi(getenv("CONTENT_LENGTH")));
	}
	else
	{
		printf("%s", getenv("QUERY_STRING"));
	}
	return 0;
}
