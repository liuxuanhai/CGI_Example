#!/usr/bin/python
# coding=utf-8
#模块引用，如下
#import	function
#但是引入的模块内应该只定义函数，变量，类等比较好
#使用时必须用：模块名.var_fun
import importmo

importmo.printinfo("yu_yuan", 22)
print "模块变量：", importmo.abc

#一个模块只会被导入一次，不管你执行了多少次import。这样可以防止导入模块被一遍又一遍地执行。
'''
From…import 语句
Python 的 from 语句让你从模块中导入一个指定的部分到当前命名空间中。语法如下：
from modname import name1[, name2[, ... nameN]]
例如，要导入模块 fib 的 fibonacci 函数，使用如下语句：
from fib import fibonacci
这个声明不会把整个 fib 模块导入到当前的命名空间中，它只会将 fib 里的 fibonacci 单个引入到执行这个声明的模块的全局符号表。

From…import* 语句
把一个模块的所有内容全都导入到当前的命名空间也是可行的，只需使用如下声明：
from modname import *
这提供了一个简单的方法来导入一个模块中的所有项目。然而这种声明不该被过多地使用。
例如我们想一次性引入 math 模块中所有的东西，语句如下：
from math import *
'''


'''
搜索路径
当你导入一个模块，Python 解析器对模块位置的搜索顺序是：
1、当前目录
2、如果不在当前目录，Python 则搜索在 shell 变量 PYTHONPATH 下的每个目录。
3、如果都找不到，Python会察看默认路径。UNIX下，默认路径一般为/usr/local/lib/python/。
模块搜索路径存储在 system 模块的 sys.path 变量中。变量里包含当前目录，PYTHONPATH和由安装过程决定的默认目录。

PYTHONPATH 变量
作为环境变量，PYTHONPATH 由装在一个列表里的许多目录组成。PYTHONPATH 的语法和 shell 变量 PATH 的一样。
在 Windows 系统，典型的 PYTHONPATH 如下：
set PYTHONPATH=c:\python27\lib;
在 UNIX 系统，典型的 PYTHONPATH 如下：
set PYTHONPATH=/usr/local/lib/python
命名空间和作用域
变量是拥有匹配对象的名字（标识符）。命名空间是一个包含了变量名称们（键）和它们各自相应的对象们（值）的字典。
一个 Python 表达式可以访问局部命名空间和全局命名空间里的变量。如果一个局部变量和一个全局变量重名，则局部变量会覆盖全局变量。
每个函数都有自己的命名空间。类的方法的作用域规则和通常函数的一样。
Python 会智能地猜测一个变量是局部的还是全局的，它假设任何在函数内赋值的变量都是局部的。
因此，如果要给函数内的全局变量赋值，必须使用 global 语句。
global VarName 的表达式会告诉 Python， VarName 是一个全局变量，这样 Python 就不会在局部命名空间里寻找这个变量了。
例如，我们在全局命名空间里定义一个变量 Money。我们再在函数内给变量 Money 赋值，然后 Python 会假定 Money 是一个局部变量。然而，我们并没有在访问前声明一个局部变量 Money，结果就是会出现一个 UnboundLocalError 的错误。取消 global 语句的注释就能解决这个问题。
'''

Money = 2000
def AddMoney():
	# 想改正代码就取消以下注释,确定为全局变量:
	# global Money
	# 或者如下，确定为局部变量
	Money = 2
	Money = Money + 1
	print "Money:", Money
	    
print Money
AddMoney()
print Money

'''
dir()函数
dir() 函数一个排好序的字符串列表，内容是一个模块里定义过的名字。
返回的列表容纳了在一个模块里定义的所有模块，变量和函数。如下一个简单的实例：
'''

# 导入内置math模块
#在这里，特殊字符串变量__name__指向模块的名字，__file__指向该模块的导入文件名。
import math
 
content = dir(math)
	 
print content;

'''
globals() 和 locals() 函数
根据调用地方的不同，globals() 和 locals() 函数可被用来返回全局和局部命名空间里的名字。
如果在函数内部调用 locals()，返回的是所有能在该函数里访问的命名。
如果在函数内部调用 globals()，返回的是所有在该函数里能访问的全局名字。
两个函数的返回类型都是字典。所以名字们能用 keys() 函数摘取。

reload() 函数
当一个模块被导入到一个脚本，模块顶层部分的代码只会被执行一次。
因此，如果你想重新执行模块里顶层部分的代码，可以用 reload() 函数。该函数会重新导入之前导入过的模块。语法如下：
reload(module_name)
	在这里，module_name要直接放模块的名字，而不是一个字符串形式。比如想重载 hello 模块，如下：
	reload(hello)
'''

'''
Python中的包
包是一个分层次的文件目录结构，它定义了一个由模块及子包，和子包下的子包等组成的 Python 的应用环境。
简单来说，包就是文件夹，但该文件夹下必须存在 __init__.py 文件, 该文件的内容可以为空。__int__.py用于标识当前文件夹是一个包。
考虑一个在 package_runoob 目录下的 runoob1.py、runoob2.py、__init__.py 文件，test.py 为测试调用包的代码，目录结构如下：
test.py
package_runoob
|-- __init__.py
|-- runoob1.py
|-- runoob2.py
'''

# 导入 Phone 包
from package_runoob.runoob1 import runoob1
from package_runoob.runoob2 import runoob2
 
runoob1()
runoob2()
'''
貌似不需要加模块名来调用
我们只在每个文件里放置了一个函数，但其实你可以放置许多函数。你也可以在这些文件里定义Python的类，然后为这些类建一个包
'''

'''
系统相关的信息模块: import sys

sys.argv 是一个 list,包含所有的命令行参数.    
sys.stdout sys.stdin sys.stderr 分别表示标准输入输出,错误输出的文件对象.    
sys.stdin.readline() 从标准输入读一行 sys.stdout.write("a") 屏幕输出a    
sys.exit(exit_code) 退出程序    
sys.modules 是一个dictionary，表示系统中所有可用的module    
sys.platform 得到运行的操作系统环境    
sys.path 是一个list,指明所有查找module，package的路径.  

操作系统相关的调用和操作: import os

os.environ 一个dictionary 包含环境变量的映射关系   
os.environ["HOME"] 可以得到环境变量HOME的值     
os.chdir(dir) 改变当前目录 os.chdir('d:\\outlook')   

注意windows下用到转义     
os.getcwd() 得到当前目录     
os.getegid() 得到有效组id os.getgid() 得到组id     
os.getuid() 得到用户id os.geteuid() 得到有效用户id     
os.setegid os.setegid() os.seteuid() os.setuid()     
os.getgruops() 得到用户组名称列表     
os.getlogin() 得到用户登录名称     
os.getenv 得到环境变量     
os.putenv 设置环境变量     
os.umask 设置umask     
os.system(cmd) 利用系统调用，运行cmd命令   

内置模块(不用import就可以直接使用)常用内置函数：

help(obj) 在线帮助, obj可是任何类型    
callable(obj) 查看一个obj是不是可以像函数一样调用    
repr(obj) 得到obj的表示字符串，可以利用这个字符串eval重建该对象的一个拷贝    
eval_r(str) 表示合法的python表达式，返回这个表达式    
dir(obj) 查看obj的name space中可见的name    
hasattr(obj,name) 查看一个obj的name space中是否有name    
getattr(obj,name) 得到一个obj的name space中的一个name    
setattr(obj,name,value) 为一个obj的name   
space中的一个name指向vale这个object    
delattr(obj,name) 从obj的name space中删除一个name    
vars(obj) 返回一个object的name space。用dictionary表示    
locals() 返回一个局部name space,用dictionary表示    
globals() 返回一个全局name space,用dictionary表示    
type(obj) 查看一个obj的类型    
isinstance(obj,cls) 查看obj是不是cls的instance    
issubclass(subcls,supcls) 查看subcls是不是supcls的子类  

##################    类型转换  ##################

	chr(i) 把一个ASCII数值,变成字符    
	ord(i) 把一个字符或者unicode字符,变成ASCII数值    
	oct(x) 把整数x变成八进制表示的字符串    
	hex(x) 把整数x变成十六进制表示的字符串    
	str(obj) 得到obj的字符串描述    
	list(seq) 把一个sequence转换成一个list    
	tuple(seq) 把一个sequence转换成一个tuple    
	dict(),dict(list) 转换成一个dictionary    
	int(x) 转换成一个integer    
	long(x) 转换成一个long interger    
	float(x) 转换成一个浮点数    
	complex(x) 转换成复数    
	max(...) 求最大值    
	min(...) 求最小值  
'''
