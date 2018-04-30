#!/usr/bin/python2.7
# coding=utf-8

"""
#########################################################################################
      > File Name: pysmtp.py
      > Author: Ywl
      > Descripsion:
      > Created Time:     Fri 13 Oct 2017 02:36:19 PM CST
      > Modify Time: 
#########################################################################################
"""
'''
Python SMTP发送邮件
SMTP（Simple Mail Transfer Protocol）即简单邮件传输协议,它是一组用于由源地址到目的地址传送邮件的规则，由它来控制信件的中转方式。
python的smtplib提供了一种很方便的途径发送电子邮件。它对smtp协议进行了简单的封装。
Python创建 SMTP 对象语法如下：
import smtplib

smtpObj = smtplib.SMTP( [host [, port [, local_hostname]]] )
参数说明：
host: SMTP 服务器主机。 你可以指定主机的ip地址或者域名如: runoob.com，这个是可选参数。
port: 如果你提供了 host 参数, 你需要指定 SMTP 服务使用的端口号，一般情况下 SMTP 端口号为25。
local_hostname: 如果 SMTP 在你的本机上，你只需要指定服务器地址为 localhost 即可。
Python SMTP 对象使用 sendmail 方法发送邮件，语法如下：
SMTP.sendmail(from_addr, to_addrs, msg[, mail_options, rcpt_options]

参数说明：
from_addr: 邮件发送者地址。
to_addrs: 字符串列表，邮件发送地址。
msg: 发送消息
这里要注意一下第三个参数，msg 是字符串，表示邮件。我们知道邮件一般由标题，发信人，收件人，邮件内容，附件等构成，发送邮件的时候，要注意 msg 的格式。这个格式就是 smtp 协议中定义的格式。

实例
以下执行实例需要你本机已安装了支持 SMTP 的服务，如：sendmail。
以下是一个使用 Python 发送邮件简单的实例：
'''

import smtplib
#带图片的
from email.mime.image import MIMEImage
#import email
from email.mime.text import MIMEText
#发送带附件的邮件，首先要创建MIMEMultipart()实例，然后构造附件，如果有多个附件，可依次构造，最后利用smtplib.smtp发送
from email.mime.multipart import MIMEMultipart
from email.header import Header

from email.utils import formataddr
#如果我们本机没有 sendmail 访问，也可以使用其他邮件服务商的 SMTP 访问（QQ、网易、Google等）。

# 第三方 SMTP 服务
mail_host="smtp.263.net"  #设置服务器
mail_user="wangliang_yu@sunwave.com.cn"    #用户名
mail_pass="S155wtx"   #口令
sender = '园宝~'
receivers = 'yuanyuan_pan@sunwave.com.cn'  # 接收邮件，可设置为你的QQ邮箱或者其他邮箱


#创建一个带附件的实例
message = MIMEMultipart()

subject = 'Python SMTP'
message['Subject'] = Header(subject, 'utf-8')
message['from'] = formataddr(["园宝lg", mail_user])  # 括号里的对应发件人邮箱昵称、发件人邮箱账号
message['To']= formataddr(["媳妇儿", receivers])              # 括号里的对应收件人邮箱昵称、收件人邮箱账号

mail_msg = """
<p>Python...</p>
<p><a href="123.207.22.202/svn/Book/Book">我的svn服务器，能放小说哦</a></p>
"""

mail_msg += """
<p>图片演示：</p>
<p><img src="cid:image1"></p>
"""
# 三个参数：第一个为文本内容，第二个 plain 设置文本格式，第三个 utf-8 设置编码
#message = MIMEText('Python 邮件发送测试...', 'plain', 'utf-8')
#html格式

#增加图片
msgAlternative = MIMEMultipart('alternative')
message.attach(msgAlternative)

# 指定图片为当前目录
fp = open('test.jpg', 'rb')
msgImage = MIMEImage(fp.read())
fp.close()

# 定义图片 ID，在 HTML 文本中引用
msgImage.add_header('Content-ID', '<image1>')
message.attach(msgImage)

#邮件正文内容
message.attach(MIMEText(mail_msg, 'html', 'utf-8'))
#message = MIMEText(mail_msg, 'html', 'utf-8')
 

# 构造附件1，传送当前目录下的 test.db 文件
#att1 = MIMEText(open('test.db', 'rb').read(), 'base64', 'utf-8')
#att1["Content-Type"] = 'application/octet-stream'
# 这里的filename可以任意写，写什么名字，邮件中显示什么名字
#att1["Content-Disposition"] = 'attachment; filename="test.db"'
#message.attach(att1)
	 
# 构造附件2，传送当前目录下的 get_post.py 文件
att2 = MIMEText(open('pysmtp.py', 'rb').read(), 'base64', 'utf-8')
att2["Content-Type"] = 'application/octet-stream'
att2["Content-Disposition"] = 'attachment; filename="get_post.py"'
message.attach(att2)
 
try:
	smtpObj = smtplib.SMTP()
	smtpObj.connect(mail_host, 587)    # 587 为 代理 端口号
	smtpObj.login(mail_user,mail_pass)
	smtpObj.sendmail(sender, [receivers,], message.as_string())
	print "邮件发送成功"
	smtpObj.quit()  # 关闭连接
except smtplib.SMTPException:
	print "Error: 无法发送邮件"

