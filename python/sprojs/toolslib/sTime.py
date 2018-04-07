#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
      > File Name: sTime.py
      > Project Name: toolslib
      > Author: Ywl
      > Email: astralrovers@outlook.com
      > Descripsion:
      > Site:   
      > Software:   PyCharm Community Edition
      > Created Time:     2018/4/6 10:50 
      > Modify Time:
"""
import time
#一小时
def OS_HOUR(s_time):
    return s_time * 60 * 60 * 1000 * 1000
#一分钟
def OS_MIN(s_time):
    return s_time * 60 * 1000 * 1000
#一秒
def OS_SEC(s_time):
    return s_time * 1000 * 1000
#一毫秒
def OS_MS(s_time):
    return s_time * 1000
#一微秒
def OS_US(s_time):
    return s_time
#系统时间，毫秒
def getOSTime():
    return time.time() * 1000000
#微秒级延时
def OSTimeDelay(s_time):
    time.sleep(s_time / 1000 / 1000)

def getStrOSTime(format='%Y %B %d %A %H:%M:%S'):
    return time.strftime(format, time.localtime())

def printOSTime():
    print(time.strftime("%Y %B %d %A %H:%M:%S", time.localtime()))

if __name__ == '__main__':
    printOSTime()
    print(getStrOSTime())
    start = time.clock()
    OSTimeDelay(OS_US(1))
    end = time.clock()
    print("一微秒 %f" % (end - start))
    OSTimeDelay(OS_SEC(1))
    print("一秒")