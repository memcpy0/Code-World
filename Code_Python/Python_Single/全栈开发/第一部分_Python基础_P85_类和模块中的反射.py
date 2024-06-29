# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-19 10:36
program     : 学习反射
"""

# 反射 ******
# 内置方法
"""
反射：用字符串数据类型的变量名来访问这个变量的值
* 反射的方法： getattr hasattr | setattr, delattr
语法：
    - 命名空间.xxx ==> getattr(命名空间, 'xxx')
1 类.名字
2 对象.名字
3 模块.名字
    - import module_name
    - getattr(module, '名字')
4 自己的文件.名字
"""
# eval 这个东西，只能用来执行明确的写在代码里的语句

# ----------------------------------------------------------------------
# 通过类调用的：静态属性 类方法 静态方法
class Student:
    ROLE = "STUDENT"

    @classmethod
    def check_course(cls):
        print("查看课程")

    @staticmethod
    def login():
        print("登录:)")

# 反射查看属性
print(Student.ROLE)             # STUDENT
print(getattr(Student, 'ROLE')) # STUDENT

# 反射调用方法
Student.check_course()              # 查看课程
getattr(Student, "check_course")()  # 类方法 查看课程
getattr(Student, "login")()         # 静态方法

# 有就执行方法
num = 'check_course'        # 可以input(">>>")
if hasattr(Student, num):   # >>>check_course
    getattr(Student, num)() # 查看课程

# ----------------------------------------------------------------------
# 通过对象调用的：实例属性 实例方法
class A:
    def __init__(self, name):
        self.name = name

    def func(self):
        print("In func")

a = A('alex')
if hasattr(a, 'name'):
    print(getattr(a, 'name'))  # alex

getattr(a, 'func')()   # In func

# ----------------------------------------------------------------------
# 别人的模块
import os
# getattr(os, 'rename')('f-string.py', 'P0_f-string.py') # == os.rename
# rename = os.renames

def wahaha():
    print('娃哈哈')

def qqxing():
    print('QQ星')

# 自己的文件
# 模块哪个导入哪个没导入，都应记录下来
# sys中的方法都是与Python解释器相关的
import sys
# sys.modules 这个方法，表示所有的在当前Python程序中导入的模块
# '__main__': <module '__main__' from 'D:/Python_Projects/Pycharm_Projects/Python14期全栈开发/P85_类中的反射.py'>
# print(sys.modules)
my_file = sys.modules['__main__']
my_file.wahaha()   # 娃哈哈
my_file.qqxing()   # QQ星

getattr(my_file, 'wahaha')()  # 娃哈哈
getattr(my_file, 'qqxing')()  # QQ星