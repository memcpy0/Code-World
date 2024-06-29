# -*- coding:utf-8 -*-
import requests
from lxml import etree
import re
import json
# import time
import os

headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
    }

def getFMSound():
    url = "https://www.ximalaya.com/revision/play/album?albumId=269179"
          #&pageNum=1&sort=-1&pageSize=30"
    r = requests.get(url, headers=headers)
    # r 'requests.models.Response'对象 | r.content bytes | r.content.decode() json类型的str
    result = json.loads(r.content.decode())  # -> dict
    temp = result['data']['tracksAudioPlay']
    pass

def getShangyeHref():
    shangye_url = "https://www.ximalaya.com/shangye/top"
    html = requests.get(shangye_url, headers=headers).text
    shangye  = etree.HTML(html)
    shangye_id = shangye.xpath('//div[@class="e-2997888007 rrc-list"]/div/a/@href')
    for i in shangye_id:
        print(i.split('/')[-2])
    print(len(shangye_id))

class Xima(object):
    def __init__(self):
        self.headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
        }
        self.book_url = []
        '''
        self.start_url = "https://www.ximalaya.com/revision/play/album?albumId=269179&pageNum={}&sort=-1&pageSize=30"
        for i in range(8):
            url = self.start_url.format(i+1)
            self.book_url.append(url)
        # print(self.book_url)
        '''

    def get_id(self):
        # 50个 财经免费榜
        self.start_url = "https://www.ximalaya.com/revision/play/album?albumId={}&pageNum={}&sort=-1&pageSize=30"
        shangye_url = "https://www.ximalaya.com/shangye/top"
        html = requests.get(shangye_url, headers=self.headers).text
        shangye  = etree.HTML(html)
        shangye_id = shangye.xpath('//div[@class="e-2997888007 rrc-list"]/div/a/@href')
        id_list = []
        for i in shangye_id:
            id_list.append(i.split('/')[-2])
        for r in id_list:
            for i in range(8):
                url = self.start_url.format(r, i + 1)
                self.book_url.append(url)

    def get_book_msg(self):
        '''获得当前书的音频信息'''
        all_list = []
        for url in self.book_url:
            r = requests.get(url, headers=self.headers)
            ret = r.content.decode()
            python_dict = json.loads(ret)
            book_list = python_dict['data']['tracksAudioPlay']
            # print(book_list)

            for book in book_list:
                list = {}
                list['src'] = book['src']
                list['name'] = book['trackName']
                list['albumName'] = book['albumName']
                #print(list)
                all_list.append(list)
        return all_list # 这是所有音频的信息, 是一个列表，里面是每一个音频信息的字典
    
    def save(self, all_list):
        for i in all_list:
            i['albumName'] = re.sub(r'[?\\*|"<>:/]', '', i['albumName'])
            i['name'] = re.sub(r'[?\\*|"<>:/]', '', i['name'])
            dir_name = "xima/{}".format(i['albumName'])
            if not os.path.exists(dir_name):
                os.makedirs(dir_name)
            file_name = "{}.mp4".format(i['name'])

            with open(os.path.join(dir_name, file_name), 'ab') as f:
                r = requests.get(i['src'], headers=self.headers)
                ret = r.content
                f.write(ret)
                print(file_name + "下载完成!")

    def run(self):
        self.get_id()
        all_list = self.get_book_msg()
        self.save(all_list)

if __name__ == "__main__":
    xima = Xima()
    xima.run()
    #getFMSound()
    #getShangyeHref()

