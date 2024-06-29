# -*- coding:utf-8 -*-
# -*- topic:      -*-
# -*- arthor:张平   -*-

import requests
import os
from urllib.request import urlretrieve

# 下载王者荣耀英雄图片
def heroImgDownload(url, headers):
    r = requests.get(url, headers=headers)
    json_data = r.json()  # type为dict
    hero_num = len(json_data['list'])
    print("一共有%d个英雄" % hero_num)
    hero_list = json_data['list']
    hero_img_dir = './王者荣耀英雄封面'
    if not os.path.exists(hero_img_dir):
        os.makedirs(hero_img_dir)
    for each_hero in hero_list:
        hero_cover_url = each_hero['cover']
        hero_name = each_hero['name']
        filename = hero_name + '.jpg'
        print("正在下载%s的图片" % hero_name)
        urlretrieve(hero_cover_url, os.path.join(hero_img_dir, filename))

# 打印所有的英雄ID和姓名
def heroHelper(url, headers):
    print("*" * 100)
    print(" " * 8 + "欢迎使用<王者荣耀>出装小助手")
    print("*" * 100)
    r = requests.get(url, headers=headers)
    json_data = r.json()
    hero_list = json_data['list']
    flag = 0
    for each_hero in hero_list:
        flag += 1
        print("%s的ID为%s" % (each_hero['name'], each_hero['hero_id']), end='\t\t')
        if flag == 3:
            print('\n', end='')
            flag = 0

# 获取武器的信息

if __name__ == "__main__":
    heros_url = "http://gamehelper.gm825.com/wzry/hero/list?channel_id=90009"
    """
    headers = {
        "User-Agent":
        "Mozilla/5.0 (iPhone; CPU iPhone OS 7_1_2 like Mac OS X) AppleWebKit/537.51.2 (KHTML, like Gecko) Version/7.0 Mobile/11D257 Safari/9537.53"
    }
    """
    headers = {
        "Accept":"text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
        "Accept-Encoding":"gzip, deflate",
        "Accept-Language":"zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2",
        "Connection":"keep-alive",
        "Cookie":"PHPSESSID=980bigheti6csve8088lgppe15",
        "Host":"gamehelper.gm825.com",
        "Upgrade-Insecure-Requests": "1",
        "User-Agent":"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0",
    }
    # heroImgDownload(heros_url, headers)
    heroHelper(heros_url, headers)
    # 英雄出装
    hero_id = input('请输入要查询的英雄ID')
    hero_detail_url = "http://gamehelper.gm825.com/wzry/hero/detail?hero_id={}&channel_id=90009a".format(hero_id)
    weapon_url = "http://gamehelper.gm825.com/wzry/equip/list?hero_id={}&channel_id=90009a".format(hero_id)






