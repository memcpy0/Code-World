# -*- coding:utf-8 -*-
# -*- topic:获取王者荣耀官网的数据/图片信息 -*-
# -*- arthor:张平   -*-

import json
import os
from urllib.request import urlretrieve
import requests

def pathTest(dirpath):
    if not os.path.exists(dirpath):
       os.makedirs(dirpath)

if __name__ == "__main__":
    #url = "http://pvp.qq.com/web201605/js/herolist.json"
    #headers = {
    #    "User-Agent":"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
    #}
    #r = requests.get(url, headers=headers)
    #js = r.json()

    #urlretrieve(url, 'KingGlory.json')

    with open('KingGlory.json', 'r', encoding='utf-8-sig') as f: # 文件包含BOM字符
        jsonFile = json.load(f)
        """
        Deserialize ``fp`` (a ``.read()``-supporting file-like object containing
        a JSON document) to a Python object.
        """
    dirname = 'D:/Python_Projects/Pycharm_Projects/Python网络爬虫爬取/王者荣耀皮肤'
    pathTest(dirname)
    for m in range(82):
        cName = jsonFile[m]['cname'] # 英雄中文名称
        eName = jsonFile[m]['ename'] # 英雄数字名称
        skinName = jsonFile[m]['skin_name'].split("|")
        skinNum = len(skinName)
        #print(skinName, skinNum)
        dirname1 = dirname + '/' + str(cName)
        pathTest(dirname1)
        for n in range(1, skinNum+1):
            pictureUrl = 'http://game.gtimg.cn/images/yxzj/img201606/skin/hero-info/'+\
                str(eName)+'/'+str(eName)+'-bigskin-'+str(n)+'.jpg'
            filename = cName + skinName[n-1] + '.jpg'
            urlretrieve(pictureUrl, os.path.join(dirname1, filename))
        print(cName+'的皮肤下载成功!')
