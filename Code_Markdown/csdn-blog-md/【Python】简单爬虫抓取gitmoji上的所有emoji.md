@[toc]
# 1. 开始说明
[https://gitmoji.dev/](https://gitmoji.dev/)网站上展示了许多emoji，它们常被用在Git提交消息、`README.md` 和GitHub Wiki页面：
![在这里插入图片描述](https://img-blog.csdnimg.cn/c87b24502dd34cf0b72a744523638b9b.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

一个个把这五六十个emoji复制下来太麻烦了，所以用十几分钟写了个小爬虫。

---
# 2. 分析网站页面
很简单的结构，每个 `article` 标签对都包含了一个emoji的全部信息——图片、emoji码、使用说明，我们只用提取出它们，然后写入到相关文件即可：
![在这里插入图片描述](https://img-blog.csdnimg.cn/7b7428c3e474401e9620f67e52d1b4b9.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 3. 实际代码和效果
代码很简单，不用多说：
```py
# -*- coding: utf-8 -*-
import os
import re
import time
import requests
from bs4 import BeautifulSoup as BS

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
            emojiCodeTag = tag.find("button", class_ = "gitmoji-clipboard-code").code
            emojiTextTag = tag.find("p")
            f.write("# " + emojiTag.string + " " + emojiCodeTag.string + " " + emojiTextTag.string + "\n")

if __name__ == "__main__":
    emojiURL = "https://gitmoji.dev" # emoji资源网址
    process(getEmojiTags(getSoup(getHTML(emojiURL))))
```
VS Code中执行很成功：
![在这里插入图片描述](https://img-blog.csdnimg.cn/5b15cfa3382c412cbe8816072f04d0a5.png)打开相应输出文件如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/a1e8a6d82e804337929424e9d2e8ce6a.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

