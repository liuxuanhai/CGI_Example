#!/usr/bin/python
# coding=utf-8

"""
#########################################################################################

      > File Name: binary-search.py

      > Author: Ywl

      > Descripsion: Only applicable to ordered sequences

      > Created Time:     Mon 12 Mar 2018 06:34:45 PM PDT

      > Modify Time: 

#########################################################################################
"""
ls = [3, 6, 8, 9, 10, 15, 20]

def binSearch(be_sorted_list, key):
    head = 0                    
    tail = len(be_sorted_list)-1        
    while head < tail:          
        mid = int((head+tail) / 2)
        if be_sorted_list[mid] == key:
            return mid
        elif be_sorted_list[mid] > key:
            tail = mid - 1          
        elif be_sorted_list[mid] < key:
            head = mid + 1          
if __name__ == "__main__":
    binSearch(ls, 9)
