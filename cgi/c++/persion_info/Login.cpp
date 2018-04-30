/******************************************    *******************************
      > File Name: Login.cpp
      > Author: Ywl
      > Descripsion:
      > Created Time:     Wed 13 Sep 2017 10:35:00 PM CST
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

#define TEST_MODE	0

class LoginInfo{
public:
	string AckCmd;
	string OperMode;
	string LoginName;
	string LoginPass;
};

#define TEST_DB_PATH "/yu_yuan/www//sourcefile/sqlite3db/user_pass.db"
#define DB_NAME		"user_pass"

#define POST_JSON_DATA_ERR "post_json_data_err"
#define LOGIN_OPER "user_login_on"
#define REGISTER_OPER "user_register_new"

enum{
	LOGIN_ON_AJAX = 8,
	REGISTER_AJAX = 9,
	UNKNOWN_AJAX = 0xFF
};
enum{
	OPER_CURRECT = 0,
	SQLITE_OPER_ERR = 1,
	SELECT_DATA_ERR = 2,
	LOGIN_USER_PASS_ERR = 8,
	REGISTER_EXSIT_USER_ERR = 9,
	LOGIN_USER_NOT_EXSIT = 10,
	UNKNOWN_ERR = 0xFF
};

static const char *LoginTable[] = {
	"应答标志",
	"操作类型",
	"用户名",
	"密码"
};
static const char *LoginTableEn[] = {
	"AckCmd",
	"OperMode",
	"UserName",
	"UserPass"
};
typedef struct{
	char UserName[60];
	char UserPass[13];
}SELE_USER_INFO;
static SELE_USER_INFO User_info;
static char UserExsit = 0;
static int callback(void *data, int argc, char **argv, char **azColName)
{
	if(argc < 0)
	{
		return -1;
	}
	strcpy(User_info.UserName, argv[0]);
	strcpy(User_info.UserPass, argv[1]);
	UserExsit = 1;
	return 0;
}
unsigned char LoginOnCheck(LoginInfo sqlrequest)
{
	sqlite3 *db;
	char sql[1024];
	char *errmsg;
	sprintf(sql, "select * from %s where %s glob '%s';", DB_NAME, LoginTableEn[2], sqlrequest.LoginName.data());
	if(sqlite3_open(TEST_DB_PATH, &db) < 0)
	{
		return SQLITE_OPER_ERR;
	}
	else
	{
		if(sqlite3_exec(db, sql, callback, NULL, &errmsg) != SQLITE_OK)
		{
			sqlite3_close(db);
			sqlite3_free(errmsg);
			//cout << errmsg << endl;
			return SELECT_DATA_ERR;
		}
		else
		{
			sqlite3_close(db);
		}
		if(!UserExsit)
		{
			return LOGIN_USER_NOT_EXSIT;
		}
		if(strcmp(User_info.UserPass, sqlrequest.LoginPass.data()))
		{
			return LOGIN_USER_PASS_ERR;
		}
	}
	return OPER_CURRECT;
}
unsigned char RegisterNew(LoginInfo sqlrequest)
{
	sqlite3 *db;
	char sql[1024];
	char *errmsg;
	sprintf(sql, "select * from %s where %s glob '%s';", DB_NAME, LoginTableEn[2], sqlrequest.LoginName.data());
	if(sqlite3_open(TEST_DB_PATH, &db) < 0)
	{
		return SQLITE_OPER_ERR;
	}
	else
	{
		if(sqlite3_exec(db, sql, callback, NULL, &errmsg) != SQLITE_OK)
		{
			sqlite3_close(db);
			sqlite3_free(errmsg);
			//cout << errmsg << endl;
			return SELECT_DATA_ERR;
		}
		if(UserExsit)
		{
			return REGISTER_EXSIT_USER_ERR;
		}
		memset(sql, 0, sizeof(sql));
		sprintf(sql, "insert into %s values('%s','%s');", DB_NAME, sqlrequest.LoginName.data(), sqlrequest.LoginPass.data());
		if(sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK)
		{
			sqlite3_close(db);
			return SQLITE_OPER_ERR;
		}
	}
	return OPER_CURRECT;
}
	
unsigned char GetOperMode(string opermode)
{
	if(opermode == LOGIN_OPER){
		return LOGIN_ON_AJAX;
	}else if(opermode == REGISTER_OPER){
		return REGISTER_AJAX;
	}else{
		return UNKNOWN_AJAX;
	}
}
int main(int argc, char **argv)
{
	Cgicc cgi;
	const CgiEnvironment& env = cgi.getEnvironment();
	cout << HTTPContentHeader("text/plain;charset=utf-8");
	LoginInfo sqlrequest;
	string webMethod;
	string requestStr;
	unsigned char ok = 0xFF;
	unsigned char dOperation;
#if !TEST_MODE
	webMethod = env.getRequestMethod();
	if(webMethod == "GET")
	{
		cout << "not use this method";
	}
	else if(webMethod == "POST")
#endif
	{
#if TEST_MODE
		sqlrequest.OperMode = LOGIN_OPER;
		sqlrequest.LoginName = "admin";
		sqlrequest.LoginPass = "astralrovers";
#else
		requestStr = env.getPostData();
#endif
		Json::Reader reader;
		Json::Value rootreq;
		Json::Value rootres;
		Json::StyledWriter styled_writer;
#if !TEST_MODE
		if(!reader.parse(requestStr, rootreq))
		{
			cout << POST_JSON_DATA_ERR;
		}
		sqlrequest.OperMode = rootreq["OperMode"].asString();
		sqlrequest.LoginName = rootreq["LoginName"].asString();
		sqlrequest.LoginPass = rootreq["LoginPass"].asString();
#endif
		dOperation = GetOperMode(sqlrequest.OperMode);
		rootreq["OperMode"] = dOperation;
		switch(dOperation)
		{
			case LOGIN_ON_AJAX:
				ok = LoginOnCheck(sqlrequest);
				break;
			case REGISTER_AJAX:
				ok = RegisterNew(sqlrequest);
				break;
			default:
				ok = UNKNOWN_ERR;
				break;
		}
		rootreq["AckCmd"] = ok;
		rootreq["LoginName"] = "";
		rootreq["LoginPass"] = "";
		//rootres.append(rootreq);
		cout << styled_writer.write(rootreq); 
	}
	return 0;
}
