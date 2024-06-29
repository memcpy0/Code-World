# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-17 13:39
program     : 练习递归和二分查找的使用
"""

# 最大递归深度 Python默认是1000，但到不了
import sys
sys.setrecursionlimit(1000)  # 可以调整递归深度， 但是不一定能跑到这里

# 递归最大的作用在于遍历树形结构
import os
def search_dirctory(filepath, n):
    directory_file = os.listdir(filepath)
    for i in directory_file:
        df = os.path.join(filepath, i)
        if os.path.isdir(df):
            print('\t'*n + i)
            search_dirctory(df, n+1)
        else:
            print('\t'*n+i)

filepath = r'D:\Python_Projects\Pycharm_Projects\Python14期全栈开发'
search_dirctory(filepath, 0)

# 二分查找
lst = [11, 22, 33, 44, 55, 66, 77, 88, 99]
## 第一种方案
def bisearch(seq, n):
    left = 0
    right = len(seq) - 1
    while left <= right:
        middle = (left + right) // 2
        if seq[middle] == n:
            return middle
        elif seq[middle] < n:
            left = middle + 1
        elif seq[middle] > n:
            right = middle - 1
    return None

print(bisearch(lst, 100))
from bisect import bisect
from bisect import bisect
## 第二种方案
def recursive_bisearch(seq, n, left=0, right=None):
    if right is None: right = len(seq) - 1
    middle = (left + right) // 2
    if left > right:
        return None
    if seq[middle] == n:
        return middle
    elif seq[middle] < n:
        left = middle + 1
    elif seq[middle] > n:
        right = middle - 1
    return recursive_bisearch(seq, n, left, right)

print(recursive_bisearch(lst, 100))

## 第三种方案 切
# lst = [11, 22, 33, 44, 55, 66, 77, 88, 99]
def recursive_cut_bisearch(seq, n, left=0, right=None):
    if right is None: right = len(seq) - 1
    middle = (left + right) // 2
    if left > right:
        return None
    if seq[middle] == n:
        return seq[middle]
    elif seq[middle] < n:
        left = middle + 1
        # dsf = [2]  # 切片可以得到空，索引则会报错
        # print(dsf[2:])
        seq = seq[left:]
    elif seq[middle] > n:
        right = middle - 1
        seq = seq[:right+1]  # right 注意
    return recursive_cut_bisearch(seq, n)

print(recursive_cut_bisearch(lst, 100))

