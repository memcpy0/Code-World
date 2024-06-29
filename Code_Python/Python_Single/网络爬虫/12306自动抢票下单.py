# -*- coding:utf-8 -*-
# -*- arthor:张平  -*-
# 通过python实现12306官网的登录
# 1.模拟浏览器，访问登录页面 method = get
# 2.下载验证码图片          method = get
# 3.模拟登录    formdata   method = post

import requests
from urllib.request import urlretrieve


def rotate(matrix):
    """
    :type matrix: List[List[int]]
    :rtype: void Do not return anything, modify matrix in-place instead.
    """
    lst = []
    n = len(matrix)
    for row in range(n):
        for col in range(n):
            lst.append(matrix[col][row])
        lst.reverse()
        matrix.append(lst)
        print(matrix)
        lst = []
    for i in range(n):
        matrix.pop(0)
        print(i, matrix)

rotate([[1,2,3],[4,5,6],[7,8,9]])