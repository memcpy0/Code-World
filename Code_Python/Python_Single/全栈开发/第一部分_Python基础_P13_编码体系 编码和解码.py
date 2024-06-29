# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-01 21:51
program     : 编码解码的介绍
"""

# di = {'2':'4dfakl'}
# di.setdefault('3', 'kal')
# print(di)
"""
编码：
ASCII码 1byte 8bit(7位+补零) 2**7=128个字符
GBK 国标码 兼容ASCII码 2bytes 16位 可表示6万多个常用汉字(又称双字节字符) 等同于ANSI   
    如：字母A 0100 0001  # ASCII码               
    0000 0000 0100 0001 # 国标码 
Unicode 万国码 开始是每个字符两个字节 容纳欧洲字符 但无法编码汉字 USC-2
        再扩充到了四个字节 可以表示40亿个字符，太浪费空间 USC-4
        又提出了新的UTF编码 可变长度编码
        UTF-8 每个字符占据的字节数不定，根据文字具体内容进行编码，最少占据1个字节
        *   英文 1byte就够了 8bit
        *   欧洲文字 2bytes 16bit
        *   汉字 2bytes太小 4bytes太多 就使用3bytes 24位，既满足了中文，又满足了节约
        UTF-16 每个字符最少占据2个字节 16位
        UTF-32 每个字符最少占据4个字节 32位

python2 只能使用ASCII
python3 默认使用Unicode
"""

# encode() 编码之后的内容是bytes类型的数据
# *中文编码
# 想要存储，就必须进行编码
s = '马化腾的儿子被绑架了'
s = s.encode('utf-8')  # 把Unicode字符串str编码成UTF-8的bytes形式
# UTF-8 30个字节 10个汉字 每个字3个字节
# b'\xe9\xa9\xac #\xe5\x8c\x96 #\xe8\x85\xbe #\xe7\x9a\x84 #\xe5\x84\xbf
#   \xe5\xad\x90 #\xe8\xa2\xab #\xe7\xbb\x91 #\xe6\x9e\xb6 #\xe4\xba\x86'
# print(s)

g = '马化腾的儿子被绑架了'
g = g.encode('GBK')
# GBK编码 20个字节 10个汉字 每个字2个字节
# b'\xc2\xed #\xbb\xaf #\xcc\xda #\xb5\xc4 #\xb6\xf9 #\xd7\xd3
#   \xb1\xbb #\xb0\xf3 #\xbc\xdc #\xc1\xcb'
# print(g)

s2 = '中'.encode('utf-8') # b'\xe4\xb8\xad'
g2 = '中'.encode('GBK')   # b'\xd6\xd0'
# print(s2, '\n', g2)

# *英文编码 之后的结果是英文本身
s3 = 'hello'.encode('utf-8') # b'hello'
g3 = 'hello'.encode('GBK')   # b'hello'
# print(s3, '\n', g3)

# decode() 解码之后是我们熟悉的字符串, 用什么编码就要用什么解码
bs4 = b'\xc2\xed\xbb\xaf\xcc\xda\xb5\xc4\xb6\xf9\xd7\xd3\xb1\xbb\xb0\xf3\xbc\xdc\xc1\xcb'
s4 = bs4.decode('GBK')
print(s4) # 马化腾的儿子被绑架了
