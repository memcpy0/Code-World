# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-18 22:23
program     : 主要学习封装
"""
"""
面向对象的三大特性:
1 多态 Python没有多态，有的是鸭子类型
2 继承 
3 封装
* 广义的封装：实例化一个对象，给对象空间封装一个属性
* 狭义的封装：私有制
  - 私有成员：私有静态字段(变量)；私有方法；私有对象属性
             __field          __func() self.__filed

* 对于私有静态字段，类的外部和派生类均不可访问；只能在本类的内部访问

"""

#http://www.cnblogs.com/jin-xin/articles/9214247.html
# print("index" in dir(list))
# print("index" in dir(str))
# print("index" in dir(tuple))
# True
# True
# True

class GamePerson:
    name = 'alex'
    # 私有静态字段
    __age = 1000

    def func(self):
        # 对于私有精态字段，类的内部可以通过实例和类名访问
        print(self.__age)  # 100
        print(GamePerson.__age) # 100
        print("In func……")

        # 对于私有方法，类的内部可以访问
        print(self.__func1())  # In __func1
        print(GamePerson.__func1(self))  # In __func1

        self.__img = 33
        # 对于私有对象属性，类的内部可以访问
        print(self.__img)
    def __func1(self):
        print("In __func1")

class Tex(GamePerson):

    def func2(self):
        print(Tex.__name__, self.name)  # alex
        # print(Tex.__age)
        # print(self.__func1())


alex = GamePerson()
tex = Tex()
# 对于私有静态属性，类的内部是可以访问的
# alex.func()

# AttributeError: 'GamePerson' object has no attribute '__age'
# 实例化对象不能访问私有静态字段
# print(alex.__age)

# AttributeError: type object 'GamePerson' has no attribute '__age'
# 类名无法访问私有静态字段
# print(GamePerson.__age)

# AttributeError: type object 'Tex' has no attribute '_Tex__age'
# 子类的实例化对象也不能访问
# tex = Tex()
# tex.func2()

# 实际上是可以访问的；python内部做了一个名称改写
# print(alex._GamePerson__age)  # 1000

# print(alex.__dict__)  # 为{} 其查询仅限于实例化对象的属性
# print(GamePerson.__dict__)
# {'__module__': '__main__', 'name': 'alex', '_GamePerson__age': 1000, 'func':
# <function GamePerson.func at 0x000001FB6EEC6950>, '__dict__': <attribute '__dict__' of 'GamePerson' objects>,
# '__weakref__': <attribute '__weakref__' of 'GamePerson' objects>, '__doc__': None}

# ---------------------------------------------------------------------------------------------------------------

# 对于私有方法，类的内部可以访问
alex.func()

# AttributeError: 'GamePerson' object has no attribute '__func1'
# 实例化对象不能访问私有方法
# alex.__func1()

# AttributeError: type object 'GamePerson' has no attribute '__func1'
# 类名不能访问私有方法
# GamePerson.__func1()

# AttributeError: 'Tex' object has no attribute '_Tex__func1'
# 同理子类的实例化对象也不能访问
# tex = Tex()
# tex.func2()

# 实际上是可以访问的, 通过实例化对象的调用；python内部做了一个名称改写
# alex._GamePerson__func1()
# 子类的实例化对象也可以改写名称进行访问
# tex._GamePerson__func1()

# print(alex.__dict__)  # {}
# print(GamePerson.__dict__)
# {'__module__': '__main__', 'name': 'alex', '_GamePerson__age': 1000,
# 'func': <function GamePerson.func at 0x000002268EAF6950>,
# '_GamePerson__func1': <function GamePerson.__func1 at 0x000002268EAF6C80>,
# '__dict__': <attribute '__dict__' of 'GamePerson' objects>,
# '__weakref__': <attribute '__weakref__' of 'GamePerson' objects>, '__doc__': None}

# --------------------------------------------------------------------------------------
# 对于私有对象属性，可以在类内部访问

# 实例化对象不能访问私有对象属性
# print(alex.__img)

# print(alex._GamePerson__img) # 33
print(alex.__dict__)  # {'_GamePerson__img': 33}
print("_GamePerson__img" in GamePerson.__dict__)  # False
