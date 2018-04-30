#!/usr/bin/python
# coding=utf-8

"""
#########################################################################################

      > File Name: sxml.py

      > Author: Ywl

      > Descripsion:

      > Created Time:     Wed 28 Mar 2018 08:17:49 PM CST

      > Modify Time: 

#########################################################################################
"""
from xml.parsers.expat import ParserCreate

class DefaultSaxHandler(object):
    def start_element(self, name, attrs):
        print('sax:start_element:%s, attrs:%s' % (name, attrs))

    def end_element(self, name):
        print('sax:end_element:%s' % name)
    def char_data(self, text):
        print('sax:char_data:%s' % text)

xml = r'''<?xml version="1.0"?>
<ol>
    <li><a href="/python">Python</a></li>
    <li><a href="/ruby">Ruby</a></li>
</ol>
'''

handler = DefaultSaxHandler()
parser = ParserCreate()
parser.StartElementHandler = handler.start_element  #节点开始名称以及属性
parser.CharacterDataHandler = handler.char_data     #节点内容，包含换行空格
parser.EndElementHandler = handler.end_element      #节点结尾
parser.Parse(xml)
