#!/usr/bin/python
# coding=utf-8

"""
#########################################################################################
      > File Name: listdir.py
      > Author: Ywl
      > Descripsion:
      > Created Time:     Mon 16 Oct 2017 04:14:55 PM CST
      > Modify Time: 
#########################################################################################
"""
import os

print "Content-type:text/html"
print
print """
<html>
<head>
<meta charset='utf-8'>
<title>文件列表</title>
</head>
<body>
"""

filelist = os.listdir("./")
listlen = len(filelist)
print "<ul>"
i = 0 
for index in filelist:
	if os.path.isdir(index):
		pass
	else:
		print "<li><span>%s</span></li>" % (index)
print "</ul>"
print """
</body>
</html>
"""
