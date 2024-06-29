# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-27 15:19
program     : 学习错误和异常处理
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

"""
异常处理:
    - 异常与错误
    - 错误类型 就是错误类 (类就是类型)
    - 异常处理机制
        try:
        except ValueError:  -\
                              -> except (ValueError, IndexError) as e:         
        except IndexError:  -/ 
        except Exception as e:
            print(type(e), e, e.__traceback__.tb_lineno)
        else:
            # 没有引起异常的时候执行
        finally:
            # 无论如何都会执行
            # 适合做操作系统资源归还的工作
    - 万能异常和其他异常分支合作:
        万能异常永远要放在所有异常的最后
    - 主动抛出异常
        except:
            # 在抓住Exception之后, 抛出异常之前, 再做一些事情
            raise # 原封不动的抛出try语句中出现的异常
    
    - 自定义异常
        继承一个Exception类, 自定义一个__init__()方法
    - Exception BaseException
        有三个异常Exception无法处理

    - 断言
        assert 布尔值 为True往下走
        等同于:
        if True:
            # do something
        else:
            raise Error
    - 使用异常处理的注意事项
        断言 raise 都是写框架的时候才用的
"""
class ZhangException(Exception):
    pass

# raise ZhangException("You can do better")
