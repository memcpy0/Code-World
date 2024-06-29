# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-28 22:03
program     : 使用struct模块解决黏包问题 方式2
"""#

"""
struct模块
# 什么是固定长度的bytes
# 为什么要用固定长度的bytes
"""
import struct

ret = struct.pack('i', 4096)  # 'i' 所代表的就是int, 将有限长度范围的数字转换为bytes
print(ret)  # b'\x00\x10\x00\x00'

num = struct.unpack('i', ret)
print(num)  # (4096,)
