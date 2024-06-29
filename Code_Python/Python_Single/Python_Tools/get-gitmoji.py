# -*- coding: utf-8 -*-
import os
import re
import time
import requests
from bs4 import BeautifulSoup as BS

emojiURL = "https://gitmoji.dev" # emoji资源网址
# 输入url,输出html,出现异常时要多次调用,直到成功为止
headers = {"User-Agent" : "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:84.0) Gecko/20100101 Firefox/84.0"} 
def getHTML(url):
    try:
        r = requests.get(url, headers = headers)
        r.raise_for_status() # 响应状态码,出错则抛出异常
        r.encoding = "utf-8"
        return r.text
    except Exception as ex:
        print(type(ex))
        time.sleep(10)
        return getHTML(url)

# 输入html,输出soup对象
def getSoup(html):
    return BS(html, "lxml")

# 输入soup,输出一个列表,含有所有emoji图、code码、使用说明的标签
def getEmojiTags(soup):
    tags = soup.find_all("article")
    return tags

# 输入emoji标签列表,遍历所有标签,分别将其emoji图、code码、使用说明作为一行写入文件
# 格式如: # 🎨 :art: Improve structure / format of the code.
def process(emojiTags):
    with open("emojis.txt", "a", encoding="utf-8") as f:
        for tag in emojiTags:
            emojiTag = tag.find("header")
            # emojiCodeTag = tag.find("button", class_ = "gitmoji-clipboard-code").code
            emojiTextTag = tag.find("p")
            # f.write("# " + emojiTag.string + " " + emojiCodeTag.string + " " + emojiTextTag.string + "\n")
            f.write("[\"" + emojiTextTag.string + "\", [\"" + emojiTag.string + "\"]]\n")

if __name__ == "__main__":
    process(getEmojiTags(getSoup(getHTML(emojiURL))))