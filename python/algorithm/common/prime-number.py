#!/usr/bin/python3
# coding=utf-8

"""
#########################################################################################

      > File Name: prime-number.py

      > Author: Ywl

      > Descripsion:

      > Created Time:     Sun 11 Mar 2018 06:49:28 PM PDT

      > Modify Time: 

#########################################################################################
"""
import math

def check_prime_number(checked_number):
    if checked_number < 2:
        return False
    sqrt_number = int(math.sqrt(checked_number))
    for index in range(2, sqrt_number+1):
        if (checked_number % index) == 0:   #余数为0，则不是质数
            return False
    return True

def print_prime_number(max_number):
    for index in range(0, max_number+1):
        if check_prime_number(index):
            print('%d, ' % index, end='');
    print("")

if __name__ == "__main__":
    print_prime_number(100)
