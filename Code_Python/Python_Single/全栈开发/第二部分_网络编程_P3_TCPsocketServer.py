# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-27 22:37
program     : 使用socket写服务端程序
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

sk = socket.socket()
# 避免服务重启的时候报address already in use
sk.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

sk.bind(('127.0.0.1', 62222))
sk.listen(4)

conn, addr = sk.accept()  # 阻塞 直到获取到一个客户端的连接, 已经三次握手建立了一个连接

while True:
    info = conn.recv(1024).decode("utf-8") # 阻塞, 直到收到一个客户端发来的消息
    print(info)
    if info == "bye":
        break
    conn.send(input('>>> ').encode("utf-8"))

conn.close() # 关闭连接
sk.close()   # 关闭socket对象, 不关闭还能继续接受连接

