@[toc]

# 1. 观察网站
[https://konachan.net/post](https://konachan.net/post)，这个网站上图的质量还不错：
![在这里插入图片描述](https://img-blog.csdnimg.cn/3d13d7c0f5dc4ab0a56269babddcb124.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

而且提供了高级检索功能：
![在这里插入图片描述](https://img-blog.csdnimg.cn/0ef892a84c0b487090366a6a840ea7b6.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
什么是 `Questionable` 和 `Explicit` ？我很好奇：
![请添加图片描述](https://img-blog.csdnimg.cn/49b9d2f923a046b69346af92009da84d.gif)

看了一下网站的Wiki，明白了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/52a788bc120e4475a6eea6d960eb82ed.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
由于本人电脑的显示分辨率是1920x1280，所以我希望电脑壁纸至少要有1920x1080或以上尺寸，所以在高级检索界面修改了一下 `Width` 和 `Height` ，提交后检索结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/1e13c6728f874024be6f6a32ebec260f.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
F12检查网页，发现每张图片都被包含在一个 `<li></li>` 标签对中，大图的链接在其中的类名为 `directlink largeimg` 的 `<a></a>` 标签对的 `href` 属性中：
![在这里插入图片描述](https://img-blog.csdnimg.cn/4d418d369f234402978d6f91ca1dfbe1.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
翻页也很简单，就是在 `https://konachan.net/post?page=` 后面添加数字。有时候可能遇到某一页没有任何图片的情况，~~检查是否有 `Nobody here but us chickens!` 这句话或者~~判断图片节点的数量是否为零即可：
![在这里插入图片描述](https://img-blog.csdnimg.cn/be8df1c62e8f4e7d89bb0c1750d12e37.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 2. 实际代码
以下代码不保证没有Bug，如要使用请自行修改：
```py
# -*- coding: utf-8 -*-
import os
import time
import requests
from bs4 import BeautifulSoup as BS

url = "https://konachan.net/post?page={}&tags=width:{}..+height:{}.." # 页面,宽度下限,高度下限
outDir = "C:\\pics" # 图片输出文件夹
headers = {"User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:91.0) Gecko/20100101 Firefox/91.0"}

# 输入URL,得到响应内容
def getResponse(url):
    try:
    	time.sleep(100) # 慢点爬
        r = requests.get(url, headers=headers)
        r.raise_for_status() # 响应状态码,出错则抛出异常 
        return r
    except Exception as ex: # 出错就递归抓取
        print(type(ex)) 
        return getResponse(url)

# 输入html,输出soup对象
def getSoup(html):
    return BS(html, "lxml")

# 输入soup,输出图片节点列表
def getImagesList(soup):
    images = soup.find_all("a", class_="directlink largeimg")
    return images

# 输入图片编号和URL,保存图片
def saveImage(imageNum, imageURL):
    if (os.path.exists(outDir) == False):
        os.mkdir(outDir)
    with open(outDir + "\\{}.png".format(imageNum), 'wb') as f:
        image = getResponse(imageURL).content
        f.write(image)

def process(endPage, width, height):
    imageNum = 1 # 以数字为图片命名
    for i in range(1, endPage):
        html = getResponse(url.format(i, width, height)).text
        soup = getSoup(html)
        imagesList = getImagesList(soup)
        for j in range(len(imagesList)):
            saveImage(imageNum, imagesList[j]['href'])
            imageNum = imageNum + 1

if __name__ == "__main__":
    # endPage = int(input("输入要抓取的终止网页页码:\n"))
    # width = int(input("输入图片最低宽度:\n"))
    # height = int(input("输入图片最低高度:\n"))
    endPage = 30 # 只抓取第1页到第30页的图片
    width, height = 1920, 1080
    process(endPage, width, height) # 从第1页抓取到第endPage页,图片尺寸大于(width,height)
```
---
# 3. 运行效果
下载的图片文件夹如下，大概263张，因为有几张是用户重复上传的：
![在这里插入图片描述](https://img-blog.csdnimg.cn/293f1c745412489ca9e3b5bb9225ec33.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

放上一张能过审的：
![在这里插入图片描述](https://img-blog.csdnimg.cn/ac4302bda2bb4edcae554cb66277aee6.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

