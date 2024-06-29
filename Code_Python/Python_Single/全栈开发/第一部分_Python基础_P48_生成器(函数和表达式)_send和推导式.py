# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-14 0:35
program     : 探讨生成器本质、练习生成器函数和表达式、推导式
"""

"""
- 生成器：其本质就是迭代器
- 三种生成方法：
    1 通过生成器函数
    2 通过生成器表达式创建生成器
    3 通过数据转换
- 生成器函数: 当函数中出现了yield语句时，该函数就是生成器函数
"""
def func():
    # 1个yield语句
    print("周杰伦")
    a = yield '昆凌'  # 赋值在第一个yield执行之后才发生; 从外部send来
    print(a)
    print("王力宏")
    yield "李云迪"
    print("笛卡尔积")
    yield "笛卡尔"
    print("Nihaoa")

"""
__next__() 可以让生成器向下走一步
send()     也可以让生成器向下执行一次
"""

g = func()            # <generator object func at 0x000002367B3A5570>
print(next(g))        # 运行，从头走到第一个yield语句完
print(g.send(1))      # 从上一个yield走到下一个yield语句；向a发送一个值
print(g.__next__())   # 运行，从第一个yield语句之后走到第二个yield语句完

# return是直接返回结果，结束函数的调用
# yield 返回结果，可以让函数分段执行
# print(g.__next__())   # Nihaoa StopIteration  最后一个yield语句之后一般不写代码，尽管会执行

# for i in func():
    # 最后一个不报错，这揭示了for语句运行的内部机制：
    # 即对可迭代对象用iter()取其迭代器对象，在反复调用next()产生值，并用异常处理掉StopIteration返回
    # 而迭代器(包括生成器)的next()也是同样的执行模式
    # print(i)

def func2():
    print("w")
    a = yield 's'
    print(a)

    print('w')
    b = yield 'e'
    print(b)

    print('w')
    c = yield 'x'
    print(c)

l = func2()
print(l.__next__()) # 移动到第一个yield或者从一个yield移动到下一个yield
print(l.send(1))    # 先给上一个yield传值，然后则让生成器向下移动一次(只可以从上一个yield语句移动到下一个yield)
print(l.send(2))
# l.send(3)    # 先给c, 再要移动到下一个yield, 没有就StopIteration

#---------------------生成器表达式语法和推导式基本一样--------------------------------------------

ge = (i for i in range(10) if i % 2 == 0)  # 返回一个生成器即迭代器
print(list(ge))    # 对ge这些Iterable调用iter()再调用next()执行到尽头

ge2 = (i2 for i2 in ge)  # 已经到尽头，现在再iter()和next()只为空
ge3 = (i3 for i3 in ge)  # 同上
print(list(ge2))
print(list(ge3))

s = func2()     # 返回一个生成器即迭代器
print(list(s))  # 只能执行iter()和next()完，一次性
# w             # 从结果知道list()转换和for循环一样遵循迭代协议，即使用iter()和next()
# None
# w
# None
# w
# None
# ['s', 'e', 'x']   # list()又会将yield的值收集在列表中
g = (i for i in s)  # 生成器不能第二次使用
print(list(g))      # []

# for i in range(1, 10): pass
# print(i)