# -*- coding:utf-8 -*-

"""
author      : myRealization
create_time : 2018-08-30 10:48
program     : 编写一个纸牌类
"""
# import pyperclip
# import re, string
# import time, datetime
# import requests, urllib.request
# from lxml import etree
# from bs4 import BeautifulSoup
# import os, glob, sys, subprocess, threading
# from multiprocessing.dummy import Pool
# from PIL import Image
# import PyPDF2, reportlab, csv, json
# import smtplip, imapclient, pyzmail
import collections
import random
"""
虽然FrenchDeck隐式地继承了object类， 但功能却不是继承而来的。我们通过数据
模型和一些合成来实现这些功能。通过实现__len__和__getitem__这两个特殊方
法，FrenchDeck就跟一个Python自有的序列数据类型一样，可以体现出Python的核心
语言特性（例如迭代和切片）。
同时这个类还可以用于标准库中诸如random.choice、reversed 和sorted这些函数。
另外，对合成的运用使得__len__和__getitem__的具体实现可以代理给self._cards这个Python列表
"""
# >>> Point = namedtuple('Point', ['x', 'y'])
# 不是像使用元组那样返回元组的实例，而是返回一个元组的子类， 需要自己实例化
# Returns a new subclass of tuple with named fields.
Card = collections.namedtuple('Card', ['rank', 'suit']) # 比元组强在有属性
# 具名元组还有一些自己专有的属性
# 几个最有用的：
# 类属性_fields: 是一个包含这个类所有字段名称的元组
# 类方法_make(iterable): _make()通过接受一个可迭代对象来生成这个类的一个实例，它的作用跟Card(*delhi_data)是一样的
# 实例方法_asdict(): 把具名元组以 collections.OrderedDict 的形式返回，我们可以利用它来把元组里的信息友好地呈现出来


class FrenckDeck:
    ranks = [str(n) for n in range(2, 11)] + list('JQKA')
    suits = 'spades diamonds clubs hearts'.split()

    def __init__(self):
        self._cards = [Card(rank, suit) for suit in self.suits
                                        for rank in self.ranks] # 实例列表
    def __len__(self):
        return len(self._cards) # 返回纸牌数量13*4=52

    def __getitem__(self, item): # 可以看做一个序列
        return self._cards[item] # 抽牌

suit_values = dict(spades=3, hearts=2, diamonds=1, clubs=0)
def spades_high(card):
    rank_value = FrenckDeck.ranks.index(card.rank) # 点数判断 2->0 A->11
    return rank_value * len(suit_values) + suit_values[card.suit]

if __name__ == '__main__':
    # beer_card = Card('7', 'diamonds')
    # print(beer_card) # Card(rank='7', suit='diamonds')
    deck = FrenckDeck()
    print(len(deck))   # 52
    print(deck[0], deck[-1]) # 可索引 Card(rank='2', suit='spades') Card(rank='A', suit='hearts')
    print(random.choice(deck)) # len(deck) return deck[i]
    print(deck[:3], deck[12::13]) # 可切片

    # for card in deck: # __getitem__ 可迭代
    #     print(card)
    # for card in reversed(deck):
    #     print(card)
    for card in sorted(deck, key=spades_high): # A custom key function 用spades_high函数来做排序
        print(card)



