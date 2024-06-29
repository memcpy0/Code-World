# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-10 00:25
program     : 学习线程的锁
"""

"""
即使有了GIL锁 线程还不一定是安全的
因为时间片的轮转问题 时间片的轮转时间是很微小的
GIL锁只会锁住线程 而非数据
"""

from threading import Thread, Lock
import time

def func():
    global n
    # n -= 1 不会不安全
    lock.acquire()
    temp = n  # 从进程中获取n
    time.sleep(0.01)  # 执行了一些代码
    n = temp - 1      # 得到结果 在存储回进程
    lock.release()

n = 100
lock = Lock()
t_lst = []
for i in range(100):
    t = Thread(target=func)
    t.start()
    # t.join()  # 同步线程 都是0 但是时间耗费久 同步串行共100*0.01=1s
    t_lst.append(t)

[t.join() for t in t_lst] # 异步线程 不会有100*0.01=1s的情况 同步阻塞 强制每一个线程都真正执行完毕 发挥出作用
# 都是0
# 但在上面的temp情况中同样会失误 比如从全局变量取得某数据 赋值给其他变量 做了一些操作 在写回的情况下会出现数据不安全
# 为此 要加锁 # 但慢 lock和join好像没区别 可是锁可以对线程的时间执行更精细的控制
# 不会像在for循环中join 出现100个线程串行(同步)time.sleep(2) 100*2共200秒的事

print(n)

"""
科学家吃面问题 死锁
"""
chopsticks = Lock()
m = Lock()


