/******************************************    *******************************
      > File Name: get_post.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     Tue 29 Aug 2017 04:06:27 PM CST
      > Modify Time: 
 *********************************    ***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	printf("Content-type:text/html;charset=\"utf-8\"\r\n\r\n");
	char *webMethod = NULL;
	unsigned int webLen = 0;
	char cGet;
	if(NULL == (webMethod = getenv("REQUEST_METHOD")))
	{
		fprintf(stderr, "getenv error\n");
		return -1;
	}
	if(strcmp(webMethod, "GET") == 0)
	{
		printf("this is get method\r\n");
		printf("SERVER NAME:%s\r\n", getenv("SERVER_NAME"));
		printf("REMOTE_ADDR:%s\r\n", getenv("REMOTE_ADDR"));
		fprintf(stdout, "input para is:%s\r\n", getenv("QUERY_STRING"));
	}
	else if(strcmp(webMethod, "POST") == 0)
	{
		printf("this is post method</br>\n");
		webLen = atoi(getenv("CONTENT_LENGTH"));
		int i = 0;
		for(i = 0; i < webLen; i++)
		{
			cGet = fgetc(stdin);
			fputc(cGet, stdout);
		}
		printf("</br>\n");
		printf("over!!!!</br>\n");
	}
#if 0
	printf("<html>\n");
	printf("<head>\n");
	printf("<meta charset=\"utf-8\">\n");
	printf("<title>GET POST方法</title>\n");
	printf("</head>\n");
	printf("<body>\n");
	printf("<p>first cgi</p>\n");
	printf("</body>\n");
	printf("</html>\n");
#endif
	return 0;
}
