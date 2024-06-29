# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-08-30 14:14
program     : 初步学习Pygame开发
"""
# import pyperclip
# import re, string
# import time, datetime
# import requests, urllib.request
# from lxml import etree
# from bs4 import BeautifulSoup
import sys #, os, glob, subprocess, threading
# from multiprocessing.dummy import Pool
# from PIL import Image
# import PyPDF2, reportlab, csv, json
# import smtplip, imapclient, pyzmail
import random, pygame
from pygame.locals import * # 导入pygame中所有的常量

"""
* 这个游戏一共有5个元素：背景颜色 生命值 得分 小球 挡板
* 小球在水平方向的位置随机出现，按照一定的速度垂直下落
* 我们用挡板接小球，接到则分数上涨，否则生命值下降，当生命值用完的时候游戏结束
"""
# 1.定义一个输出字体的函数 准备工作
# 1.1 render()绘制内容 1.2 blit()函数来绘制
# 1.3 pygame.display.set_mode() 获得显示系统访问 创建一个窗口
# 1.4 pygame.font.Font() 将文本输出到图形的窗口
def printText(font, x, y, text, color=(255, 255, 255)):
    # True 抗锯齿 提高图像的质量
    imgText = font.render(text, True, color)
    screen.blit(imgText, (x, y))

# 初始化pygame
pygame.init()

# 窗口大小可调
screen = pygame.display.set_mode((600, 500), pygame.RESIZABLE)
# 设置窗口标题
pygame.display.set_caption('接小球')

#设置窗口的图标效果，为一个surface对象
icon = pygame.image.load("timg-1.jpeg")
pygame.display.set_icon(icon)

# 将文本输出到图形的窗口
font1 = pygame.font.Font(None, 24)

# 1.6 设置鼠标光标是否可见
pygame.mouse.set_visible(True)

# 1.7 设置元素的颜色
white = 255, 255, 255
red = 220, 50, 50
yellow = 230, 230, 50
blue = 0, 0, 100

# 1.8 玩几次(生命值)
lives = 3
score = 0

# 1.9 游戏结束的标记 鼠标点击的位置 初始化挡板的位置 小球的位置 速度变量
gameover = True
mouse_x = mouse_y = 0

# 初始化挡板的位置
pose_x = 300
pose_y = 460

# 小球的位置 水平位置随机 垂直下落
bomb_x = random.randint(0, 500)
bomb_y = -50

# 小球的速度变量
speed = 1

# 2.循环中创建一个事件处理程序
while True:
    for event in pygame.event.get(): # 从队列中获取全部的事件
        if event.type == QUIT:
            sys.exit()
        elif event.type == MOUSEMOTION: # 事件是鼠标的类型
            mouse_x, mouse_y = event.pos
        elif event.type == MOUSEBUTTONUP: # 事件是鼠标抬起的类型
            if gameover:
                gameover = False
                lives = 3
                score = 0

    # 2.1 获得所有键盘按钮的状态
    keys = pygame.key.get_pressed()
    # 2.2 如果我们的事件是计算机的退出键，就退出
    if keys[K_ESCAPE]:
        sys.exit()

    screen.fill(blue)

    if gameover:
        printText(font1, 100, 200, '<CLICK TO PLAY>')
    else:
        # 如果没有结束，小球动起来， 通过速度变量改变垂直方向的值
        bomb_y += speed
        # 什么时候接住了? 屏幕height500 挡板40
        # 没接住小球 重新生成小球 生命值减一 生命值为0时结束游戏
        if bomb_y > 500:
            bomb_x = random.randint(0, 500)
            bomb_y = -50
            lives -= 1
            if lives == 0:
                gameover = True

        # 接住小球
        elif bomb_y > pose_y:
            if bomb_x > pose_x and bomb_x < pose_x+120:
                score += 10
                bomb_x = random.randint(0, 500)
                bomb_y = -50
        # 绘制小球
        pygame.draw.circle(screen, yellow, (bomb_x, int(bomb_y)), 30, 0)

        # 2.3 挡板位置的判断
        pose_x = mouse_x
        if pose_x < 0:
            pose_x = 0
        elif pose_x > 500:
            pose_x = 500

        # 2.4 画挡板        # 0 实心
        pygame.draw.rect(screen, red, (pose_x, pose_y, 120, 40), 0)

    # 2.5 输出我们的生命值 得分值
    printText(font1, 0, 0, "LIVES:"+str(lives))
    printText(font1, 500, 0, "SCORE :" + str(score))
    # 2.6 更新界面
    pygame.display.update()