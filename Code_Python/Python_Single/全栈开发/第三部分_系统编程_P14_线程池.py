# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-11 17:11
program     : 学习线程池
"""#

"""
concurrent.futures 统一了入口和方法 简化了操作
使用concurrent.futures开启线程池
可以拿到返回值
"""

from concurrent import futures
import time
import random

def func(n):
    print(n)
    time.sleep(random.randint(1, 3))
    return '*'*5

def call(args):
    print(args.result())

thread_pool = futures.ThreadPoolExecutor(5) # 默认是CPU个数*5 20个线程
"""
lst = []
for i in range(10):
    # submit 合并了创建线程对象和start的功能
    f = thread_pool.submit(func, 'hello, world')
    # print(f.result())  # 成了同步串联 每次等f执行完拿到结果
    lst.append(f)

thread_pool.shutdown()   # close() join()

for f in lst:  # 按顺序添加 按顺序打印
    print(f.result())  # f.result() 阻塞 等f完得到结果
"""
# thread_pool.map(func, range(10))  # 天生异步 接受可迭代对象的数据 不支持返回值
thread_pool.submit(func, 1).add_done_callback(call)