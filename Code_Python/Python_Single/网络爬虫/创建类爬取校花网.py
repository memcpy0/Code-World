# -*- coding:utf-8 -*-
# -*- topic:创建爬取校花网的类 -*-
# -*- arthor:张平   -*-

import os
import re
from urllib.request import urlretrieve
import requests

class Spider(object):
    def __init__(self):
        # 初始化一个Session对象
        self.session = requests.session()

    def get(self, url):
        try:
            # 返回响应对象
            return self.session.get(url)
        except Exception as e:
            print(e)
            print("GET下载错误")

    def run(self, start_url):
        """ 获取首页面的HTML源码 """
        response = self.get(start_url)
        response.encoding = "gb2312"
        if response:
            html = response.text
            img_items = self.get_img_items_by_html(html)[:119]
            #print(len(img_items), img_items[1])
            for img_item in img_items:
                self.save_img(img_item)
                print('正在下载%s的图片' % img_item['file_name'])
            print("下载完成!")

    def get_img_items_by_html(self, html):
        #<img width="210" alt="南京航空航天大学校花陈都灵" src="http://www.xiaohuar.com/d/file/20140811101923185.jpg">
        # 获得全部校花的信息集合，包括姓名和照片URL
        imgs = re.findall(r'<img width="210".*?>', html)
        #print(len(imgs), imgs[1])
        img_items = []
        begin_url = 'http://www.xiaohuar.com'
        # 信息列表，元素为字典
        if imgs:
            for img in imgs:
                img_url = re.findall(r'src="(.*?)"', img)[0]
                img_item = {
                    'file_name':re.findall(r'alt="(.*?)"', img)[0],
                    'url':img_url if img_url.startswith(begin_url) else begin_url+img_url
                }
                img_item['file_name'] = img_item['file_name'] + '.' + img_item['url'].split('.')[-1]
                img_items.append(img_item)
            return img_items

    def save_img(self, img_item):
        try:
            urlretrieve(img_item['url'], os.path.join(dirname, img_item['file_name']))
        except Exception as e:
            print(e)
            print("save_img下载出错")

if __name__ == "__main__":
    dirname = './校花网2014校花图片'
    if not os.path.exists(dirname):
        os.makedirs(dirname)
    first_url = 'http://www.xiaohuar.com/2014.html'
    spider = Spider()
    spider.run(first_url)


