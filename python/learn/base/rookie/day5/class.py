#!/usr/bin/python
# coding=utf-8
'''
类的创建和使用
与c++差别不大
'''
#基类
class TestClass:
	'一个用于测试的基类'
#相当于类里面定义的静态变量属性，所有类的实例共享
	classcount = 0
#__init__:构造函数，self第一个参数表示实例本身，可以换成其他标识符
	"""
	self 代表类的实例，self 在定义类的方法时是必须有的，虽然在调用时不必传入相应的参数
	self代表类的实例，而非类
	类的方法与普通的函数只有一个特别的区别——它们必须有一个额外的第一个参数名称, 按照惯例它的名称是 self
	"""
	def __init__(self):
#self.xx 表示类的属性
#相当于公有变量
		self.name = "xxx"
		self.age = 0
#相当与私有变量
		self.__sla = 100
		TestClass.classcount += 1
	def printstatic(self):
		print "第一种访问方式是访问静态成员变量", TestClass.classcount
	def printname(self):
		print "第二种方式是访问本实例的成员变量", self.name
	def printself(self):
		print "第三种方式是访问类本身-地址：%s,类名：%s" % (self, self.__class__)
	def __del__(self):
		print "销毁:", self.__class__.__name__

oneclass = TestClass()
print "直接访问实例的成员变量", oneclass.name
#print "有公有属性？", oneclass.testvar
print oneclass.age
oneclass.name = "yu_yuan"
print oneclass.name
#print "能访问私有变量么：", oneclass.__sla
#不能，是要报错的

print "直接访问静态的变量", TestClass.classcount
xxx = TestClass()
#确实是
print "公有变量是公共的么：", xxx.classcount

oneclass.printstatic()
oneclass.printname()
oneclass.printself()

'''
下面这些操作是属于实例的
你可以添加，删除，修改类的属性，如下所示：

emp1.age = 7  # 添加一个 'age' 属性
emp1.age = 8  # 修改 'age' 属性
del emp1.age  # 删除 'age' 属性

你也可以使用以下函数的方式来访问属性：

getattr(obj, name[, default]) : 访问对象的属性。
hasattr(obj, name) : 检查是否存在一个属性。
setattr(obj, name, value) : 设置一个属性。如果属性不存在，会创建一个新属性。
delattr(obj, name) : 删除属性。

hasattr(emp1, 'age')    # 如果存在 'age' 属性返回 True。
getattr(emp1, 'age')    # 返回 'age' 属性的值
setattr(emp1, 'age', 8) # 添加属性 'age' 值为 8
delattr(empl, 'age')    # 删除属性 'age'
'''

'''
Python内置类属性
__dict__ : 类的属性（包含一个字典，由类的数据属性组成）
__doc__ :类的文档字符串
__name__: 类名
__module__: 类定义所在的模块（类的全名是'__main__.className'，如果类位于一个导入模块mymod中，那么className.__module__ 等于 mymod）
__bases__ : 类的所有父类构成元素（包含了一个由所有父类组成的元组）
'''

print "Employee.__doc__:", TestClass.__doc__
print "Employee.__name__:", TestClass.__name__
print "Employee.__module__:", TestClass.__module__
print "Employee.__bases__:", TestClass.__bases__
print "Employee.__dict__:", TestClass.__dict__

'''
python对象销毁(垃圾回收)
	Python 使用了引用计数这一简单技术来跟踪和回收垃圾。
	在 Python 内部记录着所有使用中的对象各有多少引用。
	一个内部跟踪变量，称为一个引用计数器。
	当对象被创建时， 就创建了一个引用计数， 当这个对象不再需要时， 也就是说， 这个对象的引用计数变为0 时， 它被垃圾回收。但是回收不是"立即"的， 由解释器在适当的时机，将垃圾对象占用的内存空间回收。
	a = 40      # 创建对象  <40>
	b = a       # 增加引用， <40> 的计数
	c = [b]     # 增加引用.  <40> 的计数

	del a       # 减少引用 <40> 的计数
	b = 100     # 减少引用 <40> 的计数
	c[0] = -1   # 减少引用 <40> 的计数
	垃圾回收机制不仅针对引用计数为0的对象，同样也可以处理循环引用的情况。循环引用指的是，两个对象相互引用，但是没有其他变量引用他们。这种情况下，仅使用引用计数是不够的。Python 的垃圾收集器实际上是一个引用计数器和一个循环垃圾收集器。作为引用计数的补充， 垃圾收集器也会留心被分配的总量很大（及未通过引用计数销毁的那些）的对象。 在这种情况下， 解释器会暂停下来， 试图清理所有未引用的循环。
'''
del oneclass

#继承
"""
在python中继承中的一些特点：
1：在继承中基类的构造（__init__()方法）不会被自动调用，它需要在其派生类的构造中亲自专门调用。
2：在调用基类的方法时，需要加上基类的类名前缀，且需要带上self参数变量。区别于在类中调用普通函数时并不需要带上self参数
3：Python总是首先查找对应类型的方法，如果它不能在派生类中找到对应的方法，它才开始到基类中逐个查找。（先在本类中查找调用的方法，找不到才去基类中找）。
如果在继承元组中列了一个以上的类，那么它就被称作"多重继承" 。
"""
class personinfo(TestClass):
	def __init__(self):
#为嘛要初始化父类
		TestClass.__init__(self)
#python3可用的方式，一次性
#super(personinfo, self).__init__()
		self.number = 0
	def printnumber(self):
		print "派生类方法：", self.number
#销毁的时候先销毁派生类，再销毁父类
	def __del__(self):
		print "销毁派生类"

twoclass = personinfo()
print "派生类属性：", twoclass.number
print "访问基类属性：", twoclass.name
twoclass.printnumber()
twoclass.printname()
print "查看计数：", TestClass.classcount
del twoclass

'''
单下划线、双下划线、头尾双下划线说明：
__foo__: 定义的是特列方法，类似 __init__() 之类的。
_foo: 以单下划线开头的表示的是 protected 类型的变量，即保护类型只能允许其本身与子类进行访问，不能用于 from module import *
__foo: 双下划线的表示的是私有类型(private)的变量, 只能是允许这个类本身进行访问了。
基础重载方法
下表列出了一些通用的功能，你可以在自己的类重写：
	序号	方法, 描述 & 简单的调用
	1	__init__ ( self [,args...] )
	构造函数
	简单的调用方法: obj = className(args)
	2	__del__( self )
	析构方法, 删除一个对象
	简单的调用方法 : del obj
	3	__repr__( self )
	转化为供解释器读取的形式
	简单的调用方法 : repr(obj)
	4	__str__( self )
	用于将值转化为适于人阅读的形式
	简单的调用方法 : str(obj)
	5	__cmp__ ( self, x )
	对象比较
	简单的调用方法 : cmp(obj, x)
'''

#运算符重载
class Vector:
	def __init__(self, a, b):
		self.a = a
		self.b = b

	def __str__(self):
		return 'Vector (%d, %d)' % (self.a, self.b)

	def __add__(self,other):
		return Vector(self.a + other.a, self.b + other.b)

v1 = Vector(2,10)
v2 = Vector(5,-2)
print v1 + v2
