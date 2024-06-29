# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-17 23:20
program     : 学习多继承的各种情况
"""
"""
继承的进阶：
继承：单继承和多继承

类：新式类、经典类
- 新式类：凡是继承object的都是新式类，python3中的类都是新式类
- 经典类：不继承object的类都是新式类
    * python2经典类和新式类混合，所有的类都默认不继承object，为经典类，也可以选择继承object

单继承：经典类和新式类一样的继承顺序 A B(A) C(B) : C -> B -> A -> object
多继承：新式类：遵循广度优先: 每个节点有且只走一次
       经典类：遵循深度优先: 一条道走到黑      
           A
       B       C
       ^_D   E
        ^_ F
"""
class A:
    def func(self):
        print("In A")
class B(A):
    pass
    # def func(self):
    #     print("In B")
class C(A):
    pass
    # def func(self):
    #     print("In C")
class D(B):
    pass
    # def func(self):
    #     print("In D")
class E(C):
    pass
    # def func(self):
    #     print("In E")

class F(D, E):
    pass
    # def func(self):
    #     print("In F")

print(F.__mro__, F.mro(), sep='\n')
f1 = F()
f1.func()
