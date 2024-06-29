# -*- coding:utf-8 -*-

"""
author      : myRealization
create_time : 2018-08-30 00:20
program     : 实验列表推导和生成器表达式以及array
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
import array
"""
列表推导的作用只有一个：生成列表。
如果想生成其他类型的序列，生成器表达式就派上了用场。
虽然也可以用列表推导来初始化元组、数组或其他序列类型，但是生成器表达式是更好的
选择。
"""

symbols = '$&*￥#@'
# array(typecode [, initializer]) -> array
"""
array的构造方法需要两个参数，因此括号是必需的。
array构造方法的第一个参数指定了数组中数字的存储方式。
"""
arr = array.array('I', (ord(symbol) for symbol in symbols))
print(arr)

def ellipsis():
    ...

