# -*- coding: utf-8 -*-

# Define here the models for your scraped items
#
# See documentation in:
# http://doc.scrapy.org/en/latest/topics/items.html

import scrapy


class Pachong1Item(scrapy.Item):
    # define the fields for your item here like:
    title = scrapy.Field()
    news_url = scrapy.Field()  # 百度首页新闻链接
