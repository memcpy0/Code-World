# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-27 23:39
program     : 用socket来写UDP服务端程序
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
import socket
"""
UDP的server 不需要进行监听 也不需要建立连接
在启动服务之后只能被动的等待客户端发送信息过来
客户端发送消息的时候 还会同时发送地址信息过来
消息回复的时候 不仅需要发送消息 还要把对方的地址填写过去
"""
sk = socket.socket(type=socket.SOCK_DGRAM)  # datagram数据报文
sk.bind(('127.0.0.1', 8080))

while True:
    msg, addr = sk.recvfrom(1024)
    print(msg.decode('utf-8'), addr.decode("utf-8"))
    if msg == b'bye':
        break
    sk.sendto(input('SERVER >>> ').encode('utf-8'), addr)

sk.close()

