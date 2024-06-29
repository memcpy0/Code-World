# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-26 23:53
program     : 了解config配置
"""
import configparser
# 以前配置项目使用文本文件
"""
configparser 一种配置的模块就出现了
    有一种固定格式的配置文件.ini
    这个固定的模块就来解析这些字符串
[section]
s = sssss # option
"""
config = configparser.ConfigParser()

# 查找文件内容, 基于字典形式
print(config.sections())          # []
config.read('config_example.ini')
print(config.sections())     # ['bitbucket.org', 'topsecret.server.com']

print('bytebong.com' in config)   # False
print('bitbucket.org' in config)  # True

print(config['bitbucket.org']['compression'])  # yes

for key in config['bitbucket.org']:
    print(key)
    # user
    # serveraliveinterval
    # compression
    # compressionlevel
    # forwardxll

# ----------------------------------------------------------
# 不跟Java打交道的话就可以不太了解
# 现在使用settings.py这种方式
