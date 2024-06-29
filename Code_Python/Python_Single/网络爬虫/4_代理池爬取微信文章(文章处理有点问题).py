# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-11 11:46
program     : requests代理池proxies使用
"""

import requests  #, urllib.request
from lxml import etree
import os

def spider(url):
    r = requests.get(url, headers=headers) #, proxies=proxies)
    return etree.HTML(r.text)

def getAllUrl(word, pages):
    # 内容词, 页数
    for page in range(1, int(pages)+1):
        url = "http://weixin.sogou.com/weixin?oq=&query={}&_sug_type_=1&sut=0&lkt=0%2C0%2C0&s_from=input&ri=0&_sug_=n&" \
              "type=2&sst0=1536660464794&page={}&ie=utf8&p=40040108&dp=1&w=01015002&dr=1".format(word, page)
        selector = spider(url)
        every_url = selector.xpath('//*[starts-with(@id,"sogou_vr_11002601_title_")]/@href')
        all_url.extend(every_url)

# 爬取详情页
def spiderXiangQing(url):
    selector = spider(url)
    title = selector.xpath('//*[@id="activity-name"]/text()')[0].strip()
    content = selector.xpath('//*[@id="img-content"]')[0]
    article = content.xpath('string(.)').strip() # .replace('\r', '').replace('\n', '')
    article.encode('utf-8').decode('utf-8')
    towrite(article, title)

def towrite(item, title):
    try:
        with open('./微信文章/'+title.replace('|', '')+'.txt', 'w', encoding='utf-8') as f:
            f.write(item)
            print("正在下载", title)
    except Exception as e:
        print("下载失败", title, e)

if __name__ == "__main__":
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
    }
    # proxies = {
    #     "https" : "http://118.190.95.35:9001",
    #     "https" : "http://61.135.217.7:80",
    #     "https" : "http://182.88.89.230:8123",
    #     "https" : "http://180.118.243.229:61234",
    #     # "https" : "http://110.73.40.111:8123",
    # }
    os.makedirs('./微信文章/', exist_ok=True)
    keyword = input("请输入要搜索的关键词:")
    search_pages = input("请输入要抓取的页数(必须是自然数):")
    all_url = []
    getAllUrl(keyword, search_pages)
    for url in all_url:
        spiderXiangQing(url)
