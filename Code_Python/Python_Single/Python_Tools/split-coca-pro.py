# -*- coding: utf-8 -*-

import os
import re
import sys
import timeit

# 全局变量,默认设置

# 保持coca单词文件(按照词频排序)中单词的顺序,同时进行去重
# remove duplicates while maintaining the order
def remove_dup_keeping_order(input_file):
    with open(input_file, "r") as fin:



############################################################################################
# 用法: 读入经split-coca-helper.py处理过的单词文件,得到一个单词列表words
# - 如果指定了--help参数,将输出命令帮助
# - 如果指定了--range参数和整数N M,将截取单词列表中对应区间的单词,
# 以每行单个单词的格式,输出到单词文件cocaN-M.txt
# - 如果指定了--batch-group参数和整数N,将单词列表按照顺序分割为len(words)/N
# 个单词组,分别输出到单词文件中
