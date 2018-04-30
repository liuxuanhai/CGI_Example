/******************************************    *******************************
      > File Name: createtable.cpp
      > Author: Ywl
      > Descripsion:
      > Created Time:     Thu 31 Aug 2017 11:06:11 AM CST
      > Modify Time: 
 *********************************    ***************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#if 0
int sqlite3_exec 
( 
 sqlite3* ppDb, /* An open database */ 
 const char sql, / SQL to be evaluated */ 
 int (callback)(void,int,char**,char**), /* Callback function */ 
 void , / 1st argument to callback */ 
 char *errmsg / Error msg written here */ 
 ); 
第1个参数不再说了，是sqlite3_open函数得到的指针。 
第2个参数constchar*sql是一条sql 语句，以\0结尾。 
第3个参数sqlite3_callback 是回调，sqlite3每查询到一条结果，都会去调用你提供的这个函数。 
第4个参数void*是你所提供的指针，你可以传递任何一个指针参数到这里，这个参数最终会传到回调函数里面，如果不需要传递指针给回调函数，可以填NULL。 
第5个参数char** errmsg 是错误信息。注意是指针的指针。sqlite3里面有很多固定的错误信息。执行sqlite3_exec 之后，执行失败时可以查阅这个指针。 
回调函数的格式：

int sqlite_callback( 
		void* pvoid, /* 由 sqlite3_exec() 的第四个参数传递而来 */ 
		int argc, /* 表的列数 */ 
		char** argv, /* 指向查询结果的指针数组, 可以由 sqlite3_column_text() 得到 */ 
		char** col /* 指向表头名的指针数组, 可以由 sqlite3_column_name() 得到 */ 
		); 
#endif
/*操作成功一次便执行一次*/
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
int main(int argc, char* argv[])
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	const char *sql;

	/* Open database */
	rc = sqlite3_open("test.db", &db);
	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stdout, "Opened database successfully\n");
	}

	/* Create SQL statement */
	sql = "CREATE TABLE COMPANY("  \
		   "ID INT PRIMARY KEY     NOT NULL," \
		   "NAME           TEXT    NOT NULL," \
		   "AGE            INT     NOT NULL," \
		   "ADDRESS        CHAR(50)," \
		   "SALARY         REAL );";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "Table created successfully\n");
	}
	sqlite3_close(db);
	return 0;
}
