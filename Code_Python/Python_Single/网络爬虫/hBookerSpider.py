# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-11-16 17:32
program     : 抓取欢乐书客小说的VIP章节成PDF
"""  # 

import os
import time
import re
import json
import browser_cookie3
import requests
from selenium import webdriver
from PIL import Image
from lab import longImg2Pdf
from lxml import etree
from reportlab.pdfgen import canvas
from reportlab.pdfbase import pdfmetrics, ttfonts  #  导入reportlab的注册字体
from reportlab.lib.fonts import addMapping


pdfmetrics.registerFont(ttfonts.TTFont('sun', r'D:\Python_Projects\Fonts\simsun.ttc')) #注册字体
addMapping('cjk', 1, 0, 'sun')    #bold


headers = {
    "Accept"         : "*/*",
    "Accept-Encoding": "gzip, deflate, br",
    "Accept-Language": "zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2",
    "Cache-Control"  : "no-cache",
    "Connection"     : "keep-alive",
    "Host"           : "www.ciweimao.com",
    "Pragma"         : "no-cache",
    "Referer"        : "https://www.ciweimao.com/chapter/102598656",
    "TE"             : "Trailers",
    "User-Agent"     : "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:63.0) Gecko/20100101 Firefox/63.0"
}

def request(url, cookie_jar):
    try:
        session = requests.session()
        response = session.get(url, cookies=cookie_jar, headers=headers, verify=True)
        while response == None:
            response = session.get(url, cookies=cookie_jar, headers=headers, verify=True)
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


def img2Pdf(img_dir, title_list):
    list_dir = os.listdir(img_dir)
    list_dir.sort(key=lambda x: int(x.split(".jpg")[0]))
    foolist = zip(list_dir, title_list)

    output_filename = 'C:\\Users\\dell\\Desktop\\SISTER_LOVE.pdf'
    imgDoc = canvas.Canvas(output_filename)

    for pic, title in foolist:
        try:
            picture = Image.open(os.path.join(img_dir, pic))
            print(pic)
            pic_width, pic_height = picture.size
            if not (pic_width > 0 and pic_height > 0):
                raise Exception('可能是空文件!')

            longImg2Pdf(pdfDoc=imgDoc, font_name='sun', font_wight=16, document_size=(840, 1100),
                        article_title=title, img_file=picture, img_split_pixels=1024, mode="H")
        except Exception as e:
            print('error:', e, pic)
            continue
        else:
            print('Added %s in Pdf……' % title)
    imgDoc.save()

def hbook():
    driver.get(url)
    cookies = driver.get_cookies()
    # print(cookies)
    for i in cookies:
        driver.add_cookie(i)

if __name__ == "__main__":
    # pattern = r'<li><a href="(.*?)" target="_blank"><i class=\'icon-unlock\'></i>(.*?)</a></li>'
    # with open("hBooker.html", 'r', encoding="utf-8") as f:
    #     vip_chapter = re.findall(pattern, f.read())

    # l = list(range(66, 277))
    # for i, j in vip_chapter:
    #     jj = j.split('章')[0][1:]
    #     print(i, j)
    #     l.remove(int(jj))
    # print(len(vip_chapter))  197
    # print(l)

    # index_url = "https://www.ciweimao.com/book/100059306"
    # 获取cookie，前提是需要浏览器登陆过
    # chrome_cookiejar = browser_cookie3.chrome()
    # print(chrome_cookiejar[''])
    # print(chrome_cookiejar)
    # chrome_cookiejar = json.loads(str(chrome_cookiejar))
    # # img_url_reg = re.compile(r'<img id="vipImage" src="(.*)" />', re.S)
    # img_out_dir = "C:\\Users\\dell\\Desktop\\LILI"
    #
    # for i in range(len(vip_id)):  # 0-365
    #     url = 'http://book.sfacg.com' + vip_id[i][0]
    #     refer = request(url, cookie_jar=chrome_cookiejar)
    #
    #     etee = etree.HTML(refer.text)
    #     chapter_url = 'http://book.sfacg.com' + etee.xpath('//*[@id="vipImage"]/@src')[0]
    #
    #     refer2 = request(chapter_url, cookie_jar=chrome_cookiejar)
    #     out_file = '%s.jpg' % (i + 1)
    #     with open(os.path.join(img_out_dir, out_file), 'wb') as f:
    #         f.write(refer2.content)
    #     print(chapter_url)
    #
    # img_out_files = os.listdir(img_out_dir)
    # for rel_path in img_out_files:
    #     abs_path = os.path.join(img_out_dir, rel_path)
    #     cleanFile(abs_path)
    #
    # vip_title = [i[1] for i in vip_id]
    #
    # img2Pdf(img_out_dir, vip_title)
    url = "https://www.ciweimao.com/book/100059306"
    # r = request(url, cookie_jar=firefox_cookiejar)
    # with open("KK.png", 'wb') as f:
    #     f.write(r.content)
    location = r"C:\Program Files\Mozilla Firefox\firefox.exe"
    driver = webdriver.Firefox(firefox_binary=location)
    # for cookie in chrome_cookiejar:
    #     driver.add_cookie(cookie)
    driver.get(url)
    cookies = driver.get_cookies()
    # print(cookies)
    for i in cookies:
        driver.add_cookie(i)

    driver.get("https://www.ciweimao.com/chapter/102073628")

    driver.refresh()
    time.sleep(4)
    driver.find_element_by_class_name("qqLogin").click()

    time.sleep(5)
    # 登录表单在页面的框架中，所以要切换到该框架
    driver.switch_to.frame("ptlogin_iframe")
    # 通过使用选择器选择到表单元素进行模拟输入和点击按钮提交
    driver.find_element_by_id("switcher_plogin").click()

    ele2 = driver.find_element_by_id("u")
    ele2.clear()
    ele2.send_keys("2183927003")
    ele3 = driver.find_element_by_id("p")
    ele3.clear()
    ele3.send_keys("!@#$567890")
    driver.find_element_by_class_name("login_button").click()

    elee = driver.find_element_by_id("")
    elee.get_attribute()
    # while True:
    #     time.sleep(.5)
    #     try:
    #         el = driver.find_element_by_id("J_BtnPageNext")
    #         el.click()
    #     except Exception:
    #         continue
    #     else:
    #         break
