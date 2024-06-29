# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-28 23:25
program     : Client发送端
"""

import os
import json
import socket
import struct

sk = socket.socket()
ip_port = ("127.0.0.1", 8090)
buffer = 4096
sk.connect(ip_port)

# 发送文件的报头
head = {'filename':r'神T夏L克 S01E01中英双字.rmvb',
        'filepath':r'D:\夏洛克',
        'filesize':None}

filepath = os.path.join(head['filepath'], head['filename'])
filesize = os.path.getsize(filepath)
head['filesize'] = filesize

json_head = json.dumps(head)  # 字典转换成了字符串
bytes_head = json_head.encode("utf-8")

# 计算长度
head_len = len(bytes_head)
pack_len = struct.pack('i', head_len)

sk.send(pack_len)  # 先发包头的长度
sk.send(bytes_head)  # 再发送字节形式的报头

with open(filepath, 'rb') as f:
    while filesize:
        if filesize >= buffer:
            content = f.read(buffer)
            sk.send(content)
            filesize -= buffer
        else:
            content = f.read(filesize)
            sk.send(content)
            break
sk.close()
