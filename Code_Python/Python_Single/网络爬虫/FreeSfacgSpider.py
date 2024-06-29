# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-2
program     : 下载菠萝包中的VIP小说图片 并进行文字识别
              其中利用了已经登录过的cookie来保持登录
              而不是自动模拟登录
"""  #

import os
import re
import subprocess
import browser_cookie3
import requests
from PIL import Image
from aip import AipOcr
import time
import send2trash
from lxml import etree


headers = {
    "User-Agent": "Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 "
                  "(KHTML, like Gecko) Chrome/56.0.2924.90 Mobile Safari/537.36"
}

class BaiduAPI(object):
    def __init__(self):
        """你的APPID AK SK"""
        self.APP_ID = '11688486'
        self.API_KEY = 'EHsnhG7WBLRB4TyO60WyjV3O'
        self.SECRET_KEY = 'L7Ds0LFjBHkQD8gOaLFnyfLKxTrAC4Xz'

        self.client = AipOcr(self.APP_ID, self.API_KEY, self.SECRET_KEY)

    def picture2Text(self, filepath):
        # 读取图片
        image = self.getPicture(filepath)
        texts = self.client.basicAccurate(image)  # 通用文字识别

        allTexts = '\n'
        for word in texts['words_result']:
            allTexts = allTexts + word.get('words', '') + '\n'
        return allTexts

    @staticmethod
    def getPicture(filepath):
        time.sleep(.5)
        with open(filepath, 'rb') as fp:
            return fp.read()


def request(url, cookie_jar):
    try:
        session = requests.session()
        response = session.get(url, cookies=cookie_jar, headers=headers, verify=False)
        while response == None:
            response = session.get(url, cookies=cookie_jar, headers=headers, verify=False)
            response.raise_for_status()
        response.encoding = 'UTF-8'
    except Exception as e:
        print("HTTP请求失败!", e)
    else:
        return response


def cleanFile(filePath):
    image = Image.open(filePath)
    image = image.convert("L")
    image.save(filePath)


def cleanFile2(filePath, newFilePath):
    image = Image.open(filePath)
    # 对图片进行阈值过滤 然后保存
    image = image.point(lambda x: 0 if x < 143 else 255)
    image.save(newFilePath)
    # 调用系统的tesseract命令对图片进行OCR识别
    subprocess.call(["tesseract", newFilePath, "output"])


if __name__ == "__main__":
    pattern = r'<a href="(/vip/c/.*?)" title="(.*?)"  class="">'
    with open("FreeSfacg.html", 'r', encoding="utf-8") as f:
        vip_id = re.findall(pattern, f.read())


    # index_url = "http://book.sfacg.com/Novel/66981/MainIndex/"
    # 获取cookie，前提是需要浏览器登陆过
    chrome_cookiejar = browser_cookie3.chrome()

    # img_url_reg = re.compile(r'<img id="vipImage" src="(.*)" />', re.S)
    img_out_dir = "C:\\Users\\dell\\Desktop\\sister_lover"

    for i in range(len(vip_id)):  # 0-365
        url = 'http://book.sfacg.com' + vip_id[i][0]
        refer = request(url, cookie_jar=chrome_cookiejar)

        etee = etree.HTML(refer.text)
        chapter_url = 'http://book.sfacg.com' + etee.xpath('//*[@id="vipImage"]/@src')[0]

        refer2 = request(chapter_url, cookie_jar=chrome_cookiejar)
        out_file = '%s.jpg' % (i + 1)
        with open(os.path.join(img_out_dir, out_file), 'wb') as f:
            f.write(refer2.content)
        print(chapter_url)


    img_out_files = os.listdir(img_out_dir)
    for rel_path in img_out_files:
        abs_path = os.path.join(img_out_dir, rel_path)
        cleanFile(abs_path)

    img_out_dir = "C:\\Users\\dell\\Desktop\\sister_lover"
    baiduapi = BaiduAPI()

    txt = "C:\\Users\\dell\\Desktop\\LOVE_SISTER.txt"
    img_out_files = os.listdir(img_out_dir)
    with open(txt, 'a', encoding="UTF-8") as f:
        cnt = 0
        for rel_path in img_out_files:
            abs_path = os.path.join(img_out_dir, rel_path)
            text = baiduapi.picture2Text(abs_path)
            title = vip_id[cnt][1].replace('\u3000', ' ') + '\n'
            f.write(title+text+'\n\n')

            print('adding text...'+title)
            cnt += 1

    print("SUCCESS")
