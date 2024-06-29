# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-29 00:26
program     : hmac的检验客户端合法性和socketserver模块
"""#

import os
import socket
import hmac  # hashlib

secret_key = 'egg'
sk = socket.socket()
sk.bind(('127.0.0.1', 8080))
sk.listen(8)

def check_conn(conn):
    msg = os.urandom(32)
    conn.send(msg)
    h = hmac.new(secret_key, msg)  # secret_key 想加密的bytes
    digest = h.digest()
    client_digest = conn.recv(1024)
    return hmac.compare_digest(digest, client_digest)

conn, addr = sk.accept()
res = check_conn(conn)
if res:
    print("合法的客户端, 您可以登录了")
else:
    print("不合法的客户端!")

# ------------------------------------------------
sk = socket.socket()
secret_key = 'egg'
sk.connect(('127.0.0.1', 8080))
msg = sk.recv(1024)
h = hmac.new(secret_key, msg)
digest = h.digest()
sk.send(digest)

sk.close()