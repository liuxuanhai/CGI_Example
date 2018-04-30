#!/usr/bin/python
# coding=utf-8

import os
import cgi,cgitb

print "Content-type:text/html;charset=utf-8"
print 
print """
<html>
<head>
<meta charset=utf-8>
<title>python测试</title>
</head>
<body>
<p>测试代码</p>
<h>环境变量：</h>
"""
print "<ul>"
for key in os.environ.keys():
	print "<li><span style='color:green'>%30s</span>:%s</li>" % (key, os.environ[key])
print "</ul>"
print """
</body>
</html>
"""
