# -*- coding:utf-8 -*-
from urllib.request import urlopen
import re
# 收获：学会了urlretrieve()   urlopen .read() .decode()  re.compile  re.S  re.findall
"""
from urllib.request import urlretrieve
Help on function urlretrieve in module urllib.request:
urlretrieve(url, filename=None, reporthook=None, data=None)
    Retrieve a URL into a temporary location on disk.
    Requires a URL argument. If a filename is passed, it is used as
    the temporary file location. The reporthook argument should be
    a callable that accepts a block number, a read size, and the
    total file size of the URL target. The data argument should be
    valid URL encoded data.
    If a filename is passed and the URL points to a local resource,
    the result is a copy from local file to new file.

    Returns a tuple containing the path to the newly created
    data file as well as the resulting HTTPMessage object.

url = "http://www.cninfo.com.cn/cninfo-new/disclosure/szse_sme/download/1204702864?announceTime=2018-04-24"
dir_name = "C:\\Users\\dell\\Desktop"
filename = "雷科.PDF"
urlretrieve(url, os.path.join(dir_name, filename))

等同于以下代码，但更简单
headers = {
    "User-Agent":"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
}
r = requests.get(url, headers=headers)
with open(os.path.join("C:\\Users\\dell\\Desktop\\数据采集", filename), 'wb') as f:
    f.write(r.content)
    f.close()

# 下小说
url = "https://down.qingkan9.com/175/175887.txt"
path = url.split("/")[-1]
urlretrieve(url, path)
"""

url = "http://www.quanshuwang.com/book/0/742"
html = urlopen(url).read()
html = html.decode('gbk') # 转码
#print(html)

reg = r'<li><a href="(.*?)" title=".*?">(.*?)</a></li>'
# 目的 增加效率
reg = re.compile(reg)
urls = re.findall(reg, html)

for url in urls:
    novel_url = url[0]
    novel_title = url[1]
    chapt = urlopen(novel_url).read().decode('gbk')

    reg = r'</script>&nbsp;&nbsp;&nbsp;&nbsp;(.*?)<script type="text/javascript">'
    reg = re.compile(reg, re.S)  # S 多行匹配
    chapt_text = re.findall(reg, chapt)

    chapt_text = chapt_text[0].replace("&nbsp;&nbsp;&nbsp;&nbsp;", "")
    chapt_text = chapt_text.replace("<br />", "")
    print("正在保存%s" % novel_title)

    f = open('佣兵之王.txt', 'a')
    f.write(chapt_text)

