# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-04 23:55
program     : 下载B站视频
"""

import time, datetime
import requests  #, webbrowser, requests_download
import pyperclip
from lxml import etree
# from bs4 import BeautifulSoup
import json, re
import send2trash #,shutil
from requests.exceptions import RequestException
# from urllib.request import urlretrieve
# import threading

class BiliSpider:
    def get_page(self, page, search):
        headers = {
            "User-Agent":"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
        }
        url = 'https://api.bilibili.com/x/web-interface/search/type?jsonp=jsonp&' \
              'search_type=video&highlight=1&keyword={}&page={}'.format(search, page)
        try:
            r = requests.get(url, headers=headers)
            r.raise_for_status()
            if r.status_code == 200:
                return r.json()
            else:
                print(r.status_code)
        except RequestException:
            print('请求失败')
            return None

    def parse_page(self, html):
        # 转换成json对象
        # 转换a ``str``, ``bytes`` or ``bytearray`` instance containing a JSON document)成为python数据结构
        # data = json.loads(html)
        results = html.get('data').get('result')
        for result in results:
            image_url = result['pic']        # 获取图片地址
            video_url = result['arcurl']     # 获取视频地址
            video_author = result['author']  # 获取作者
            video_title = re.sub('<em class="keyword">.*</em>', self.search, result['title'])   # 获取视频标题
            video_play = result['play']      # 获取视频播放量
            video_date = result['pubdate']   # 获取上传时间，这里将时间戳seconds转换为标准格式
            # Convert seconds since the Epoch to a time tuple expressing local time
            timestr = time.localtime(video_date)
            video_date = time.strftime('%Y-%m-%d %H:%M:%S', timestr)
            # Date = datetime.datetime.utcfromtimestamp(video_date) # -> Date.strftime() # 同样的方法
            print(image_url, video_url, video_title, video_play, video_date)

    def run(self):
        self.search = input('输入要搜索的关键词')
        for i in range(1, 3):
            html = self.get_page(i, self.search)
            self.parse_page(html)

bili = BiliSpider()
bili.run()


