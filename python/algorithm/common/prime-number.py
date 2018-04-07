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

def checkPrimeNum(number):
    '''
    Determine if a number is a prime number
    '''
    if number < 2:
        return False
    index = 2
    sqrtNum = int(math.sqrt(number))
    while index <= sqrtNum:
        if (number % index) == 0:
            return False
        index += 1
    return True

def primeNumber(maxNum):
    '''
    Print prime numbers from 0 to maxNum
    '''
    index = 0
    while index <= maxNum:
        if checkPrimeNum(index):
            print('%d, ' % index, end='');
        index += 1
    print("")

if __name__ == "__main__":
    primeNumber(100)
