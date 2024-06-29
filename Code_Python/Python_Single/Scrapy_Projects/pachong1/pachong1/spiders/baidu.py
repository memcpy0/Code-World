# -*- coding: utf-8 -*-
import scrapy
from ..items import Pachong1Item
# from pachong1.pachong1.items import Pachong1Item

class BaiduSpider(scrapy.Spider):
    name = "baidu"  # 每一个Scrapy爬虫的唯一标识
    allowed_domains = ["www.baidu.com"]
    start_urls = ['http://www.baidu.com/']

    def parse(self, response):  # 默认的Spider组件的网页解析方法
        item = Pachong1Item()
        item['title'] = response.xpath('//*[@name="tj_trnews"]/text()').extract()[0]  # .extract() 默认提取列表
        item['news_url'] = response.xpath('//*[@name="tj_trnews"]/@href').extract_first()
        print(item['title'], item['news_url'])   # //*[@id="u_sp"]/a[1]
        return item
