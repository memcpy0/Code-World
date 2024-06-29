# ANSI文件转UTF-8
import codecs
import os 
# 文件所在目录
file_name = "C:/Users/21839/Desktop/我加载了恋爱游戏(修改版).txt"
f = codecs.open(file_name, 'r', 'ansi')
ff = f.read()
file_object = codecs.open(file_name, 'w', 'utf-8')
file_object.write(ff)

"""
file_path = None
files = os.listdir(file_path)
for file in files:
    file_name = file_path + '\\' + file
    f = codecs.open(file_name, 'r', 'ansi')
    ff = f.read()
    file_object = codecs.open(file_path + '\\' + file, 'w', 'utf-8')
    file_object.write(ff)
"""
