# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-28 19:54
program     : 学习使用UDP来发送命令; 解决黏包现象
"""#
import socket
import subprocess

sk = socket.socket(type=socket.SOCK_DGRAM)

ip_port = ('127.0.0.1', 8080)
sk.sendto('开始命令吧!'.encode('utf-8'), ip_port)

while True:
    cmd, addr = sk.recvfrom(10240)
    if cmd == b'q':
        break
    ret = subprocess.Popen(cmd.decode('gbk'), shell=True,
                           stdout=subprocess.PIPE,
                           stderr=subprocess.PIPE)
    std_out = "stdout: " + ret.stdout.read().decode('gbk')
    std_err = "stderr: " + ret.stderr.read().decode('gbk')
    print(std_out, std_err, sep='\n')

    sk.sendto(std_out.encode('utf-8'), addr)
    sk.sendto(std_err.encode('utf-8'), addr)

sk.close()