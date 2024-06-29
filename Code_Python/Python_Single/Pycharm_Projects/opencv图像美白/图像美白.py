# -*- coding:utf-8 -*-

"""
author      : myRealization
create_time : 2018-08-30 12:00
program     : 用opencv实现图像美白
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
import cv2
# 案例一 认识OpenCV
# 1 加载一张图片
image = cv2.imread(r'D:\Python_Projects\Pycharm_Projects\opencv图像美白\abc.jpg')
# 2 创建一个事件
cv2.namedWindow('image')
# 3 定义一个函数 目的：实时进行鼠标状态的监听
def draw(event, x, y, flags, param):
    # 3.1 判断鼠标的事件
    if event == cv2.EVENT_LBUTTONDOWN:
        print("鼠标------->按下")
    elif event == cv2.EVENT_MOUSEMOVE:
        print("鼠标->移动")
    elif event == cv2.EVENT_LBUTTONUP:
        print("鼠标------->抬起")

# 4 监听鼠标事件的回调 窗口和回调的函数
cv2.setMouseCallback('image', draw)
# 5 展示窗口
cv2.imshow('image', image)
# 窗口等待
cv2.waitKey(0)
# 窗口销毁
cv2.destroyAllWindows()

value = 20
image_dst = cv2.bilateralFilter(image, value, value*2, value/2)
image_dst.imwrite(r'D:\Python_Projects\Pycharm_Projects\opencv图像美白\abc.jpg')
# 图片模糊 argument 图片对象，模糊度设置(越大越模糊)
# image_dist = cv2.blur(image, (15, 15))
# 创建一个事件
cv2.namedWindow('image')
# 展示窗口
cv2.imshow('image', image_dst)
# 窗口等待
cv2.waitKey(0)
# 窗口销毁
cv2.destroyAllWindows()