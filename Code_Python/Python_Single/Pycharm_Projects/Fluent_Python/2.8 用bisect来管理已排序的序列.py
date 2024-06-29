# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-01 21:20
program     : 学习使用bisect管理(搜索/插入)已排序的序列
"""
# import pyperclip
# import re, string
# import time, datetime
# import requests, urllib.request
# from lxml import etree
# from bs4 import BeautifulSoup
import sys #, os, glob, subprocess, threading
# from multiprocessing.dummy import Pool
# from PIL import Image
# import PyPDF2, reportlab, csv, json
# import smtplip, imapclient, pyzmail
import bisect, random

HayStack = [1, 4, 5, 6, 8, 12, 15, 20, 21, 23, 23, 26, 29, 30]
Needles = [0, 1, 2, 5, 8, 10, 22, 23, 29, 30, 31]

Row_Fmt = '{0:2d} @ {1:2d}   {2}{0:<2d}'

random.seed(1729)
my_list = []

def demo(bisect_lr):
    for needle in reversed(Needles): # 翻转序列Needles，没多少意义
        position = bisect_lr(HayStack, needle)
        offset = position * '   |'
        print(Row_Fmt.format(needle, position, offset))

def demo2(SIZE):
    for i in range(SIZE):
        """
        choose a random item from range(start, stop[, step]).
        This fixes the problem with randint() which includes the
        endpoint
        """
        new_item = random.randrange(SIZE*2) # 不包含终点，整数
        """
        insort跟bisect一样，有lo和hi两个可选参数用来控制查找的范围。
        它也有个变体叫insort_left，这个变体在背后用的是bisect_left。
        """
        bisect.insort(my_list, new_item) # insort = insort_right
        print("%2d ->" % new_item, my_list)

if __name__ == '__main__':
    """
    bisect 的表现可以从两个方面来调教:
    * 用它的两个可选参数——lo和hi来缩小搜寻的范围。lo的默认值是0，hi的默认值是序列的长度，即len()作用于该序列的返回值。
    * bisect函数其实是bisect_right 函数的别名，后者还有个姊妹函数叫bisect_left。
    
    它们的区别在于:
    * bisect_left返回的插入位置是原序列中跟被插入元素相等的元素的位置，也就是新元素会被放置于它相等的元素的前面
    * bisect_right返回的则是跟它相等的元素之后的位置。
    这个细微的差别可能对于整数序列来讲没什么用，但是对于那些值相等但是形式不同的数据类型来讲，结果就不一样了。
    比如说虽然1==1.0的返回值是True，1和1.0其实是两个不同的元素。
    """
    if input('left or right:') == 'left':
        bisect_lr = bisect.bisect_left
    else:
        bisect_lr = bisect.bisect_right # bisect = bisect_right

    print('DEMO', bisect_lr.__name__)
    print('haystack ->', '  '.join('%2d' % n for n in HayStack))
    demo(bisect_lr)

    demo2(7)
