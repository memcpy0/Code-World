# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-29 12:53
program     :  

"""#

import socket
ip_port = ('127.0.0.1', 8080)

sk = socket.socket()
sk.connect(ip_port)

while True:
    msg = input('Client >>> ')
    if msg == 'q':
        sk.send(b'q')
        break
    sk.send(msg.encode('utf-8'))
    info = sk.recv(1024)
    print(info.decode('utf-8'))

sk.close()