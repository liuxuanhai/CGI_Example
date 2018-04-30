/******************************************    *******************************
      > File Name: ajax_sqlite_test.cpp
      > Author: Ywl
      > Descripsion:
      > Created Time:     Thu 31 Aug 2017 03:51:09 PM CST
      > Modify Time: 
 *********************************    ***************************************/
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

#include <cgicc/CgiDefs.h> 
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h> 

#include <json/json.h>

using namespace std;
using namespace cgicc;

#define TEST_DB_PATH "/home/ubuntu/workspace/mycode/sourcefile/sqlite3db/persion_info.db"
#define DB_NAME		"persion_info"
#define TABLE_END	"TableEnd"

/*
 切忌使用换行符，否则在ajax得到返回值时前后会多出换行符
 */
struct persion_info{
	int Numbering;
	string Name;
	int Age;
	string Gender;
	string Idcard;
	string Phonenumber;
	string Birthday;
	string Homeaddr;
	string Email;
};
static const char *AttriTableEn[] = {
	"Numbering",
	"Name",
	"Age",
	"Gender",
	"Idcard",
	"Phonenumber",
	"Birthday",
	"Homeaddr",
	"Email",
	"TableEnd"
};

static const char *AttriTable[] = {
	"序号",
	"姓名",
	"年龄",
	"性别",
	"身份证号",
	"手机号码",
	"出生日期",
	"家庭住址",
	"电子邮箱",
	"TableEnd"
};

static int result = 0;
static char *selRes[9] = {NULL};
static int callback(void *data, int argc, char **argv, char **azColName)
{
	if(result)
	{
		return 0;
	}
	result++;
	char i = 0;
	for(i = 0; i < argc; i++)
	{
		selRes[i] = argv[i];
	}
	//selRes = argv[0];
	return 0;
}
char selectDataMib(string request, string attri, string condition)
{
//	const char *data = request.data();
	sqlite3 *db;
	char sql[250];
	sprintf(sql, "select %s from %s where %s like '%s';", request.data(), DB_NAME, attri.data(), condition.data());
	if(sqlite3_open(TEST_DB_PATH, &db) < 0)
	{
		return -1;
	}
	else
	{
		if(sqlite3_exec(db, sql, callback, NULL, NULL) != SQLITE_OK)
		{
			sqlite3_close(db);
			return -1;
		}
		else if(result == 0)
		{
			sqlite3_close(db);
			return 0;
		}
		else
		{
			sqlite3_close(db);
		}
	}
	return 1;
	//cout << data << endl;
}
int main(int argc, char **argv)
{
	Cgicc cgi;
	const CgiEnvironment& env = cgi.getEnvironment();

	//cout << HTTPContentHeader("application/x-www-form-urlencoded;charset=utf-8");
	cout << HTTPContentHeader("text/plain;charset=utf-8");
	string webMethod;
	string requestStr;
	webMethod = env.getRequestMethod();
	if(webMethod == "GET")
	{
		cout << "not use this method";
	}
	else if(webMethod == "POST")
	{
		requestStr = env.getPostData();
		/*
		 把string转换为char* 有3中方法： 
		 1。data 
		 如： 
		 string str="abc"; 
		 char *p=str.data(); 
		 2.c_str 
		 如：string str="gdfd"; 
		     char *p=str.c_str(); 
			 3 copy 
			 比如 
			 string str="hello"; 
			 char p[40]; 
			 str.copy(p,5,0); //这里5，代表复制几个字符，0代表复制的位置
			 *(p+5)='/0'; //要手动加上结束符
			 cout < <p;
		 */
		Json::Reader reader;
		Json::Value rootreq;
		if(!reader.parse(requestStr, rootreq))
		{
			cout << "select err";
		}
		persion_info sqlrequest;
		sqlrequest.Name = rootreq["Name"].asString();
#if 0
		ofstream swap;
		swap.open("./swap.txt", std::ofstream::binary);
		if(!swap.is_open())
		{
			cout << "select err";
			return -1;
		}
		swap << requestStr << "type: " << env.getContentType() << endl;
		swap << flush;
		swap.close();
		cout << requestStr;
		return 0;
#endif
		char ok = 0;
		char i = 0;
		char j = 0;
		Json::Value rootres;
		Json::StyledWriter styled_writer;
		for(j = 0; j < 9; j++)
		{
			if((ok = selectDataMib("*", AttriTable[j], sqlrequest.Name)) == -1)
			{
				cout << "select err";
				return ok;
			}
			else if(ok == 0)
			{
				continue;
			}
			else
			{
				for(i = 0; i < 9; i++)
				{
					//rootres["Numbering"] = Json::Value(atoi(selRes[0]));
					//rootres["Name"].append(selRes[1]);
					rootres[AttriTableEn[i]].append(selRes[i]);
				}
				cout << styled_writer.write(rootres); 
				return 0;;
				//cout << selRes;			
			}
		}
		cout << "查无此人";
	}
	return 0;
}
