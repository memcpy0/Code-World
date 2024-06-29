# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-03 14:05
program     : 练习面向对象编程和docstring的用法
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
# from collections import *
from math import hypot

class Point:
    'Represents a point in two-dimensional geometric coordinates'

    def __init__(self, x=0, y=0):
        '''Initialize the position of a new point. The x and y coordinates
        can be specified. If they are not, the point defaults to the origin.'''
        self.move(x, y)

    def move(self, x, y):
        "Move the point to a new location in two-dimensional space."
        self.x = x
        self.y = y

    def reset(self):
        "Reset the point back to the geometric origin: 0, 0."
        self.move(0, 0)

    def calculate_distance(self, other_point):
        """Calculate the distance from this point to a second point
        passed as a parameter.

        This function uses the Pythagorean Theorem to calculate the distance
        between the two points. The distance is returned as a float.
        """
        return hypot(self.x-other_point.x, self.y-other_point.y)

# point = Point(3, 5)
# print(point.x, point.y)  # 3 5
# 未添加初始化方法之前
# p = Point()
# p.reset()  # == Point.reset(p)
# print(p.x, p.y)  # 0 0

# p1 = Point()
# p2 = Point()
# p1.reset()
# p2.move(5, 0)
# print(p2.calculate_distance(p1))  # 5.0
# assert (p2.calculate_distance(p1) == p1.calculate_distance(p2)) # True

# p1.move(3, 4)
# print(p1.calculate_distance(p2))  # 4.47213595
# print(p1.calculate_distance(p1))  # 0.0