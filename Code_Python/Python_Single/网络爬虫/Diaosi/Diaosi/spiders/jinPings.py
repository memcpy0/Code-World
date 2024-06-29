# -*- coding: utf-8 -*-
import scrapy
"""
Scrapy组件讲解：
1. Spider         对用户而言, Spider是spider爬虫开发的最核心组件, 用户实现其子类
2. Engine
3. Scheduler
4. Downloader
5. Item Pipelines
6. Middleware

数据流对象：
1. REQUEST  
   scrapy.Request(url, [callback, method='GET', headers, body, cookies, meta, encoding='utf-8', priority=0,
                  dont_filter=False, errback)
   * url: 请求页面的url地址(必选)
   * callback: 页面解析函数，默认使用scrapy.Spider的子类的parse方法
2. RESPONSE

3. ITEM
"""

"""
1. 在Scrapy中编写爬虫, 实质是实现一个scrapy.Spider的子类

"""

class JinpingsSpider(scrapy.Spider):
    name = "jinPings"                                                            # 每个爬虫的唯一标识，使用命令创建时的名字
    allowed_domains = ["https://www.diaosixs.org/jingpinxiaoshuo/5_1.html"]      #
    start_urls = ['http://https://www.diaosixs.org/jingpinxiaoshuo/5_1.html/']   # 定义爬虫爬取的起始点，可以是一个或多个

    """
    Parse方法：一个页面下载完成后，Scrapy引擎会回调我们指定的一个页面解析函数，默认是parse方法解析页面
    一个解析方法要完成两项任务：
    * 提取页面中的数据，使用Xpath或CSS选择器
    * 提取页面中的链接，并生成对链接界面的下载请求
    每一项从页面中提取的数据及每一个对链接页面的下载请求都有yield语句提交给Scrapy引擎
    """
    def parse(self, response):
        # 提取每一本小说的链接, 信息在<span class="s2"><a href=...>title</a></span>中, 可以使用response.css
        print()
