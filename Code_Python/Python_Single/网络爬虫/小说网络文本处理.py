# -*- coding:utf-8 -*-
import re, requests
url = 'http://www.ltxs52.com/0_55/33052.html'
chapt = requests.get(url)
chapt.encoding = 'gbk'
chapt = chapt.text
reg = r'<div id="content">&nbsp;&nbsp;&nbsp;&nbsp;(.*?)</div>'
reg = re.compile(reg, re.S)  # S 多行匹配
chapt_text = re.findall(reg, chapt)
chapt_text = chapt_text[0].replace("&nbsp;&nbsp;&nbsp;&nbsp;", "").replace("<br />", "").replace('.', '').replace('\n\n\n', '\n')
with open('jj.txt', 'a') as f:
    f.write(chapt_text)
# print(chapt_text)