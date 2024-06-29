# -*- coding :utf-8 -*-

from lxml import etree
import requests
#import time
from multiprocessing.dummy import Pool as pl
#from bs4 import BeautifulSoup 

def spider(url):
    html = requests.get(url, headers=headers, timeout=233)
    #time.sleep(1)
    book = etree.HTML(html.text)
    chapname = book.xpath('//*[@id="main_body"]/h1/text()')[0]
    chaptext = book.xpath('//*[@id="content"]/p/text()')

    with open ("D:/Python_Projects/Pycharm_Projects/Python网络爬虫爬取/花千骨.txt", 'a', encoding='utf-8') as f:
        try:
            f.writelines(str(chapname)+'\n')
            for chapt in chaptext:
                chapt = chapt.replace("\u3000", '')
                f.writelines(chapt+'\n')
            f.writelines('\n')
            f.close()
            print("正在下载:", chapname)
        except:
            print("下载出错！")

if __name__ == "__main__":
    headers={"User-Agent":"Mozilla/5.0"}
    start_url = "http://www.136book.com/huaqiangu/"
    pool = pl(4)
    r = requests.get(start_url,headers=headers,timeout=23)
    selector = etree.HTML(r.text)
    chaplist = selector.xpath('//*[@id="book_detail"]/ol/li/a/@href')
    """
    for chap in chaplist:
        #chap_url = chap.xpath('''//*[@id="book_detail"]/ol/li/a/@href''')[0]
        #all_url.append(chap_url)
        spider(chap) 
    """
    pool.map(spider, chaplist)
    pool.close()
    pool.join()
        

