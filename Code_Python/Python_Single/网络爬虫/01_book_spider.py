# -*- coding: utf-8           -*-
# -*- topic : 本子下载整合为PDF -*-
# -*- author: myRealization   -*-

import os
import sys
import time
import datetime
import requests
import webbrowser
import re
import pyperclip
from lxml import etree
import send2trash # shutil
from multiprocessing.dummy import Pool
from PIL import Image
from reportlab.pdfgen import canvas
# from reportlab.lib.pagesizes import  A3, A2
from reportlab.lib.utils import ImageReader
# import PyPDF2
import random

def tryPrint(arg):
    try:
        print(arg)                             # unprintable filename
    except UnicodeEncodeError:
        print(arg.encode())                    # try raw byte string

class hBook(object):
    getcount = 0
    def __init__(self):
        self.session = requests.session()
        self.headers = {
        "User-Agent" : "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
        }
        self.__index = {
            0 : "https://www.xieeji.cc/", 
            1 : "https://www.xieeji.cc/wuyiniao/",
            2 : "https://www.xieeji.cc/shaonv/",  # lifan # benzi
            3 : "https://www.xieeji.cc/shenshe/",
            4 : "https://www.svmh.net/shaonvmanhua/", 
            5 : "http://www.svmh.net/kalie/"
        }
        self.dirname = "./本子合集/"
        os.makedirs(self.dirname, exist_ok=True)
        self.picFileList = []                                 # 初始化一个记录图片名顺序的列表
        self.pool = Pool(6)

    def informationPrint(self):
        sep = "*" * 100
        print(sep)
        date = datetime.datetime.now()
        print("今天是" + date.strftime('%Y-%m-%d,%H:%M:%S') + ", 欢迎使用本子下载工具!")
        print(sep)
        for key, value in self.__index.items():
            print(key, "  ", value, "\n")
        print("本工具支持:选择上面几个URL之一，通过复制"
              "网站本子ID，下载整合想要的本子图片为PDF")
        print("ID格式如下:\n\t"
              "* lifan/14458\n\t"
              "* benzi/11468\n\t"
              "* gongkou/11195\n\t"
              "* ribenmanhua/14040"
              "注:此时应选择网站序列号2!")
        print("第0号是初始的网址，可以下载不限于1-3号的各种图片!");
        print(sep)

    def surfWebsite(self):
        for web in random.choices(list(self.__index.values()), k=2):
            webbrowser.open(web)

    def getUrlReponse(self, url):
        try:
            self.response = self.session.get(url, headers=self.headers)
            self.response.raise_for_status()
            self.response.encoding = 'UTF-8'
            hBook.getcount += 1
            if self.response is None and hBook.getcount < 5:
                self.response = self.getUrlReponse(url)
        except Exception as exception:
            tryPrint('GetUrlResponse Error: %s from %s' % (exception, url))
        else:
            return self.response

    def picUrlFinder(self, start_url):
        html = self.getUrlReponse(start_url).text
        html = etree.HTML(html)
        name = html.xpath("/html/body/h1/text()")[0]
        bookname = re.sub(r'[*\\/":?<>|：。（），；]', '', name)
        self.dirname += bookname
        os.makedirs(self.dirname, exist_ok=True)

        pages = html.xpath('//*[@id="mh_content"]/div[3]/div/ul/li[1]/a/text()')[0]
        comic_pages = re.findall(r'共(.*?)页: ', pages)[0]
        print("该漫画本子有%s页." % comic_pages)

        url = start_url.replace('.html', '_{}.html')
        for ul in (url.format(i) for i in range(1, int(comic_pages) + 1)):
            time.sleep(.2)
            comic_html = self.getUrlReponse(ul).text
            try:
                comic_url = re.findall(r'<img alt=".*?" src="(.*?)" />', comic_html)
                if not comic_url:
                    comic_url = etree.HTML(comic_html).xpath('//*[@id="imgshow"]/img/@href')
                comic_name = comic_url[0].split('/')[-1]
                self.picFileList.append(os.path.abspath(os.path.join(self.dirname, comic_name)))
                yield comic_url[0]   # 使用生成器而非list
            except Exception as e:
                tryPrint('PicUrlFinder Error: %s from %s' % (exception, ul))

    def picDownloader(self, pic_url):
        time.sleep(.2)
        try:
            bytes_content = b''
            if self.getUrlReponse(pic_url):
                bytes_content = self.getUrlReponse(pic_url).content
        except Exception as exception:
            tryPrint('PicDownloader Error: %s from %s' % (exception, pic_url))
        else:
            if bytes_content:
                filename = os.path.basename(pic_url)
                with open(os.path.join(self.dirname, filename), 'wb') as f:
                    f.write(bytes_content)
                    print("Image {} Downloaded from {}……".format(filename, pic_url))
            else:
                print("------------------------NONE!----------------------------------")

    def img2Pdf(self):
        os.chdir("./本子合集")                                      # ./本子合集/
        output_filename = os.path.basename(self.dirname) + '.pdf'  # ./本子合集/漫画图片文件夹 # 漫画名.pdf
        imgDoc = canvas.Canvas(output_filename)

        imgfile_objs = []
        for comic in self.picFileList:
            try:
                imgfile_objs.append(Image.open(comic))
            except Exception as exception:
                tryPrint('PicFileList Error: %s from %s' % (exception, comic))

        img_sizes = [img.size for img in imgfile_objs]
        max_width = max(img_sizes)[0]
        max_height = max(img_sizes, key=lambda x: x[1])[1]

        print("max_width: %s" % max_width, "max_height: %s" % max_height)
        imgDoc.setPageSize((max_width, max_height))
        document_width, document_height = max_width, max_height

        for comic in imgfile_objs:
            try:
                comic_width, comic_height = comic.size
                print("comic_width: %s, comic_height: %s" % (comic_width, comic_height), "\t")

                imgDoc.drawImage(ImageReader(comic),
                                 (document_width - comic_width) / 2,
                                 (document_height - comic_height) / 2,
                                 comic_width, comic_height, preserveAspectRatio=True)
                imgDoc.showPage()
            except Exception as exception:
                tryPrint('Img2Pdf Error: %s from %s' % (exception, comic))
            else:
                print('Added in Pdf……')
        imgDoc.save()

    def clear(self):
        os.chdir('..')                                    # 注意之前的当前文件夹的变更
        if os.path.exists(self.dirname):
            if os.path.isdir(self.dirname):
                send2trash.send2trash(self.dirname)       # 将该图片文件夹放入回收站
                print("该文件夹已经删除!")

    def queryWebsite(self):
        if len(sys.argv) == 3:
            website_id, comic_id = sys.argv[1:]
            print("输入的网站ID是:%s, 输入的本子ID是:%s" % (website_id, comic_id))
        else:
            website_id = int(input("请输入网站ID:"))
            print("请输入本子ID: ")
            comic_id = pyperclip.paste()
            print(comic_id)

        website = self.__index[int(website_id)]
        self.hkUrl = website + comic_id + '.html'
        if int(website_id) in range(0, 6):
            start_time1 = time.clock()
            self.pool.map(self.picDownloader, self.picUrlFinder(self.hkUrl))
            self.pool.close()
            self.pool.join()
            # g = []
            # for res in self.picUrlFinder(self.hkUrl):
            #     g.append(gevent.spawn(self.picDownloader, res))
            # gevent.joinall(g)
            # for i in self.picUrlFinder(self.hkUrl):
            #     self.picDownloader(i)
            print("总下载时间是{}s!".format(round(time.clock()-start_time1)))

            start_time2 = time.clock()
            self.img2Pdf()
            print("总整合时间是{}s!".format(round(time.clock()-start_time2)))
            self.clear()
        else:
            print("你的网站ID输入错误!")

if __name__ == "__main__":
    hk = hBook()
    # hk.surfWebsite()
    hk.informationPrint()
    hk.queryWebsite()
