# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-29 16:22
program     : 体会子进程和父进程的异步执行
"""#
import os, time
from multiprocessing import Process
# multiprocessing 处理进程的一个综合的封装
# Process和创建进程相关
"""
在同一个进程中 程序确实是从上到下依次执行
- 开启进程的方法一：
    创建进程对象 传要执行的函数和参数
    使用start()使用进程对象
    join就是将主进程阻塞到join的位置 等待P进程结束

主进程和子进程异步执行
    如果主进程中的代码已经结束了 子进程还没结束 程序会等待子进程结束

守护进程
    守护进程会随着主进程的代码执行结束而结束
    而正常的子进程没有执行完的时候主进程要一直等着
    - 设置一个守护进程
        p.daemon = True 进程开启之前设置
    - 守护进程中不能开启子进程 不过这个没什么意义
    - 守护进程不会等待其他子进程的结束

- 开启进程的方法二：
    创建类对象 继承Process
    实现一个run方法
    实例化自己的类对象 调用start() 进程会自动执行自己写的run()
    
    如果在子类(子进程)中定义了自己的方法, 不能通过类直接调用 应该在run方法中self.method()
不然该方法会直接在主进程中调用 而非在子进程中执行

- 进程的其他方法
    process.is_alive()  # 是否活着 True代表进程还在 False代表进程不在了
    process.terminate() # 异步操作 让操作系统结束进程 但是不一定会立刻结束 
                          只是给出一个指令 接着干别的事 继续主进程
- 进程的其他属性
    pid  查看这个进程的ID 
    name 查看这个进程的名字
    
"""

def func(money):
    print('父进程', os.getppid(), '子进程', os.getpid())
    # print(123)
    # time.sleep(3)
    print("取钱: %s" % money)

n = 100
def func2():
    global n
    n = n - 1
    print(n)

class MyProcess(Process):

    def __init__(self, arg1, arg2, arg3):
        super().__init__()
        self.arg1 = arg1
        self.arg2 = arg2
        self.arg3 = arg3

    def run(self):
        print('MyRUN子进程:', os.getpid(), self.arg1, self.arg2, self.arg3)
        self.walk()

    def walk(self):
        print("MyWalk子进程:", os.getpid())

def timer():
    while True:
        time.sleep(1)
        print("过去了1秒")

if __name__ == '__main__':
    print('-----主进程begin-----', os.getpid())

    # p1 = Process(target=func, args=(1,))
    # p1.start()  # 直到执行了start才有了进程 给操作系统一个指令, 操作系统什么时候执行与我无关 我不知道操作系统先启动哪个进程 它先启动哪个我不知道
    # # p1.join()   # 又异步阻塞主进程 但不影响子进程
    #
    # p2 = Process(target=func, args=(2,))
    # p2.start()
    #
    # p3 = Process(target=func, args=(3,))
    # p3.start()

    p_lst = [] # 使用列表管理 达到既使用进程达成异步 又在最后执行主进程
    # for循环启动多个进程
    for i in range(10):
        p = Process(target=func, args=(i,))
        p.start()
        p_lst.append(p)
        # p.join() # 这样就相当于同步了 没什么意义
    # p.join() # 这样就相当于等待第九个进程结束后在执行主进程 至于其他的子进程的顺序仍然不可知

    for p in p_lst:
        p.join() # 这里每个进程已经同时异步进行了 只是要等待每个进程都结束再执行主进程
    print('-----主进程end------', os.getpid())
    print("取完钱了")

    my_process = MyProcess(1, 2, 3)
    my_process.start()
    # my_process.walk() # MyWalk子进程: 21952 直接在主进程中调用了

    """
    主进程与子进程间的数据隔离
    - 正常情况下，进程和进程之间的数据是完全隔离的
        * 主进程和子进程数据隔离
        * 子进程和子进程数据隔离
    - 新开了一个子进程时，相当于重新执行了一次模块的非自测代码
    
    """
    # n = 100 NameError: name 'n' is not defined
    for i in range(10):
        p = Process(target=func2)
        p.start()
    print("主进程", n)  # 主进程 100

    protect_p = Process(target=timer)
    # protect_p.daemon = True  # 一定要在开启进程之前设置
    protect_p.start()

    for j in range(100):
        time.sleep(.1)
        print('*'*j)

    if protect_p.is_alive():
        protect_p.terminate()
        print(protect_p.is_alive())  # True 异步操作