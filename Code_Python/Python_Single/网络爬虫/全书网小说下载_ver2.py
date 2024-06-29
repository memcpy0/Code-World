# -*- coding:utf-8 -*-
import requests
import re

url = "http://www.quanshuwang.com/book/0/742"
headers = {
    "User-Agent":"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
}
r = requests.get(url, headers=headers)
r.encoding = r.apparent_encoding
html = r.text
#print(html)

reg = r'<li><a href="(.*?)" title=".*?">(.*?)</a></li>'
re.compile(reg, re.S)
urls = re.findall(reg, html)
#print(reg)

for url in urls:
    novel_url = url[0]
    novel_title = url[1]

    r = requests.get(novel_url, headers=headers)
    r.encoding = 'gbk'
    chapt = r.text

    reg = r'</script>&nbsp;&nbsp;&nbsp;&nbsp;(.*?)<script type="text/javascript">'
    reg = re.compile(reg, re.S)  # S 多行匹配
    chapt_text = re.findall(reg, chapt)

    chapt_text = chapt_text[0].replace("&nbsp;&nbsp;&nbsp;&nbsp;", "")
    chapt_text = chapt_text.replace("<br />", "")
    print("正在保存%s" % novel_title)

    f = open('佣兵之王.txt', 'a')
    f.write(chapt_text)


