# -*- coding:utf-8 -*-
# -*- arthor:张平  -*-
import requests
from lxml import etree
from PIL import Image
#import time
#import os
import csv
from urllib.request import urlretrieve
#from multiprocessing.dummy import Pool

# 本节
# 1.学会了验证码的处理
# 2.还有session()的使用
# 3.post()和data参数
# 4.PIL.Image.open(f).show()

def writeCsv(item):
    with open('douban.csv', 'a', encoding="utf-8") as f:
        writer = csv.writer(f)
        try:
            writer.writerow(item)
        except Exception as e:
            print(e)

if __name__ == "__main__":
    headers = {"User-Agent": "Mozilla/5.0"}
    start_url = 'https://www.douban.com/accounts/login'
    r = requests.get(start_url, headers=headers)
    r.encoding = r.apparent_encoding
    html = r.text
    selector = etree.HTML(html)

    captcha_url = selector.xpath('//*[@id="captcha_image"]/@src')[0]
    captcha_id = captcha_url.split("=")[1].split("&")[0]

    file_path = 'D:/Python_Projects/Pycharm_Projects/Python网络爬虫爬取/captcha.jpg'
    urlretrieve(captcha_url, file_path)
    img = Image.open('captcha.jpg')
    img.show()
    captcha_word = input(u"请输入验证码: ")
    formdata = {
        "captcha-id": captcha_id,
        "captcha-solution": captcha_word,
        "form_email": "2183927003@qq.com",
        "form_password": "!wddbmm123",
        "source": "index_nav"
    }
    response = requests.session()
    r = response.post(start_url, headers=headers, data=formdata)
    r.encoding = "utf-8"
    selector = etree.HTML(r.text)
    writeCsv(['用户', '标题'])
    #article_block = selector.xpath('')
    users = selector.xpath('/html/body/div[3]/div[1]/div/div[1]/div[2]/div[2]/div/div/div/div[1]/div[2]/a/text()')
    #print(users)
    titles = selector.xpath('//html/body/div[3]/div[1]/div/div[1]/div[2]/div[2]/div/div/div/div[2]/div[1]/div[2]/div/a/text()')
    #print(titles)
    item = list(zip(users, titles))
    print(item)
    for u, t in item:
        writeCsv([u, t])
    print("成功！")
















