# -*- coding:utf-8 -*-

"""
author      : myRealization
create_time : 2018-09-01 22:07
program     : 
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
import array, random
"""只包含数字的列表，那么array.array比list更高效。
创建数组需要一个类型码，这个类型码用来表示在底层的C语言应该存放怎样的数据类型。
比如b类型码代表的是有符号的字符（signed char），因此array('b')创建出的数组就只能存放一个字节大小的整数，范围从 -128
到127，这样在序列很大的时候，我们能节省很多空间。而且Python不会允许你在数组里存放除指定类型之外的数据。
"""
# random() -> x in the interval [0, 1). # 双精度浮点数组（类型码是 'd'）
floats = array.array('d', (random.random() for i in range(10**7)))
print(floats[-1])

# 数组支持所有跟可变序列有关的操作，包括.pop、.insert 和.extend。
# 另外，数组还提供从文件读取和存入文件的更快的方法，如.frombytes和.tofile
fp = open('floats.bin', 'wb')
# 面对各类需求时，我们可能会有更好的选择。比如，要存放
# 1000 万个浮点数的话，数组（array）的效率要高得多，因为数组在背后存的并不是
# float对象，而是数字的机器翻译，也就是字节表述。
floats.tofile(fp) # Write all items (as machine values) to the file object f.
fp.close()

floats2 = array.array('d')
fp = open('floats.bin', 'rb')
floats2.fromfile(fp, 10**7) # Read n objects from the file object f and append them to the end of the array.
fp.close()
print(floats2[-1])
print(floats==floats2)

