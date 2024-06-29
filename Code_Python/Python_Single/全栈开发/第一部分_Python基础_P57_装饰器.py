# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-17 20:45
program     : 补习装饰器
"""#

import time

def wrapper(func):
    def inner():
        start = time.clock()
        func()
        print(time.clock()-start)
    return inner

# @wrapper
def example():  # 函数的定义
    print("哈哈哈")

# 这就是装饰器
# 利用了函数的特性:
#     1. 函数可以像普通变量一样赋值给别人
#     2. 函数可以作为参数传进别的函数中，也可以作为别的函数的返回值
#     3. 函数闭包的使用
# 下面一句就是装饰器的作用 用@只不过是省了这么一行字
# example = wrapper(example)
# 即不改变函数原来的调用方式 又动态的给函数添加功能

example()  # 函数的调用

# 开放 封闭原则
# 1. 对添加新功能是开放的
# 2. 对于硬改已经存在的代码是封闭的
# 装饰器的作用就是在遵循开放封闭原则
# 对原有的代码不硬改，又添加了新的功能
# 同时又让原来调用了这个函数的代码无需修改调用方式
