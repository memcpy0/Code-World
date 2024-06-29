# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-20
program     : 图片裁切实验
"""  # 

import os
from reportlab.pdfgen import canvas
from reportlab.lib.utils import ImageReader
from PIL import Image
from reportlab.pdfbase import pdfmetrics, ttfonts  #  导入reportlab的注册字体
from reportlab.lib.fonts import addMapping


pdfmetrics.registerFont(ttfonts.TTFont('sun', r'D:\Python_Projects\Fonts\simsun.ttc')) #注册字体
addMapping('cjk', 1, 0, 'sun')    #bold


def cleanFile(img):
    return img.convert("L")

def longImgSplit(picture, output_dir, height_split_pixels=None, width_split_pixels=None):
    if not (height_split_pixels or width_split_pixels):
        raise Exception("Need height's or width's split pixels!")
    elif height_split_pixels and width_split_pixels:
        raise Exception("Function is not done!")

    pic_width, pic_height = picture.size

    def _innerSplit(split_pixels, mode="H"):  # -> "H" or "W"
        left = upper = 0
        right = lower = 0
        if mode == "H":
            right = pic_width
            lower = split_pixels
            split_times = pic_height // split_pixels + 1
        elif mode == "W":
            right = split_pixels
            lower = pic_height
            split_times = pic_width // split_pixels + 1
        else:
            raise Exception("No this mode!")

        while split_times > 0:
            if split_times == 1:
                if mode == "H":
                    temp_file = picture.crop((left, upper, right, pic_height))
                elif mode == "W":
                    temp_file = picture.crop((left, upper, pic_width, lower))
            else:
                temp_file = picture.crop((left, upper, right, lower))
            temp_file = cleanFile(temp_file)
            # if split_times == 1:
            #     temp_file.resize((pic_width, split_pixels))
            temp_file.save(os.path.join(output_dir, "%s.png" % split_times))
            split_times -= 1
            if mode == "H":
                upper += split_pixels
                lower += split_pixels
            elif mode == "W":
                left += split_pixels
                right += split_pixels
            else:
                raise Exception("No this mode!")

    if height_split_pixels:
        _innerSplit(height_split_pixels, mode="H")
    elif width_split_pixels:
        _innerSplit(width_split_pixels, mode="W")


def longImgSplitGeneraotr(picture, split_pixels=None, mode="H"):
    pic_width, pic_height = picture.size
    if (split_pixels > pic_height and mode == "H") or (split_pixels > pic_width and mode == "W"):
        raise Exception("Your split pixels are too big!")
    if not split_pixels:
        raise Exception("Need height's or width's split pixels!")
    else:
        left = upper = 0
        right = lower = 0
        if mode == "H":
            right = pic_width
            lower = split_pixels
            split_times = pic_height // split_pixels + 1
        elif mode == "W":
            right = split_pixels
            lower = pic_height
            split_times = pic_width // split_pixels + 1
        else:
            raise Exception("No this mode!")

        while split_times > 0:
            if split_times == 1:
                if mode == "H":
                    temp_file = picture.crop((left, upper, right, pic_height))
                elif mode == "W":
                    temp_file = picture.crop((left, upper, pic_width, lower))
            else:
                temp_file = picture.crop((left, upper, right, lower))
            temp_file = cleanFile(temp_file)
            yield temp_file
            split_times -= 1
            if mode == "H":
                upper += split_pixels
                lower += split_pixels
            elif mode == "W":
                left += split_pixels
                right += split_pixels
            else:
                raise Exception("No this mode!")



def longImg2Pdf(pdfDoc, font_name, font_wight, document_size, article_title, img_file, img_split_pixels, mode):
    pdfDoc.setPageSize(document_size)
    pdfDoc.setFont(font_name, font_wight)

    document_width, document_height = document_size
    pdfDoc.drawString(30, document_height-30, article_title)   # 30, 1070

    for pic in longImgSplitGeneraotr(img_file, split_pixels=img_split_pixels, mode=mode):
        # print(pic)
        img = ImageReader(pic)
        pic_width, pic_height = pic.size
        pdfDoc.drawImage(img,
                         30, 1054-pic_height,
                         pic_width, pic_height, preserveAspectRatio=True)
        pdfDoc.showPage()

if __name__ == "__main__":
    output_filename = 'C:\\Users\\dell\\Desktop\\SISTER_LOVE.pdf'
    imgDoc = canvas.Canvas(output_filename)

    picture = Image.open("C:\\Users\\dell\\Desktop\\my.jpg")
    # text_object = imgDoc.beginText()
    # text_object.setTextOrigin(40, 1180)
    # text_object.textLine("八 嗓音考虑是否考虑啊")
    # imgDoc.drawText(text_object)

    longImgSplit(picture, "C:\\Users\\dell\\Desktop", height_split_pixels=1024)  # 728 2200

    longImg2Pdf(pdfDoc=imgDoc, font_name='sun', font_wight=16, document_size=(840, 1100),
            article_title="八 嗓音考虑是否考虑啊", img_file=picture, img_split_pixels=1024, mode="H")

    imgDoc.save()
    print("tt")
