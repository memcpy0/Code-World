# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-07 11:51
program     : 学习进程锁和信号
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

from multiprocessing import Process
from multiprocessing import Lock
from multiprocessing import Semaphore
from multiprocessing import Event
from multiprocessing import Queue
import random
import time
"""
锁 同步化多个进程对某些变量的访问 Lock
降低了效率 提高了安全度
在并发编程中使用 

信号量 最多只能有几个访问 Semaphore 一次性最多进入几个 后面出来一个才能进去一个

事件 所有的阻塞都是同步的 recv accpet input sleep
阻塞多个进程 异步 如lock semaphore
    - 事件 标志 同时使得所有的进程都陷入阻塞 同时都恢复
    event.set()
    event.clear()
    event.wait()
    event.is_set()
    
"""
def func(num):
    print(num)

def sing(sem, index):
    sem.acquire()
    print("进入KTV", index*'*')
    time.sleep(random.randint(1, 5))
    print("出了KTV", index)
    sem.release()

def car(e, i):
    e.wait()
    print("%s车通过" % i)

def traffic_light(e):
    while True:
        if e.is_set():
            time.sleep(3)
            print("红灯亮")
            e.clear()    # 绿变红
        else:
            time.sleep(3)
            print("绿灯亮")
            e.set()      # 红变绿

def parent_son_q_put(q):
    q.put('hello')

def son_son_q_get(q):
    print(q.get())   # hello

if __name__ == '__main__':
    lock = Lock()
    for j in range(20):
        lock.acquire()  # 需要锁 拿钥匙
        p = Process(target=func, args=(j,))
        p.start() # 操作系统先让哪个进程运行就不知道
        # 要么join，要么上锁
        # p.join()
        lock.release() # 释放锁 还钥匙

    """
    sem = Semaphore(5)
    for i in range(12):
        p = Process(target=sing, args=(sem,i))
        p.start()
    """

    """
    进程中通信 可以使用multiprocessing 的Queue模块
    1 队列有两种创建方式 第一种不传参数 这个队列长度限制；传参数 创建一个由最大长度限制的队列
    2 提供两个重要方法 put get
    """
    q = Queue()
    pt = Process(target=parent_son_q_put, args=(q,))
    pt.start()
    # print(q.get()) # 通过队列实现了父子进程间的通信
    pt2 = Process(target=son_son_q_get, args=(q,))
    pt2.start()

    """
    event = Event()  # 实例化一个事件/交通信号灯 默认红
    # event.set()      # 将标志变成非阻塞/交通灯变绿
    # event.wait()     # 刚实例化的一个事件对象 默认是阻塞信号/默认是红灯
    #                  # 如果是绿色 就不阻塞 要先看灯 红灯停绿灯行
    #
    # event.clear()    # 将标志变成阻塞/交通灯变红
    # event.wait()
    # tra = Process(target=traffic_light, args=(event,))  # 红绿灯控制多个进程的阻塞和开放 等待和通行
    # tra.daemon = True
    # tra.start()
    # for i in range(100):
    #     if i % 6 == 0:  # 0
    #         time.sleep(random.randint(1, 3))
    #     car_pro = Process(target=car, args=(event, i))
    #     car_pro.start()
    """