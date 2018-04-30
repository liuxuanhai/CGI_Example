/******************************************    *******************************
  > File Name: downloadfile.cpp
  > Author: Ywl
  > Descripsion:
  > Created Time:     Tue 10 Oct 2017 09:19:40 AM CST
  > Modify Time: 
 *********************************    ***************************************/


#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include <sqlite3.h>

#include <cgicc/CgiDefs.h> 
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h> 

#include <json/json.h>

using namespace std;
using namespace cgicc;

#define ROOT_WEBDIR			"/yu_yuan/webdata/"
#define MAX_FILE_LEN  (1024*30)
#define TEST	0

int main()
{
	Cgicc cgi;
	const CgiEnvironment& env = cgi.getEnvironment();
	char namebuf[512];
	string webMethod;
	string requestStr;
	string filename;
	string op;
	webMethod = env.getRequestMethod();
	if(webMethod == "GET")
	{
		//get post同样解析
		form_iterator fi = cgi.getElement("downLoadKey");  
		if( !fi->isEmpty() && fi != (*cgi).end()) { 
			//(*cgi).end() == cgi.getElements().end()
			//**fi == fi->getValue() == cgi("downLoadKey")
			if("Astralrovers" != **fi)
			{
				return 0;
			}
			fi = cgi.getElement("filename");  
			if( !fi->isEmpty() && fi != (*cgi).end()) {  
				filename = **fi;
				//FILE *fp;
				char filebuf[MAX_FILE_LEN];
				char cmd[65535];
				struct stat sb;

				sprintf(cmd, "%s%s", ROOT_WEBDIR, filename.data());
				stat(cmd, &sb); //取待下载文件的大小

				//输出HTTP头信息，输出附加下载文件、文件长度以及内容类型
				//printf("Content-Disposition:attachment;filename=%s\r\n", filename.data());
				cout << "Content-Disposition:attachment;filename=" << filename << "\r\n";
				//printf("Content-Length:%ld\r\n", sb.st_size);
				cout << "Content-Length:" << sb.st_size << "\r\n";
				//cout << "Content-Type:application/octet-stream\r\n\r\n";
				cout << HTTPContentHeader("application/octet-stream");
				sprintf(cmd, "%s%s", ROOT_WEBDIR, filename.data());
#if 0
				if(fp=fopen(cmd, "r+b")){  
					//成功打开文件，读取文件内容
					do{
						int rs = fread(filebuf, 1, sizeof(filebuf), fp);

						fwrite(filebuf, rs, 1, stdout);
					}while(!feof(fp));
					fclose(fp); 
				}
#else
				ifstream ifile(cmd, std::ios::binary);
				if(ifile.is_open())
				{
					while(!ifile.eof())
					{
//						ifile.read(filebuf, sizeof(filebuf));
						ifile >> filebuf;
						cout << filebuf;
					}
					ifile.close();
				}
#endif
			}
		}else{
			cout << HTTPContentHeader("text/plain;charset=utf-8");
			cout << "No text entered for first name" << endl;  
		}
	}
#if 1
	else if(webMethod == "POST")
	{
		cout << HTTPContentHeader("text/plain;charset=utf-8");
		requestStr = env.getPostData();
		Json::Reader reader;
		Json::Value rootreq;

		Json::Value rootres;
		Json::StyledWriter styled_writer;
		if(!reader.parse(requestStr, rootreq))
		{
			cout << "err";
			return 0;
		}
		op = rootreq["Op"].asString();
		filename = rootreq["FileName"].asString();
		ifstream ifile;
		sprintf(namebuf, "%s%s", ROOT_WEBDIR, filename.data());
		ifile.open(namebuf);
		if(ifile.is_open())
		{
			rootres["Op"] = "ok";
			rootres["FileName"] = filename.data();
			rootres["downLoadKey"] = "Astralrovers";
			cout << styled_writer.write(rootres); 
			ifile.close();
		}
		else
		{
			cout << "err";
			return 0;
		}
	}
#endif
	return 0;
}
