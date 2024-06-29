# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-11 16:33
program     : 学习线程模块的定时器和条件
"""#

from threading import Timer, Condition, Thread
import time

def hello():
    while True:              # 这个线程一直在
        time.sleep(1)
        print("hello world")

"""
while True:         # 每隔一段时间要开启一个线程
    t = Timer(1, hello)  # 定时开启一个线程 执行一个任务
    t.start()            # 定时 多久之后 second 要执行的任务: 函数名

# sleep的时间
# 短 就在线程中while True
# 长 就在主线程中while True
"""
t = Timer(1, hello)


"""
条件 = 锁+wait的功能
"""
def run(n):
    con.acquire()
    con.wait()      # 等着notify的信号
    print("run the thread:%s" % n)
    con.release()

if __name__ == "__main__":
    con = Condition()

    for i in range(10):
        t = Thread(target=run, args=(i,))
        t.start()

    while True:
        imp = input()
        if imp == 'q':
            break

        con.acquire()  # 递归锁
        if imp == 'all':
            con.notify_all()

        con.notify(int(imp))   # 传递信号 notify(1) --> 可以放行一个线程
        con.release()
        print("-------------")
