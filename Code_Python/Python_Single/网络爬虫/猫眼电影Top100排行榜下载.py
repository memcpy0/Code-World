# -*- coding:utf-8 -*-
# -*- topic:猫眼电影TOP100信息爬取存储为CSV -*-
# -*- arthor:张平   -*-

import requests
#import urllib.request
import json, csv
import re
#from bs4 import BeautifulSoup
from lxml import etree
import time, datetime
#import os

def csvWriter(item, cnt):
    with open("猫眼电影排行榜TOP100.csv", 'a+', newline='', encoding='gbk') as f:
        writer = csv.writer(f)
        if cnt == 0:
            writer.writerow(['排名', '电影名称', '主演', '上映时间'])
        for i in item:
            writer.writerow(i)

def jsonWriter(content):
    with open('猫眼六六六.json', 'a', encoding='utf-8') as f:
        f.write(json.dumps(content, ensure_ascii=False)+'\n')

def getPage(url, cnt):
    time.sleep(1)
    r = requests.get(url, headers=headers)
    r.encoding = 'utf-8'
    html = etree.HTML(r.text)
    """
    rank_reg = re.compile(r'<i class="board-index board-index-(.*?)">')
    rank = re.findall(rank_reg, htm)
    star_reg = re.compile(r'<p class="star">(.*?)</p>', re.S)
    star = re.findall(star_reg, htm)
    star = [s.strip() for s in star]
    release_reg = re.compile(r'<p class="releasetime">(.*?)</p>')
    release_time = re.findall(release_reg, htm)
    """
    rank = [i for i in range(cnt*10+1, (cnt+1)*10+1)]
    name = html.xpath('//p[@class="name"]/a/text()')
    star = [s.strip() for s in html.xpath('//p[@class="star"]/text()')]
    release_time = html.xpath('//p[@class="releasetime"]/text()')
    item = list(zip(rank, name, star, release_time))
    csvWriter(item, cnt)

def getOnePage(url):
    time.sleep(1)
    r = requests.get(url, headers=headers)
    r.encoding = 'utf-8'
    htm = r.text
    html = etree.HTML(r.text)
    name = html.xpath('//p[@class="name"]/a/text()')
    star = [s.strip() for s in html.xpath('//p[@class="star"]/text()')]
    release_time = html.xpath('//p[@class="releasetime"]/text()')
    for it in range(10):
        yield {
            'index':it+1,
            'name':name[it],
            'star':star[it],
            'releasetien':release_time[it]
        }

if __name__ == "__main__":
    start_url = "http://maoyan.com/board/4?offset={}" # TOP100榜
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
    }

    for i in range(10):
        #getPage(start_url.format(i*10), i)
        for content in getOnePage(start_url.format(i*10)):
            jsonWriter(content)
            print(content)
