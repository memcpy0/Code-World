# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-28 19:18
program     : 基于TCP实现远程执行命令, 在server端下发命令; 体验粘包现象
"""#

import socket
import struct
"""
TCP会发生黏包，UDP不会
TCP有拆包机制, 面向连接, 面向流的, 提供高可靠性服务
面向流的通信是无信息保护边界的
当(本质就是)接受端不确定数据长度时, 过长而几个recv过小就会发生黏包, 
多个send间隔很短且数据过短也会(缓冲机制 TCP协议的优化算法)

解决黏包问题:
# 先告诉接受段要发送的数据大小
# 首先 发送一下这个数据到底有多大
# 再根据数据大小接受数据
"""
sk = socket.socket()
sk.bind(('127.0.0.1', 8080))
sk.listen(8)

conn, addr = sk.accept()

while True:
    cmd = input("Server Cmd >>> ")
    if cmd == 'q':
        conn.send(b'q')
        break
    conn.send(cmd.encode('gbk'))

    length = conn.recv(4) # .decode('utf-8') # 4
    # conn.send(b'ok')
    num = struct.unpack('i', length)[0]
    info2 = conn.recv(num).decode('gbk')
    print(info2)

conn.close()
sk.close()

