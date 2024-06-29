# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09012 15:40
program     : 抓取西刺代理IP；检验其有效性
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

# -*- coding:utf8 -*-

import requests
import re
import time
import urllib.request
import socket
socket.setdefaulttimeout(3)

headers = {
    'Accept': '*/*',
    'Accept-Language': 'zh-CN,zh;q=0.8',
    'User-Agent': 'Mozilla/5.0 (X11; Fedora; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.115 Safari/537.36',
    'Hosts': 'hm.baidu.com',
    'Referer': 'http://www.xicidaili.com/nn',
    'Connection': 'keep-alive'
}

# 指定爬取范围（这里是第1~1000页）
for i in range(1, 2):

    url = 'http://www.xicidaili.com/nn/' + str(i)
    req = requests.get(url=url, headers=headers)
    res = req.text

    # 提取ip和端口
    ip_list = re.findall(r"(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}).*?(\d{2,6})", res, re.S)

    # 将提取的ip和端口写入文件
    f = open("ip.txt","a+")
    for li in ip_list:
        ip = li[0] + ':' + li[1] + '\n'
        print(ip)
        f.write(ip)
    time.sleep(2)       # 每爬取一页暂停两秒

inf = open("ip.txt")    # 这里打开刚才存ip的文件
lines = inf.readlines()
proxys = []
for i in range(0, len(lines)):
    proxy_host = "http://" + lines[i]
    proxy_temp = {"http":proxy_host}
    proxys.append(proxy_temp)

# 用这个网页去验证，遇到不可用ip会抛异常
url = "http://ip.chinaz.com/getip.aspx"
# 将可用ip写入valid_ip.txt
ouf = open("valid_ip.txt", "a+")

for proxy in proxys:
    try:
        res = requests.get(url, proxies=proxy).text
        valid_ip = proxy['http'][7:]
        print('valid_ip: ' + valid_ip)
        ouf.write(valid_ip)
    except Exception as e:
        print(proxy)
        print(e)
        continue