# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-19 22:52
program     : 全面复习面向对象
"""

"""
# 面向对象：
    - 什么时候用面向对象
      * 处理复杂的对象关系
        如复杂的电商程序；
          公司/学校的人事管理/功能的系统
          QQ好友 陌生人 群 组
    - Python中一切皆对象
      * 基础数据类型 都是对象
    - 类型type和自定义类的关系: 类型和类是一个东西
      所有的类包括type都继承自object，所有的类包括object都是由type实例化(创建)的
    - 创建一个类
      * class 类名 语法级别的
      * 类也是被创建的，type创建类
      * type是所有类的元类，object是所有类的父类
      * class A(metaclass=ABCMeta) ABCBeta创建了这个类，那么ABCMeta就是A的元类
    - 创建一个对象
      * 类名() 实例化
      * __new__() 创造了一个对象的空间
      * 之后做__init__()初始化
    
    **type(obj)的结果就是这个对象所属的类
    **type(类) 的结果就是创建这个类的元类，大多数情况就是type，除非你指定ABCMeta

# 类 class LeiMing
    - 类是什么时候被加载的，类名是什么时候生效的(创建一个类名引用指向类空间)
    - 类
      * 静态属性/静态字段/静态变量
"""

# print(issubclass(type, object))  # True
# print(isinstance(object, type))  # True
# print(type(type))                # <class 'type'>
# print(type(object))              # <class 'type'>

from abc import ABCMeta
class B(metaclass=ABCMeta):pass
# print(type(B))                   # <class 'abc.ABCMeta'>

class C(metaclass=type):
    pass
# print(type(C))                   # <class 'type'>

class Person:
    Role = 'alxe'
    # print(Person.Role)
    print(Role)
    def func(self):
        print(Person.Role)
        pass

p = Person()
# print(p.func)        # 0x0000027B9E93AA20
# print(Person.func)   # 0x0000027B9E936D90
p.func()  # alxe

"""
# 对象
    - 类创建对象的过程就是实例化的过程  # 构造new 初始化init
    - 可以通过指针找到类空间的内容
    - 对象本身内部也存储了一些只属于对象的属性

# 组合: 什么有什么的关系
    - 一个类的对象作为另一个类对象的属性
   
# 继承：什么是什么的关系
    - 单继承 多继承
      * 单继承：如果子类的对象调用某个方法 从子类->父类->object寻找
               如果子类有但想用父类的方法：super(子类, self).方法(除了self之外的参数)
                                       父类.方法(self, 其他参数)
    - 经典类 新式类
      * 多继承：新式类：广度优先 -> C3算法
                 * mro方法查看继承顺序
                 * Python3默认继承object，所以都是新式类
                 * super().func() 遵循mro算法，在类的内部不用传子类和self
               经典类：深度优先
                 * super(子类, self).func() 
                 * Python2默认不继承object
    - 子类 父类
"""

class A:
    def func(self):
        print("A")

class D(A):
    def func(self):
        super(D, self).func()
        print("D")

class E(A):
    def func(self):
        super(E, self).func()
        print("E")

class F(D, E):
    def func(self):
        super(F, self).func()
        print("F")
f = F()
f.func()
# super(子类, self).func() 实际上是遵循mro的顺序来执行的，即使有的类不是父类
# A
# E
# D
# F

"""
    - 抽象类和接口类：
      * 不能被实例化
      * 规范子类当中必须实现某个方法
      * 有原生的实现抽象类的方法，没有原生的实现接口类的方法
    - java 
      * 只支持类的单继承   抽象类
      * 只好创建Interface 接口类 支持多继承的规范
      * 在java中有区别，java中的抽象类可以实现方法，接口类不能实现方法，pass       
"""
"""
# 多态：
    - 一种类型的多种形态，一个父类有多个子类，每个子类都是父类的一种形态
    
# 封装：
    - 广义
    - 狭义：私有化，只能在类的内部使用
    - 如：静态变量、私有方法、私有的对象属性、私有类方法、私有静态方法
    - 在类中存储_类名__名字
    - 私有：不能在类的外部使用也不能被继承
    - 在类的内部可以以__名字使用，在类的外部最好不要使用

# property 装饰器函数，内置函数，帮助你将类中的方法伪装成属性
    - 调用方法的时候不需要主动加()
    - 让程序的逻辑更合理
    - @方法名.setter   装饰器，修饰property装饰的属性时会调用这个装饰器装饰的方法
    - @方法名.deleter  装饰器，删除被装饰的属性
    
# @classmethod 类方法的装饰器 内置函数
    - 使用类名调用，默认传类名作为第一个参数
    - 不用对象命名空间中的内容，而是用类命名空间中的静态属性、类方法、静态方法
"""

class Goods:
    __discount = 0.8

    def __new__(cls, *args, **kwargs):
        return object.__new__(cls)

    def __init__(self, price):
        self.__price = price

    @property
    def price(self):
        return self.__price * Goods.__discount

    @classmethod
    def change_discount(cls, num):
        cls.__discount = num

# apple = Goods(10)
# banana = Goods(20)
# print(apple.price, banana.price)
# apple.change_discount(1)
# print(apple.price, banana.price)

"""
# @staticmethod 静态方法的装饰器 内置函数
    - 如果一个类里面的方法，既不需要用到self中的资源，也不用cls的资源
    - 相当于一个普通的函数
    - 但是由于某种原因，还要把这个方法放在类中，这个时候就将这个方法变成一个静态方法
    - 某种原因：
      * 完全想用面向对象编程，所有的函数都必须放到类里
      * 某个功能确确实实是这个类中的方法，但是没有用到和这个类有关系的资源      

# 反射
    - 从某个指定的命名空间中，通过字符串数据类型的变量名来获得变量的值
    - 类名反射：静态属性 类方法 静态方法
    - 对象反射：对象属性 实例方法
    - 模块   : 模块中的方法
    - 自己模块中：
         import sys
         my_module = sys.module['__main__']
         getattr(my_module, '变量名')
    hasattr/getattr/setattr/deleter            
    - 变量名：只能拿到一个字符串的版本
      * 从文件里拿
      * 交互拿：input/网络传输
      
# 进阶
    - 内置方法/魔术方法/双下方法
    - __名字__不是直接被调用的
    - 间接调用：内置函数/面向对象中特殊语法/python提供的语法糖
    - __str__  : str(obj), 要求必须实现了__str__, 要求返回值必须是str类型
    - __call__ : 对象() 用类写装饰器
    - __len__  : len(obj), 要求obj必须实现了__len__, 要求这个方法的返回值必须是数字int类型
    - __new__  : 在实例化的过程中, 最先执行的方法, 在执行init之前, 用来创建一个对象, 构造方法
    - __init__ : 在实例化的过程中, 在new执行之后，自动触发的一个特殊的用于初始化的方法
"""



