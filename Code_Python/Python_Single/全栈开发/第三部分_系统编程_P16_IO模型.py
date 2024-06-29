# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-11 20:50
program     : 学习IO模型
"""

"""
IO模型
    - 阻塞IO blocking IO      工作效率低
    - 非阻塞IO nonblocking IO 工作效率更高 CPU多次请求负担
    - IO多路复用  操作系统进行IO多路复用的机制 select 这个模型可以监听多个socket对象 
                 能够有效减少阻塞带来的时间损耗 且在一定程度上减少CPU的负担
    - 信号驱动IO
    - 异步IO      工作效率高 CPU负担少
"""
import socket

sk = socket.socket()
sk.bind(('127.0.0.1', 8080))
sk.listen(5)
sk.setblocking(False) # non-blocking
while True:
    try:
        conn, addr = sk.accept()
        msg = conn.recv(1024)  # 这里的recv不会阻塞 多并发 但是连接无法保存下来
        print(msg)
    except BlockingIOError:
        print("BLockingIOError")

