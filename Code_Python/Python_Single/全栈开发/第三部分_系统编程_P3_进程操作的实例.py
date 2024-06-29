# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-06 18:23
program     : 学习用进程来开启服务器的多服务
"""

# import os, glob, sys, subprocess, threading
# from multiprocessing.dummy import Pool
from multiprocessing import Process
import socket

def chat(connection):
    connection.send(b'hello')
    res = connection.recv(1024)
    print(res)

if __name__ == "__main__":
    sk = socket.socket()
    sk.bind(('127.0.0.1', 62222))

    sk.listen(5)

    while True:
        conn, addr = sk.accept()
        p = Process(target=chat, args=(conn,))
        p.start()
    conn.close()
    sk.close()