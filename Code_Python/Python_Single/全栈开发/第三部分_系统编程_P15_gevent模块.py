# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-11 20:06
program     : 学习gevent模块
"""

"""
协程可以实现socket聊天
# (cpucount+1) * CPU_count*5 * 500并发 == 50000
"""
from gevent import monkey; monkey.patch_all()
from threading import current_thread
import time
import gevent

def fun1():
    print(current_thread().name)  # DummyThread-1
    print(123)
    time.sleep(1)
    print(456)

def fun2():
    print(current_thread().name)
    print('haha')
    time.sleep(1)
    print('10jq')

# g1 = gevent.spawn(fun1)  # 遇到它认识的IO会自动切换的模块 不认识的不会
# g2 = gevent.spawn(fun2)
# gevent.joinall([g1, g2])

# --------------------------------------------------------------
# 效率测试
def task(arg):
    time.sleep(1)
    print(arg)

def sync_func():
    for i in range(10):
        task(i)

def async_func():
    g_1 = []
    for i in range(10):
        g_1.append(gevent.spawn(task, i))
    gevent.joinall(g_1)

start = time.time()
sync_func()
print(time.time()-start)

start = time.time()
async_func()
print(time.time()-start)
