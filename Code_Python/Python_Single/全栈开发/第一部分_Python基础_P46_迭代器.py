# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-14 0:06
program     : 探讨可迭代对象、迭代器
"""
# 使用dir()来看数据类型中是否有__iter__方法(支持iter()内置函数)确定是否是Iterable
print('__iter__' in dir(list))  # True
# 使用dir()来看数据类型中是否有__next__方法(支持next()内置函数)确定是否是Iterator
print('__next__' in dir(list))  # False

# 对可迭代对象使用__iter__()或iter()会返回一个迭代器
lst = ['anal', 'sex', 'book']
it = iter(lst)  # lst.__iter__()
# 模拟for循环
while True:
    try:
        name = next(it)  # it.__next__()
        print(name)
    except StopIteration:     # 拿完了
        break

from collections import Iterable, Iterator
print(isinstance(lst, Iterable))  # True
print(isinstance(lst, Iterator))  # False

print(isinstance(it, Iterable))   # True # 判断是否是可迭代的 迭代器一定是可以迭代的
print(isinstance(it, Iterator))   # True # 迭代器里一定要有__next__(), 同时也一定要有__iter__()
# 所以迭代器也一定是可迭代对象，对迭代器使用iter()返回的迭代器是它本身

# 可迭代对象 str list tuple dict f
# 迭代器 f(文件句柄)
f = open('./P13_编码体系 编码和解码.py')
print(isinstance(f, Iterator))  # True

"""
迭代器的特点:
1 节省内存
2 惰性机制
3 只能往前拿，不能倒退
"""