#!/usr/bin/python
# coding=utf-8

"""
#########################################################################################

      > File Name: quicksort.py

      > Author: Ywl

      > Descripsion:

      > Created Time:     Tue 24 Apr 2018 03:27:29 AM PDT

      > Modify Time: 

#########################################################################################
"""
# 基本思想是将数组分为比基准值大和小的两组，然后递归，直到只剩下一个元素时返回 */
def quicksort(be_sort_list):
    if len(be_sort_list) < 2:
        return be_sort_list     #基线条件， 即只有一个元素时不需要排序，直接返回
    else:
        pivot = be_sort_list[0]     #基准值,也没必要每次都将基准值取第一个
        less = [i for i in be_sort_list[1:] if i <= pivot]       #所有比基准值小的
        greater = [i for i in be_sort_list[1:] if i > pivot]    #所有比基准值大的
        return quicksort(less) + [pivot] + quicksort(greater)

if __name__ == "__main__":
    print(quicksort([5,3,7,1,9,2,10]))
