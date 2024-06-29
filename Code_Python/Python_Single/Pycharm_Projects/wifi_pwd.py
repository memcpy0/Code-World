# -*- coding:utf-8 -*-
import itertools as its

# 迭代器
words = "z1234567890"
r = its.product(words, repeat=5)
#for i in r:
#    print(i)
with open("wifi_pwd.txt", 'a') as f:
    for i in r:
        f.write("".join(i))
        f.write("\n")
    f.close()


