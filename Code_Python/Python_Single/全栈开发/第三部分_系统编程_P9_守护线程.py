# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-07 23:41
program     : 学习守护线程
"""
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

"""
守护线程 t.setDaemon(True)
  - 守护进程 或 守护线程 都是当主进程或主线程中的代码完毕 就立刻关闭守护进程或守护线程
  - 不设置守护线程时 程序会等待子线程结束再结束

假如守护线程执行后 又开了一个新的非守护线程 程序就会等待该线程结束再结束
"""
from threading import Thread
import time

def func():
    print("开始执行子线程")
    time.sleep(3)
    print("子线程执行完毕")

t = Thread(target=func)
t.setDaemon(True)     # 进程设置守护线程 是一个属性 daemon=True
t.start()             # 不设置守护线程时 程序会等待子线程结束

# 守护进程 或 守护线程 都是当主进程或主线程中的代码完毕 就立刻关闭守护进程或守护线程
t2 = Thread(target=func)
t2.start()  # --> 在这里程序会等待不是守护线程的t2执行结束 代码执行完毕 守护线程再结束了主线程不一定结束
# t2.join()   # 加了这句话 就是说执行完这句话 代码就结束了 z
