#!/usr/bin/python
# coding=utf-8
"""
标准数据类型
在内存中存储的数据可以有多种类型。
例如，一个人的年龄可以用数字来存储，他的名字可以用字符来存储。
Python 定义了一些标准类型，用于存储各种类型的数据。
Python有五个标准的数据类型：
Numbers（数字）
String（字符串）
List（列表）
Tuple（元组）
Dictionary（字典）
"""
#不同与C/C++的变量操作
#变量定义时不需要指定类型且必须初始化，根据初始化值确定类型
one = 321 #整型
two = 321.6 #浮点型
three = "hell" #字符串
#多个变量同赋值
a = b = c = 1
print "统一赋值：",a,b,c
#也可以为多个对象指定多个变量
a1, b1, c1 = 1, 3.2, "yu_yuan"
#可用del删除对象的引用
del a,a1
'''
Python支持四种不同的数字类型：
int（有符号整型）
long（长整型[也可以代表八进制和十六进制]）
float（浮点型）
complex（复数）

长整型也可以使用小写 l，但是还是建议您使用大写 L，避免与数字 1 混淆。Python使用 L 来显示长整型。
Python 还支持复数，复数由实数部分和虚数部分构成，可以用 a + bj,或者 complex(a,b) 表示， 
复数的实部 a 和虚部 b 都是浮点型。
'''

'''
如果你要实现从字符串中获取一段子字符串的话，可以使用变量 [头下标:尾下标]，
就可以截取相应的字符串，其中下标是从 0 开始算起，可以是正数或负数，下标可以为空表示取到头或尾。
加号（+）是字符串连接运算符，星号（*）是重复操作。
'''
str = "i love you!"
print "从1~5:",str[1:5]
print "第4个:",str[3]
print "第0个之后的:",str[0:]
print "输出两次:",str * 2
print "连接起来:",str + "TEST"

'''
Python列表
List（列表） 是 Python 中使用最频繁的数据类型。
列表可以完成大多数集合类的数据结构实现。它支持字符，数字，字符串甚至可以包含列表（即嵌套）。
列表用 [ ] 标识，是 python 最通用的复合数据类型。
列表中值的切割也可以用到变量 [头下标:尾下标] ，就可以截取相应的列表，从左到右索引默认 0 开始，从右到左索引默认 -1 开始，下标可以为空表示取到头或尾。
加号 + 是列表连接运算符，星号 * 是重复操作
'''

list = [ 'runoob', 786 , 2.23, 'john', 70.2 ]
tinylist = [123, 'john']
 
print list               # 输出完整列表
print list[0]            # 输出列表的第一个元素
print "可以当二维元组么：",list[0][1:]	#二维操作
print list[1:3]          # 输出第二个至第三个的元素 
print list[2:]           # 输出从第三个开始至列表末尾的所有元素
print tinylist * 2       # 输出列表两次
print list + tinylist    # 打印组合的列表

list[0] = 1234
print "值可以修改为任意类型：",list[0]

'''
Python元组
元组是另一个数据类型，类似于List（列表）。
元组用"()"标识。内部元素用逗号隔开。但是元组不能二次赋值，相当于只读列表。
'''
tuple = ( 'runoob', 786 , 2.23, 'john', 70.2 )
tinytuple = (123, 'john')
 
print tuple               # 输出完整元组
print tuple[0]            # 输出元组的第一个元素
print tuple[1:3]          # 输出第二个至第三个的元素 
print tuple[2:]           # 输出从第三个开始至列表末尾的所有元素
print tinytuple * 2       # 输出元组两次
print tuple + tinytuple   # 打印组合的元组

'''
Python 字典
字典(dictionary)是除列表以外python之中最灵活的内置数据结构类型。列表是有序的对象结合，
字典是无序的对象集合。
两者之间的区别在于：字典当中的元素是通过键来存取的，而不是通过偏移存取。
字典用"{ }"标识。字典由索引(key)和它对应的值value组成
'''
#类似于键值对
dict = {}
dict['one'] = "This is one"
dict[2] = "This is two"
 
tinydict = {'name': 'john','code':6734, 'dept': 'sales'}

print dict['one']          # 输出键为'one' 的值
print dict[2]              # 输出键为 2 的值
print tinydict             # 输出完整的字典
print tinydict.keys()      # 输出所有键
print tinydict.values()    # 输出所有值
#print tinydict+dict	字典不能加
