/******************************************    *******************************
      > File Name: uploadfile.cpp
      > Author: Ywl
      > Descripsion:
      > Created Time:     Wed 23 Aug 2017 11:21:45 PM CST
      > Modify Time: 
 *********************************    ***************************************/
#include <iostream>
#include <vector>  
#include <string>  
#include <fstream>

#include <stdio.h>  
#include <stdlib.h> 

#include <cgicc/CgiDefs.h> 
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h>

using namespace std;
using namespace cgicc;

#define ROOT_WEBDIR			"/yu_yuan/webdata/"

int main()
{
    Cgicc cgi;
	//这行必须要加，否则无法识别输出内容
	cout << HTTPContentHeader("text/plain;charset=utf-8");

    // 获取要被上传的文件列表
	const_file_iterator file = cgi.getFile("userfile");
	ofstream ofile;
	string FileName = file->getFilename();
	char namebuf[512];
	sprintf(namebuf, "%s%s", ROOT_WEBDIR, FileName.data());
	ofile.open(namebuf, std::ios::binary);
    if(file != cgi.getFiles().end()) {
        // 在 cout 中发送数据类型
//        ofile << HTTPContentHeader(file->getDataType());
        // 在 cout 中写入内容
        file->writeToStream(ofile);
	}
	ofile.close();
	cout << "ok";
    return 0;
}
