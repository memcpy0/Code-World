# -*- coding:utf-8 -*-
# -*- arthor:张平  -*-

import os
import time
import requests
import re
from lxml import etree
from bs4 import BeautifulSoup
from urllib.request import urlretrieve
from multiprocessing.dummy import Pool

class Ckingiot(object):
    def __init__(self, headers, start_url='http://www.ckingiot.com'):
        self.session = requests.session()
        self.start_url = start_url
        self.headers = headers

    def getUrlReponse(self, url):
        try:
            self.response = self.session.get(url)
            self.response.encoding = 'gb2312'
        except Exception as e:
            print("HTTP请求失败!")
        else:
            return self.response

    def queryVideoPic(self):
        """
        * /list/index35.html亚洲av            * /list/index36.html日韩经典
        * /list/index37.html国内自拍          * /list/index38.html欧美激情
        * /list/index39.html综合AV           * /list/index40.html强奸乱伦
        * /list/index41.html伦理影片          * /list/index42.html巨乳专区
        * /html/part/index43.html亚洲性图     * /html/part/index44.html网友自拍
        * /html/part/index45.html欧美色图	 * /html/part/index46.html丰乳肥臀
        * /html/part/index47.html国产模特	 * /html/part/index48.html唯美清纯
        * /html/part/index49.html丝袜诱惑	 * /html/part/index50.html卡通漫画
        """
        sepl = "*" * 100
        print(sepl)
        print(" " * 20 + "欢迎使用CKingiot网站视频图片下载工具!")
        print(sepl)
        index = {
            35: '亚洲av', 36: '日韩经典', 37: '国内自拍', 38: '欧美激情',
            39: '综合AV', 40: '强奸乱伦', 41: '伦理影片', 42: '巨乳专区',
            43: '亚洲性图', 44: '网友自拍', 45: '欧美色图', 46: '丰乳肥臀',
            47: '国产模特', 48: '唯美清纯', 49: '丝袜诱惑', 50: '卡通漫画'
        }
        cnt = 0
        for key, value in index.items():
            cnt += 1
            if cnt <= 4:
                print(' ' * 2, value, "\t-->", key, end='|')
            else:
                cnt = 1
                print("\n", end="")
                print(' ' * 2, value, "\t-->", key, end='|')
        print('\n' + sepl)
        print("本工具支持")
        print("\t\t1.单个视频输入视频页URL下载到指定文件夹\n\t\t2.跨页抓取视频文件地址存放到CSV文件\n\t\t"
              "3.单页套图输入URL下载到指定文件夹\n\t\t4.跨页抓取套图文件存放到指定文件夹等功能")
        print(sepl)
        video_pic_id = int(input("请输入功能序列号:"))
        if video_pic_id == 1:
            video_url = "http://www.ckingiot.com/view/index5556.html" # input("请输入视频页的地址:")
            self.videoDownload(video_url)
        elif video_pic_id == 2:
            pass
        elif video_pic_id == 3:
            pass
        else:
            print("你的输入有误!")

    def videoDownload(self, video_url):
        html = self.getUrlReponse(video_url).text
        video_play_url = re.findall(r"<a title='.*?' href='(.*?)' target=\"_blank\">", html)[0]
        print(video_play_url)
        # html = etree.HTML(html)
        # video_play_url = html.xpath('/html/body/div[8]/div/div[2]/ul/ul/li/a/@href')
        print(self.start_url+video_play_url)

        video_play_html = self.getUrlReponse(self.start_url+video_play_url).text
        video_js = re.findall(r'<script type="text/javascript" src="(.*?)"></script>', video_play_html)[0]
        print(self.start_url+video_js)

        video_store_js = self.getUrlReponse(self.start_url+video_js).text
        res = re.findall(r"[$](.*?)[$]", video_store_js)
        print(res)

    def videoDownloadMore(self, url):
        pass

    def picDownload(self, url):
        pass

    def picDownloadMore(self, url):
        pass


if __name__ == "__main__":
    #url = "http://ckingiot.com/playdata/228/7140.js?45241.15"
    headers = {
        "User-Agent" : "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
    }
    ck = Ckingiot(headers=headers)
    ck.queryVideoPic()

    #filename = '1.mp4'
    #pathname = './Ckingiot资源'
    #if not os.path.exists(pathname):
    #    os.mkdir(pathname)

    #video_url = "https://201806.53didi.com/20180810/14/1/xml/91_79d8b38531e742eeee33b2aab0c88d3e.mp4"
    # htm = html.split('$')[1]
    #reg = re.compile(r"[$](.*?)[$]")
    #res = re.findall(reg, html)[0]
    # urlretrieve(htm, os.path.join(pathname, filename))
    # print("Success!")
