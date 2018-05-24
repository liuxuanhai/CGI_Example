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
from functools import wraps

#装饰类了解一下?

#带参数的装饰器
def arg_deco(text):
    def s_deco(fun):
        @wraps(fun)
        def wrapper(*args, **kargs):
            print("...", text)
            fun(*agrs, **kargs)
        return wrapper
    return s_deco


#可以和使用多个装饰器，从上到下的顺序
@arg_deco("hello")
def func2():
    print("func2")


def deco(fun):
    @wraps(fun)     #由于给fun重新赋值会导致原函数的原信息被改变，比如docstring、__name__
                    #wraps也是一个装饰器，作用是将原函数的信息拷贝给新的fun
    def wrapper(*args,**kwargs):
        startTime = time.time()
        print("function start")
        fun(*args,**kwargs)    #这里就执行原函数了
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

'''
加上@实际等于 
func = deco(func)
func()
使用@时候就可以省略函数赋值，增强可读性
'''


if __name__ == '__main__':
    f = func
    f(3,4)
    #func()
