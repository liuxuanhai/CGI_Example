#!/usr/bin/python
# coding=utf-8


import sys
import serial
import time

try:  
	ser = serial.Serial(
			port = '/dev/ttyUSB0', 
			baudrate = 9600,
			parity = 'N',
			stopbits = 1,
			bytesize = 8,
			timeout = None)  
except Exception, e:  
	print 'open serial failed.'  
	exit(1) 

data = ''

while True:
	n = ser.inWaiting()
	data = ser.read(n)
	if data != '':
		print data
	
'''
read(n):n is want to read
readall()
inWating():return how long have recv
'''

'''
Features

same class based interface on all supported platforms
access to the port settings through Python 2.2+ properties
port numbering starts at zero, no need to know the port name in the user program
port string (device name) can be specified if access through numbering is inappropriate
support for different bytesizes, stopbits, parity and flow control with RTS/CTS and/or Xon/Xoff
working with or without receive timeout
file like API with “read” and “write” (“readline” etc. also supported)
	The files in this package are 100% pure Python. They depend on non standard but common packages on Windows (pywin32) and Jython (JavaComm). POSIX (Linux, BSD) uses only modules from the standard Python distribution)
	The port is set up for binary transmission. No NULL byte stripping, CR-LF translation etc. (which are many times enabled for POSIX.) This makes this module universally useful.

	黑体字的部分注意一下,特别是最后一项.全部是二进制传输.

	sudo easy_install pyserial
	然后测试一下

	sudo python(这是为了一会给串口读写权限)
	>>> import serial
	>>> ser=serial.Serial('/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0')
	>>> ser
	Serial<id=0x7fc5c503ca10, open=True>(port='/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0', baudrate=9600, bytesize=8, parity='N', stopbits=1, timeout=None, xonxoff=False, rtscts=False, dsrdtr=False)
	>>> ser.timeout=1
	>>> ser
	Serial<id=0x7fc5c503ca10, open=True>(port='/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0', baudrate=9600, bytesize=8, parity='N', stopbits=1, timeout=1, xonxoff=False, rtscts=False, dsrdtr=False)
	有时候如果你使用/dev/ttyUSB0行不通的话,就直接使用/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0

	 

	timeout=None, # set a timeout value, None for waiting forever
	上面timeout的意思是,如果是none,那么程序永远会死在这里.一直等待
	timeout=0 # non-blocking mode (return immediately on read)
	timeout = 0: 非阻塞形式 (读完之后就返回,费时的io操作,就直接交给后台处理了)
	timeout=x # set timeout to x seconds (float allowed)
	超时时间一到,程序就是继续执行
	 

	 

	————————–没错我是分割线–开始————————————
	小知识:
	读写串行口时，既可以同步执行，也可以重叠（异步）执行。
	在同步执行时，函数直到操作完成后才返回。这意味着在同步执行时线程会被阻塞，从而导致效率下降。
	在重叠执行时，即使操作还未完成，调用的函数也会立即返回。费时的I/O操作在后台进行，这样线程就可以干别的事情。
	例如，线程可以在不同的句柄上同时执行I/O操作，甚至可以在同一句柄上同时进行读写操作。”重叠”一词的含义就在于此。

	阻塞的定义：

	对于read，block指当串口输入缓冲区没有数据的时候，read函数将会阻塞在这里，一直到串口输入缓冲区中有数据可读取，read读到了需要的字节数之后，返回值为读到的字节数；
	对于write，block指当串口输出缓冲区满，或剩下的空间小于将要写入的字节数，则write将阻塞，一直到串口输出缓冲区中剩下的空间大于等于将要写入的字节数，执行写入操作，返回写入的字节数。
	非阻塞的定义：

	对于read，no block指当串口输入缓冲区没有数据的时候，read函数立即返回，返回值为0。
	对于write，no block指当串口输出缓冲区满，或剩下的空间小于将要写入的字节数，则write将进行写操作，写入当前串口输出缓冲区剩下空间允许的字节数，然后返回写入的字节数。
	—————————-没错我是分割线–结束———————————-

	Be carefully when using “readline”. Do specify a timeout when opening the serial port otherwise it could block forever if no newline character is received. Also note that “readlines” only works with a timeout. “readlines” depends on having a timeout and interprets that as EOF (end of file). It raises an exception if the port is not opened correctly.

	Parameters for the Serial class

	ser = serial.Serial(
			port=None,              # number of device, numbering starts at
# zero. if everything fails, the user
# can specify a device string, note
# that this isn't portable anymore
# if no port is specified an unconfigured
# an closed serial port object is created
			baudrate=9600,          # baud rate
			bytesize=EIGHTBITS,     # number of databits
			parity=PARITY_NONE,     # enable parity checking
			stopbits=STOPBITS_ONE,  # number of stopbits
			timeout=None,           # set a timeout value, None for waiting forever
			xonxoff=0,              # enable software flow control
			rtscts=0,               # enable RTS/CTS flow control
			interCharTimeout=None   # Inter-character timeout, None to disable
			)
	Methods of Serial instances

	open()                  # open port
	close()                 # close port immediately
	setBaudrate(baudrate)   # change baud rate on an open port
	inWaiting()             # return the number of chars in the receive buffer
	read(size=1)            # read "size" characters
	write(s)                # write the string s to the port
	flushInput()            # flush input buffer, discarding all it's contents
	flushOutput()           # flush output buffer, abort output
	sendBreak()             # send break condition
	setRTS(level=1)         # set RTS line to specified logic level
	setDTR(level=1)         # set DTR line to specified logic level
	getCTS()                # return the state of the CTS line
	getDSR()                # return the state of the DSR line
	getRI()                 # return the state of the RI line
	getCD()                 # return the state of the CD line
	Attributes of Serial instances
	readonly

	portstr                 # device name
	BAUDRATES               # list of valid baudrates
	BYTESIZES               # list of valid byte sizes
	PARITIES                # list of valid parities
	STOPBITS                # list of valid stop bit widths
	New values can be assigned to the following attributes, the port will be reconfigured, even if it’s opened at that time:(即使是打开的情况下也会重新配置???liub)

	port                    # port name/number as set by the user
	baudrate                # current baud rate setting
	bytesize                # byte size in bits
	parity                  # parity setting
	stopbits                # stop bit with (1,2)
	timeout                 # timeout setting
	xonxoff                 # if Xon/Xoff flow control is enabled
	rtscts                  # if hardware flow control is enabled
'''
