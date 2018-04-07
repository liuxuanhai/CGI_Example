#!/usr/bin/python
# coding=utf-8

"""
#########################################################################################

      > File Name: decorator.py

      > Author: Ywl

      > Descripsion:

      > Created Time:     Thu 22 Mar 2018 12:37:13 AM PDT

      > Modify Time: 

#########################################################################################
"""
import time

def deco(func):
    def wrapper(*args,**kwargs):
        startTime = time.time()
        print("function start")
        func(*args,**kwargs)    #这里就执行原函数了
        endTime = time.time()
        msecs = (endTime - startTime)*1000
        print("time is %d ms" % msecs)
    return wrapper


#装饰器，扩展原函数功能，在函数之前添加一个装饰器，那么执行原函数相当于执行新函数，即加上装饰期之后的
@deco
def func(a,b):
    print("hello，here is a func for add :")
    time.sleep(1)
    print("result is %d" %(a+b))

if __name__ == '__main__':
    f = func
    f(3,4)
    #func()
