# -*- coding:utf-8 -*-
import requests
import re
import os

# http://tu.duowan.com/gallery/137337.html#p1
url = "http://tu.duowan.com/index.php?r=show/getByGallery/&gid=137325&_=1532956702294"
r = requests.get(url)

pic_dict = r.json()  # 将json格式的数据转换为字典
dir_name = pic_dict['gallery_title'] # 套图文件夹

"""
S.replace(old, new[, count]) -> str # 整体替换
Return a copy of S with all occurrences of substring
old replaced by new.  If the optional argument count is
given, only the first count occurrences are replaced.

def filter(string, punct):  # 单体过滤
    return "".join([s for s in string if s not in punct])
"""

dir_name = re.sub(r'[?\\*|"<>:/]', '', dir_name) # 规划文件夹格式

if not os.path.exists(dir_name): # 判断是否存在
    os.makedirs(dir_name)        # 创建文件夹
"""
exists(path)
    Test whether a path exists.  Returns False for broken symbolic links
"""

for index, item in enumerate(pic_dict['picInfo']):
    img_name = '{}.{}.{}'.format(index,  # 序号
                                 re.sub(r'[?\\*|"<>:/]', '', item['add_intro']),  # 图片名称
                                 item['source'].split(".")[-1]  # 图片后缀
                                 )
    r = requests.get(item['source'])
    print(img_name)
    with open(os.path.join(dir_name, img_name), 'wb') as f:
        f.write(r.content)



