#!/usr/bin/python
# coding=utf-8

"""
#########################################################################################

      > File Name: task_master.py

      > Author: Ywl

      > Descripsion: 分布式对进程，使用队列交换数据

      > Created Time:     Tue 27 Mar 2018 08:00:00 PM CST

      > Modify Time: 

#########################################################################################
"""
import random, time, queue
from multiprocessing.managers import BaseManager

#发送队列
send_queue = queue.Queue()
#接收队列
recive_queue = queue.Queue()

class QueueManager(BaseManager):
    pass

#注册队列到网络上，callable参数关联Queue对象
QueueManager.register('get_send_queue', callable=lambda: send_queue)
QueueManager.register('get_recive_queue', callable=lambda: recive_queue)
#绑定地址(默认是本地)，绑定端口，设置验证码
manager = QueueManager(address=('', 5000), authkey=b'abc')
#启动
manager.start()

#获得网络访问的Queue对象
send = manager.get_send_queue()     #这儿的函数和注册时的一致
recive = manager.get_recive_queue()

#放数据到队列,本地的话可以这样直接用
for i in range(10):
    n = random.randint(0, 100000)
    print('put send %d...' % n)
    send.put(n)

#读队列数据
print('Try  get recive...')

for i in range(10):
    r = recive.get(timeout=10)  #为嘛读不出来，报错竟然是队列为空
    print('Recive : %s' % r)
#关闭
manager.shudown()
print('master exit')
'''
在分布式网络，另一台设备上
# task_worker.py
import time, sys, queue
from multiprocessing.managers import BaseManager
# 创建类似的QueueManager:
class QueueManager(BaseManager):
pass
# 由于这个QueueManager只从网络上获取Queue，所以注册时只提供名字:
QueueManager.register('get_task_queue')
QueueManager.register('get_result_queue')
# 连接到服务器，也就是运行task_master.py的机器:
server_addr = '127.0.0.1'
print('Connect to server %s...' % server_addr)
# 端口和验证码注意保持与task_master.py设置的完全一致:
m = QueueManager(address=(server_addr, 5000), authkey=b'abc')
# 从网络连接:
m.connect()
# 获取Queue的对象:
task = m.get_task_queue()
result = m.get_result_queue()
# 从task队列取任务,并把结果写入result队列:
for i in range(10):
try:
n = task.get(timeout=1)
print('run task %d * %d...' % (n, n))
r = '%d * %d = %d' % (n, n, n*n)
time.sleep(1)
result.put(r)
except Queue.Empty:
print('task queue is empty.')
# 处理结束:
print('worker exit.')
任务进程要通过网络连接到服务进程，所以要指定服务进程的IP。
'''
