#!/usr/bin/python3
# coding=utf-8

"""
#########################################################################################

      > File Name: myIo.py

      > Author: Ywl

      > Descripsion:

      > Created Time:     Mon 26 Mar 2018 08:26:12 PM CST

      > Modify Time: 

#########################################################################################
"""
from io import StringIO, BytesIO
#在内存中创建IO
f1 = StringIO()
f1.write('hello')
f1.seek(0, 0)
print(f1.read())

#2进制
f2 = BytesIO()
f2.write('鱼'.encode('utf-8'))
f2.seek(0, 0)
print(f2.read())
f1.close()
f2.close()


#变量序列化存储
import pickle
with open('dump.txt', 'wb') as f3:
    d = dict(name='yu', age=23)
    pickle.dump(d, f3)#存储到文件

#反序列化
with open('dump.txt', 'rb') as f3:
    d = pickle.load(f3)
    print(d)

import json
#json 序列化
d = dict(name='yu', age=23)
jsonstr = json.dumps(d)
print(jsonstr)
#反序列化
d1 = json.loads(jsonstr)
print(type(d1), d1)
#对象序列化
class Yuwa():
    def __init__(self, name, age, sex):
        self.name = name
        self.age = age
        self.sex = sex
def classtodict(s):
    return {
            'name' : s.name,
            'age' : s.age,
            'sex' : s.sex
    }
s = Yuwa('Yuan', 22, 'girl')
#jsonstr = json.dumps(s, default=classtodict)
jsonstr = json.dumps(s, default=lambda obj : obj.__dict__)
print(jsonstr)


#反序列化一个对象
def dicttoclass(d):
    return Yuwa(d['name'], d['age'], d['sex'])
print(json.loads(jsonstr, object_hook=dicttoclass))
