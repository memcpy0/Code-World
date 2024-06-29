# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-16 18:51
program     : 学习functools模块的偏函数
"""#

from functools import partial
import itertools

# import os
# print(os.path.dirname(__file__))
# print(os.path.abspath(__file__))

# 偏函数 指定一个默认参数 包装成另一个函数
print(int("10000", base=2))
print(int("1000", base=2))
print(int("100", base=2))
print(int("10", base=2))

int2 = partial(int, base=2)
print(int2("10000"))
