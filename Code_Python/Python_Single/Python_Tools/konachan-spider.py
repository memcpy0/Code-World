# -*- coding: utf-8 -*-
import os
import time
import requests
from bs4 import BeautifulSoup as BS

url = "https://konachan.net/post?page={}&tags=width:{}..+height:{}.." # 页面,宽度下限,高度下限
outDir = "C:\\Users\\21839\\Pictures\\image" # 图片输出文件夹
headers = {"User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:91.0) Gecko/20100101 Firefox/91.0"}

# 输入URL,得到响应内容
def getResponse(url):
    try:
        time.sleep(100) # 慢点爬
        r = requests.get(url, headers=headers)
        r.raise_for_status() # 响应状态码,出错则抛出异常 
        return r
    except Exception as ex: # 出错就递归抓取
        print(type(ex))
        return getResponse(url)

# 输入html,输出soup对象
def getSoup(html):
    return BS(html, "lxml")

# 输入soup,输出图片节点列表
def getImagesList(soup):
    images = soup.find_all("a", class_="directlink largeimg")
    return images

# 输入图片编号和URL,保存图片
def saveImage(imageNum, imageURL):
    if (os.path.exists(outDir) == False):
        os.mkdir(outDir)
    with open(outDir + "\\{}.png".format(imageNum), 'wb') as f:
        image = getResponse(imageURL).content
        f.write(image)

def process(endPage, width, height):
    imageNum = 1 # 以数字为图片命名
    for i in range(1, endPage):
        html = getResponse(url.format(i, width, height)).text
        soup = getSoup(html)
        imagesList = getImagesList(soup)
        for j in range(len(imagesList)):
            saveImage(imageNum, imagesList[j]['href'])
            imageNum = imageNum + 1

if __name__ == "__main__":
    # endPage = int(input("输入要抓取的终止网页页码:\n"))
    # width = int(input("输入图片最低宽度:\n"))
    # height = int(input("输入图片最低高度:\n"))
    endPage = 30 # 只抓取第1页到第30页的图片
    width, height = 1920, 1080
    process(endPage, width, height) # 从第1页抓取到第endPage页,图片尺寸大于(width,height)