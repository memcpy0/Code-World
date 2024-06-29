# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-26 22:29
program     : 学习序列化模块json pickle
"""

import json
import dbm
import pickle
import shelve
"""
- 用pickle序列化：指得是把其他格式的数据转换得到一个字符串的结果 过程就叫序列化
    # 字典/列表/数字/对象 -> 序列化(成为字符串)
- 为什么要用序列化?
    # 1. 要把内容写入文件
    # 2. 网络传输数据
- 字符串反序列化 
    # 字典/列表/数字/对象 <- 字符串 反序列化
    # 不能用eval
- 序列化和反序列化的模块工具
    # pickle * 不跨语言; 但几乎可以支持Python所有的数据类型
    # json   * 跨语言; 但支持的数据类型有限
    # shelve * 建立在dbm和pickle上面
"""

# json只提供四个方法
# 操作内存
dic = {'aaa':'bbb', 'ccc':'ddd'}
str_dic = json.dumps(dic)
print([str_dic])          # '{"aaa": "bbb", "ccc": "ddd"}'
dic2 = json.loads(str_dic)
print(dic2)               # {'aaa': 'bbb', 'ccc': 'ddd'}

# 操作文件
with open('json_dump.json', 'w') as f:
    json.dump(dic, f)
with open('json_dump.json', 'r') as f:
    dic = json.load(f)  # {'aaa': 'bbb', 'ccc': 'ddd'}
    print(dic)

"""
json格式的限制一: 
    json格式的字典键key必须是字符串数据类型; 
    且json中的字符串必须是双引号;
    如果数字为key, dump之后会强行转成字符串数据类型
"""
di = {1:2, 3:4}
str_di = json.dumps(di)   # '{"1": 2, "3": 4}'
print([str_di])
di = json.loads(str_di)   # {'1': 2, '3': 4}
print(di)

"""
json格式的限制二: 
    json不支持元组作value, 对元组作为value的字典会把元组强制转换成列表
    json完全不支持元组作key, 会报错!(这验证了json格式的字典的键必须是字符串)
"""
d = {'abc':(1, 2, 3)}
str_d = json.dumps(d)  # '{"abc": [1, 2, 3]}'
print([str_d])
d = json.loads(str_d)  # {'abc': [1, 2, 3]}
print(d)

"""
json格式的限制三:
    json使用dump和load操作文件时, 可以多次dump, 不可以load出来;
    这样不适合用dump, 而是应该用dumps
"""
my_dict = {'abc':(1, 2, 3)}
my_dict2 = {'first':(4, 5, 6)}
with open('json_demo', 'w') as f:
    str_1 = json.dumps(my_dict)
    str_2 = json.dumps(my_dict2)
    f.write(str_1+'\n')
    f.write(str_2+'\n')

with open('json_demo', 'r') as f:
    for line in f:
        ret = json.loads(line)
        print(ret)

"""
json格式的限制四:
    json对中文的支持不完美, 打印和在文件中存的时候会不完美
json格式的限制五:
    json不支持集合set
"""
chin_dic = {'中国':'中文'}
ret = json.dumps(chin_dic)  # {"\u4e2d\u56fd": "\u4e2d\u6587"}
print(ret)
ret2 = json.dumps(chin_dic, ensure_ascii=False)  # {"中国": "中文"}
print(ret2)

with open('json_demo_encoding.json', 'w', encoding='utf-8') as f:
    json.dump(chin_dic, f, ensure_ascii=False)

"""
json的其他参数: 是为了让程序员看的更方便, 但是会相对浪费空间
dumps/dump:
skipkeys=False, ensure_ascii=True, check_circular=True,
allow_nan=True, cls=None, indent=None, separators=None,
default=None, sort_keys=False

load
cls=None, object_hook=None, parse_float=None,
parse_int=None, parse_constant=None, object_pairs_hook=None

loads
encoding=None, cls=None, object_hook=None, parse_float=None,
parse_int=None, parse_constant=None, object_pairs_hook=None
"""

# ---------------------------------------------------------------------------------
"""
pickle 四个方法
几乎支持Python的所有对象

向文件中dump多个值在load出来的要用try:...except EOFError...
"""
pic_dic = pickle.dumps(dic)
print(pic_dic)
# b'\x80\x03}q\x00(X\x03\x00\x00\x00aaaq\x01X\x03\x00\x00\x00
# bbbq\x02X\x03\x00\x00\x00cccq\x03X\x03\x00\x00\x00dddq\x04u.'
new_dic = pickle.loads(pic_dic)
print(new_dic)   # {'aaa': 'bbb', 'ccc': 'ddd'}

with open('pickle_demo', 'wb') as f:
    pickle.dump(dic, f)
with open('pickle_demo', 'rb') as f:
    new_dic = pickle.load(f)
    print(new_dic)  # {'aaa': 'bbb', 'ccc': 'ddd'}

# ---------------------------------------------------------------------------------
"""
shelve 
    open flag writeback
"""
f = shelve.open('shelve_demo')
f['key'] = {'key1':'value1', 'key2':'value2'}
f.close()

f = shelve.open('shelve_demo')
content = f['key']
print(content)
f.close()
