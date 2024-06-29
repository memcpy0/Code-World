# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-25 21:47
program     : 从已有的PDF文档切割生成新的PDF文档
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
import PyPDF2  #, reportlab, csv, json
# import smtplip, imapclient, pyzmail
# from collections import *

# page_object = pdf_reader.getPage(927)  # return object<pdf.PageObject>` instance
# print(page_object.extractText().encode("UTF-8").decode("UTF-8"))

# def add_pdf_page:
# ab!

def copy_pdf_page(pdf_in_file_path, start_page=0, last_page=None, step=1):
    with open(pdf_in_file_path, 'rb') as f_in, \
            open(pdf_in_file_path.split('.')[0]+'_COPY'+'.pdf', 'wb') as f_out:

        pdf_reader = PyPDF2.PdfFileReader(f_in)
        pdf_writer = PyPDF2.PdfFileWriter()

        if not last_page:
            last_page = pdf_reader.numPages

        for page_num in range(start_page, last_page, step):
            page_object = pdf_reader.getPage(page_num)
            pdf_writer.addPage(page_object)
        pdf_writer.write(f_out)
    print("完成PDF的拷贝!")

if __name__ == "__main__":
    in_path = r'D:\编程书籍\Python学习手册(第4版)_看图王.pdf'  # input("请输入要切割并拷贝的PDF文档路径:")
    copy_pdf_page(in_path, step=100)


