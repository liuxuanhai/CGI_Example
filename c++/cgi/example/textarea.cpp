/******************************************    *******************************
      > File Name: textarea.cpp
      > Author: Ywl
      > Descripsion:
      > Created Time:     Wed 23 Aug 2017 11:05:40 PM CST
      > Modify Time: 
 *********************************    ***************************************/
#include <iostream>
#include <vector>  
#include <string>  
#include <stdio.h>  
#include <stdlib.h> 
 
#include <cgicc/CgiDefs.h> 
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h> 

using namespace std;
using namespace cgicc;

int main ()
{
    Cgicc formData;

    cout << "Content-type:text/html\r\n\r\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<meta charset=\"utf-8\">\n";
    cout << "<title>向 CGI 程序传递文本区域数据</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";

    form_iterator fi = formData.getElement("textcontent");  
    if( !fi->isEmpty() && fi != (*formData).end()) {  
        cout << "Text Content: " << **fi << endl;  
    }else{
        cout << "No text entered" << endl;  
    }

    cout << "<br/>\n";
    cout << "</body>\n";
    cout << "</html>\n";

    return 0;
}