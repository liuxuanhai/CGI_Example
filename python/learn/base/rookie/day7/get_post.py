#!/usr/bin/python
# coding=utf-8

import cgi,cgitb

#get post方法处理表单

#创建表单实例
form = cgi.FieldStorage()

#解析数据
site_name = form.getvalue('name')
site_url = form.getvalue('url')

print "Content-type:text/html"
print
print "<html>"
print "<head>"
print "<meta charset=\"utf-8\">"
print "<title>PYTHON CGI 测试实例</title>"
print "</head>"
print "<body>"
print "<h2>%s网站：%s</h2>" % (site_name, site_url)
print "</body>"
print "</html>"
