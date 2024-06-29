# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-07 15:57
program     : 学习双向通信的管道
"""#
# import pyperclip
# import re, string
# import time, datetime
# import json, csv
# import requests, urllib.request
# from lxml import etree
# from bs4 import BeautifulSoup
# import os, glob, sys, subprocess, threading
# from multiprocessing.dummy import Pool
# from PIL import Image
# import PyPDF2, reportlab, csv, json
# import smtplip, imapclient, pyzmail
# from collections import *
from multiprocessing import Pipe, Process, Lock
"""
 ______________
 |_____________| 管道不安全 可能有多个进程读取同一个数据
 队列 = 管道 + 锁 --> 队列用在同一台机器的多个进程之间通信是最好的

pi1, pi2 = Pipe()  # 支持双向通信
pi1.send('hello')
print(pi2.recv())  # 有数据就拿，没数据就阻塞

pi2.send('hi')
print(pi1.recv())

pi2.close()
print(pi1.recv()) # 另一端关闭就EOFError
"""
def func(p, l):
    foo, son = p
    foo.close()
    while True:
        try:
            l.acquire()
            print(son.recv())
            l.release()
        except EOFError:
            l.release()
            break

if __name__ == '__main__':
    foo, son = Pipe()
    l = Lock()
    p1 = Process(target=func, args=((foo, son), l))
    p1.start()
    p2 = Process(target=func, args=((foo, son), l))
    p2.start()

    son.close()
    foo.send('hello')  # 实现了主进程和子进程间的通信
    foo.send('hello')
    foo.send('hello')
    foo.send('hello')
    foo.close()
