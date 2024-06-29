# -*- coding:utf-8 -*-
# -*- topic:阳光电影链接爬取 -*-
# -*- arthor:张平   -*-


#http://www.ygdy8.com/html/gndy/dyzz/20180709/57114.html
import re
import time
import requests
# 网站入口 通过这个入口可以找到几乎所有的种子
url = "http://www.ygdy8.com/html/gndy/dyzz/list_23_{}.html"
headers = {
        "User-Agent" : "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
    }
# requests.exceptions.ConnectionError: ('Connection aborted.', RemoteDisconnec……  远程服务器强制关闭了链接
for page in range(1, 165):
    page_url = url.format(page)
    time.sleep(1)
    try:
        response = requests.get(page_url, headers=headers)
        response.encoding = 'gb2312'
    except:
        print("请求错误")
    # 和网站统一编码格式就不会出现乱码了
    html = response.text
    movie_list = re.findall(r'<a href="(.*?)" class="ulink">.*?</a>', html)
    for u in movie_list:
        url = "http://www.ygdy8.com" + u
        response = requests.get(url, headers=headers)
        response.encoding = 'gb2312'
        # 提取网站中的种子链接
        ftp_url = re.findall(r'<td style="WORD-WRAP: break-word" bgcolor="#fdfddf"><a href="(.*?)">.*?</a></td>', response.text)[0]
        with open("./电影种子地址.txt", 'a') as f:
            f.write(ftp_url+'\n')
        time.sleep(.1)
    print('{}页完成!'.format(page))