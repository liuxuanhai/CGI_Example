#!/usr/bin/python
# coding=utf-8

"""
#########################################################################################

      > File Name: enum.py

      > Author: Ywl

      > Descripsion:

      > Created Time:     Mon 19 Mar 2018 08:42:47 PM CST

      > Modify Time: 

#########################################################################################
"""
'''
枚举的一些用法
'''
from enum import Enum, unique
#不允许值重复则加上@unique
#@unique
class Color(Enum):
    red = 1
    black = 2
    green = 3
    blue = 1

#枚举的成员不能重复,但是值可以，不过第二个被视作第一个的别名，迭代器也会跳过第二个
"""
class Color(Enum):
    red = 1
    red = 2
    green = 3
    blue = 1

"""
print(Color(1))
for member in Color: #Color.__members__.items() 可以迭代全部
    print(member)
#枚举访问
#1 值访问成员
print(Color(2))
#名称访问值
print(Color['green'])
#通过成员，来获取它的名称和值
member = Color(3)
print(member.name)
print(member.value)

