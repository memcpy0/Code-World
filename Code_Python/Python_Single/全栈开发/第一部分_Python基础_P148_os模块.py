# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-26 20:05
program     : 探索OS库和os.path模块
"""

import os  # , glob, sys, subprocess, threading

"""
探索os库
"""
print(os.getcwd())  # 表示当前工作文件夹的绝对路径 即在哪个目录下执行这个py文件
print(__file__)     # 表示当前的py文件的绝对路径 除非文件路径改变否则绝对不变

os.chdir('D:\Python_Projects\Pycharm_Projects') # 改变当前工作文件夹
print(os.getcwd())  # D:\Python_Projects\Pycharm_Projects

# print(os.curdir)  # .
# print(os.pardir)  # ..

# -------------------------------------------------------------------------------------
"""
创建文件夹、删除文件夹
os.makedirs和os.removedirs的源代码可以一看
"""
# os.mkdir('./dir')       # 创建单级目录
# os.mkdir('./dir/dir2')  # 在dir存在的时候在其下面在创建一个单级目录
# os.makedirs('./dir3/dir4', exists_ok=True) # 创建多级目录

# os.rmdir('./dir/dir2')  # 删除单级目录, 且目录一定要为非空文件夹, 不能有其他文件或文件夹
# os.removedirs('./dir/dir2')  # 删除多级文件夹
# 递归删除上一级目录, 只要删除当前文件夹之后, 发现上一级目录也为空了, 就把上一级目录也删除掉
# 如果发现上一级目录有其他文件, 就停止
"""
删除文件 
os.remove()  # 删除一个文件
重命名一个文件/目录
os.rename('old_name', 'new_name')
"""

# -------------------------------------------------------------------------------------
"""
浏览文件夹
os.listdir()
"""
# print(os.listdir('.'))

# -------------------------------------------------------------------------------------
"""
获取文件/目录状态
"""
print(os.stat(r'D:\Python_Projects\全栈开发\第一部分_Python基础_P148_os模块.py'))
# os.stat_result(st_mode=33206,              # i node 保护模式
#                st_ino=5910974510937847,    # 节点号
#                st_dev=2452501177,          # 驻留的设备
#                st_nlink=1,                 # i node的链接数
#                st_uid=0,                   # 所有者的用户ID
#                st_gid=0,                   # 所有者的组ID
#                st_size=1700,               # 普通文件以字节问单位的大小
#                st_atime=1537964897,        # 上一次访问的时间
#                st_mtime=1537964897,        # 最后一次修改的时间
#                st_ctime=1537964897)        # 由操作系统报告的"ctime", 在某些系统(Unix)是最新的元数据更改的时间, 在其他系统
#                                              上是创建的时间
# --------------------------------------------------------------------------------------
# 目录路径分隔符
print(os.sep)      # \      当前所在的操作系统平台上的目录分隔符 win为"\" Linux为"/"
print(os.linesep)  # '\r\n' 当前文件所用的行终止符           win为"\r\n" Linux为"\n"
print(os.pathsep)  # ;      当前操作系统上的分割文件路径的字符 win为; Linux为:
# print(os.fork())
print(os.name)     # nt为win, posix为Linux

print(os.sep.join(["D:\Python_Projects\Pycharm_Projects\Python14期全栈开发", "P148_os模块.py"]))
# D:\Python_Projects\Pycharm_Projects\Python14期全栈开发\P148_os模块.py
# 这样的使用, 跨平台

# --------------------------------------------------------------------------------------
"""
执行终端命令
os.system 运行shell命令, 直接显示
os.popen  运行shell命令, 获取执行结果
"""
# os.system('dir')  # pycharm为utf-8, 操作系统是什么编码就返回什么编码 像exec
ret = os.popen('dir')
print(ret.read())   # 可以像字符串一样操作它 # 像eval

# --------------------------------------------------------------------------------------
# 查看环境变量
print(os.environ)

# --------------------------------------------------------------------------------------
"""
探索os.path模块
"""

"""
os.path.abspath(path)              规范目录路径的斜杠使用

os.path.dirname(path)              返回path的目录, 其实就是os.path.split(path)[0]
os.path.basename(path)             返回path最后的文件名, 如path以\或/结尾, 返回空值, 即os.path.split(path)[1]

os.path.isabs(path)                如果path是绝对路径, 返回True

os.path.exists(path)               如果path存在, 返回True, 否则False
os.path.isfile(path)               如果path是一个存在的文件, 这返回True, 否则False
os.path.isdir(path)                如果path是一个存在的文件夹, 这返回True, 否则False

os.path.split(path)                将path分割成目录和文件名(或最后一级目录)二元组返回
os.path.join(path1[, path2[,...])  将多个路径组合后返回, 第一个绝对路径之前的参数将被忽略

os.path.getatime(path)             返回path所指的目录或文件的最后访问时间
os.path.getmtime(path)             返回path所指的目录或文件的最后修改时间
os.path.getsize(path)              返回path的大小
"""

print(os.path.split('D:\Python_Projects\Pycharm_Projects\Python14期全栈开发'))

# 统计一个文件夹的整体大小(连同其内部的文件)
