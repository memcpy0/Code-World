# -*- coding:utf-8 -*-
import requests
import os
import time
import re
#from lxml import etree
from bs4 import BeautifulSoup
from urllib.request import urlretrieve

# 获取豆瓣妹子图片
def query_chose():
    girlsAttr = {
         0 : "所有" ,
         2 : "大胸妹",
         3 : "美腿控",
         4 : "有颜值",
         5 : "大杂烩",
         6 : "小翘臀",
         7 : "黑丝袜"
    }
    for num in girlsAttr:
        print(num, ":", girlsAttr[num])
    attr = int(input("请输入要抓取的妹子图片中妹子的属性:"))
    start_page = int(input("请输入要抓取的起始页面数:"))
    end_page = int(input("请输入要抓取的终止页面数:"))
    return attr, start_page, end_page

def test_path(path):
    if not os.path.exists(path):
        os.makedirs(path)

def getGirlsPic(page_offset):
    url = "https://www.dbmeinv.com/index.htm?cid={}&pager_offset={}".format(girlattr, page_offset)
    r = requests.get(url)
    r.encoding = r.apparent_encoding
    html = r.text

    """
    reg = r'"img_error(this);" src="(.*?)"/> </a>'
    re.compile(reg, re.S)
    reg = re.findall(reg, html)
    print(reg)
    """

    x = 0
    soup = BeautifulSoup(html, 'html.parser')  # 创建对象 解析网页
    grill = soup.find_all('img')  # 找到img标签
    for i in grill:
        imgsrc = i.get('src')     # 找到src属性 .get() Beautifulsoup的方法
        imgtitle = i.get('title')
        imgtitle = re.sub(r'[?\\*|"<>:/]', '', imgtitle)
        x += 1
        print("正在下载第%d张图片" % x)
        urlretrieve(imgsrc, os.path.join(dirname, '%s.jpg' % imgtitle))
        time.sleep(1)


dirname = 'D:/Python_Projects/Pycharm_Projects/网络爬虫爬取/豆瓣妹子'
test_path(dirname)
girlattr, st_page, ed_page = query_chose()

for page in range(st_page, ed_page+1):
    print("正在下载第{}页图片".format(page))
    getGirlsPic(page)