# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-11-15 13:00
program     : 模仿学习书中
"""  # 

import sys
from PyQt5.QtWidgets import QWidget

out = sys.stdout
sys.stdout = open(r'C:\Users\dell\Desktop\Qwidget.txt', 'w')
help(QWidget)
sys.stdout.close()
sys.stdout = out
