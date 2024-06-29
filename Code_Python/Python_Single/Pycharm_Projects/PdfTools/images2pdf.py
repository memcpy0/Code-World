# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-08-29 13:00
program     : 将系列图片拼接成PDF 使用PDF默认的A2大小 经常有超出的时候
"""

from os import listdir
from os.path import join, isfile, isdir
from re import search,  IGNORECASE
from PIL import Image
from reportlab.pdfgen import canvas
from reportlab.lib.utils import ImageReader
from reportlab.lib.pagesizes import A2

def img_search(mypath, filenames):
    for lists in listdir(mypath):
        path = join(mypath, lists)
        if isfile(path):
            expression = r'[\w]+\.(jpg|png|jpeg)$'
            if search(expression, path, IGNORECASE):
                filenames.append(path)
        elif isdir(path):
            img_search(path, filenames)

def img2Pdf():
    # 841.8897637795277 1190.5511811023623 
    mypath = input('Input the image folder please:')
    output_file_name = mypath + '.pdf'
    imgDoc = canvas.Canvas(output_file_name) 
    imgDoc.setPageSize(A2)
    document_width, document_height = A2
    filenames = []

    img_search(mypath, filenames)
    for comic in filenames:
        comic_file = Image.open(comic)

        comic_width, comic_height = comic_file.size
        print("comic_width:{}, comic_height:{}".format(comic_width, comic_height), end='\t\t')
        if not (comic_width > 0 and comic_height > 0):
            raise Exception('可能是空文件!')
        try:
            imgDoc.drawImage(ImageReader(comic_file),
                             (document_width-comic_width)/2,
                             (document_height-comic_height)/2,
                             comic_width, comic_height, preserveAspectRatio=True)
            imgDoc.showPage()
        except Exception as e:
            print('error:', e, comic)
        else:
            print('Adding Pdf Done..')
    imgDoc.save()

img2Pdf()
