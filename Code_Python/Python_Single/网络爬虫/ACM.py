# -*- coding:utf-8 -*-
# -*- arthor:张平  -*-

# ACM题目
# 输入一个n，输出其n!最后一位非零的整数
# n < 10^100
# 思路一：循环得到n!, 再反复取余%
# 思路二：
import time

def func1(n):
    b_time = time.time()
    temp = 1
    for i in range(1, n + 1):
        temp = temp * i
    # 取最后非零的整数
    while True:
        res, rest = divmod(temp, 10)
        if rest == 0: # 代表整除
            temp = res
        else:         # 余数不等于零，则代表找到了最后一位不为0的数
            print(rest)
            break
    e_time = time.time()
    print(e_time - b_time)

def func2(n):
    b_time = time.time()
    temp = 1
    for i in range(1, n + 1):
        temp = temp * i
        while True:
            res, rest = divmod(temp, 10)  # res商 rest余数
            # 取每一次结果的最后一个非零的数
            if rest == 0:
                temp = res
            else:  # 余数不为零表示取得了最后的数
                temp = rest
                print(temp)
                break
    e_time = time.time()
    print(e_time - b_time)

n = int(input("请输入n:"))
temp = 1
for i in range(1, n + 1):
    temp = temp * i
temp = int(str(temp).replace('0', '')[-1])
print(temp)
    #func1(n)
    #func2(n)





