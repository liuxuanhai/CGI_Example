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

#define TEST_MODE	1

//操作编号
#define ACC_SELECT_AJAX		0	//精确查找
#define BLU_SELECT_AJAX		1	//模糊查找
#define INSERT_AJAX			2	//新增信息
#define ACC_DELETE_AJAX		3	//精确删除
#define BLU_DELETE_AJAX		4	//模糊删除
#define ADD_COLUMN_AJAX		5	//增加属性
#define ACC_MODIFY_AJAX		6	//精确修改
#define BLU_MODIFY_AJAX		7	//模糊修改
#define UNKNOWN_AJAX		0xFF	//最大
//错误标识
#define OPER_CURRECT		0	//操作正确
#define SQLITE_OPER_ERR		1	//数据库操作出错
#define SELECT_DATA_ERR		2	//查询错误
#define SELECT_DATA_NULL	3	//未查询到
#define INSERT_DATA_ERR		4	//新增信息错误
#define INSERT_HAVE_IDCARD	5	//存在相同身份证号
#define INSERT_HAVE_PHONE	6	//存在相同电话号码
#define INSERT_HAVE_EMAIL	7	//存在相同电子邮箱
#define UNKNOWN_ERR			0xFF	//未知错误

#define TEST_DB_PATH "../sourcefile/sqlite3db/person_info.db"
#define DB_NAME		"person_info"
#define TABLE_END	"TableEnd"
#define ACC_SELECT	"精确查找"
#define BLU_SELECT	"模糊查找"
#define INSERT_DATA	"新增信息"
#define ACC_DELETE	"精确删除"
#define BLU_DELETE	"模糊删除"
#define ADD_COLUMN	"增加属性"
#define ACC_MODIFY	"精确修改"
#define BLU_MODIFY	"模糊修改"
#define _UNKNOWN_	"未知命名"
#define NOT_SELECT	"查无此人"
#define POST_JSON_DATA_ERR "post_json_data_err"

/*
 切忌使用换行符，否则在ajax得到返回值时前后会多出换行符
 */
struct persion_info{
	string Opermode;
	string Numbering;
	string Name;
	string Age;
	string Gender;
	string Idcard;
	string Phonenumber;
	string Birthday;
	string Homeaddr;
	string Email;
};
typedef struct {
	char Opermode[30];
	char Numbering[64];
	char Name[60];
	char Age[4];
	char Gender[6];
	char Idcard[25];
	char Phonenumber[16];
	char Birthday[16];
	char Homeaddr[1024];
	char Email[60];
}PERSION_INFO_C;
static const char *AttriTableEn[] = {
	"AckCmd",
	"Opermode",
	"Numbering",
	"Name",
	"Age",
	"Gender",
	"Idcard",
	"Phonenumber",
	"Birthday",
	"Homeaddr",
	"Email",
	TABLE_END
};

static const char *AttriTable[] = {
	"应答标志",
	"命令模式",
	"序号",
	"姓名",
	"年龄",
	"性别",
	"身份证号",
	"手机号码",
	"出生日期",
	"家庭住址",
	"电子邮箱",
	TABLE_END
};
typedef struct{
	unsigned int tatol;
	char numbering[1024][64];
}SAVE_SEARCH;
static unsigned int result = 0 ;
static unsigned int pos = 0;
static PERSION_INFO_C selRes[1024];
static SAVE_SEARCH save_search_info;
static int callback_select(void *data, int argc, char **argv, char **azColName)
{
	if(argc < 8)
		return -1;
	unsigned int i = 0;
	for(; i < save_search_info.tatol; i++)
	{
		if(!strcmp(save_search_info.numbering[i], argv[0]))
		{
			return 0;
		}
	}
	strcpy(save_search_info.numbering[i], argv[0]);
	save_search_info.tatol++;
	strcpy(selRes[result].Numbering,  argv[0]);
	strcpy(selRes[result].Name,  argv[1]);
	strcpy(selRes[result].Age,  argv[2]);
	strcpy(selRes[result].Gender,  argv[3]);
	strcpy(selRes[result].Idcard,  argv[4]);
	strcpy(selRes[result].Phonenumber,  argv[5]);
	strcpy(selRes[result].Birthday,  argv[6]);
	strcpy(selRes[result].Homeaddr,  argv[7]);
	strcpy(selRes[result].Email,  argv[8]);
	result++;
	return 0;
}
static char getinsertcolumn = 0;
static int callback_insert(void *data, int argc, char **argv, char **azColName)
{
	getinsertcolumn++;
	return 0;
}
unsigned char selectDataMib(unsigned char selectmode, string request, string attri, string condition)
{
	sqlite3 *db;
	char sql[1024];
	if(selectmode == ACC_SELECT_AJAX)
	{
		sprintf(sql, "select %s from %s where %s glob '%s';", request.data(), DB_NAME, attri.data(), condition.data());
	}
	else if(selectmode == BLU_SELECT_AJAX)
	{
		sprintf(sql, "select %s from %s where %s like '%%%s%%';", request.data(), DB_NAME, attri.data(), condition.data());
	}
	else
	{
		return UNKNOWN_AJAX;
	}
	if(sqlite3_open(TEST_DB_PATH, &db) < 0)
	{
		return SQLITE_OPER_ERR;
	}
	else
	{
		if(sqlite3_exec(db, sql, callback_select, NULL, NULL) != SQLITE_OK)
		{
			sqlite3_close(db);
			return SELECT_DATA_ERR;
		}
		else
		{
			sqlite3_close(db);
		}
	}
	return OPER_CURRECT;
}
unsigned char GetOperMode(string opermode)
{
	if(opermode == ACC_SELECT){
		return ACC_SELECT_AJAX;
	}else if(opermode == BLU_SELECT){
		return BLU_SELECT_AJAX;
	}else if(opermode == INSERT_DATA){
		return INSERT_AJAX;
	}else if(opermode == ACC_DELETE){
		return ACC_DELETE_AJAX;
	}else if(opermode == BLU_DELETE){
		return BLU_DELETE_AJAX;
	}else if(opermode == ADD_COLUMN){
		return ADD_COLUMN_AJAX;
	}else if(opermode == ACC_MODIFY){
		return ACC_MODIFY_AJAX;
	}else if(opermode == BLU_MODIFY){
		return BLU_MODIFY_AJAX;
	}else{
		return UNKNOWN_AJAX;
	}
}
unsigned char ReqRes(unsigned char selectmode, const char *atrr, string condition)
{
	unsigned char ok = 0;
	if((ok = selectDataMib(selectmode, "*", atrr, condition)) != OPER_CURRECT)
	{
		return ok;
	}
	if(result == 0)
	{
		return SELECT_DATA_NULL;
	}
	else
	{
		return OPER_CURRECT;
	}

}
unsigned char SelectInsert(unsigned char selectmode, string condition)
{
	char i = 0;
	char j = 0;
	unsigned char ok = 0;
	Json::Value rootres;
	Json::Value perres;
	Json::StyledWriter styled_writer;
	for(i = 3; strcmp(AttriTable[i], TABLE_END); i++)
	{
		if(selectmode == ACC_SELECT_AJAX)
		{
			if((i == 3) || (i == 6))
			{
				ok = ReqRes(selectmode, AttriTable[i], condition);
				//操作数据库错误
				if((ok == SQLITE_OPER_ERR) || (ok == SELECT_DATA_ERR))
				{
					return ok;
				}//未查询到
				else if(ok == SELECT_DATA_NULL)
				{
					continue;
				}
				else
				{
					for(j = 0; j < result; j++)
					{
						//此处不能使用追加，必须赋值
						perres[AttriTableEn[0]] = OPER_CURRECT;
						perres[AttriTableEn[1]] = ACC_SELECT_AJAX;
						perres[AttriTableEn[2]] = selRes[j].Numbering;
						perres[AttriTableEn[3]] = selRes[j].Name;
						perres[AttriTableEn[4]] = selRes[j].Age;
						perres[AttriTableEn[5]] = selRes[j].Gender;
						perres[AttriTableEn[6]] = selRes[j].Idcard;
						perres[AttriTableEn[7]] = selRes[j].Phonenumber;
						perres[AttriTableEn[8]] = selRes[j].Birthday;
						perres[AttriTableEn[9]] = selRes[j].Homeaddr;
						perres[AttriTableEn[10]] = selRes[j].Email;
						//append是追加，连成数组
						rootres.append(perres);
					}
					cout << styled_writer.write(rootres); 
					return OPER_CURRECT;
				}
			}
		}
		else if(selectmode == BLU_SELECT_AJAX)
		{
			ok = ReqRes(selectmode, AttriTable[i], condition);
			//操作数据库错误
			if((ok == SQLITE_OPER_ERR) || (ok == SELECT_DATA_ERR))
			{
				return ok;
			}//未查询到
			else if(ok == SELECT_DATA_NULL)
			{
				continue;
			}
			else
			{
				for(j = pos; j < result; j++)
				{
					//此处不能使用追加，必须赋值
					perres[AttriTableEn[0]] = OPER_CURRECT;
					perres[AttriTableEn[1]] = BLU_SELECT_AJAX;
					perres[AttriTableEn[2]] = selRes[j].Numbering;
					perres[AttriTableEn[3]] = selRes[j].Name;
					perres[AttriTableEn[4]] = selRes[j].Age;
					perres[AttriTableEn[5]] = selRes[j].Gender;
					perres[AttriTableEn[6]] = selRes[j].Idcard;
					perres[AttriTableEn[7]] = selRes[j].Phonenumber;
					perres[AttriTableEn[8]] = selRes[j].Birthday;
					perres[AttriTableEn[9]] = selRes[j].Homeaddr;
					perres[AttriTableEn[10]] = selRes[j].Email;
					//append是追加，连成数组
					rootres.append(perres);
				}
				pos = result;
			}
		}
		else
		{
			return UNKNOWN_AJAX;
		}
	}
	if(result == 0)
	{
		return SELECT_DATA_NULL;
	}
	cout << styled_writer.write(rootres); 
	return OPER_CURRECT;
}
unsigned char InsertColumn(persion_info sqlrequest)
{
	char i = 0;
	sqlite3 *db;
	char sqlcount[256];
	const char *threeNotRep[3];
	const char *threeNotRepAttr[3] = {"身份证号", "手机号码", "电子邮箱"};
	char **dbresult;
	char *errmsg;
	int nrom;
	int ncol;
	int rc;	
	threeNotRep[0] = sqlrequest.Idcard.data();
	threeNotRep[1] = sqlrequest.Phonenumber.data();
	threeNotRep[2] = sqlrequest.Email.data();
	if(sqlite3_open(TEST_DB_PATH, &db) < 0)
	{
		return SQLITE_OPER_ERR;
	}
	else
	{
		for(i = 0; i < 3; i++)
		{
			memset(sqlcount, 0, sizeof(sqlcount));
			sprintf(sqlcount, "select * from %s where %s like '%s';", DB_NAME, threeNotRepAttr[i], threeNotRep[i]);
			if(sqlite3_exec(db, sqlcount, callback_insert, NULL, NULL) != SQLITE_OK)
			{
				sqlite3_close(db);
				cout << "exec err" << endl;
				return SQLITE_OPER_ERR;
			}
			//存在相同电话号码
			if(getinsertcolumn)
			{
				sqlite3_close(db);
				switch(i)
				{
					case 0 :
						return INSERT_HAVE_IDCARD;
					case 1 :
						return INSERT_HAVE_PHONE;
					case 2 :
						return INSERT_HAVE_EMAIL;
					default:
						return UNKNOWN_ERR; 
						break;
				}
			}
		}

		memset(sqlcount, 0, sizeof(sqlcount));
		sprintf(sqlcount, "select count(*) from %s;", DB_NAME);
		rc = sqlite3_get_table(db, sqlcount, &dbresult, &nrom, &ncol, NULL);
		if(rc != SQLITE_OK)
		{
			sqlite3_free_table(dbresult);
			sqlite3_close(db);
			return SQLITE_OPER_ERR;
		}
		sqlite3_free_table(dbresult);
		char *sql;
		sql = (char *)malloc(4096);
		memset(sql, 0, 4096);
		sprintf(sql, "insert into %s (%s,%s,%s,%s,%s,%s,%s,%s,%s) values(%d,'%s',%d,'%s','%s','%s','%s','%s','%s');",\
				DB_NAME, AttriTable[2],AttriTable[3],AttriTable[4],AttriTable[5],\
				AttriTable[6],AttriTable[7],AttriTable[8],AttriTable[9],AttriTable[10],\
				atoi(dbresult[1])+1,\
				sqlrequest.Name.data(),\
				atoi((sqlrequest.Age).data()),\
				sqlrequest.Gender.data(),\
				sqlrequest.Idcard.data(),\
				sqlrequest.Phonenumber.data(),\
				sqlrequest.Birthday.data(),\
				sqlrequest.Homeaddr.data(),\
				sqlrequest.Email.data());
		if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
		{
			sqlite3_close(db);
			free(sql);
			sqlite3_free(errmsg);
			return INSERT_DATA_ERR;
		}
		else
		{
			sqlite3_close(db);
		}
		free(sql);
	}
	return OPER_CURRECT;
}
int main(int argc, char **argv)
{
	Cgicc cgi;
	const CgiEnvironment& env = cgi.getEnvironment();
	cout << HTTPContentHeader("text/plain;charset=utf-8");
	persion_info sqlrequest;
	string webMethod;
	string requestStr;
	unsigned char ok = 0xFF;
	unsigned char dOperation;
	save_search_info.tatol = 0;
#if !TEST_MODE
	webMethod = env.getRequestMethod();
	if(webMethod == "GET")
	{
		cout << "not use this method";
	}
	else if(webMethod == "POST")
	{
		requestStr = env.getPostData();
#else
	{
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
#endif
#if TEST_MODE
		rootreq["AckCmd"] = 0xFF;
		rootreq["Opermode"] = "新增信息";
		rootreq["Name"] = "余";
		rootreq["Age"] = "22";
		rootreq["Gender"] = "女";
		rootreq["Idcard"] = "511623";
		rootreq["Phonenumber"] = "138";
		rootreq["Birthday"] = "1994";
		rootreq["Homeaddr"] = "浙江";
		rootreq["Email"] = "qqq@qq.com";
#endif
		sqlrequest.Opermode = rootreq["Opermode"].asString();
		dOperation = GetOperMode(sqlrequest.Opermode);
		rootreq["Opermode"] = dOperation;
		switch(dOperation)
		{
			case ACC_SELECT_AJAX :
			case BLU_SELECT_AJAX :
				sqlrequest.Name = rootreq["Name"].asString();
				ok = SelectInsert(dOperation, sqlrequest.Name);
				if(ok == OPER_CURRECT)
				{
					return 0;
				}
				break;
			case INSERT_AJAX :
				sqlrequest.Name = rootreq["Name"].asString();
				sqlrequest.Age = rootreq["Age"].asString();
				sqlrequest.Gender = rootreq["Gender"].asString();
				sqlrequest.Idcard = rootreq["Idcard"].asString();
				sqlrequest.Phonenumber = rootreq["Phonenumber"].asString();
				sqlrequest.Birthday = rootreq["Birthday"].asString();
				sqlrequest.Homeaddr = rootreq["Homeaddr"].asString();
				sqlrequest.Email = rootreq["Email"].asString();
				ok = InsertColumn(sqlrequest);
				break;
			case ACC_DELETE_AJAX :
			case BLU_DELETE_AJAX :
				break;
			case ACC_MODIFY_AJAX :
			case BLU_MODIFY_AJAX :
				break;
			case ADD_COLUMN_AJAX :
				break;
			default:
				ok = UNKNOWN_ERR;
				break;
		}
		rootreq["AckCmd"] = ok;
		rootres.append(rootreq);
		cout << styled_writer.write(rootres); 
	}
	return 0;
}
