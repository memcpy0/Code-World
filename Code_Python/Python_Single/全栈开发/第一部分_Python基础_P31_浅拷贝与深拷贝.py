# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-02 21:52
program     : 深浅拷贝的使用
"""

import copy  # __all__ = ["Error", "copy", "deepcopy"]

list1 = ['金毛狮王', '紫衫龙王']
# 列表，直接这样赋值，实际上是引用的内存地址的赋值，内存中实际上只有一个列表 所以变量无类型，对象有类型
list2 = list1
# list2.append('白毛鹰王')
# print(list1, list2) #  ['金毛狮王', '紫衫龙王', '白毛鹰王'] # ['金毛狮王', '紫衫龙王', '白毛鹰王']

# 浅拷贝
list3 = list1[:]
# list3.append('杨莲亭')
# print(list1, list3)  # ['金毛狮王', '紫衫龙王'] # ['金毛狮王', '紫衫龙王', '杨莲亭']

list4 = list1.copy()
# list4.append('Copy')
# print(list1, list4)  # ['金毛狮王', '紫衫龙王'] # ['金毛狮王', '紫衫龙王', 'Copy']

# 深拷贝
lst1 = ['超人', '蜘蛛侠', '葫芦娃', ['川普', '卡扎菲']]
lst2 = lst1
# print(id(lst1)==id(lst2))  # True

lst3 = lst1.copy()  # lst3 = lst1[:]
# lst3.append('??')
# print(lst1, lst3) # ['超人', '蜘蛛侠', '葫芦娃', ['川普', '卡扎菲']]  # ['超人', '蜘蛛侠', '葫芦娃', ['川普', '卡扎菲'], '??']

# lst3[3].append('??')
# print(lst1, lst3) # ['超人', '蜘蛛侠', '葫芦娃', ['川普', '卡扎菲', '??']] # ['超人', '蜘蛛侠', '葫芦娃', ['川普', '卡扎菲', '??']]
# for i in range(4):
#     print(id(lst1[i]) == id(lst3[i]))
# 全是True # 因为容器类型存储的是内存地址的引用，所以浅拷贝后的两个列表ID不同，
# 但里面的同一位置的引用指向的元素是一样的，所以里列表只有一个，改变它就同时改变两个列表
# print(id(lst1)==id(lst3)) # False

lst4 = copy.deepcopy(lst1)
lst4[3].append('????')
# ['超人', '蜘蛛侠', '葫芦娃', ['川普', '卡扎菲']] # ['超人', '蜘蛛侠', '葫芦娃', ['川普', '卡扎菲', '????']]
# print(lst1, lst4)
# print(id(lst1)==id(lst4)) # False
for i in range(4):
    print(id(lst1[i]) == id(lst4[i]))
# True True True False  # 深拷贝是将列表中的可变类型如列表进行复制，所以可变对象的ID不同