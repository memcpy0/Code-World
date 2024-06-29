# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-11
program     : 使用本子下载器中的页数下载法和漫步法下载不确定的多页内容
"""

"""
单本小说下载步骤：
  1 输入小说介绍页的ID，抓取小说目录首页的页数字符
  2 循环页数构造不同页面的url，获取每一页的所有章节的ID，替换m为www，all为章节ID，从而请求网络版而非移动版
  3 对每一页的文章进行下载和清理
  4 存储为TXT文件

全类别爬取：
  1 对不同类别的页面选择不同的页数，循环页数构造不同页面的url
  2 获取每一页的所有小说的ID，即介绍页和小说目录页ID元素
  3 进行单本小说下载的步骤
"""

import datetime, time, os
import requests
import re #, pyperclip
from lxml import etree
# import send2trash # shutil
# from multiprocessing.dummy import Pool
# import random

def addChaptTitle(cnt):  # cnt为int 1024
    # if not isinstance(cnt, str):
    cnt_str = str(cnt)   # '1024'
    if len(cnt_str) > 1:
        cnt_head, cnt_tail = cnt_str[:1], cnt_str[1:]  # 1 024 # 0
        return chapt_dict[int(cnt_head)]+ addChaptTitle(cnt_tail)  # '一'
    else:
        return chapt_dict[int(cnt_str)]


class hText(object):

    def __init__(self):
        self.session = requests.session()
        self.__headers = {
            "User-Agent" : "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
        }
        # self.__proxies = {
        #     "http" : "http://1.199.193.95:808",
        #     "https" : "https://220.191.82.63:45017"
        # }
        self.__index = {
            1 : "http://m.ltxs52.com/{}/all_{}.html",  # 基础URL
            2 : "http://m.ltxs52.com/sort-3-{}/",
            3 : "http://m.ltxs52.com/sort-5-{}/",   # 精品辣文小说
            4 : "http://m.ltxs52.com/sort-7-{}/",   # 其他小说
        }
        self.__dirname = "./LT小说/"
        os.makedirs(self.__dirname, exist_ok=True)
        # self.__pool = Pool(4)

    def getUrlReponse(self, url):
        try:
            self.response = self.session.get(url, headers=self.__headers) #, proxies=self.__proxies)
            if self.response is None:
                self.response = self.session.get(url, headers=self.__headers) #, proxies=self.__proxies)
            self.response.raise_for_status()
            self.response.encoding = 'gbk'
        except Exception as e:
            print("HTTP请求失败!", e)
        else:
            return self.response

    def reg_filter(self, text):
        s = re.sub(r'[?\\*|"<>:/]', '', text)
        return s if not '&nbsp;' in text else s.replace('&nbsp;', '')

    def informationPrint(self):
        sepl = "*" * 100
        print(sepl)
        # 类方法 直接用类调用 # date = datetime.datetime.fromtimestamp(time.time())
        date = datetime.datetime.now()
        print("今天是" + date.strftime('%Y-%m-%d, %H:%M:%S, %A') + ", 欢迎使用龙腾小说下载工具!")
        print(sepl)
        for key, value in self.__index.items():
            print(key, "  ", value, end='\n')
        print("本工具支持:输入相应小说页ID，下载小说内容\n或选择两个URL之一(两类小说)，直接遍历下载多页目录的小说。")
        print("ID格式如下: * 0_668")
        print(sepl)

    def onebook(self, _id):
        pages = 0
        chapt_cnt = 0
        ul = self.__index[1].format(_id, 1) # 填写小说ID和章节第一页的URL
        if self.getChaptPagesWithTitle(ul):
            pages, title = self.getChaptPagesWithTitle(ul)
            pages = int(pages) + 1
            self.title = self.reg_filter(title)
            print(self.title, ul)

        for s in self.getChaptIds(_id, pages):
            chapt_cnt += 1
            sl = self.__index[1].replace('all_{}.html', s).replace('http://m', 'http://www').format(_id)
            self.download(sl, chapt_cnt)
            print(sl, 'Done!')

    def morebook(self, kind):
        for d in self.getBookIDsList(kind):
            self.onebook(d)

    def getBookIDsList(self, cnt):
        """得到小说类别页的所有小说的ID列表"""
        index_pages = 0
        if cnt == 2:
            index_pages = 67
        elif cnt == 3:
            index_pages = 17  # 320本
        for i in range(1, index_pages):
            books_url_index = self.__index[2].format(i)
            time.sleep(.3)
            book_ids = etree.HTML(self.getUrlReponse(books_url_index).text)
            for x in book_ids.xpath("/html/body/div[5]/p/a[2]/@href"):
                yield x.replace('/', '')

    def getChaptPagesWithTitle(self, url):
        """返回一次，得到小说的章节目录页的页数和小说名"""
        time.sleep(.3)
        page_text = self.getUrlReponse(url).text
        reg1 = re.compile(r'<h3>(.*?)</h3>')
        result1 = re.findall(reg1, page_text)
        reg2 = re.compile(r'\(第\d/(\d*)页\)当前\d*条/页')  # \d一个数字字符 0-9 ()分组匹配 \( 转义
        result2 = re.findall(reg2, page_text)
        # try:
        if result2:
            return result2[0], result1[0]
        # except IndexError:
        #     return None  # 没有其他return语句或return语句条件不成立时，默认会返回None

    def getChaptIds(self, book_id, pages):
        """返回多次，得到每一章的ID"""
        for p in range(1, pages):
            time.sleep(.3)
            chapt_text = self.getUrlReponse(self.__index[1].format(book_id, p)).text
            chapt_ids = etree.HTML(chapt_text)
            chapt_ids = chapt_ids.xpath('/html/body/div[2]/ul/li/a/@href')
            for c in chapt_ids:
                yield c.split('/')[-1]

    def download(self, url, cn):
        """下载一章的内容"""
        time.sleep(.3)
        chapt = self.getUrlReponse(url).text
        try:
            reg1 = re.compile(r'<h1>(.*?)</h1>')
            chapt_title = re.findall(reg1, chapt)[0]
            # if not re.search(r'第.*?章', chapt_title):
            x = ''.join(chapt_dict[int(i)] for i in str(cn))
            chapt_title = '第{}章 *{}'.format(x, chapt_title)

            reg2 = r'<div id="content">&nbsp;&nbsp;&nbsp;&nbsp;(.*?)</div>'
            reg2 = re.compile(reg2, re.S)  # S 多行匹配
            chapt_text = re.findall(reg2, chapt)
            chapt_text = chapt_text[0].replace("&nbsp;&nbsp;&nbsp;&nbsp;", "").replace("<br />", "").replace('.', '')

            with open(self.__dirname+self.title+'.txt', 'a', encoding='utf-8') as f:
                f.write('\n\n'+chapt_title+'\n\n'+chapt_text+'\n')
        except Exception as e:
            print(e)

if __name__ == "__main__":
    chapt_dict = {
        1: "一", 2: "二",
        3: "三", 4: "四",
        5: "五", 6: "六",
        7: "七", 8: "八",
        9: "九", 0: "零",
    }
    # print(addChaptTitle(1024))
    # novel_url =  "http://m.ltxs52.com/" + input("输入小说书名页的URL地址中的ID:") + '/all.html'
    bk = hText()
    bk.informationPrint()
    bk.onebook(input("请输入小说ID:"))


