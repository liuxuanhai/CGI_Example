#!/usr/bin/python
# coding=utf-8
import os
path = os.listdir(".")
print path
#数组遍历
for p in path:
	if os.path.isdir(p):
		print p
	else:
		print p
