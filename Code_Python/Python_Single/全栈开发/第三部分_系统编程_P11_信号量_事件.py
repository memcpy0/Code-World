# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-11 15:36
program     : 学习信号量、事件
"""#

from threading import Semaphore, Thread
from threading import Event
import time, random

def func(n, sem):
    sem.acquire()
    print("thread -%s start" % n)
    time.sleep(1)
    print("thread -%s end" % n)
    sem.release()

sem = Semaphore(3) # 一把锁 多把钥匙

sem.acquire()
sem.acquire()
sem.acquire()
print(111)
sem.release()
sem.release()
sem.release()

for i in range(20):
    Thread(target=func, args=(i, sem)).start()

"""
信号量 与 线程池
相同点 在信号量acquire之后，和线程池一样 同时在执行的只能有n个
不同点 # 开的线程数不一样，线程池来说，一个就5个线程，信号量可以有几个任务就开几个线程
      # 对有信号量限制的程序来说 可以进行多个线程
"""

"""
Event 标志 flag 
    # 刚刚创建的时候 flag=False 阻塞
    # wait() flag=False 阻塞 
             flag=True  非阻塞
    # set()   flag --> True 
    # clear() flag --> False
"""

def conn_mysql():
    count = 1
    while not e.is_set():
        if count > 3:
            raise TimeoutError
        print("尝试连接第%s次" % count)
        count += 1
        e.wait(0.5) # 一直阻塞变成了只阻塞0.5
    print("连接成功")

# 检测数据库的连接是否正常
def check_conn():
    # 模拟连接检测的时间
    time.sleep(random.randint(1, 3))
    e.set()


e = Event()
check = Thread(target=check_conn)
check.start()

conn = Thread(target=conn_mysql)
conn.start()
