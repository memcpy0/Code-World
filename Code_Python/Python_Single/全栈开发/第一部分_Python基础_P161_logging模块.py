# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-27 14:02
program     : 学习日志模块logging
"""
import logging
"""
logging
功能: 
    - 1. 日志格式的规范
    - 2. 操作的简化
    - 3. 日志的分级管理
不能做的事:
    - 自动生成你要打印的内容
    - 需要自己在开发的时候配置好
使用:
    - 普通配置型
    - 对象配置型 
模式:
    - NOTSET
    - 调试模式
    - 基础信息
    - 警告
    - 错误
    - 严重错误 FATAL
"""
import logging

# logging.basicConfig 参数配置
# logging.basicConfig(level=logging.DEBUG,
#                     format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',
#                     datefmt='%a, %d %b %Y %H:%M:%S')

# logging.debug('debug message')      # 调试信息
# logging.info('info message')        # 基础信息
# logging.warning('warning message')  # 警告
# logging.error('error message')      # 错误
# logging.critical('critical error message') # 严重错误

# basicConfig 不能将一个log信息既输出到屏幕上, 也输出到文件中
# 文件的编码也有问题

"""
logging对象的操作形式来操作日志文件

# 创建一个logger对象
# 创建一个文件管理操作符
# 创建一个屏幕管理操作符
# 创建一个日志管理的格式

# 文件管理操作符 绑定一个格式
# 屏幕管理操作符 绑定一个格式

# logger对象 绑定 文件管理操作符
# logger对象 绑定 屏幕管理操作符
"""
logger = logging.getLogger()
logger.setLevel(logging.DEBUG)

fh = logging.FileHandler('test2.log', encoding='utf-8')

sh = logging.StreamHandler()

format1 = logging.Formatter("%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s")

fh.setFormatter(format1)
sh.setFormatter(format1)

logger.addHandler(fh)
logger.addHandler(sh)

logger.debug('debug message')      # 调试信息
logger.info('info message')        # 基础信息
logger.warning('warning message')  # 警告
logger.error('error message')      # 错误
logger.critical('critical error message') # 严重错误
