# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-03 15:28
program     : 
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
# from collections import *
class SecretString:
    '''A not-at-all secure way to store a secret string.'''

    def __init__(self, plain_string, pass_phrase):
        self.__plain_string = plain_string
        self.__pass_phrase = pass_phrase

    def decrypt(self, pass_phrase):
        '''Only show the string if the pass_phrase is correct.'''
        if pass_phrase == self.__pass_phrase:
            return self.__plain_string
        else:
            return ''

secret_string = SecretString('ACME: Top Secret', 'antwerp')
print(secret_string.decrypt('antwerp'))
print(2**16)
