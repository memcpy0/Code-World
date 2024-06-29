# -*- coding:utf-8 -*-
import requests
from urllib.request import urlretrieve
import re
import json
import time

# 获取JS源代码 获取英雄ID
# 拼接URL地址
# 获取下载图片地址
# 下载图片
headers = {
    "User-Agent":"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
}

def getLolImage():
    url = "http://lol.qq.com/biz/hero/champion.js"
    # 获得JS源代码 .text str类型 .content bytes字节
    js_response = requests.get(url, headers=headers).content #
    # print(type(js_response))
    # 字节类型转码成字符串 .decode()
    html_js = js_response.decode()

    reg = r'"keys":(.*?),"data"'
    re.compile(reg, re.S)
    list_js = re.findall(reg, html_js) # 列表

    # str-> dict
    dict_js = json.loads(list_js[0])
    #print(dict_js)

    # dict_js['12'] Alistar
    pic_list = []
    for key in dict_js:
        # 英雄ID
        # print(key)
        for i in range(20):
            num = str(i)
            if i < 10:
                hero_num = "00" + num
            else:
                hero_num = "0" + num
            numstr = key + hero_num
            real_url = "http://ossweb-img.qq.com/images/lol/web201310/skin/big" + str(numstr) + ".jpg"
            # print(real_url)
            pic_list.append(real_url)

    list_filepath = []
    path = "D:\\Python_Projects\\Pycharm_Projects\\Python网络爬虫爬取\\LoLPic\\"
    for name in dict_js.values():
        for i in range(20):
            file_path = path + name + str(i) + '.jpg'
            list_filepath.append(file_path)

    # 下载图片
    n = 0
    for pic_url in pic_list:
        #print(pic_url)

        res = requests.get(pic_url, headers=headers)
        time.sleep(1)
        n += 1
        if res.status_code == 200:
            print("正在下载%s" % pic_url)
            with open(list_filepath[n], 'wb') as f:
                f.write(res.content)

getLolImage()