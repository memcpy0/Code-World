# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-07 22:43
program     : 学习socket和多线程的结合
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
from threading import Thread

def func(connection):
    connection.send(b'hello')
    info = connection.recv(1024)
    print(info)


sk = socket.socket()
sk.bind(('127.0.0.1', 8080))
sk.listen(5)

while True:
    conn, addr = sk.accept()
    Thread(target=func, args=(conn,)).start()

sk.close()
