#!/usr/bin/python
# coding=utf-8

"""
#########################################################################################

      > File Name: bubblesort.py

      > Author: Ywl

      > Descripsion:

      > Created Time:     Tue 24 Apr 2018 03:59:54 AM PDT

      > Modify Time: 

#########################################################################################
"""
def bubblesort(be_sort_list):
    for i in range(len(be_sort_list)):
        for j in range(len(be_sort_list)-i-1):
            if be_sort_list[j] > be_sort_list[j+1]: #如果前一个比后一个大或者小就交换,始终从第一个开始比较
                temp = be_sort_list[j]
                be_sort_list[j] = be_sort_list[j+1]
                be_sort_list[j+1] = temp
    return be_sort_list

print(bubblesort([2,6,8,3,2,4]))
