# -*- coding:utf-8 -*-
import requests
import re
from lxml import etree
#from bs4 import BeautifulSoup
import os
#import time
"""
这个脚本并非用来持续爬取全站或某一种类的小说，而是我输入url就爬取相应的小说
"""
"""
def sortKind():
    pass

def test_path(dirname):
    if not os.path.exists(dirname):
        os.makedirs(dirname)
"""
def reg_filter(text):
    return re.sub(r'[?\\*|"<>:/]', '', text)

def getHtml(url):
    r = requests.get(url, headers=headers) # ISO-8859-1 GB2312
    r.encoding = "gbk"
    html = r.text
    content = etree.HTML(html)
    return content

def test_url(url):
    content = getHtml(url)
    if re.search('all', url):
        page = content.xpath('/html/body/div[2]/ul/li/a/@href')
    else:
        page = content.xpath('//*[@id="nr1"]/text()')
    return True if page else False

def getTitle(url):
    chapt = getHtml(url)
    book_title = chapt.xpath('//*[@id="_52mb_h1"]/text()')[0]
    book_title = reg_filter(book_title)
    with open(os.path.join(dir_name, book_title + '.txt'), 'a') as f:
        f.write('')
    return os.path.join(dir_name, book_title + '.txt')

def getChapterId(chapters_url):
    chapt = getHtml(chapters_url)
    chapt_id = chapt.xpath('/html/body/div[2]/ul/li/a/@href')
    chapt_title = chapt.xpath('/html/body/div[2]/ul/li/a/text()')
    id_list = list(zip(chapt_title, chapt_id))
    #print(id_list)
    return id_list

def getChapterContent(book_title, id_list):
    url = "http://m.ltxs52.com"
    for title, i in id_list:
        f = open(book_title, 'a', encoding='utf-8')
        f.writelines(title+'\n')
        for p in range(1, 20):
            # url = "http://m.ltxs52.com/0_668/40871_1.html"
            chapt_url = url + i.split('.')[-2] + '_{}.html'.format(p)
            #if test_url(chapt_url):
            page = getHtml(chapt_url)
            page = page.xpath('//*[@id="nr1"]/text()')
            # page = re.sub("-->>(.*?)（(.*?)）", '', page)
            for nr in page:
                f.write(nr + '\n')
        print(title, "下载成功!")
    print("完成下载！")

if __name__ == "__main__":
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
    }
    dir_name = "D:\\Python_Projects\\Pycharm_Projects\\Python网络爬虫爬取\\LT小说"
    # test_path(dir_name)
    novel_url =  "http://m.ltxs52.com/" + input("输入小说书名页的URL地址中的ID:") + '/'
    end_pages = int(input("请输入目录章节的网页页数:")) + 1
    #end_page =  int(input("章节页数:")) + 1
    bk = getTitle(novel_url)
    for i in range(1, end_pages):
        string = "all_%d.html" % i
        chapters_url = str(novel_url)+ string  # ???
        #if test_url(chapters_url):
        id_list = getChapterId(chapters_url)
        getChapterContent(bk, id_list)

