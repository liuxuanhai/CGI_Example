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

def binSearch(sLs, get):
    head = 0                        #list head
    tail = len(sLs)-1               #list tail
    while head < tail:              #do not over
        mid = int((head+tail)/2)
        if sLs[mid] == get:
            return mid
        elif sLs[mid] > get:
            tail = mid-1            #prev
        elif sLs[mid] < get:
            head = mid+1            #next
    return None
if __name__ == "__main__":
    print(binSearch(ls, 9))
