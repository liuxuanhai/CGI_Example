/******************************************    *******************************
      > File Name: sqlitetest.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     Thu 31 Aug 2017 10:18:50 AM CST
      > Modify Time: 
 *********************************    ***************************************/

#include <stdio.h>
#include <sqlite3.h>
int main(int argc, char **argv)
{
	sqlite3 *db;
	int rc;
	rc = sqlite3_open("test.db", &db);
	if(rc < 0)
	{
		printf("sqlite3 open err\n");
	}
	else
	{
		printf("open ok\n");
	}
	sqlite3_close(db);
	return 0;
}
