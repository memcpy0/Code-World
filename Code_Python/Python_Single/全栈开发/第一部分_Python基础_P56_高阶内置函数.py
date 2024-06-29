# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-17 13:15
program     : 练习lambda、sorted、filter、map等高阶函数
"""

# -----------------------------------------------------------------------------
"""
lambda 匿名函数
语法：
* 函数名 = lambda 参数: 返回值
注意：
1. 函数的参数可以有多个，用逗号隔开
2. 匿名函数不管有多复杂，且逻辑结束后直接返回数据
3. 返回值和正常的函数一样，可以是任意数据类型，如数值、列表、元组、生成器
4. 所有的匿名函数的__name__名字都是lambda
5. 调用时没有什么特别之处，像正常的函数调用即可
"""
# x 参数 # : 后面是函数体，直接return的内容
a = lambda x: x+x  # 一行搞定一个函数，但是无法完成复杂的函数操作

print(a.__name__)  # 查看函数名  # <lambda>
print(a(2))        # 调用匿名函数 # 4

b = lambda x, y, z: (x+1, y+2, z+3)
print(b(1, 1, 1))  # [2, 3, 4] (2, 3, 4))

c = lambda x, y, z: (i*2  for i in (x, y, z))
print(c(1, 1, 1))  # <generator object <lambda>.<locals>.<genexpr>

# -----------------------------------------------------------------------------
"""
sorted 排序函数
语法：
* sorted(iterable, key=None, reverse=False)
    - key: 排序函数，sorted使用排序函数对可迭代对象中的每个元素求值，根据值来排序
    - reverse: 默认升序，True为降序
注意：
1. 是list.sort()的通用版本
"""
lst = [1, 3, 4, 2, 0, 6, 8]
lst.sort()   # 对列表原地改动
print(lst)
print(sorted(lst, reverse=True))  # 返回新列表

# 自定义排序规则
lst2 = ['大秧歌a', '小智aa', '魔化晓美焰aaaa', '尼尔机械纪元aaa', '倚天屠龙aaaaaaa']
# ['小智', '大秧歌', '倚天屠龙', '魔化晓美焰', '尼尔机械纪元']
print(sorted(lst2, key=len))

lst3 = ['大秧歌a', '小智', '魔化晓美焰aaaa', '尼尔机械纪元aaa', '倚天屠龙aaaaaaa']
# ['小智', '大秧歌a', '尼尔机械纪元aaa', '魔化晓美焰aaaa', '倚天屠龙aaaaaaa']
print(sorted(lst3, key=lambda x: x.count('a')))

# -----------------------------------------------------------------------------
"""
filter 筛选函数
语法：
* filter(function, iterable) -> Iterator
    - function: 对iterable中的每个元素求值，根据值的真假，为True则返回
注意：
1. 返回迭代器
"""
lst4 = [1, 2, 3, 4, 5, 6, 7, 8, 9]
res = filter(lambda x: x % 2, lst4)
from collections import Iterator
print('__iter__' in dir(res))     # True
print(isinstance(res, Iterator))  # True
print(list(res))              # [1, 3, 5, 7, 9]

# -----------------------------------------------------------------------------
"""
map 映射函数
语法：
* map(function, iterable1, [iterable2, [……)  -> Iterator
    - function: 对可迭代对象的每一个元素求值，处理的结果一迭代器返回
    - 如果有多个可迭代对象时，function应该接受多个参数，且几个可迭代对象中元素相对应
"""
lst5 = [1, 3, 4, 5, 6, 7]
it = map(lambda x: x**2, lst5)
print(list(it))   # [1, 9, 16, 25, 36, 49]

lst6 = [1, 3, 4, 5, 6, 7]
it2 = map(lambda x, y: x+y, lst5, lst6)
print(list(it2))  # [2, 6, 8, 10, 12, 14]