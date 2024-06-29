# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-11 17:30
program     : 学习协程的基本概念 greenlet gevent
"""#

"""
使用多线程和多进程是有成本的
如果可以在一个线程中实现并发 就要使用协程
如果一个线程中的两个函数可以切换运行 就是并发

对于协程来说 始终是在一个线程中的函数间切换 不涉及到同步的概念

如何设计切换呢?
"""
import greenlet  # 在单线程中切换状态的代码模块
import gevent

def func1():
    print(1)
    yield
    print(3)
    yield

def func2():
    g = func1()
    next(g)
    print(2)
    next(g)
    print(4)

# func2()

"""
使用协程实现生产者消费者模型
"""
def consumer():
    while True:
        n = yield  # 3
        print("消费了一个包子%s" % n)

def producer():
    g = consumer()  # 1
    next(g)         # 2
    for i in range(10):
        print("生产了包子%s" % i) #
        g.send(i)   # 4

# producer()

"""
greenlet是底层模块 只负责切换 不能规避IO阻塞时间
gevent内部封装了greenlet 协程模块

协程的效率与正常程序的效率谁高呢?
在代码之间切换执行 反而会降低效率 切换不能规避IO时间
"""
def eat1():
    print("吃")
    g2.switch()
    print("吃2")
    g2.switch()

def eat2():
    print("饺子")
    g1.switch()
    print("白切鸡")

g1 = greenlet.greenlet(eat1)
g2 = greenlet.greenlet(eat2)
g1.switch()

def fun1():
    print(123)
    gevent.sleep(10)
    print(456)

def fun2():
    print('haha')
    gevent.sleep(10)
    print('10jq')

g1 = gevent.spawn(fun1)  # 遇到它认识的IO会自动切换的模块 不认识的不会
g2 = gevent.spawn(fun2)
# g1.join()
# g2.join()
gevent.joinall([g1, g2])  # 等同于上两步
