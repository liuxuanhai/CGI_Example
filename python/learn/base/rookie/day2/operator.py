#!/usr/bin/python
# coding=utf-8

#一些区别于c/c++的运算符
'''
'//' 除法结果取整(浮点数相同),'**'幂操作
'''
a = 10
b = 20
c = 3.9
print "b // c = %f" % (b//c)
print "a**b = %ld" % (a**b)
#逻辑运算符有区别：and == && or == || not == !
a = 10
b = 20

if ( a and b ):
	print "1 - 变量 a 和 b 都为 true"
else:
	print "1 - 变量 a 和 b 有一个不为 true"

if ( a or b ):
	print "2 - 变量 a 和 b 都为 true，或其中一个变量为 true"
else:
	print "2 - 变量 a 和 b 都不为 true"

# 修改变量 a 的值
a = 0
if ( a and b ):
	print "3 - 变量 a 和 b 都为 true"
else:
	print "3 - 变量 a 和 b 有一个不为 true"

if ( a or b ):
	print "4 - 变量 a 和 b 都为 true，或其中一个变量为 true"
else:
	print "4 - 变量 a 和 b 都不为 true"

if not( a and b ):
	print "5 - 变量 a 和 b 都为 false，或其中一个变量为 false"
else:
	print "5 - 变量 a 和 b 都为 true"


#成员运算符，in ,not in		测试实例中包含了一系列的成员，包括字符串，列表或元组
a = 10
b = 20
list = [1, 2, 3, 4, 5 ];
 
if ( a in list ):
	print "1 - 变量 a 在给定的列表中 list 中"
else:
	print "1 - 变量 a 不在给定的列表中 list 中"
	    
if ( b not in list ):
	print "2 - 变量 b 不在给定的列表中 list 中"
else:
	print "2 - 变量 b 在给定的列表中 list 中"
					 
# 修改变量 a 的值
a = 2
if ( a in list ):
	print "3 - 变量 a 在给定的列表中 list 中"
else:
	print "3 - 变量 a 不在给定的列表中 list 中"

'''
Python身份运算符
身份运算符用于比较两个对象的存储单元
is 是判断两个标识符是不是引用自一个对象	x is y, 类似 id(x) == id(y) , 如果引用的是同一个对象则返回 True，否则返回 False
is not 是判断两个标识符是不是引用自不同对象	x is not y ， 类似 id(a) != id(b)。如果引用的不是同一个对象则返回结果 True，否则返回 False。
'''
a = 20
b = 20
 
if ( a is b ):
	print "1 - a 和 b 有相同的标识"
else:
	print "1 - a 和 b 没有相同的标识"

if ( a is not b ):
	print "2 - a 和 b 没有相同的标识"
else:
	print "2 - a 和 b 有相同的标识"

# 修改变量 b 的值
b = 30
if ( a is b ):
	print "3 - a 和 b 有相同的标识"
else:
	print "3 - a 和 b 没有相同的标识"

if ( a is not b ):
	print "4 - a 和 b 没有相同的标识"
else:
	print "4 - a 和 b 有相同的标识"

"""
is 与 == 区别：
is 用于判断两个变量引用对象是否为同一个， == 用于判断引用变量的值是否相等。
>>> a = [1, 2, 3]
>>> b = a
>>> b is a 
True
>>> b == a
True
>>> b = a[:]
>>> b is a
False
>>> b == a
True
"""

