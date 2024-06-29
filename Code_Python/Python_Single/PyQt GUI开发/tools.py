# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-11-15 19:53
program     : 模仿学习书中
"""  # 

import os

# 得到目录下所有的UI文件
def listUiFile():
    ui_list = []
    files = os.listdir(dir)
    for file_name in files:
        if os.path.splitext(file_name)[1] == '.ui':
            ui_list.append(os.path.join(dir, file_name))
    return ui_list

# 把扩展名为.ui的文件名字符串
def transPyFile(filename):
    return os.path.splitext(filename)[0] + '.py'

def runMain():
    list = listUiFile()
    for ui_file in list:
        py_file = transPyFile(ui_file)
        cmd_msg = 'pyuic5 -o {pyfile} {uifile}'.format(pyfile=py_file, uifile=ui_file)
        os.system(cmd_msg)

if __name__ == "__main__":
    # UI文件所在的路径
    dir = input("INPUT your ui file path: ")
    runMain()
