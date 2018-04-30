#!/usr/bin/python
# coding=utf-8

"""
#########################################################################################
      > File Name: glbtest.py
      > Author: Ywl
      > Descripsion:        测试globalmem虚拟字符设备驱动
      > Created Time:     2017年10月26日 星期四 09时13分48秒
      > Modify Time: 
#########################################################################################
"""

import os
import fcntl
import sys
'''
打开设备描述符
'''
glb_name = "/dev/globalmem0"

testfile = os.open(glb_name, os.O_RDWR)
if testfile < 0 :
    print "open globalmem driver failed"
else:
    try:
        '''读取内存文件'''
        fcntl.ioctl(testfile, 0x1, 0)
        os.lseek(testfile, 0, os.SEEK_SET)
        os.write(testfile, "这是第一次写入的内容")
        os.lseek(testfile, 0, os.SEEK_SET)
        strbuf = os.read(testfile, 40)
        print strbuf

        fcntl.ioctl(testfile, 0x1, 0)
        os.lseek(testfile, 0, os.SEEK_SET)
        os.write(testfile, "这是第二次写入的内容")
        os.lseek(testfile, 0, os.SEEK_SET)
        strbuf = os.read(testfile, 40)
        print strbuf
    except:
        os.close(testfile)
    else:
        os.close(testfile)

