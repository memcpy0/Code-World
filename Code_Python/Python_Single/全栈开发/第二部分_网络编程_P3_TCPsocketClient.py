# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-27 22:37
program     : 用socket写客户端程序
"""
import socket
sk = socket.socket()

sk.connect(('127.0.0.1', 62222))

while True:
    sk.send(input('>>> ').encode("utf-8"))
    info = sk.recv(1024).decode("utf-8")
    print(info)
    if info == "bye":
        sk.send(b'bye')
        break

sk.close()
