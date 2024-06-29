# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-07 21:20
program     : 学习线程和在Python中使用
"""

import os
import threading
from threading import Thread
# import _thread as thread

"""
线程也有自己的独立空间

线程和进程之间的关系:
区别:
  - 地址空间和其他资源: 进程间相互独立，同一进程中的线程之间可以相互共享
  - 通信: 进程之间通信IPC，线程中可以直接读写进程数据段(如全局变量)
  - 调度和切换: 线程上下文切换比进程上下文要快得多
  - 在多线程操作系统中，进程不是一个可执行的实体
    * 线程是CPU调度的最小单位，是能独立运行的基本单位
    * 进程是资源分配的最小单位
进程包含着线程    

程序类型:
  - 高计算 无等待
  - 高IO  多等待 阻塞 涉及到网络 文件 就有阻塞

GIL 全局解释器锁 CPython解释器 是解释器 

使用模块：thread和threading

开启多线程的两种方法 函数和类
threading.enumerate(), threading.activeCount()
threading.currentThread(), threading.current_thread().getName(),
threading.current_thread().name, threading.current_thread().ident
"""

class MyThread(Thread):
    count = 0
    def __init__(self, i):
        super().__init__()
        self.j = j

    def run(self):
        MyThread.count += 1  # 多个子线程共享这个属性
        print('hello %s, thread world' % self.j, os.getpid())

def func(i):
    # time.sleep(1)
    print('hello %s, thread world' % i, os.getpid(), end=' ')
    print(threading.currentThread(), threading.current_thread().getName(),
          threading.current_thread().name, threading.current_thread().ident)

thread_list = []

# 不需要写在自测代码中了，因为进程的子进程会重新执行顶层代码 而子线程只会执行指定的函数 同时共享内存空间
for i in range(10):
    t = Thread(target=func, args=(i,))
    t.start()
    thread_list.append(t)
    # t.join()  # 这样就跟同步一样了 没意义了!

# 返回正在执行的线程列表 和 数目
print(threading.enumerate(), threading.activeCount())

[t.join() for t in thread_list]  # 仍然异步开启多线程 但与主线程同步了

print(os.getpid())

# -----------------------------------
for j in range(10):
    p = MyThread(j)
    p.start()
    # print(p.count)

print(threading.enumerate(), threading.activeCount())
