# -*- coding:utf-8 -*-
# -*- arthor:张平  -*-
import requests
import csv
import os
from lxml import etree
from urllib.request import urlretrieve
from multiprocessing.dummy import Pool  # 导入线程池

def writeCsv(item):
    with open('qfang_xiaoqu.csv', 'a', encoding="GB2312") as f:  # 以追加形式写入
        writer = csv.writer(f)
        try:
            writer.writerow(item)
        except Exception as e:
            print(e)

def spider(url):
    # 爬取详情页信息
    try:
        r = requests.get(url, headers=headers)
        html = r.text
        selector = etree.HTML(html)
        region_name = selector.xpath("/html/body/div[2]/div/div[1]/h2/text()")[0]
        region_area = selector.xpath("/html/body/div[2]/div/div[1]/div/text()")[0]
        region_area = "".join(i.strip() for i in region_area)
        region_averprice = selector.xpath("/html/body/div[5]/div/div[2]/div[1]/p[2]/span/text()")[0]
        region_fee = selector.xpath("/html/body/div[5]/div/div[2]/div[2]/ul/li[5]/p/text()")[0].split('/')[0]
        item = [region_name, region_area, region_averprice, region_fee]
        writeCsv(item)

        region_pic = selector.xpath("/html/body/div[5]/div/div[1]/div[1]/img/@src")[0]
        dir_name = 'D:/Python_Projects/Pycharm_Projects/Python网络爬虫爬取/小区图片'
        file_name = '{}.jpg'.format(region_name)
        if not os.path.exists(dir_name):
            os.makedirs(dir_name)
        urlretrieve(region_pic ,os.path.join(dir_name, file_name))
        print("正在下载:%s" % region_name)
    except:
        return

if __name__ == "__main__":
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
    }
    title_list = ["小区名称", "小区板块", "小区元/平方米", "小区物业费元/平米・月"]
    writeCsv(title_list)
    start_url = "https://nanjing.qfang.com/garden/n"
    pool = Pool(4)   # 初始化四个线程池， 默认为CPU的核心数
    url_list = []
    for i in range(1, 5):
        r = requests.get(start_url + str(i), headers=headers)
        html = r.text
        selector = etree.HTML(html)
        regionlist = selector.xpath("/html/body/div[3]/div[2]/div/div[3]/ul/li/div[1]/p[1]/a/@href")
        url_list.extend("http://nanjing.qfang.com" + region for region in regionlist)
    pool.map(spider, url_list)
    pool.close()
    pool.join()


