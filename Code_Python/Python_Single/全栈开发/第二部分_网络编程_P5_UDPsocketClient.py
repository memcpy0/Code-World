# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-27 23:39
program     : 用socket来写UDP服务端程序
"""
import socket

sk = socket.socket(type=socket.SOCK_DGRAM)  # UDP
ip_port = ("127.0.0.1", 8080)
# sk.bind(ip_port)

while True:
    info = '来自1号的消息: ' + input("CLIENT >>> ")
    sk.sendto(info.encode('utf-8'), ip_port)
    ret, addr = sk.recvfrom(1024)
    print(ret.decode("utf-8"), addr.decode("utf-8"))
    if ret == b'bye':
        break
sk.close()
