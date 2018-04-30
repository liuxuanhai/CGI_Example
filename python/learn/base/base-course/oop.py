#!/usr/bin/python
# coding=utf-8

"""
#########################################################################################

      > File Name: oop.py

      > Author: Ywl

      > Descripsion:

      > Created Time:     Thu 22 Mar 2018 01:51:37 AM PDT

      > Modify Time: 

#########################################################################################
"""
from types import MethodType

class A():
    commuty = 'class'       #类属性,所有实例公有
    def __init__(self, a=22, b=33):
        self.a = a          #public 成员变量
        self._b = b         #protect
        self.__c = a + b    #private
    def Aprint():
        print('class A')

    def getA(a):        #成员方法
        self.a = a
    def setComm(self, f):
        A.commuty = f
    #普通方法
    def setCommon():
        #无法访问成员变量和类变量
        print('setComon')
    #类方法,同成员方法
    #@classmethod
    #静态,差不多同普通方法
    #@staticmethod

class B(A):
    def __init__(self, a=98):
        #A.__init__(self)初始化父类
        super(B, self).__init__(a)#新式初始化全部父类
        print('class B')

oop1 = A()
print(getattr(oop1, 'a'))
oop1.commuty = 3
oop2 = A()
print('oop1.commuty = 3' + ' -'*5)
print(oop2.commuty)
print(A().commuty)
A.commuty = 'class A'#此处修改类属性，所有实例访问都会改变
#A().commuty = 'class A'#这条语句与上面一条的区别
print('A.commuty = \'class A\'' + ' -'*5)
print(oop2.commuty)
print(A().commuty)
A().commuty = 'class C'#其实是 swap = A();swap.commuty = 'class C'
print('A().commuty = \'class C\'' + ' -'*5)
print(oop2.commuty)
print(A().commuty)
oop1.setComm('class D') #此处修改类属性，所有实例访问都会改变
print('oop1.setComm(\'class D\') ' + ' -'*5)
print(oop2.commuty)
print(A().commuty)
print(A().a)

oop3 = B()
print('oop3.a', oop3.a)
print(oop3.commuty)


print('--------------------------------------------------------------------------------------------------------------------\n\n\n')

class C(object):
    #为嘛会报错
    #__slots__ = ('name, age')       #此时给对象或者类添加属性，会被限制在这两个属性之间
    pass

oop4 = C()
oop5 = C()
C.name = 'ywl'      #给类添加属性，所有实例有效
oop4.age = 15       #实例有效
print(oop4.name + ' -- ' + oop5.name)
print(oop4.age)
print(hasattr(oop5, 'age'))

def setSex(self, sex):
    self.sex = sex
C.setSex = MethodType(setSex, C)    #给对象添加函数
oop4.setSex('women')
print(oop4.sex)

