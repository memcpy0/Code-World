# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-25 20:28
program     : 学习random模块
"""
# from multiprocessing.dummy import Pool
# from PIL import Image
# import PyPDF2, reportlab, csv, json
# import smtplip, imapclient, pyzmail
# from collections import *
import random

"""
随机：在某个范围内取得每一个值的概率是相同的
# 随机小数:
    - 无限循环小数 在float里面
    - 无限不循环小数 不在float里面
"""
print(random.random())      # 0-1之内的随机小数  # 0.4903864924393957
print(random.uniform(1, 5))  # 1-5 任意范围之内的随机小数 # 2.4084861309074888

"""
# 随机整数 *****
"""
print(random.randint(1, 2))    # [1, 2] 包含2在内的范围内随机取整数
print(random.randrange(1, 2))  # [1, 2) 不包含2在内的范围内随机取整数
print(random.randrange(1, 10, 2))  # [1, 10) 不包含10在内的范围内随机取奇数

"""
# 随机抽取一个值
"""
lst = [1, 2, 3, 'aaa', ('w', 'q')]
print(random.choice(lst))

"""
# 随机抽取多个值
"""
print(random.sample(lst, 2))
# population, weights=None, *, cum_weights=None, k=1
print(random.choices(lst, k=2))

"""
随机打乱数组
"""
lst = [1, 2, 3, 4, 5, 6]
random.shuffle(lst)
print(lst)   # [4, 5, 1, 2, 6, 3]

# 生成随机验证码4位数字
print("Capt:%s" % random.randint(1000, 9999))  # Capt:4595

# 生成随机验证码多位数字
# 用函数
def random_capt(n=4):
    return ''.join([str(random.randint(0, 9)) for i in range(n)])

print(random_capt())  # 0791

# 生成随机验证码多位数字或字母
# 用函数 每一位出现大小、小写、数字的几率都是1/3
def random_capt_with_num_char(n=6):
    code = ''
    for i in range(n):
        rand_num = str(random.randint(0, 9))
        rand_alpha = chr(random.randint(97, 122))
        rand_alpha_upper = chr(random.randint(65, 90))
        atom_code = random.choice([rand_num, rand_alpha, rand_alpha_upper])
        code += atom_code
    return code

print(random_capt_with_num_char()) # E4x471

class Error(Exception):
    pass

# 生成随机验证码 纯数字或字母+数字或纯字母
def random_capt_with_num_or_char_or_num_plus_char(n=6, alpha_flag=True, num_flag=True):
    code = ''
    if alpha_flag and num_flag:
        for i in range(n):
            rand_num = str(random.randint(0, 9))
            rand_alpha = chr(random.randint(97, 122))
            rand_alpha_upper = chr(random.randint(65, 90))
            atom_code = random.choice([rand_num, rand_alpha, rand_alpha_upper])
            code += atom_code
    elif alpha_flag and not num_flag:
        for i in range(n):
            rand_alpha = chr(random.randint(97, 122))
            rand_alpha_upper = chr(random.randint(65, 90))
            atom_code = random.choice([rand_alpha, rand_alpha_upper])
            code += atom_code
    elif not alpha_flag and num_flag:
        return ''.join([str(random.randint(0, 9)) for i in range(n)])
    else:
        raise Error("You can't have a capt without char or number!")
    return  code

print(random_capt_with_num_or_char_or_num_plus_char(alpha_flag=True, num_flag=False))
