# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-28 19:54
program     : 学习使用UDP来发送命令; 解决黏包现象
"""#
import socket

"""
TCP会发生黏包，UDP不会
"""
sk = socket.socket(type=socket.SOCK_DGRAM)

sk.bind(('127.0.0.1', 8080))
msg, addr = sk.recvfrom(1024)
print(msg.decode('utf-8'))

while True:
    cmd = input("UDP SERVER >> ")
    if cmd == 'q':
        sk.sendto(cmd.encode('utf-8'), addr)
        break
    sk.sendto(cmd.encode('gbk'), addr)

    msg, addr = sk.recvfrom(10240)
    print(msg.decode('utf-8'))

    msg2, addr2 = sk.recvfrom(10240)
    print(msg2.decode('utf-8'))

sk.close()