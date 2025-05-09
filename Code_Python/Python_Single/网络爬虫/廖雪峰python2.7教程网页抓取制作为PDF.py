# -*- coding:utf-8 -*-

"""
author      : myRealization
create_time : 2018-08-30 10:00
program     : 廖雪峰python2.7教程网页抓取制作为PDF
"""
# import pyperclip
# import re, string
import time #, datetime
import requests #, urllib.request
# from lxml import etree
from bs4 import BeautifulSoup
import sys, threading #, os, glob,  subprocess
# from multiprocessing.dummy import Pool
# from PIL import Image
import pdfkit #, PyPDF2, reportlab
# import csv, json
# import smtplip, imapclient, pyzmail
import lxml
import queue
import codecs
reload(sys)
sys.setdefaultencoding('utf8')

class crawl:
    def __init__(self):
        self.file = odecs.open("python.txt", 'w', encoding='utf-8')
        self.html_template = """
        <!DOCTYPE html>
<html lang="en">
<head>
     <meta charset="UTF-8">
     <link rel="stylesheet" type="text/css" href="demo.css">
 </head>
 <body>
 {0}
 </body>
 </html>"""
        self.options = {
            'page-size': 'Letter',
            'margin-right': '0.75in',
            'margin-bottom': '0.75in',
            'margin-left': '0.75in',
            'encoding': "UTF-8",
            'custom-header': [
                ('Accept-Encoding', 'gzip')
            ],
            'cookie': [
                ('cookie-name1', 'cookie-value1'),
                ('cookie-name2', 'cookie-value2'),
            ],
            'outline-depth': 10,
        }
        self.files=[]
        self.q = queue.Queue() #
        self.headers = {
            "User-Agent": 'Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.96 Mobile Safari/537.36'}
        self.lock=threading.Lock() #

    def parse_url(self):
        """
        得到所有的章节链接，并且将章节的链接存储在队列中
        :return:
        """
        url = 'http://www.liaoxuefeng.com/wiki/001374738125095c955c1e6d8bb493182103fac9270762a000'
        res = requests.get(url=url, headers=self.headers)
        soup = BeautifulSoup(res.text, 'lxml')
        lis = soup.find("div", class_='x-sidebar-left-content').find_all("ul", class_='uk-nav uk-nav-side')[1].find_all(
            "li")
        for li in lis:
            url = "http://www.liaoxuefeng.com" + li.find('a').get('href')
            self.q.put(url)

    def parse_html(self):
        """
        提取队列中的URL，请求获得其中的内容，然后写入模板、
        title:提取文章中的标题
        center_tag：使用BeautifulSoup新创建的节点，用于居中显示
        title_tag:新创建的h1标签，用于包裹提取的标题
        content：提取的正文内容，这里其他的包括评论都没有提取，这个用不到
        :return:
        """
        while not self.q.empty():
            url = self.q.get()
            print(url)
            self.lock.acquire()
            res = requests.get(url, headers=self.headers)
            soup = BeautifulSoup(res.text, 'lxml')
            self.lock.release()
            try:

                title = soup.find("div", class_='x-content').find("h4").get_text()  # 标题
                center_tag = soup.new_tag('center')
                title_tag = soup.new_tag("h1")
                title_tag.string = title
                center_tag.insert(1, title_tag)
                content = soup.find("div", class_='x-wiki-content')  # 内容
                content.insert(1, center_tag)
                img_tag = content.find_all("img")  #获取内容的中的所有img标签
                for img_url in img_tag:
                    #修改内容中的img标签的src，将其补全为绝对路径，否则不能正常显示
                    img_url['src'] = 'http://www.liaoxuefeng.com' + img_url['src']
                file_name = url.split('/')[-1] + '.html'
                html = self.html_template.format(content)  # 得到的每一章节的内容
                self.files.append(file_name)      #这里将每一章节的内容写入到文件中，以便后面直接提取
                print(file_name)

                with codecs.open(file_name, 'wb', encoding='utf-8') as f:
                    f.write(html)

            except:
                print('**************************************************')

    def write_pdf(self):
        pdfkit.from_file(self.files,'demo.pdf',options=self.options)

if __name__ == '__main__':
    demo = crawl()

    threads=[]
    demo.parse_url()
    for i in range(0,40):
        t2=threading.Thread(target=demo.parse_html,args=[])
        threads.append(t2)
    for t in threads:
        t.start()
    for t in threads:
        t.join()
    print("chen")

    demo.write_pdf()