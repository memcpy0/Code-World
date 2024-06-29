# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-07 19:14
program     : 学习multiprocessing的Manager类
"""

from multiprocessing import Manager, Process, Lock

"""
Pipe 管道 双向通信 数据不安全
Queue 管道 + 锁 双向通信 数据安全
JoinableQueue 数据安全
    put和get的一个计数机制 每次get数据之后发送task_done put端接受到计数-1
    直到计数为0就能感知到
Manager是一个类 提供了可以数据共享的一个机制 提供了很多数据类型 dict list pipe
    不提供数据安全的机制
"""

def func(dic):
    print(dic)
    # time.sleep(3)

    dic['count'] -= 1

def function(i):
    print(i)

if __name__ == "__main__":
    m = Manager()
    # lock = Lock()
    d = m.dict({'count':100})

    l = []
    for i in range(100):
        p = Process(target=function, args=(i,))
        p.start()
        # l.append(p)
        # p.join()   # 变成了同步执行 等待
    [p.join() for p in l]  #  异步执行 同步等待
    print(d)

