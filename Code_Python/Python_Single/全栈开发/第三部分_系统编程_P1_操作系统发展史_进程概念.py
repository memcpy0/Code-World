# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-29 14:49
program     : 学习操作系统和进程概念
"""#

"""
# 多道程序操作 遇见IO就切 
    操作系统开始成熟 先后出现了作业调度管理、处理机管理、存储器管理、外部设备管理、文件系统管理……
      CPU工作 --_--_--    -A CPU _B CPU
      IO 工作   -__-      -A IO  _B IO 

    由于多个程序同时在计算机中运行, 开始有了空间隔离的概念, 只有内存空间的隔离, 才能保持程序的安全运行

# 分时系统
    1s分成3份 叫做时间片 轮转 时分复用

# 实时系统 对一个任务实时响应
    优点 快
    缺点 能处理的任务少
  
# 通用操作系统 兼具了多种系统的特性

# 进程
    - 程序
    - 只有在运行当中的程序 -> 进程
    - 为什么要引入进程的概念 -> 隔离资源
    - 进程是操作系统中资源分配的最小单位
    
    - 进程的调度算法:
      * 先来先服务 FCFS
      * 短作业优先算法 
      * 时间片轮转算法
      * 多级反馈算法
        -进程 Process
        - 1 = [p3, p4]
        - 2 = [p1, p2]
        - 3 = []
    
    - 现代操作系统中

# 并行 多者同时执行 多核 -> 微观上 要求有多个处理器
# 并发 多者交替执行     -> 宏观上

# 阻塞   input accept recv 
# 非阻塞 
# 同步   只管调度发指令 不等结果
# 异步   调度之后 一定等到结束
    
    - 同步阻塞
    - 异步阻塞
    - 同步非阻塞
    - 异步非阻塞
    
# 进程 新进程的创建是由一个已经存在的进程执行了一个创建进程的系统调用创建的
  - 进程的结束
    * 正常的退出
    * 出错退出
    * 严重错误
    * 被杀死
"""
import os
import time
from multiprocessing import Process

# print(os.getpid())  # 获取当前运行的Python程序的进程id
# print(os.getppid())  # parent process id 获取当前进程的id

# multiprocessing 处理进程的一个综合的封装
# Process和创建进程相关
# 同一个进程的确是从上到下执行的

def func(money):
    print('子进程', os.getpid())
    print('父进程', os.getppid())
    print(123)

if __name__ == '__main__':
    p = Process(target=func, args=(100000,))
    print('主进程', os.getpid())
    p.start()      # 直到执行了start才有了进程 给操作系统一个指令, 操作系统什么时候执行与我无关

    time.sleep(10) # 异步阻塞主进程 但不影响子进程 子进程先执行完毕
    print('主进程', os.getpid())
