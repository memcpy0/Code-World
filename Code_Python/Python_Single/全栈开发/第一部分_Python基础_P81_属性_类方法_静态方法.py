# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-18 23:38
program     : 学习属性、类方法、静态方法
"""

# ---------------------------------------------------------------------
"""
属性: 将一个方法伪装成一个属性，在代码的级别上没有本质的提升，但是让其看起来更合理
* 属性可以得到，get；也可以修改，set
"""
# 属性求值和改值
class Person:
    def __init__(self, name, age):
        self.name = name
        if isinstance(age, int):
            # print(isinstance(1, int))
            # print(type(1) is int)
            # print(1 is int)    错误
            self.__age = age  # 设置为私有的，就是不想让别人知道和访问
        else:
            print("你的输入有误，请输入数字")

    @property
    def age(self):
        return self.__age  # 但是要用的话，就要在类的内部定义方法

    @age.setter
    def age(self, value):
        print("In setter!")
        if type(value) is int:
            self.__age = value

    @age.deleter
    def age(self):
        print("In deleter!")
        del self.__age

p1 = Person("帅哥", 18)
# print(p1.age())  # 没有改为属性前，返回了年龄18
# print(p1.age())  # 改为属性后 TypeError: 'int' object is not callable
# get值
print(p1.age)  # 访问属性(通过属性间接访问内部的私有对象属性)

# p1.age = 19  # 没有设置setter前 AttributeError: can't set attribute
# set值    # 改值就会自动执行类中的age.setter装饰的方法，将改的值传入
p1.age = 22    # 打印了666

# delete值 # 使用del删除值时就会自动执行age.deleter装饰的方法
del p1.age     # 打印了999
# print(p1.age) # AttributeError: 'Person' object has no attribute '_Person__age'

p2 = Person("大哥", 22)
p2.age = 33

# ---------------------------------------------------------------------
"""
实例方法：通过实例化对象调用的方法，实例方法中第一个参数的熟称self，由python自动将实例对象传入

类方法：通过类名调用的方法，类方法中第一个参数的熟称cls，由python自动将类名(类空间)传入
注意：
只能被类名调用(实例对象调用，传给cls的也是该对象的所属类)
* 应用场景：
1 类中有些方法是不需要传入实例对象的
2 对类中的静态变量进行改变，要用类方法
3 继承中，子类通过类名执行父类的类方法，并把子类的类空间传给父类
         从而可以对子类的内容进行修改

静态方法：通过类名调用
1 作为一个代码块，结构清晰
2 复用性强，可以被继承的子类调用
"""


class A:
    name = 'alex'
    cnt = 1

    @classmethod
    def func1(cls):
        print(cls)
        print("In classmethod func1\n")
        return cls.name + str(cls.cnt)


    @staticmethod
    def func2():
        print('In staticmethod func2\n')

    def func3(self):
        print("In func3\n")


a = A()
# a.func()  # A.func(a)

print(a.func1())  # <class '__main__.A'>  # In classmethod func1
print(A.func1())  # <class '__main__.A'>  # In classmethod func1

A.func2()
a.func2()

class B(A):
    pass

b = B()
B.func1()
B.func2()

b.func1()
b.func2()


