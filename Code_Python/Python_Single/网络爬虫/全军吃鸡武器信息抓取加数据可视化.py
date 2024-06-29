# -*- coding:utf-8 -*-
# -*- topic: 绝地求生全军出击武器信息抓取加数据可视化 -*-
# -*- arthor:张平   -*-

import re
import requests
import pygal        #SVG图表库
"""
DESCRIPTION
    This package holds all available charts in pygal, the Config class
    and the maps extensions namespace module.
"""
# 面向对象编程
# "http://pubgm.qq.com/zlkdatasys/a20171229jdcs/list.shtml"
# "http://pubgm.qq.com/zlkdatasys/a20171229jdcs/akm.shtml"
# "http://pubgm.qq.com/zlkdatasys/a20171229jdcs/scarly.shtml"

class Game(object):
    # 用以下载和绘制吃鸡游戏的雷达图
    def __init__(self): # 双下划线，魔法方法，自动执行
        self.url = "http://pubgm.qq.com/zlkdatasys/a20171229jdcs/list.shtml"
        self.response = requests.get(self.url)
        html = self.response.content.decode("gbk") # 2进制 bytes类数据可以使用decode()转换为str
        #self.response.encoding = self.response.apparent_encoding
        #html = self.response.text
        #print(html)

        res = re.compile(r'<a href="(.+?)" target="_blank" class="btn_xq datum_list_sp"') # re.S 意味着无视换行符多行匹配
        self.gunurl = re.findall(res, html)[0:7]
        #print(self.gunurl)

        name = re.compile(r'<div class="qx_tab_name">(.*?)</div>', re.S)
        self.names = re.findall(name, html)[1:8]
        #print(self.names)

    def plotGame(self):
        # 绘制雷达图
        data = []
        for i in self.gunurl:
            gun_url = "http://pubgm.qq.com" + i
            html = requests.get(gun_url).text

            reg1 = re.compile(r'<span style="width:(.*?)%;"></span>')
            self.reg1 = re.findall(reg1, html)
            #print(self.reg1)
            data.append([int(reg) for reg in self.reg1])

        # 调用Radar这个类，并设置雷达图的填充fill='True', 及数值范围range(0, 100)
        radar_chart = pygal.Radar()
        radar_chart.title = "步枪性能"
        radar_chart.x_labels = ['射速', '威力', '射程', '稳定']
        for ff, property in zip(self.names, data):
            print(ff, property)
            # 绘制雷达图区域
            radar_chart.add(ff, property)

        # 保存图像
        radar_chart.render_to_file('radar_chart1.svg')

if __name__ =="__main__":
    game = Game()
    game.plotGame()

