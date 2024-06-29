# -*- coding:utf-8 -*-

import requests
import json
import re
from csv import writer

#import os
query = input("请输入要搜索的关键词:")
# 翻页尚未写出
page = input("请输入要抓取的页数深度:")

url = 'https://s.taobao.com/search?q={}&imgfile=&js=1&stats_click=search_radio_all:1&initiative_id=staobaoz_20180911&ie=utf8'.format(query)
headers = {
    "User-Agent":"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
}

r = requests.get(url)
html = r.text
r.encoding = 'UTF-8'

data = re.findall(r'g_page_config = (.*?)g_srp_loadCss', html, re.S)[0]
data = data.strip(" \n;")
data = json.loads(data)  # 将json格式字符转换化为字典方便处理
data = data['mods']['itemlist']['data']['auctions']  # 所有的商品信息

# 数据持久化 写文件、数据库
"""
[…… 'excel', 'excel_tab', 'field_size_limit', 'get_dialect', 'list_dialects', 're',
 'reader', 'register_dialect', 'unix_dialect', 'unregister_dialect', 'writer']
"""
with open("淘宝搜索{}的结果.csv".format(query), 'w', encoding="GB2312") as f:
    csv_writer = writer(f)
    csv_writer.writerow(['标题' , '标价', '购买人数', '是否包邮', '是否天猫', '地区' , '店名', 'url'])

    for item in data:
        temp = {
            'title': item['raw_title'],
            'view_price': item['view_price'],
            'view_sales': item['view_sales'],
            'view_fee': "否" if float(item['view_fee']) else '是',
            'isTmall': "是" if item['shopcard']['isTmall'] else '否',
            'area': item['item_loc'],
            'name': item['nick'],
            'detail_url': item['detail_url']
        }
        it = '{title},{view_price},{view_sales},{view_fee},{isTmall},{area},{name},{detail_url}\n'.format(**temp)
        item_list = it.split(',')
        csv_writer.writerow(item_list)

