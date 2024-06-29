# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-11-15 13:16
program     : 模仿学习书中
"""  # 

import sys
from PyQt5 import QtWidgets, QtCore

app = QtWidgets.QApplication(sys.argv)
widget = QtWidgets.QWidget()
widget.resize(360, 360)
widget.setWindowTitle("Hello, PyQt5!")
widget.show()
