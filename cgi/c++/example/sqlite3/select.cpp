/******************************************    *******************************
      > File Name: select.cpp
      > Author: Ywl
      > Descripsion:
      > Created Time:     Thu 31 Aug 2017 12:01:53 PM CST
      > Modify Time: 
 *********************************    ***************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
/*记录回调函数的次数，在查询时查到一条便调一次函数，argc表示表的列数，argv表示查询到的数据指针
  （指向一条完整的数据，通过偏移指向下一列，使用数组方式取出当前列的数据）*/
static int roll = 0;
static int callback(void *data, int argc, char **argv, char **azColName){
	int i;
	fprintf(stderr, "%s: ", (const char*)data);
	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("roll = %d\n", roll++);
	return 0;
}

int main(int argc, char* argv[])
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char *sql;
	const char* data = "Callback function called";

	/* Open database */
	rc = sqlite3_open("test.db", &db);
	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stderr, "Opened database successfully\n");
	}

	/* Create SQL statement */
	sql = "SELECT * from COMPANY";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}
	sqlite3_close(db);
	return 0;
}
