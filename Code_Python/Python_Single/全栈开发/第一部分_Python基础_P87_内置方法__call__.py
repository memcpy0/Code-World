# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-19 21:40
program     : __call__; __len__;
              __new__ 单例; __init__;
              __str__; __repr__;
              __del__;
              __add__;
              __getitem__; __setitem__; __delitem__;
              __hash__
"""

"""
__名字__
  类中的特殊方法
  魔术方法
  双下方法
所有的双下方法没有需要你在外部调用的, 而
是总有一些其他的内置函数 特殊语法来自动触发这些方法
"""

# ()调用与__call__
class A:

    def __call__(self, *args, **kwargs):
        print('执行了call方法了')

class B:
    def __init__(self, cls):
        print('在A实例化之前做一些事情')
        self.a = cls()  # 相当于A() 实例化A
        self.a()        # A() 调用__call__方法
        print('在实例化A之后做一些事情')


# a = A()
# a() # 对象() 相当于调用了__call__方法

# A()()  # 相当于实例化一个对象,  再和上面的结果一样, 相对于调用__call__ 方法
# B(A)

# ---------------------------------------------------------------------------------------------------------------------
"""
len()和__len__()
len()方法调用的就是类的__len__()方法
__len__()方法的返回值就是len()函数的返回值
"""


class Beta(object):

    def __init__(self):
        self.lst = [1, 2, 3]

    def __len__(self):
        return len(self.lst)

# b = Beta()
# print(len(b))

# ---------------------------------------------------------------------------------------------------------------------

"""
__new__()  用于开辟空间的构造方法
__init__()  用于给实例空间初始化属性的初始化方法
创建一个类的实例的时候
1. 开辟一个空间, 属于该对象
   - 先使用object.__new__(cls)方法, 创建一块空间
   - 不写的话, object自动执行new()方法
2. 把对象空间传给self, 再执行__init__()
3. 将对象self返回给调用者
"""

class Single(object):

    def __new__(cls, *args, **kwargs):
        obj = object.__new__(cls)  # 返回一个对象
        print("In new")
        return obj

    def __init__(self):
        print("In init")

# s = Single()

"""
单例：
如果一个类从头到尾只有一个实例, 那么这个类就是一个单例类
"""
class Singl(object):

    _ISINSTANCE = None
    def __new__(cls, *args, **kwargs):
        if not cls._ISINSTANCE:
            cls._ISINSTANCE = object.__new__(cls)
        return cls._ISINSTANCE

    def __init__(self, name, age): # 走到这里的时候空间已经开辟了, 不可能只开辟一个实例空间
        self.name = name
        self.age = age

"""
单例模式:
每次开辟的实例空间都是同一块空间, 后面赋值的属性会覆盖以前的属性
如同C语言中的共用体
"""
# s1 = Singl('alex', 44)
# s2 = Singl('太白', 100)  # 太白
# print(s1.name, s1.age)  # 太白 100
# print(s2.name, s2.age)  # 太白 100

# ---------------------------------------------------------------------------------------------------------------------
"""
str与__str__()
str(obj)相当于执行对象的__str__()方法
格式化字符串 %s 也相当于执行obj.__str__()方法, 得到其返回值
print一个数据类型的时候会调用该对象的__str__()
"""
"""
repr与__repr__
repr(object) %r 相当于执行对象的__repr__()方法
__repr__()是__str__的备胎, 如果有__str__方法, print %s str()都会先去执行__str__方法, 并且使用__str__方法的返回值
如果没有__str__方法, 那么print %s str()都会执行__repr__

在子类中使用__str__, 先去找子类的__str__, 如果没有的话往上找, 只要父类不是object, 那么久执行父类的__str__方法
但是如果除了object之外的父类都没有__str__方法, 那么就执行子类的__repr__方法
如果子类也没有__repr__就会向上找父类的__repr__方法
一直找不到, 就会执行object类中的__str__方法
"""

d = '123'
# print(d)        # 123
# print(repr(d))  # '123'

class AT:

    def __init__(self, name):
        self.name = name

    def __repr__(self):
        return repr(self.name)
    
    def __str__(self):
        return str(self.name)


# ---------------------------------------------------------------------------------------------------------------------
# __add__ +
class MyType:
    def __init__(self, s):
        self.s = s

    def __add__(self, other):
        return self.s.count('*') + other.s.count('*')

# obj1 = MyType("akfwie*23*kal*")
# obj2 = MyType("qiqru**iq8*qi212**")
# print(obj1+obj2)  # 8

# ---------------------------------------------------------------------------------------------------------------------
# sys = __import__('sys')
# print(sys.getdefaultencoding())  # utf-8
# print(sys.getfilesystemencoding())  # utf-8
# print(sys.executable)
# ---------------------------------------------------------------------------------------------------------------------

"""
__del__与del 析构方法(与构造方法相反)


"""
class Del:

    def __del__(self):
        # 析构方法 del A的对象 会自动触发这个方法
        print("执行我Del了")

# e = Del()
# del e  # 删除对象时调用__del__ 先执行__del__的内容，再回收
# print(e)  # <__main__.Del object at 0x000001BEFFA5D198> 执行我了
"""
最后用完了e对象后Python解析器自动垃圾回收，执行了__del__方法
"""

class File:
    # file-like object
    def __init__(self, file_path):
        self.f = open(file_path, encoding="UTF-8")

    def read(self):
        self.f.read(1024)

    def __del__(self):  # 是去归还/释放一些在创建对象的时候借用的一些资源
        # del 对象的时候      程序员触发
        # Python解释器自动执行垃圾回收机制的时候
        # 知道这是一个文件对象，要在垃圾回收之前先执行关闭文件操作释放操作系统资源，再del回收内存中的对象
        # 这样不管主动或者被动，这个对象总会被清理掉，被清理就会触发__del__方法，从而执行这里的关闭操作，归还操作系统的文件资源
        self.f.close()
        print("执行我File了")

# f = File("P87_内置方法__call__.py")
# f.read()  # 执行我File了  # 再del回收内存资源前就关闭了文件

# ---------------------------------------------------------------------------------------------------------------------
"""
item系列 都是与[]有关的操作，item可能是列表、元组的索引和切片，字典的键
__getitem__: list[] dict[]
__setitem__: list[] = …… dict[] = ……
__delitem__: del 
"""

# class MyItem:
#     """
#     这里拐弯使用了属性设置的方法实现了与item相关的操作
#     """
#     def __getitem__(self, item):
#         print("执行__getitem__")
#         return getattr(self, item)
#
#     def __setitem__(self, key, value):
#         print("执行了__setitem__")
#         setattr(self, key, value)
#
#     def __delitem__(self, key):
#         delattr(self, key)
#
# item = MyItem()
# item['k1'] = 'v1'  # 相当于执行了__setitem__，在这里将item设置了一个k1的属性为v1 item.k1=v1
# print(item['k1'])  # 相当于执行了__getitem__，在这里得到了item的属性，item.k1
#
# del item['k1']     # 相当与执行了__delitem__，在这里delattr了self的属性

class MyItem2:
    """
    这里真正实现了与item相关的操作
    """

    def __init__(self, lst):
        self.lst = lst

    def __getitem__(self, item):
        print("执行__getitem__")
        return self.lst[item]

    def __setitem__(self, key, value):
        print("执行了__setitem__")
        self.lst[key] = value

    def __delitem__(self, key):
        print('执行了__delitem__')
        self.lst.pop(key)

    def __repr__(self):
        return '<MyItem> %s' % self.lst

    def __del__(self):
        print("执行了__del__")

# my_item2 = MyItem2([1, 2, 3])
# print(my_item2[1])    # 执行__getitem__  2
#
# del my_item2[2]       # 执行了__delitem__
# print(my_item2)       # <MyItem> [1, 2]
#
# my_item2[2:] = [2, 4] # 执行了__setitem__
# print(my_item2)       # <MyItem> [1, 2, 2, 4]
# 执行了__del__
# --------------------------------------------------------------------------------------
"""
__hash__与hash
字典寻址 集合寻址/去重
先判断哈希值，如果哈希值一样，再判断值，相等则覆盖，不等则另寻址
hash(obj) --> obj对象内部必须实现了__hash__

对同一个对象，在一次执行中的多次哈希得到的值相等
            在多次执行间的哈希得到的值不等
"""

# ---------------------------------------------------------------------------------------
"""
__eq__与=
"""