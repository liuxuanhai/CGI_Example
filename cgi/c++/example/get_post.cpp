/******************************************    *******************************
      > File Name: get_post.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     Tue 29 Aug 2017 04:06:27 PM CST
      > Modify Time: 
 *********************************    ***************************************/

#include <iostream>
#include <vector>  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cgicc/CgiDefs.h> 
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h> 
using namespace std;
using namespace cgicc;

int main(int argc, char **argv)
{
	Cgicc cgi;
	//获取环境变量必须这样定义
	const CgiEnvironment& env = cgi.getEnvironment();
	//设置格式
	//cout << HTTPHTMLHeader() << HTMLDoctype(HTMLDoctype::eStrict) << endl; 
	//cout << HTTPHTMLHeader() << endl; 
	//设置编码，在返回中文时能正确显示
	cout << HTTPContentHeader("text/html;charset=utf-8") << endl; 
#if 0
	//cout << "Content-type:text/html;charset=\"utf-8\"\r\n\r\n";
	cout << html() << endl;
	cout << head() << endl;
	cout << meta().set("charset", "utf-8") << endl;
	cout << title() << "C++ CGI" << title() << endl;
	cout << head() << endl;
#endif
	char *webMethod = NULL;
	unsigned int webLen = 0;
	char cGet;
	//读取AJAX请求类型，GET,POST
	if(NULL == (webMethod = getenv("REQUEST_METHOD")))
	{
		cout << "getenv error" << endl;
		return -1;
	}
	//cout << body() << endl;
	if(strcmp(webMethod, "GET") == 0)
	{
#if 0
		cout << p() << "this is get method" << p() << endl;
		cout << p() << "SERVER NAME:" << getenv("SERVER_NAME") << p() << endl;
		cout << p() << "REMOTE_ADDR:" << getenv("REMOTE_ADDR") << p() << endl;
		cout << p() << "input para is:" << getenv("QUERY_STRING") << p() << endl;
#endif
		cout << env.getPostData() << endl;
	}
	else if(strcmp(webMethod, "POST") == 0)
	{
	//	cout << p("this is post method") << endl;
		//webLen = atoi(getenv("CONTENT_LENGTH"));
	//	webLen = env.getContentLength();
	//	cout << p() << webLen << p() << endl;
	//	cout << h1() << endl;
		cout << env.getPostData() << endl;
		//输出值如何在页面不刷新的情况下显示到表格中
#if 0
		int i = 0;
		for(i = 0; i < webLen; i++)
		{
			cin >> cGet;
			cout << cGet;
		}
#endif
	//	cout << endl;
	//	cout << h1() << endl;
	//	cout << p("over!!!!") << endl;
	}
//	cout << body() << endl;
//	cout << html() << endl;
	return 0;
}
