# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-07 19:39
program     : 学习进程池
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
from multiprocessing import Pool
import os
import time

"""
进程池 
# 为什么出现: 开过多的进程 1.开启新的进程浪费时间 2.操作系统调度太多的进程也会影响效率
# 造一个池子 放4个进程(员工)来完成工作
# 发任务
# 用进程池里的进程轮流完成多个任务 谁先完成某个任务就去领下一个任务
# 当所有的任务都处理完了 进程池关闭 回收所有的进程
# CPU 的count+1

多进程来处理IO多的操作
"""


def func(i):
    time.sleep(.2)
    print(i, '子进程', os.getpid())
    return i-i


if __name__ == '__main__':
    # p = Pool(5)
    # p.map(func, range(20)) # 异步执行

    # p.close()   # 是不允许往进程池中在添加任务 必须加在join之前
    # p.join()    # 等待进程池中子进程完成所有任务 阻塞主进程 不用的话主进程会直接结束
    # print("-->")
    # print(os.cpu_count())

    pt = Pool(5)
    res_l = []
    for i in range(20):
        # pt.apply(func, args=(i,))  # apply是同步提交任务的机制 不一定要用
        # pt.apply_async(func, args=(i,))
        # 为得到进程的返回结果在进程池中使用res.get() # 实际上一般的Process是得不到返回值的 必须通过IPC通信才行
        res = pt.apply_async(func, args=(i,)) # , callback=func 直接将返回值给回调函数, 在主进程中完成的
        res_l.append(res)                     # 这样就没有res
        # print(res.get())  # 要结果 就会阻塞 变成了同步 可以这么做

    pt.close()
    pt.join()

    for i in res_l:
        print(i.get())  # 明显更快
