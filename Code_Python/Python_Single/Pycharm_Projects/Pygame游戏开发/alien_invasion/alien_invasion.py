# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-17 09:22
program     : 学习Pygame的使用
"""
# import pyperclip
# import re, string
# import time, datetime
# import json, csv
# import requests, urllib.request
# from lxml import etree
# from bs4 import BeautifulSoup
# import os, glob, sys, subprocess, threading
# from multiprocessing.dummy import Pool
# from PIL import Image
# import PyPDF2, reportlab, csv, json
# import smtplip, imapclient, pyzmail
# from collections import *

import sys, os
sys.path = [os.path.dirname(__file__)] + sys.path

import pygame
from pygame.locals import *
from settings import Settings
import game_functions as gf
from ship import Ship

bg_color = (230, 230, 230)

def run_game():
    # 初始化游戏、设置并创建一个屏幕对象 # 会做pygame.display.init()
    pygame.init()
    ai_settings = Settings()
    # screensize = width, height = 1200, 800
    screen = pygame.display.set_mode((ai_settings.screen_width, ai_settings.screen_height), RESIZABLE)
    pygame.display.set_caption("Alien Invasion")

    # 创建一艘飞船
    ship = Ship(ai_settings, screen)

    # 开始游戏的主循环
    while True:
        # 监视键盘和鼠标事件
        gf.check_events(ship)
        ship.update()
        # 每次循环时都更新屏幕
        gf.update_screen(ai_settings, screen, ship)

run_game()
