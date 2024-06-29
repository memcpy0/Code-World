# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-26 23:49
program     : 学习hashlib模块
"""

import hashlib
"""
算法模块 提供了许多常见的摘要算法
* 用于密码密文校验
* 文件的一致性校验

- 能够把一个字符串数据类型的变量, 转换成一个定长的、密文的字符串, 其中每一个字符都是一个16进制数
    # 'alex3614' -> '14892985019480293'
    # 字符串 -> 密文
    # 密文 不可逆的 字符串
    # 算法: 不管这个字符串有多长, 对于同一个字符串, 用相同的算法和手段进行摘要, 
           无论在任何环境下, 多少次执行, 在任何语言中, 获得的值总是相同
           只要不是相同的字符串, 得到的结果一定不同
           
hashlib.md5()算法, 输入bytes类型的字符串, 
返回一个32位的字符串, 每一个字符都是一个16进制
算法相对简单, 摘要速度较快, 安全性较sha低 撞库

# 防止撞库, 加盐
# 动态加盐 使用usename加盐
```
username = input()
password = input()
md5_obj = hashlib.md5(username.encode())
md5_obj.update(password.encode())
print(md5_obj.hexdigest())
"""

md5_obj = hashlib.md5('任意的字符串作为盐'.encode('utf-8'))
md5_obj.update(b'ismd5')
res = md5_obj.hexdigest()
# 70bfb9e755132e037f1ce8519541afd5 32 <class 'str'>
# 4c748f47ef354676409de1c5df90c760 32 <class 'str'> 加盐后
print(res, len(res), type(res))

"""
hashlib.sha1()也是一个算法, 40位的字符串, 每一个字符都是一个16进制 
算法相对复杂, 计算速度也慢
"""
sha_obj = hashlib.sha1()
sha_obj.update(b"shake eat you")
res2 = sha_obj.hexdigest()
# 03ea5854809732902d46bc9610e32ac3bf95e8f0 40 <class 'str'>
print(res2, len(res2), type(res2))

"""
文件的一致性校验

文件太大, 循环按1024字节读, 大文件的一致性校验
"""
def compare_md5(file_path1, file_path2, file_size=1024):
    with open(file_path1, 'rb') as f1, open(file_path2, 'rb') as f2:
        md5_1, md5_2 = hashlib.md5(), hashlib.md5()
        while True:
            char1 = f1.read(file_size)
            char2 = f2.read(file_size)
            md5_1.update(char1)
            md5_2.update(char2)
            if not char1 and not char2:break
        return md5_1.hexdigest() == md5_2.hexdigest()

        # return md5_1.hexdigest() == md5_2.hexdigest()

print(compare_md5('P147_sys模块.py', 'P147_sys模块.py')) # 3f3124f0a9845c4d56faa080a2d44540
