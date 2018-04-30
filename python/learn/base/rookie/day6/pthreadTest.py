#!/usr/bin/python
# coding=utf-8
'''
Python中使用线程有两种方式：函数或者用类来包装线程对象。
函数式：调用thread模块中的start_new_thread()函数来产生新线程。语法如下:
thread.start_new_thread ( function, args[, kwargs] )
	参数说明:
	function - 线程函数。
	args - 传递给线程函数的参数,他必须是个tuple类型。
	kwargs - 可选参数。
'''

import thread
import time

#定义一个线程函数
def pthread_test_handler(threadName):
	count = 0
	while count < 5:
		count += 1
		time.sleep(2)
		print "线程：%s，时间：%s" % (threadName, time.ctime(time.time()))

'''
try:
	thread.start_new_thread(pthread_test_handler, ("测试线程", ))	#这个地方的传参是个不定长元组，非要在后面以','结束?
except:
	print "无法创建线程"

while True:
	pass
'''

"""
Python通过两个标准库thread和threading提供对线程的支持。thread提供了低级别的、原始的线程以及一个简单的锁。
thread 模块提供的其他方法：
threading.currentThread(): 返回当前的线程变量。
threading.enumerate(): 返回一个包含正在运行的线程的list。正在运行指线程启动后、结束前，不包括启动前和终止后的线程。
threading.activeCount(): 返回正在运行的线程数量，与len(threading.enumerate())有相同的结果。
除了使用方法外，线程模块同样提供了Thread类来处理线程，Thread类提供了以下方法:
run(): 用以表示线程活动的方法。
start():启动线程活动。
join([time]): 等待至线程中止。这阻塞调用线程直至线程的join() 方法被调用中止-正常退出或者抛出未处理的异常-或者是可选的超时发生。
isAlive(): 返回线程是否活动的。
getName(): 返回线程名。
setName(): 设置线程名
"""

#线程类的方式，许多语言都有
'''
使用Threading模块创建线程
使用Threading模块创建线程，直接从threading.Thread继承，然后重写__init__方法和run方法：
'''
import threading

exitFlag = 0

class myThread(threading.Thread):
	def __init__(self, ThreadId, name, delay):
#初始化父类
		threading.Thread.__init__(self)
		self.ThreadId = ThreadId
		self.name = name
		self.delay = delay
	def run(self):
		print "Start:", self.name
		'''
		# 获得锁，成功获得锁定后返回True
		# 可选的timeout参数不填时将一直阻塞直到获得锁定
		# 否则超时后将返回False
		'''
		threadLock.acquire()
		print_timer(self.name, self.delay, 5)
		#释放锁
		threadLock.release()
		print "Exit:", self.name

def print_timer(name, delay, counter):
	while counter:
		if exitFlag:
			#函数调用退出
			threading.Thread.exit()
		time.sleep(delay)
		print "%s: %s" % (name, time.ctime(time.time()))
		counter -= 1
			
#创建锁
threadLock = threading.Lock()

threads = []

#创建线程
thread1 = myThread(1, "线程1", 1)
thread2 = myThread(2, "线程2", 2)

#开启线程
thread1.start()
thread2.start()

#添加到线程队列
threads.append(thread1)
threads.append(thread2)

#等待所有线程退出
for t in threads:
	t.join()
print "Main Thread Exit"
#while True:
#	pass

'''
线程同步
如果多个线程共同对某个数据修改，则可能出现不可预料的结果，为了保证数据的正确性，需要对多个线程进行同步。
使用Thread对象的Lock和Rlock可以实现简单的线程同步，这两个对象都有acquire方法和release方法，对于那些需要每次只允许一个线程操作的数据，可以将其操作放到acquire和release方法之间。如下：
多线程的优势在于可以同时运行多个任务（至少感觉起来是这样）。但是当线程需要共享数据时，可能存在数据不同步的问题。
考虑这样一种情况：一个列表里所有元素都是0，线程"set"从后向前把所有元素改成1，而线程"print"负责从前往后读取列表并打印。
那么，可能线程"set"开始改的时候，线程"print"便来打印列表了，输出就成了一半0一半1，这就是数据的不同步。为了避免这种情况，引入了锁的概念。
锁有两种状态——锁定和未锁定。每当一个线程比如"set"要访问共享数据时，必须先获得锁定；如果已经有别的线程比如"print"获得锁定了，那么就让线程"set"暂停，也就是同步阻塞；等到线程"print"访问完毕，释放锁以后，再让线程"set"继续。
经过这样的处理，打印列表时要么全部输出0，要么全部输出1，不会再出现一半0一半1的尴尬场面。
'''



