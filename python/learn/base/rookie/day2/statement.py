#!/usr/bin/python
# coding=utf-8

#循环：while()用于条件语句循环执行某条语句，for常用于序列化变量的编列
lists1 = [10, 20, 30, 40]
event = []
print "while循环"
while len(lists1) > 0:
#注意出栈入栈顺序
	list1 = lists1.pop()
	event.append(list1)
print event
lists2 = [10, 30, 50, 70]
print "for 遍历"
for index in lists2:
	print index

i = 2
while(i < 100):
	j = 2
	while(j <= (i/j)):
		if not(i%j): break
		j = j + 1
	if (j > i/j) : print i, " 是素数"
	i = i + 1
																			   
print "Good bye!"

"""
Python pass 语句
Python pass是空语句，是为了保持程序结构的完整性。
pass 不做任何事情，一般用做占位语句。
Python 语言 pass 语句语法格式如下：
	pass

当你在编写一个程序时，执行语句部分思路还没有完成，这时你可以用pass语句来占位，也可以当做是一个标记，是要过后来完成的代码。比如下面这样：
>>>def iplaypython():
>>>       pass
定义一个函数iplaypython，但函数体部分暂时还没有完成，又不能空着不写内容，因此可以用pass来替代占个位置。
"""
# 输出 Python 的每个字母
for letter in 'Python':
	if letter == 'h':
		pass
		print '这是 pass 块'
	print '当前字母 :', letter

print "Good bye!"
