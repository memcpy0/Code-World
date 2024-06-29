# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-29 01:17
program     : 学习使用socketserver模块开启多线程
"""#
import socketserver

"""
BaseServer

TCPServer(BaseServer)

class ThreadingTCPServer(ThreadingMixIn, TCPServer): pass
    ThreadMixin  process_request_thread process_request

# 第一 多个类之间的继承关系要先整理 
# 第二 每一个类中有哪些方法, 要大致列出来
# 第三 所有的self对象调用要清楚地了解 到底是谁的对象
# 第四 所有的方法调用要退回到最子类的类中开始寻找 逐级向上
"""
# socket TCP 只能和一个客户端通信
# socketserver TCP 和多个客户端通信
import socketserver

class MyServer(socketserver.BaseRequestHandler):

    def handle(self):  # self.request 就相当于一个conn
        # print(self.request.recv(1024))
        while True:
            msg = self.request.recv(1024)
            if msg == b'q':
                break
            print(msg.decode('utf-8'))
            info = input('Server >>> ')
            self.request.send(info.encode('utf-8'))

if __name__ == '__main__':
    server = socketserver.ThreadingTCPServer(('127.0.0.1', 8080), MyServer)
    # thread 线程
    server.serve_forever()
