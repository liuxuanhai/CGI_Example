/******************************************    *******************************
      > File Name: sqlit3test.cpp
      > Author: Ywl
      > Descripsion:
      > Created Time:     Thu 31 Aug 2017 10:31:59 AM CST
      > Modify Time: 
 *********************************    ***************************************/

#include <iostream>
#include <vector>  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include <cgicc/CgiDefs.h> 
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h> 
using namespace std;
using namespace cgicc;
/*
 1	sqlite3_open(const char *filename, sqlite3 **ppDb)
 该例程打开一个指向 SQLite 数据库文件的连接，返回一个用于其他 SQLite 程序的数据库连接对象。
 如果 filename 参数是 NULL 或 ':memory:'，那么 sqlite3_open() 将会在 RAM 中创建一个内存数据库，这只会在 session 的有效时间内持续。
 如果文件名 filename 不为 NULL，那么 sqlite3_open() 将使用这个参数值尝试打开数据库文件。如果该名称的文件不存在，sqlite3_open() 将创建一个新的命名为该名称的数据库文件并打开。
 2	sqlite3_exec(sqlite3*, const char *sql, sqlite_callback, void *data, char **errmsg)
 该例程提供了一个执行 SQL 命令的快捷方式，SQL 命令由 sql 参数提供，可以由多个 SQL 命令组成。
 在这里，第一个参数 sqlite3 是打开的数据库对象，sqlite_callback 是一个回调，data 作为其第一个参数，errmsg 将被返回用来获取程序生成的任何错误。
 sqlite3_exec() 程序解析并执行由 sql 参数所给的每个命令，直到字符串结束或者遇到错误为止。
 3	sqlite3_close(sqlite3*)
 该例程关闭之前调用 sqlite3_open() 打开的数据库连接。所有与连接相关的语句都应在连接关闭之前完成。
 如果还有查询没有完成，sqlite3_close() 将返回 SQLITE_BUSY 禁止关闭的错误消息。
 fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
 */

int main(int argc, char **argv)
{
	Cgicc cgi;
	//获取环境变量必须这样定义
	const CgiEnvironment& env = cgi.getEnvironment();
	//设置格式
	//设置编码，在返回中文时能正确显示
	cout << HTTPContentHeader("text/html;charset=utf-8") << endl; 
	char *webMethod = NULL;
	unsigned int webLen = 0;
	char cGet;
	//读取AJAX请求类型，GET,POST
	if(NULL == (webMethod = getenv("REQUEST_METHOD")))
	{
		cout << "getenv error" << endl;
		return -1;
	}
	if(strcmp(webMethod, "GET") == 0)
	{
		cout << env.getPostData() << endl;
	}
	else if(strcmp(webMethod, "POST") == 0)
	{
		//输出值如何在页面不刷新的情况下显示到表格中
		//cout << env.getPostData() << endl;
		sqlite3 *db;
		if(sqlite3_open("test.db", &db) < 0)
		{
			cout << "open err" << endl;
		}
		else
		{
			cout << "open ok" << endl;
			sqlite3_close(db);
		}
	}
	return 0;
}
