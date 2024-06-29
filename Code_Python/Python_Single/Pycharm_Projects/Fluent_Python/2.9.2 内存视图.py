# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-02 22:56
program     : 学习使用内存视图memoryview
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
# from collections import *

"""
memoryview是一个内置类，它能让用户在不复制内容的情况下操作同一个数组的不同切片，在数据结构之间共享内存。
其中数据结构可以是任何形式，比如PIL图片、SQLite数据库和NumPy的数组等等
这个功能在处理大型数据集合的时候非常重要。
"""
import array
# 利用含有5个短整型有符号整数的数组（类型码是'h'）创建一个 memoryview
numbers = array.array('h', [-2, -1, 0, 1, 2]) # 'h' signed integer 2
memv = memoryview(numbers)
# print(len(memv)) # 5
# print(memv[0]) # -2  memv里的5个元素跟数组里的没有区别

"""
跟数组模块类似，memoryview.cast能用不同的方式读写同一块内存数据，而且内容字节不会随意移动。
它会把同一块内存里的内容打包成一个全新的memoryview对象给你。
"""
# 创建一个memv_oct，这一次是把memv里的内容转换成'B'类型，也就是无符号字符
memv_oct = memv.cast('B')

# 以列表的形式查看memv_oct的内容
print(memv_oct.tolist()) # [254, 255, 255, 255, 0, 0, 1, 0, 2, 0]
memv_oct[5] = 4  # 把位于位置5的字节赋值成4
#因为我们把占2个字节的整数的高位字节改成了4，所以这个有符号整数的值就变成1024
print(numbers)   # array('h', [-2, -1, 1024, 1, 2])
