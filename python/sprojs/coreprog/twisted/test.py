#!/usr/bin/python3
# coding=utf-8

"""
#########################################################################################

      > File Name: test.py

      > Author: Ywl

      > Descripsion:

      > Created Time:     Thu 17 May 2018 07:03:09 PM PDT

      > Modify Time: 

#########################################################################################
"""
from twisted.internet import reactor
import requests


def get_google():
    print(requests.get('https://www.baidu.com'))

def print_name(name):
    print("name", name);

reactor.callWhenRunning(print_name, "ywl")
reactor.callLater(1, get_google)

reactor.run()
