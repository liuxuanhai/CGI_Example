/******************************************    *******************************
      > File Name: test.cpp
      > Author: Ywl
      > Descripsion:
      > Created Time:     Wed 06 Sep 2017 09:14:09 AM CST
      > Modify Time: 
 *********************************    ***************************************/
#include <json/json.h>
#include <fstream>
#include <iostream>
using namespace std;
int main()
{
	Json::Value root;
	Json::FastWriter writer;
	Json::Value person;
	person["name"] = "hello world1";
	person["age"] = 100;
	root.append(person);
	person["name"] = "hello world2";
	person["age"] = 200;
	root.append(person);
	string data= writer.write(root);
	cout<<data<<endl;   
	cout<<root.toStyledString()<<endl;
	return 0;
} 
