#!/usr/bin/python
# coding=utf-8

"""
#########################################################################################

      > File Name: select_sort.py

      > Author: Ywl

      > Descripsion:

      > Created Time:     Mon 23 Apr 2018 02:32:43 AM PDT

      > Modify Time: 

#########################################################################################
"""
def find_smallest_index(be_sort_list):
    smallest = be_sort_list[0]
    smallest_index = 0
    for index in range(1, len(be_sort_list)):
        if smallest > be_sort_list[index]:
            smallest = be_sort_list[index]
            smallest_index = index

    return smallest_index

def select_sort(be_sort_list):
    sort_list = []
    for index in range(len(be_sort_list)):
        smallest_index = find_smallest_index(be_sort_list)
        sort_list.append(be_sort_list.pop(smallest_index))
    return sort_list

if __name__ == '__main__':
    print(select_sort([5, 3, 6, 2, 10]))
