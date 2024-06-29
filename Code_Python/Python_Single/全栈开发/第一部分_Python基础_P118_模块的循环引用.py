# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-24 13:28
program     : 学习模块的循环引用
"""
"""
student <-
        -> class
模块的循环引用:
* student引用class模块中的内容
* class引用student模块中的内容
* 模块中的import语句不会重复执行
* 在模块中的导入中，永远不要产生循环引用问题
* 多个模块也可能产生这种现象
A -> B
|    |
D <- C
"""
