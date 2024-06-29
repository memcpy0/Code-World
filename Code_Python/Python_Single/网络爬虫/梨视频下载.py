# -*- coding:utf-8 -*-
import requests
#import os
import re
from lxml import etree
from urllib.request import urlretrieve
import time

def download(url):
    # url = "http://www.pearvideo.com/category_9"#.format()
    headers = {
        "User-Agent":"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
    }
    r = requests.get(url, headers=headers)
    r.encoding = r.apparent_encoding
    html = r.text

    # 获取视频ID
    """
    # -regex
    reg = r'<a href="video_(.*?)" class="vervideo-lilink actplay">'
    re.compile(reg, re.S)
    res = re.findall(reg, html)
    print(res)
    """
    # -xpath
    res = etree.HTML(html)
    # /html/body/div[2]/div[1]/div/ul/li[1]/div/a
    video_id = res.xpath('//div[@class="vervideo-bd"]/a/@href')
    #print(video_id)

    # 拼接完整视频网页地址
    video_url = []
    start_url = 'http://www.pearvideo.com/'
    for i in video_id:
        video_url.append(start_url+i)

    for real_url in video_url:
        response = requests.get(real_url, headers=headers).text
        # http://video.pearvideo.com/mp4/adshort/20180807/cont-1405699-12614762_adpkg-ad_hd.mp4
        reg = r'srcUrl="(.*?)"'
        re.compile(reg)
        purl = re.findall(reg, response)

        rex = r'<h1 class="video-tt">(.*?)</h1>'
        re.compile(rex)
        pname = re.findall(rex, response)

        print("正在下载视频:%s" % pname)
        urlretrieve(purl[0], 'D:/Python_Projects/Pycharm_Projects/Python网络爬虫爬取/pearVideo/%s.mp4' % pname[0])

def downloadmore():
    # http://www.pearvideo.com/category_loading.jsp?reqType=5&categoryId=8&start=48
    n = 0
    while True:
        if n >= 48:
            return
        url = "http://www.pearvideo.com/category_loading.jsp?reqType=5&categoryId=8&start=%d" % n
        n += 12
        download(url)
        time.sleep(2)

downloadmore()

