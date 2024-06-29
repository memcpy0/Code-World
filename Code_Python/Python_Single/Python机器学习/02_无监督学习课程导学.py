# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 
program     : 
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
无标签学习：学习数据的分布和数据之间的关系
聚类 cluster       计算距离
* 欧式距离
* 曼哈顿距离
* 马氏距离
* 余弦相似度 
- 标准数据格式 [样本格式，特征个数]  # data target feature
降维 decomposition  将高维数据降至低维 sklearn库提供7种降维算法
- PCA
- FastICA  特别适合大规模数据
- NMF
- LDA
"""

"""
K-Means 以k为参数，把n个对象分为k个簇

"""
import numpy as np
import matplotlib.pyplot as plt

if __name__ == "__main__":
    data, cityName = loadData