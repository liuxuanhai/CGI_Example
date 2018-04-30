/******************************************    *******************************
      > File Name: toutf8.cpp
      > Author: Ywl
      > Descripsion:
      > Created Time:     Fri 01 Sep 2017 02:34:48 PM CST
      > Modify Time: 
 *********************************    ***************************************/
#include <iostream>
#include <string>
#include<vector>
using namespace std;

std::string ofDewarServer::string_To_UTF8(const std::string & str)  
{  
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);  
	  
	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴  
	ZeroMemory(pwBuf, nwLen * 2 + 2);  
	  
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);  
	  
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);  
	  
	char * pBuf = new char[nLen + 1];  
	ZeroMemory(pBuf, nLen + 1);  
	  
	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);  
	  
	std::string retStr(pBuf);  
	  
	delete []pwBuf;  
	delete []pBuf;  
	  
	pwBuf = NULL;  
	pBuf = NULL;  
	  
	return retStr;  
}

int main()
{
	string srcstring = "name=%E4%BD%99%E7%8E%8B%E4%BA%AE&gender=%E7%94%B7";
	cout << ofDewarServer::string_To_UTF8(srcstring) << endl;
	return 0;
}
