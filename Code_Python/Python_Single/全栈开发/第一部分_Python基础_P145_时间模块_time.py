# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-26 19:29
program     : 学习时间模块time的用法
"""

import time # , datetime

"""
* 时间戳时间 timestamp 格林威治时间 float时间类型
    # 函数
    # 英国伦敦时间 1970-1-1 00:00:00
    # 北京时间    1970-1-1 08:00:00
    # 1537959388.2681613
    # 给机器用的
"""
print(time.time())

"""
* 格式化时间  time.strftime()
    # 函数 
    # 给人用的
"""
print(time.strftime("%Y-%m-%d, %H:%M:%S"))  # 2018-09-26, 19:04:22
print(time.strftime("%c"))                  # Wed Sep 26 19:04:22 2018
print(time.strftime("%Y-%m-%d|%H:%M:%S", time.localtime()))

"""
* 结构化时间 类似元组的一个对象
    # 从给机器看的转换到给人看的 中间状态
"""
# time.struct_time(tm_year=2018, tm_mon=9, tm_mday=26, tm_hour=18, tm_min=59, tm_sec=38,
# tm_wday=2, tm_yday=269, tm_isdst=0)
print(time.localtime())

"""
从时间戳时间转换成结构化时间 
    localtime(seconds=None)
    # 没有参数就是本地时间的结构化时间元组对象
"""
print(time.localtime(3000000000))
# time.struct_time(tm_year=2065, tm_mon=1, tm_mday=24, tm_hour=13, tm_min=20, tm_sec=0,
# tm_wday=5, tm_yday=24, tm_isdst=0)

print(time.gmtime(3000000000))  # 英国时间元组
# time.struct_time(tm_year=2065, tm_mon=1, tm_mday=24, tm_hour=5, tm_min=20, tm_sec=0,
# tm_wday=5, tm_yday=24, tm_isdst=0)

"""
从结构化时间转换成时间戳
    mktime(time_tuple) -> timestamp
"""
print(time.mktime(time.localtime()))  # 1537960477.0


"""
从结构化时间转换成格式化时间(字符串)
    strftime(format, p_tuple=None)  -> string
    asctime(p_tuple=None)  # 外国人常用
"""
struct_time_obj = time.localtime()
print(time.strftime('%Y-%m-%d', struct_time_obj)) # 2018-09-26
print(time.asctime())          # Wed Sep 26 19:27:57 2018

"""
从格式化时间转换成结构化时间
    strptime(string, format)
"""
print(time.strptime('2018-09-26', '%Y-%m-%d'))
# time.struct_time(tm_year=2018, tm_mon=9, tm_mday=26, tm_hour=0, tm_min=0, tm_sec=0,
# tm_wday=2, tm_yday=269, tm_isdst=-1)

"""
从时间戳世界转换为格式化时间 # 外国人常用
    ctime(seconds=None) -> string
"""
print(time.ctime(3000000000))  # Sat Jan 24 13:20:00 2065
