# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-28 19:18
program     : 基于TCP实现远程执行命令, 在server端下发命令; 体验粘包现象
"""#

import socket
import struct
import subprocess

sk = socket.socket()
ip_port = ('127.0.0.1', 8080)
sk.connect(ip_port)

while True:
    cmd = sk.recv(1024).decode('gbk')
    if cmd == 'q':
        break
    ret = subprocess.Popen(cmd, shell=True,
                          stdout=subprocess.PIPE,
                          stderr=subprocess.PIPE)
    std_out = ret.stdout.read()
    std_err = ret.stderr.read()
    len_num = len(std_out) + len(std_err)
    num = struct.pack('i', len_num)
    sk.send(num)   # 4 2048 # str(len(std_out)+len(std_err)).encode('utf-8'))
    # sk.recv(1024)  # ok
    sk.send(std_out)
    sk.send(std_err)

sk.close()

"""
# 好处: 准确收到信息
    - 要走文件中配置一个配置项, 就是每一次recv的大小 buffer=4096
    - 当我们要发送大文件的时候, 要明确的告诉接受方要发送多大的数据, 
    以便于接受方能够准确的接受到所有数据
    - 多用在文件传输的过程中
      # 大文件的传输 一定是按照字节度 每一次度固定的字节
      # 传输的过程中 一边读一边写 接受端一边收一边写
# 坏处：多了一次交互
"""