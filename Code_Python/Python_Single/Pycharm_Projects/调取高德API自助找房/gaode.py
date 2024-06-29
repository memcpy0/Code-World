# -*- coding:utf-8 -*-
# -*- arthor:张平  -*-
import requests
from bs4 import BeautifulSoup
from urllib.parse import urljoin
import csv
import html5lib

# 学会 BeautifulSoup的select用法
# csv.writer的分隔符指定
# csv文件使用'wb'模式
# 还有.encode() ？？
# urllib.parse的urljoin连接URL

url = "http://bj.ganji.com/fang1/o{page}p{price}/"
addr = "http://bj.ganji.com/"

if __name__ == "__main__":
    start_page = 1
    end_page = 10
    price = 7        # 价格
    with open('ganji.csv', 'a') as f:
        csv_writer = csv.writer(f, delimiter=",")
        print('Start……………………')
        while start_page <= end_page:
            start_page += 1
            print('get:{}'.format(url.format(page=start_page, price=price)))

            response = requests.get(url.format(page=start_page, price=price))
            html = BeautifulSoup(response.text, 'html.parser')
            house_list = html.select('.f-list > .f-list-item > .f-list-item-wrap') # 获取房源信息

            if not house_list:
                break
            for house in house_list:
                house_title = house.select('.title > a')[0].string.encode('utf-8')
                house_addr = house.select('.address > .area > a')[-1].string.encode("utf-8")
                house_price = house.select('.info > .price > .num')[0].string.encode('utf-8')
                house_url = urljoin(addr, house.select('.title > a')[0]['href'])
                csv_writer.writerow([house_title, house_addr, house_price, house_url])

        print("End…………")
