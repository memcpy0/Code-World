# -*- coding:utf-8 -*-
import requests
import os
# http://www.lovewzly.com/api/user/pc/list/search?
# startage=21&endage=30&gender=2&startheight=161&endheight=170
# &marry=1&astro=2&lunar=5&education=40&salary=3&page=1

# 设置条件

def query_age():
    age = int(input("请输入期望对象的年龄(如20)"))

    if 21 <= age <= 30:
        startage = 21
        endage = 30
    elif 31 <= age <= 40:
        startage = 31
        endage = 40
    return startage, endage

def query_sex():
    sex = input("请输入期望对象的年龄(如女)")
    if sex == '男':
        gender = 1
    else:
        gender = 2
    return gender

def query_height():
    height = int(input("请输入期望对象的身高(如170)"))

    if 151 <= height <= 160:
        startheight = 151
        endheight = 160
    elif 161 <= height <= 170:
        startheight = 161
        endheight = 170
    elif 171 <= height <= 180:
        startheight = 171
        endheight = 180
    elif 181 <= height <= 190:
        startheight = 181
        endheight = 190
    else:
        startheight = 0
        endheight = 0

    return startheight, endheight

def query_city():
    pass

def query_lunar():
    pass

def query_marry():
    pass

