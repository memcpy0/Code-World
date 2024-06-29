# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-07 15:10
program     : 学习生产者消费者模型
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
from multiprocessing import Process, Queue
from multiprocessing import JoinableQueue
import time
import random

"""
生产者消费者模型
  -- 同步过程 要生产一个数据 然后 给一个函数 让这个函数依赖这个数据进行运算 拿到结果   
  
  -- 异步过程 主进程 生产数据 子进程使用数据 生产过慢
             子进程 生产数据 子进程处理数据 并不知道生产消费比
                生产的快 吃的慢 溢出      减少生产者 -> 找一个容器放 约束容器的容量
                生产的慢 吃的快 资源不足  增加生产者 -让生产变快
        首先 在内存中 每次只有很少的数据在内存中
        对于生产与消费的不平衡来说 增加消费者或增加生产者来调节效率
    
     通过Queue进程安全 
        每个消费者进程处理的数据不是确定的，只能用while来处理
        每个生产者产生一个数据 两个生产者就产生两个数据 不会出现多个消费者访问同一个生产的数据的情况
        生产者用发送信号数据来结束消费者        
     
     通过JoinableQueue来自动发送信号 
        消费者 taskdone
        producer.join()  等待生产者结束
"""
def producer(q, food): # 生产者
    for i in range(50):
        q.put("%s-%s" % (food, i))
        print("生产了%s-%s" % (food, i))
        time.sleep(random.randint(1, 3))
    # q.put(None)
    q.join()  # 生产完毕后 等待消费者把所有的数据都处理完

def consumer(q): # 消费者
    while True:
        time.sleep(1)
        food = q.get()
        if food == None: break
        print('吃了'+food)
        q.task_done()

if __name__ == '__main__':
    # q = Queue(10)  # 托盘
    jq = JoinableQueue(10)
    food = '包子'
    p1 = Process(target=producer, args=(jq, food))
    p1.start()

    food = '骨头'
    p2 = Process(target=producer, args=(jq, food))
    p2.start()

    c1 = Process(target=consumer, args=(jq,))
    c2 = Process(target=consumer, args=(jq,))
    c1.daemon = True
    c1.start()
    c2.daemon = True
    c2.start()

    p1.join()  # 等待p1生产完毕
    p2.join()  # 等待p2执行完毕
    # 当它们结束后 守护进程也结束了
# 生产者生产完后等待消费者把所有的数据都处理完 --> 生产者进程结束 -> 主进程结束 -> 设为守护进程的消费者进程结束

