# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-13 20:43
program     : 了解一等公民的函数(函数名作用)、函数嵌套和闭包、迭代器
"""
"""
一等公民：
    *1 函数名可以像普通变量一样传递，赋值给其他变量；
    *2 函数可以作为其他函数的参数；
    *3 函数也可以被其他函数作为返回值；
    *4 函数可以嵌套
"""

#-------------------------函数名可以像普通变量一样传递-----------------------------------
def func():
    print("今天你SYD了吗？")

# print(func)  # <function func at 0x000001BF49F43E18> #变量无类型，对象有类型
# a = func     # <function func at 0x00000184E0D73E18> 指向同一个函数对象
# print(a)
#
# func()  # 今天你SYD了吗？
# a()     # 今天你SYD了吗？ # a()就是一个函数的调用


#-------------------------函数内部多重嵌套---------------------------------------------
def func1():
    print('Z')           # 1
    def func2():
        print('Q')       # 3
        def func3():
            print('S')   # 5
        print('L')       # 4
        func3()
    print('ZH')          # 2
    func2()
# func1()

#-------------------------函数也可以被其他函数作为返回值---------------------------------
"""
这里的闭包使用是用到了函数的特性：
*4 函数可以嵌套
*3 函数也可以被其他函数作为返回值；
*1 函数名可以像普通变量一样传递，赋值给其他变量；
以及 在内部函数中调用外部函数的变量
"""
def outer():
    name = 'alex'
    def inner():
        # 在内部函数中调用了外部函数的变量，称为闭包，可以让一个局部变量常驻内存、提高效率
        print(name)
    return inner
# outer()()  # alex
# ret = outer()
# ret()        # alex
# ret()

#-------------------------闭包的好处-----------------------------------------------------
from urllib.request import urlopen
def out():
    content = urlopen('http://www.521609.com/daxuexiaohua/').read()
    def get_content():
        return content  # 在函数内部使用了外部的变量，闭包
    # (<cell at 0x000001E339820C48: bytes object at 0x000001E33BB4DAC0>,)
    # 内层函数是闭包就会返回对象，不然就为None
    print(get_content.__closure__)
    return get_content

fn = out()      # get_content # 这时就开始加载内容
# 后面需要用到这里面的内容就不需要再执行耗时的网络连接操作了
content = fn()  # 获取内容
print(content)
content2 = fn()  # 再次获取内容
print(content)






