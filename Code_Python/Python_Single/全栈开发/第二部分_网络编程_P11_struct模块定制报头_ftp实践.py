# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-28 22:16
program     : struct模块定制报头/做大文件的上传和下载
"""

"""
# 我们在网络上传输的所有数据 都叫数据包
# 数据包里的所有数据 都叫报文
报文里不止有你的数据 IP地址 mac地址 端口号
所有的报文 都有报头
报头 协议 接受多少字节
我自己定制报文
    - 比如说 复杂的应用上就会用到
    - 传输文件的时候就够复杂了
        * 文件名
        * 文件大小
        * 文件类型
        * 存储的路径
"""
# head = {'filename':'test', 'filesize':409600,
#         'filetype':'txt',  'filepath':r'\user\bin'}

"""
# 先接受报头的长度  # 先接受4个字节
# send(num) 
# send(head)  # 报头   # 接受这4个字节获取报头
# send(file)  # 从报头中获取filesize, 然后根据filesize接受文件

其实 在网络传输的过程中 处处有协议
协议就是一堆报文和报头 --字节
协议的解析过程我们不需要关心
"""
# 大文件上传下载
import socket
import struct
import json

sk = socket.socket()
ip_port = ("127.0.0.1", 8090)
buffer = 1024

sk.bind(ip_port)
sk.listen(8)

conn, addr = sk.accept()

# 接收报头的长度
head_len = conn.recv(4)
head_len = struct.unpack('i', head_len)[0]

json_head = conn.recv(head_len).decode('utf-8')
head = json.loads(json_head)

filesize = head['filesize']

with open(head['filename'], 'wb') as f:
    while filesize:
        if filesize >= buffer:
            content = conn.recv(buffer)
            f.write(content)
            filesize -= buffer
        else:
            content = conn.recv(filesize)
            f.write(content)
            break
conn.close()
sk.close()