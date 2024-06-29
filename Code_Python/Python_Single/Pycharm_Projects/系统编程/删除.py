# -*- coding:utf-8 -*-
# -*- topic:使用一些系统编程的库和函数 -*-
# -*- arthor:张平   -*-
import os, shutil, send2trash

path = r'D:\迅雷下载\[扶她&御姐X正太&萝莉&群P]水無月十三作品合集（89本，2.48G，附预览+目录）\本体（89）'
for dirname in os.listdir(path):
    if '水無月十三' in dirname and not (('翻' or '漢化') in dirname):
        # os.rmdir(dirname) # 空文件夹 # 永久删除
        # os.unlink(dirname) # 文件    # 永久删除
        shutil.rmtree(os.path.join(path, dirname)) # 文件夹和文件 # 永久删除
        print(dirname)

