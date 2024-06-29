# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-29 22:36
program     : 将系列图片拼接成PDF 使用所有图片的最大宽度和高度 没有超出的时候
"""

from os import listdir
from os.path import join, isfile, isdir
from re import search,  IGNORECASE
from PIL import Image
from reportlab.pdfgen import canvas
from reportlab.lib.utils import ImageReader


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
    
    filenames = []
    img_search(mypath, filenames)
    
    gc = [Image.open(comic).size  for comic in filenames]
    max_width = max(gc)[0]
    max_height = max(gc, key=lambda x: x[1])[1]
    print("max_width: %s" % max_width, "max_height: %s" % max_height)
    imgDoc.setPageSize((max_width, max_height))
    
    document_width, document_height = max_width, max_height
   
    for comic in filenames:
        comic_file = Image.open(comic)

        comic_width, comic_height = comic_file.size
        print("comic_width:{}, comic_height:{}".format(comic_width, comic_height), end='\t')
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
