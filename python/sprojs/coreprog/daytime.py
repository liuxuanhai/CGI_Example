import os
from socket import *

print(gethostname())
print(getservbyname('daytime', 'udp'))


HOST = 'localhost'
PORT = 13
BUFSIZE = 1024
ADDR = (HOST, PORT)

udpSock = socket(AF_INET, SOCK_DGRAM)

while True:
    data = 'get time'

    udpSock.sendto(bytes(data, 'utf-8'), ADDR)
    data, ADDR = udpSock.recvfrom(BUFSIZE)
    if not data:
        break
    print(str(data, 'utf-8'))

udpSock.close()
