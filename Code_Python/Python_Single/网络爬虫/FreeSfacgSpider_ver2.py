# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-11-01 10:00
program     : 抓取菠萝包小说的VIP章节成PDF
"""  # 
import os
import re
import browser_cookie3
import requests
from PIL import Image
from lab import longImg2Pdf
from lxml import etree
from reportlab.pdfgen import canvas
from reportlab.pdfbase import pdfmetrics, ttfonts  #  导入reportlab的注册字体
from reportlab.lib.fonts import addMapping


pdfmetrics.registerFont(ttfonts.TTFont('sun', r'D:\Python_Projects\Fonts\simsun.ttc')) #注册字体
addMapping('cjk', 1, 0, 'sun')    #bold


headers = {
    "User-Agent": "Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 "
                  "(KHTML, like Gecko) Chrome/56.0.2924.90 Mobile Safari/537.36"
}

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

    vip_title = [i[1] for i in vip_id]

    img2Pdf(img_out_dir, vip_title)