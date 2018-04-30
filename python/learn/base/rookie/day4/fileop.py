#!/usr/bin/python
# coding=utf-8

#文件I/O
'''
open 函数
你必须先用Python内置的open()函数打开一个文件，创建一个file对象，相关的方法才可以调用它进行读写。
语法：
file object = open(file_name [, access_mode][, buffering])
	各个参数的细节如下：
	file_name：file_name变量是一个包含了你要访问的文件名称的字符串值。
	access_mode：access_mode决定了打开文件的模式：只读，写入，追加等。所有可取值见如下的完全列表。这个参数是非强制的，默认文件访问模式为只读(r)。
	buffering:如果buffering的值被设为0，就不会有寄存。如果buffering的值取1，访问文件时会寄存行。如果将buffering的值设为大于1的整数，表明了这就是的寄存区的缓冲大小。如果取负值，寄存区的缓冲大小则为系统默认。
	不同模式打开文件的完全列表：
	模式	描述
	r	以只读方式打开文件。文件的指针将会放在文件的开头。这是默认模式。
	rb	以二进制格式打开一个文件用于只读。文件指针将会放在文件的开头。这是默认模式。
	r+	打开一个文件用于读写。文件指针将会放在文件的开头。
	rb+	以二进制格式打开一个文件用于读写。文件指针将会放在文件的开头。
	w	打开一个文件只用于写入。如果该文件已存在则将其覆盖。如果该文件不存在，创建新文件。
	wb	以二进制格式打开一个文件只用于写入。如果该文件已存在则将其覆盖。如果该文件不存在，创建新文件。
	w+	打开一个文件用于读写。如果该文件已存在则将其覆盖。如果该文件不存在，创建新文件。
	wb+	以二进制格式打开一个文件用于读写。如果该文件已存在则将其覆盖。如果该文件不存在，创建新文件。
	a	打开一个文件用于追加。如果该文件已存在，文件指针将会放在文件的结尾。也就是说，新的内容将会被写入到已有内容之后。如果该文件不存在，创建新文件进行写入。
	ab	以二进制格式打开一个文件用于追加。如果该文件已存在，文件指针将会放在文件的结尾。也就是说，新的内容将会被写入到已有内容之后。如果该文件不存在，创建新文件进行写入。
	a+	打开一个文件用于读写。如果该文件已存在，文件指针将会放在文件的结尾。文件打开时会是追加模式。如果该文件不存在，创建新文件用于读写。
	ab+	以二进制格式打开一个文件用于追加。如果该文件已存在，文件指针将会放在文件的结尾。如果该文件不存在，创建新文件用于读写。
	File对象的属性
	一个文件被打开后，你有一个file对象，你可以得到有关该文件的各种信息。
	以下是和file对象相关的所有属性的列表：
	属性	描述
	file.closed	返回true如果文件已被关闭，否则返回false。
	file.mode	返回被打开文件的访问模式。
	file.name	返回文件的名称。
	file.softspace	如果用print输出后，必须跟一个空格符，则返回false。否则返回true。
'''

file1 = open("test.txt", "wb");
print "文件名：", file1.name
print "文件是否关闭：", file1.closed
print "文件打开模式：", file1.mode
print "文件是否强制加空格：", file1.softspace

'''
close()方法
File 对象的 close（）方法刷新缓冲区里任何还没写入的信息，并关闭该文件，这之后便不能再进行写入。
当一个文件对象的引用被重新指定给另一个文件时，Python 会关闭之前的文件。用 close（）方法关闭文件是一个很好的习惯。
'''

file1.close()

'''
读写文件：
file对象提供了一系列方法，能让我们的文件访问更轻松。来看看如何使用read()和write()方法来读取和写入文件。
write()方法
write()方法可将任何字符串写入一个打开的文件。需要重点注意的是，Python字符串可以是二进制数据，而不是仅仅是文字。
write()方法不会在字符串的结尾添加换行符('\n')：
语法：
fileObject.write(string);

read()方法
read（）方法从一个打开的文件中读取一个字符串。需要重点注意的是，Python字符串可以是二进制数据，而不是仅仅是文字。
语法：
fileObject.read([count]);
在这里，被传递的参数是要从已打开文件中读取的字节计数。该方法从文件的开头开始读入，如果没有传入count，它会尝试尽可能多地读取更多的内容，很可能是直到文件的末尾。
'''
file1 = open("test1.txt", "ab+");
file1.write("hello\nworld\n")
'''
文件定位
tell()方法告诉你文件内的当前位置；换句话说，下一次的读写会发生在文件开头这么多字节之后。
seek（offset [,from]）方法改变当前文件的位置。Offset变量表示要移动的字节数。From变量指定开始移动字节的参考位置。
如果from被设为0，这意味着将文件的开头作为移动字节的参考位置。如果设为1，则使用当前的位置作为参考位置。如果它被设为2，那么该文件的末尾将作为参考位置
'''
import os
print "当前文件位置：", file1.tell()
file1.seek(0, 0)
file1.seek(0, os.SEEK_SET)
str1 = ""
filelen = os.path.getsize("test1.txt")
print "文件大小：", filelen 
print "文件写入内容读出来为："
while (file1.tell() != filelen):
	str1 = file1.read()
	if (str1 != ""):
		print str1,

file1.close()

'''
Python File(文件) 方法
file 对象使用 open 函数来创建，下表列出了 file 对象常用的函数：
序号	方法及描述
	1	
file.close()
	关闭文件。关闭后文件不能再进行读写操作。
	2	
file.flush()
	刷新文件内部缓冲，直接把内部缓冲区的数据立刻写入文件, 而不是被动的等待输出缓冲区写入。
	3	
file.fileno()
	返回一个整型的文件描述符(file descriptor FD 整型), 可以用在如os模块的read方法等一些底层操作上。
	4	
file.isatty()
	如果文件连接到一个终端设备返回 True，否则返回 False。
	5	
file.next()
	返回文件下一行。
	6	
file.read([size])
	从文件读取指定的字节数，如果未给定或为负则读取所有。
	7	
file.readline([size])
	读取整行，包括 "\n" 字符。
	8	
file.readlines([sizehint])
	读取所有行并返回列表，若给定sizeint>0，返回总和大约为sizeint字节的行, 实际读取值可能比sizhint较大, 因为需要填充缓冲区。
	9	
file.seek(offset[, whence])
	设置文件当前位置
	10	
file.tell()
	返回文件当前位置。
	11	
file.truncate([size])
	截取文件，截取的字节通过size指定，默认为当前文件位置。
	12	
file.write(str)
	将字符串写入文件，没有返回值。
	13	
file.writelines(sequence)
	向文件写入一个序列字符串列表，如果需要换行则要自己加入每行的换行符。
'''


'''
重命名和删除文件
Python的os模块提供了帮你执行文件处理操作的方法，比如重命名和删除文件。
要使用这个模块，你必须先导入它，然后才可以调用相关的各种功能。

rename()方法：
rename()方法需要两个参数，当前的文件名和新文件名。
语法：
os.rename(current_file_name, new_file_name)

remove()方法
你可以用remove()方法删除文件，需要提供要删除的文件名作为参数。
语法：
os.remove(file_name)
'''
import os

os.rename("test.txt", "test2.txt")
os.remove("test2.txt")

'''
Python里的目录：
所有文件都包含在各个不同的目录下，不过Python也能轻松处理。os模块有许多方法能帮你创建，删除和更改目录。
mkdir()方法
可以使用os模块的mkdir()方法在当前目录下创建新的目录们。你需要提供一个包含了要创建的目录名称的参数。

语法：
os.mkdir("newdir")

chdir()方法
可以用chdir()方法来改变当前的目录。chdir()方法需要的一个参数是你想设成当前目录的目录名称。
语法：
os.chdir("newdir")

getcwd()方法：
getcwd()方法显示当前的工作目录。
语法：
os.getcwd()


rmdir()方法
rmdir()方法删除目录，目录名称以参数传递。
在删除这个目录之前，它的所有内容应该先被清除。
语法：
os.rmdir('dirname')
'''
os.mkdir("newdir")
os.rmdir("newdir")
print "工作目录：", os.getcwd()
os.chdir("/home/ubuntu/workspace/")
print "当前工作目录：", os.getcwd()

'''
os.chmod()方法 此方法通过数值模式更新路径或文件权限。该模式可采取下列值或按位或运算组合之一：
stat.S_ISUID: Set user ID on execution.
stat.S_ISGID: Set group ID on execution.
stat.S_ENFMT: Record locking enforced.
stat.S_ISVTX: Save text image after execution.
stat.S_IREAD: Read by owner.
stat.S_IWRITE: Write by owner.
stat.S_IEXEC: Execute by owner.
stat.S_IRWXU: Read, write, and execute by owner.
stat.S_IRUSR: Read by owner.
stat.S_IWUSR: Write by owner.
stat.S_IXUSR: Execute by owner.
stat.S_IRWXG: Read, write, and execute by group.
stat.S_IRGRP: Read by group.
stat.S_IWGRP: Write by group.
stat.S_IXGRP: Execute by group.
stat.S_IRWXO: Read, write, and execute by others.
stat.S_IROTH: Read by others.
stat.S_IWOTH: Write by others.
stat.S_IXOTH: Execute by others.
语法：
os.chmod(path, mode);
参数：
path: This is the path for which mode would be set.
mode: This may take one of the above mentioned values or bitwise ORed combinations of them:
示例：
[python] view plain copy
#!/usr/bin/python  
import os, sys, stat  
# Assuming /tmp/foo.txt exists, Set a file execute by the group.  
os.chmod("/tmp/foo.txt", stat.S_IXGRP)  
# Set a file write by others.  
os.chmod("/tmp/foo.txt", stat.S_IWOTH)  
print "Changed mode successfully!!"  
This produces following result:  
Changed mode successfully!!  
os.chmod("/tmp/foo.txt", stat.S_IRWXU|stat.S_IRWXG|stat.S_IRWXO) # mode:777  

'''


