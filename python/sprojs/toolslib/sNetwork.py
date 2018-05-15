#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
#########################################################################################
      > File Name: NetSocket.py
      > Author: Ywl
      > Descripsion:
      > Created Time:     Thu 14 Dec 2017 07:31:47 PM CST
      > Modify Time:
#########################################################################################
"""
import socket,struct
from enum import Enum, unique

@unique
class sNetEnum(Enum):
    NET_UDP = 'udpsock'
    NET_TCP = 'tcpsock'
    NET_BUF_MAX = 1024              #net最大buf大小
    NET_NORMAL = 0                  #正常
    NET_SOCK_CREATE_ERR = 1         #套接字创建失败
    NET_SR_TIMEOUT = 2              #收发超时
    NET_SOCK_BREAK = 3              #套接字损坏
    NET_MSG_ERR = 4                 #消息错误
    NET_ADDR_ERR = 5                #地址错误
    NET_OTHER = 0xff                #其他错误

#网络错误
class sNetError():
    def __init__(self, errNo=sNetEnum.NET_NORMAL, errMsg='Normal'):
        self.errNo = errNo
        self.errMsg = errMsg
#接收网络数据
class sNetRcevData():
    def __init__(self, data=None, addr=None):
        self.addr = addr
        self.data = data

class sNetwork():
    def __init__(self, netType=sNetEnum.NET_UDP, eth=None, port=-1, ip=socket.gethostname(), sendtime=3, recvtime=3, sPort = -1, sIp = socket.gethostname()):
        self.ip = ip
        self.port = port
        self.eth = eth
        self.sock = -1
        self.__netType = netType
        self.__sendtime = sendtime
        self.__recvtime = recvtime
        self.sPort = sPort
        self.sIp = sIp
    #创建套接字
    def createSock(self):
        sock = sCreateSock(self.__netType, self.eth, self.port, self.ip, self.__sendtime, self.__recvtime)
        if isinstance(sock, sNetError) is not True:
            self.sock = sock
        return sock
    #发送数据
    def sendMsg(self, sProtocol=None, sPort=-1, sIp=None, msg = None):
        if self.sock == -1:
            print('sock=-1')
            return sNetError(sNetEnum.NET_SOCK_BREAK)
        elif msg == None:
            print('msg = None')
            return sNetError(sNetEnum.NET_MSG_ERR)
        elif sPort == -1 or sIp == None:
            if self.sPort == -1 or self.sIp == None:
                return sNetError(sNetEnum.NET_ADDR_ERR)
            ret = sSendData(self.sock, sProtocol, self.sIp, self.sPort, msg)
        else:
            ret = sSendData(self.sock, sProtocol, sIp, sPort, msg)
        if ret.errNo != sNetEnum.NET_NORMAL:
            self.closeSock()
        return ret
    def getMsg(self, size=sNetEnum.NET_BUF_MAX.value, protocol=None):
        if self.sock == -1:
            return  sNetError(sNetEnum.NET_SOCK_BREAK)
        data, addr = self.sock.recvfrom(size)
        if protocol != None:
            pass
        else:
            data = str(data, 'utf-8')
        return sNetError(sNetEnum.NET_NORMAL, sNetRcevData(data=data, addr=addr))
    def closeSock(self):
        if isinstance(self.sock, int) and self.sock > 0:
            self.sock.close()
            self.sock = -1

def sSendData(sock, protocol, ip, port, data):
    try:
        if protocol != None:
            pass
        else:
            data = bytes(data, 'utf-8')
        sock.sendto(data, (ip, port))
    except BlockingIOError as msg:
        return sNetError(sNetEnum.NET_SR_TIMEOUT, msg)
    except (socket.error, Exception) as msg:
        return sNetError(sNetEnum.NET_SOCK_BREAK, msg)
    else:
        return sNetError(sNetEnum.NET_NORMAL)
def sCreateSock(netType=sNetEnum.NET_UDP, eth=None, port=-1, ip=socket.gethostname(), sendtime=3, recvtime=3):
    if port == -1:
        return sNetError(sNetEnum.NET_ADDR_ERR)
    try:
        if netType == sNetEnum.NET_UDP:
            netType = socket.SOCK_DGRAM
        elif netType == sNetEnum.NET_TCP:
            netType = socket.SOCK_STREAM
        sSock = socket.socket(socket.AF_INET, netType)  #创建套接字
        #允许端口重用
        sSock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        # 允许广播
        sSock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
        sSock.bind((ip, port))
        # 缓冲大小
        sSock.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, sNetEnum.NET_BUF_MAX)
        sSock.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, sNetEnum.NET_BUF_MAX)
        # 发送超时 这个地方是个结构体，需要转换timeval有两个字段，一个tv_sec，一个tv_usec，所以是两个LL, 两个long?
        sSock.setsockopt(socket.SOL_SOCKET, socket.SO_SNDTIMEO, struct.pack('LL', sendtime, 0))
        sSock.setsockopt(socket.SOL_SOCKET, socket.SO_RCVTIMEO, struct.pack('LL', recvtime, 0))
        if netType == sNetEnum.NET_TCP:
            sSock.listen()
    except socket.error as msg:
        sSock.close()
        sSock = sNetError(sNetEnum.NET_SOCK_CREATE_ERR, ("Create %s socket error %s " % (netType, str(msg))))
    finally:
        return sSock

if __name__ == '__main__':
    import time
    server = sNetwork(port=8999,sPort=8998)
    server.createSock()

    client = sNetwork(port=8998)
    client.createSock()
    while True:
        server.sendMsg(None,msg='hello')
        time.sleep(1)
        ret = client.getMsg(protocol=None)
        print(ret.errMsg.data)


'''
TCP
ss = socket() # 创建服务器套接字
ss.bind() # 套接字与地址绑定
ss.listen() # 监听连接
inf_loop: # 服务器无限循环
    cs = ss.accept() # 接受客户端连接
    comm_loop: # 通信循环
        cs.recv()/cs.send() # 对话（接收/发送）
    cs.close() # 关闭客户端套接字
ss .close() # 关闭服务器套接字#（可选）

UDP
cs = socket() # 创建客户端套接字
cs.connect() # 尝试连接服务器
comm_loop: # 通信循环
    cs.send()/cs.recv() # 对话（发送/接收）
cs .close() # 关闭客户端套接字
'''
