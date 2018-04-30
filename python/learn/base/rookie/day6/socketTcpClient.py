#!/usr/bin/python
# coding=utf-8

import socket               # 导入 socket 模块
#SOCK_DGRAM == UDP
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)         # 创建 socket 对象
host = socket.gethostname() # 获取本地主机名
port = 8888                # 设置端口好

s.connect((host, port))
print s.recv(1024)
s.close() 
