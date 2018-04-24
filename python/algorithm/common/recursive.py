#!/usr/bin/python
# coding=utf-8

"""
#########################################################################################

      > File Name: recursive.py

      > Author: Ywl

      > Descripsion:

      > Created Time:     Tue 24 Apr 2018 01:45:22 AM PDT

      > Modify Time: 

#########################################################################################
"""
def countdown(num):
    if num <= 1:
        return 1            #基线条件，即什么时候不调用自己
    else:
        return num + countdown(num-1)   #递归条件，即什么时候调用自己
    #return 是出栈，调用是入栈

if __name__ == '__main__':
    print(countdown(5))
