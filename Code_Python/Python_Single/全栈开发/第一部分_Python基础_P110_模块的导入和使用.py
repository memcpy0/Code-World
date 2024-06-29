# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-24 12:02
program     : 学习模块的一些知识
"""

"""
模块的导入和使用

import ...
* import这个语句相当于什么？相当于执行了这个模块所在的py文件
* 模块的名字要符合变量的命名规范 # 一般模块名都是小写字母开头的py文件
* 一个模块可以多次导入吗？ 一个模块不会多次导入
* 模块执行时的事情：
    - 找到这个模块
    - 创建一个属于这个模块的内存空间
    - 执行该模块
    - 建立这个模块和它的命名空间之间的联系
    - 将它们连接到本模块中的名字上
    - 可能会有重命名
* PEP 8 规范
    - 所有模块都应该尽量放在这个文件开头
    - 不推荐在一行导入多个模块
    - 模块的导入也是有顺序的
        # 先导入内置模块
        # 再导入第三方模块
        # 最后导入自定义模块

from ... import ...
* from import这个语句相当于什么?
在文件中对全局变量的修改只会影响对模块变量的引用，不会影响其本身
* 导入的名字是
    变量：直接使用
    函数：加上()调用
    类 
* PEP 8 规范
    - 可以在一行中导入一个模块中的所有名字
    - 可能会有重命名
 
from ... import *
* 相当于？在本模块中定义了和被引用模块中的所有变量同名的变量
    即对引用的模块进行了一次拷贝
    
对已经导入的模块进行修改，不会影响导入的模块，除非重新加载
"""
# import my_module  # In My Module 这里只能导入执行一次
import my_module

# from my_module import login
from my_module import *  # 只能导入__all___中定义的字符串名字

# from importlib import reload
# reload(my_module) # In My Module  # 重新加载导入

my_module.login()
login()