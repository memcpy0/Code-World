# -*- coding:utf-8 -*-
# -*- arthor:张平  -*-
# from urllib.request import urlopen
# urlopen().readline()
import requests
from lxml import etree
from multiprocessing.dummy import Pool
import re
# import sys sys.setdefaultencoding

def zhihuDailySpider(url):
    r = requests.get(url, headers=headers)
    r.encoding = r.apparent_encoding
    html = r.text
    selector = etree.HTML(html)
    article_id = selector.xpath("/html/body/div[3]/div/div[2]/div/div/div/div/a/@href")
    article_url = [url+i for i in article_id]
    '''
    pattern = r'"/story/(.*?)"'
    re.compile(pattern, re.S)
    result = re.findall(pattern, html)
    url = url + 'story/'
    article_url2 = [url+i for i in result]
    '''
    #print(len(article_url), article_url)
    return article_url

def getarticleContent(article_url):
    r = requests.get(article_url, headers=headers)
    r.encoding = r.apparent_encoding
    html = r.text
    selector = etree.HTML(html)
    article_title = selector.xpath("/html/body/div[3]/div[1]/div/h1/text()")[0]
    article_title = re.sub(r'[?\\*|"<>:/]-\r\n', '', article_title)
    article_content = selector.xpath("string(/html/body/div[3]/div[2]/div/div[1]/div[2])")
    '''
    article_title2 = re.findall(r'<h1 class="headline-title">(.*?)</h1>', html)[0]
    reg = re.compile(r'<div class="content">(.*?)</div>', re.S)
    article_content2 = re.findall(reg, html) # 这里要有大量过滤
    print(article_title2, article_content2)
    '''
    with open("./知乎日报/{}.txt".format(article_title), 'w', encoding="utf-8") as f:
        f.write(article_content)
        print('下载{}.txt成功'.format(article_title))
        f.close()

if __name__ == "__main__":
    start_url = "http://daily.zhihu.com"
    headers = {
        "User-Agent":"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
    }
    article_url = zhihuDailySpider(start_url)
    pool = Pool(4)
    pool.map(getarticleContent, article_url)
    pool.close()
    pool.join()







