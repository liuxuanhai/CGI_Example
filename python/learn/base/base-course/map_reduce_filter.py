#!/usr/bin/python
# coding=utf-8

"""
#########################################################################################

      > File Name: map_reduce_filter.py

      > Author: Ywl

      > Descripsion:

      > Created Time:     Wed 21 Mar 2018 11:41:59 PM PDT

      > Modify Time: 

#########################################################################################
"""
from functools import reduce

def my2X(a):
    return a*a
def myStr2Int(a, b):
    return a*10 + b
def myOver10(a):
    return a > 10

print(list(map(my2X, list(range(10))))) #map是遍历每个元素然后返回一个元素
print(reduce(myStr2Int, list(range(10)))) #reduce是对迭代对象进行累计操作
print(list(filter(myOver10, list(range(100))))) #对每个元素进行过滤
def is_p(a):
    for i in range(len(str(a))):
        if str(a)[i] != str(a)[len(str(a)) - i - 1]:
            return
    return a

print(list(filter(is_p, list(range(10, 100000)))))
