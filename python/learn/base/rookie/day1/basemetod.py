#!/usr/bin/python
# coding=utf-8
#打印时默认有换行符，且以新的一行作为上一行的结束符
print "hello world!"
#不换行使用','隔开
print "hello--","world--","!"
#语句块使用tab缩进，不使用{}
if True:
	print "True"
else:
	print "False"
#变量定义不需要指定类型
one = 1
#';'不用换行连续两条语句
two = 2;three=3
#'\'是一条语句的连接符
firstvar = one + \
		   two + \
		   three
print "firstvar is : ", firstvar
#{},(),[]不需要连接符
firstarr = ['one', 
		 'two',
		 'three']
print "firstarr[0]:",firstarr[0]
#c语言方式格式化输出
print "格式化输出:one=%d,firstarr[2]=%s" % (one, firstarr[2])

#Python 可以使用引号( ' )、双引号( " )、三引号( " ''' " 或 " """ " ) 来表示字符串，
#引号的开始与结束必须的相同类型的。
#其中三引号可以由多行组成，编写多行文本的快捷语法，常用于文档字符串，
#在文件的特定地点，被当做注释。

word = 'word'
sentence = "这是一个句子。"
paragraph = """这是一个段落。
包含了多个语句"""
print "paragraph:",paragraph

'''
多行注释
'''
"""
多行注释
"""

#函数或者类方法之间最好用空行隔开

raw_input("\n\n这是一个输入函数，Please input a key:")
#多条语句
import sys; x = 'yu_yuan'; sys.stdout.write("标准输出：" + x + '\n')
