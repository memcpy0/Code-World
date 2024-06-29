# -*- coding:utf-8 -*-

"""
author      : myRealization
create_time : 2018-08-30 22:10
program     : 实现一个二维向量类, 支持向量相加、标量乘法、向量取模和友好打印
"""
# import pyperclip
# import re, string
# import time, datetime
# import requests, urllib.request
# from lxml import etree
# from bs4 import BeautifulSoup
# import os, glob, sys, subprocess, threading
# from multiprocessing.dummy import Pool
# from PIL import Image
# import PyPDF2, reportlab, csv, json
# import smtplip, imapclient, pyzmail
from math import hypot
"""
虽然代码里有6个特殊方法，但这些方法（除了 __init__）并不会在这个类自身的代码
中使用。即便其他程序要使用这个类的这些方法，也不会直接调用它们
"""
class Vector:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __abs__(self):  # abs()内置函数 --> x.__abs__()
        # Return the Euclidean distance, sqrt(x*x + y*y)
        return hypot(self.x, self.y)

    def __add__(self, other):  # 等于+
        return Vector(self.x+other.x, self.y+other.y)

    def __bool__(self):  # bool() --> x.__bool__()
        #  看是否是零向量，是则返回False
        return bool(self.x or self.y)  # 注释掉的更低效 bool(abs(self))

    def __repr__(self):  # 等于内置函数的repr-->x.__repr__()
        """
        把一个对象用字符串的形式表达出来以便辨认:
        交互式控制台和调试程序（debugger）用repr函数来获取字符串表示形式；
        在老的使用%符号的字符串格式中，这个函数返回的结果用来代替%r所代表的对象；
        同样，str.format函数所用到的新式字符串格式化语法也是利用了 repr，才把!r字段变成字符串
        """
        return 'Vector(%r, %r)' % (self.x, self.y)

    def __mul__(self, other):  # 左乘* other*vector
        """
        只实现了数字做乘数、向量做被乘数的运算，乘法的交换律则被忽略了
        在第13章里，我们将利用__rmul__解决这个问题。
        """
        return Vector(self.x*other, self.y*other)

from collections import abc