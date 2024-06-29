# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-26 20:03
program     : 探索sys模块
"""

import sys
# sys.path
# sys.argv
# sys.modules
# sys.platform()
# sys.exit("My Error")  # 默认为0；其他是整数；再其他的对象会打印出来 My Error
# exit("My Error")      # My Error
# sys.setrecursionlimit(1111)  设置最大递归深度

# print(sys.getdefaultencoding())  # Return the current default string encoding # utf-8
# print(sys.getfilesystemencoding()) # 返回在操作系统文件名中转换Unicode文件名的编码   # uft-8

# print(sys.getrecursionlimit())   # 1000
# print(sys.getwindowsversion())   # sys.getwindowsversion(major=10, minor=0, build=17134, platform=2, service_pack='')

# print(sys.__loader__.get_code('math'))