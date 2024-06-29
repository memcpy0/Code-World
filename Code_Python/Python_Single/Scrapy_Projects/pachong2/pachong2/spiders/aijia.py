# -*- coding: utf-8 -*-
import scrapy
from ..items import Pachong2Item

class AijiaSpider(scrapy.Spider):
    name = "aijia"    # 爬虫的唯一标识
    allowed_domains = ["bj.5i5j.com"]
    start_urls = ['http://bj.5i5j.com/ershoufang/n' + str(x) for x in range(1, 5)]  # 要么重写start_request

    def parse(self, response):
        xiaoqus = response.xpath("/html/body/div[4]/div[1]/div[2]/ul/li")
        for xiaoqu in xiaoqus:
            item = Pachong2Item()  #
            item['xiaoqu'] = xiaoqu.xpath("./div[2]/div[1]/p[2]/a/text()").extract()[0]
            item['zhongjia'] = xiaoqu.xpath('./div[2]/div[1]/div/p[2]/text()').extract()[0]
            yield item


