# -*- coding:utf-8 -*-
# -*- arthor:张平  -*-
import requests
from lxml import etree
import time
import csv

def writeCsv(item):
    with open("Nanjing_qfang.csv", 'a', encoding="GB2312") as f:  # 以追加形式写入
        writer = csv.writer(f)
        try:
            writer.writerow(item)  # 每次写一行
        except:
            print("Write error!")

if __name__ == "__main__":
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
    }
    title_list = ["小区名称", "小区板块", "小区每平方米均价"]
    writeCsv(title_list)

    start_url = "https://nanjing.qfang.com/garden/n"
    for i in range(1, 5): # 46p
        url = start_url + str(i)
        r = requests.get(url, headers = headers)
        r.encoding = "utf-8"
        html = r.text
        #time.sleep(1)
        selector = etree.HTML(html)

        region_name = selector.xpath('/html/body/div[3]/div[2]/div/div[3]/ul/li/div[1]/p[1]/a/text()')

        region_bigarea = selector.xpath("/html/body/div[3]/div[2]/div/div[3]/ul/li/div[1]/p[3]/span[1]/a[1]/text()")
        region_smallarea = selector.xpath("/html/body/div[3]/div[2]/div/div[3]/ul/li/div[1]/p[3]/span[1]/a[2]/text()")
        area_list = list(zip(region_bigarea, region_smallarea))
        region_area = [s1+s2 for s1, s2 in area_list]

        region_averprice = selector.xpath("/html/body/div[3]/div[2]/div/div[3]/ul/li/div[2]/p[1]/span[1]/text()")

        item = list(zip(region_name, region_area, region_averprice))
        for i in item:
            print("正在写入:%s" % i[0])
            writeCsv(i)
        """
            region_area = region.xpath("string(/div[1]/p[3]/span[1]/a)")[0]
            region_averprice = region.xpath("div[2]/p[1]/span[1]/text()")[0]
            item = [region_name, region_area, region_averprice]
            print("正在抓取:%s" % region_name)
            writeCsv(item)
        """
