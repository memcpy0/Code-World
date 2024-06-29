@[toc]

---
## 1. 分析URL和网页源代码
- 最初的域名：`http://m.diyibanzhu.store/` 手机版网页，更方便分析。
- 在最初域名后加上`wap.php?action=shuku` ，`action=shuku` 表示访问书库的小说列表：
	- http://m.diyibanzhu.store/wap.php?action=shuku&tid=8 `tid=8` 时为**其他小说**， `typeid=8` 是唯一有意义的参数值；
	- http://m.diyibanzhu.store/wap.php?action=shuku&order=1&tid=8 `order=1` 是二级选项——按照**总人气**排序，`order=2` ——按照**月人气**排序，`order=3` ——按照**新书**排序，`order=4` ——按照**字数**排序；没有 `order` 参数则直接按照**最近更新**排序。
	- wap.php?action=shuku&tid=8&totalresult=8697&pageno=290 是**末页**，`totalresult` 是总的小说数量，`pageno` 是小说列表的页号
	- `<a class="name" href="/wap.php?action=list&amp;id=10986">小说名称</a>` 小说列表中的小说链接、小说名称（`&amp;` 就是 `&`） 
- 在最初域名后加上小说链接，`action=list` 表示进入章节列表：
	- /wap.php?action=list&id=7595&uid=&totalresult=50&pageno=2 ，`totalresult` 是总的章节数量，`pageno` 是章节列表的页号
	- `<a href="/wap.php?action=article&amp;id=729693">小说章节名</a>` 章节列表中的章节链接、章节名称
	- `<div class="page-content font-large">` 中的第二个 `<p></p>` 是章节内容


---
## 2. 爬取流程
1. 先访问 `http://m.diyibanzhu.store/wap.php?action=shuku` ，进入按照**最近更新**排序的小说列表（书库）；
2. 选出这一页的小说列表（小说名称，小说链接）
	- `bs4` 的搜索方法：按照CSS类名搜索tag `soup.find_all("a", class_ = "name")`
	- CSS选择器 `li.column-2:nth-child(1) > div:nth-child(1) > a:nth-child(1)` 
	- CSS路径 `html body div.container div.mod.block.book-all-list div.bd ul li.column-2 div.right a.name` ，即有 `soup.select(.name)`
	- XPATH路径 `/html/body/div[3]/div[1]/div[2]/ul/li[2]/div/a`
4. 通过小说链接进入章节列表，选出这一页的章节列表（章节名称、章节链接），注意对**最新章节**的处理
	- CSS选择器 `div.mod:nth-child(7) > div:nth-child(2) > ul:nth-child(1) > li:nth-child(1) > a:nth-child(1)`
	- CSS路径 `html body.cover div.container div.mod.block.update.chapter-list div.bd ul.list li a` 
	- XPATH路径 `/html/body/div[3]/div[7]/div[2]/ul/li[1]/a` ，**注意章节列表为空的情况** `/html/body/div[3]/div[7]/div[2]/ul`
5. 通过章节链接进入章节，选择出文章内容，写到文本之后：XPATH路径 `/html/body/div[2]/div[4]/div[2]/div/p[2]`，**注意 `<br>`**
6. 得到**末页**范围，不断改变 `pageno` 的参数值，进入章节列表的下一页；**注意没有末页（空小说）和下一页的情况**；或者不断使用CSS选择器进行搜索，直到找不到 `"nextPage"` 为止
7. 得到**末页**范围，不断改变 `pageno` 的参数值，进入小说列表的下一页；或者不断使用CSS选择器进行搜索，直到找不到 `"nextPage"` 为止

---
## 3. 事先准备
下载 `python3` 的IDLE和解释器，然后安装这些库：
```shell
pip install requests
pip install bs4
pip install lxml
pip install xpath
```
---
## 4. 具体代码
粗略介绍一下代码中各个函数的功能：
- `getHTML(url), getSoup(html)` 只是为了避免重复语句。其中 `getHTML(url)` 如果不抛出异常则直接返回 `r.text` ，抛出异常则**递归调用** `getHTML(url)` ，直到某次直接返回得到 `r.text` 为止。
- `getNextPage(soup)` 得到下一小说列表/章节列表页的链接，如果到了**末页**，则返回 `None` 
- `getBookTags(soup)` 得到当前页的小说节点列表
- `getChapterTags(soup)` 得到当前页的章节节点列表，注意处理**最新章节**和**空书**这两种特殊情况
- `saveBook(listPage, bookName)` 根据爬取的URL文件夹路径和当前的书名，创建小说文件，接着不断调用 `getChapterTags(soup)` 得到章节列表，调用 `saveChapter(chapterPage, chapterName, file)` 得到具体章节内容并且写入文件
- `saveChapter(chapterPage, chapterName, file)` 根据章节路径爬取文本内容，和章节标题一起写入小说文件中

```py
# -*- coding: utf-8 -*-
import os
import re
import time
import requests
from bs4 import BeautifulSoup as BS

# 全局变量
bookDir = "C:/Users/21839/Downloads/"
domain =  "http://m.diyibanzhu.store/"
shukuPage = "/wap.php?action=shuku&tid=8&pageno=1"
# 输入url,输出html,出现异常时要多次调用
headers = {"User-Agent" : "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:84.0) Gecko/20100101 Firefox/84.0"} # 定制响应头
def getHTML(url):
    try:
        r = requests.get(url,headers = headers)
        r.raise_for_status() # 响应状态码,出错则抛出异常
        r.encoding = "utf-8"
        return r.text
    except Exception as ex:
        print(type(ex))
        time.sleep(1)
        return getHTML(url)

# 输入html,输出soup对象
def getSoup(html):
    return BS(html, "lxml")

# 输入soup,输出下一页的链接
# 注意小说列表的末页没有“nexPage" 
# 注意章节列表没有"nexPage"这一CSS类
def getNextPage(soup):
    nextTag = soup.find("a", class_="nextPage")
    if nextTag == None:
        return None
    return nextTag["href"]

# 输入soup,输出所有<a class="name" ...>...</a>的节点列表,不为空
def getBookTags(soup):
    return soup.find_all("a", class_ = "name") # 按照CSS类名搜索整个列表

# 输入soup,输出当前页的章节节点列表,可能为空
def getChapterTags(soup):
    tmpTag = soup.find_all("div", class_ = "mod block update chapter-list") # 找到这类节点
    if tmpTag == []:      # 这类节点为空,说明没有最新章节和章节列表
        return []
    elif len(tmpTag) == 2: # 将最新章节节点移除文档树并完全销毁
        tmpTag[0].decompose() # 之后只有章节列表
    return soup.select("div.mod.block.update.chapter-list div.bd ul.list li a")

# 输入章节列表页的链接和书名,没有输出,存为小说
def saveBook(listPage, bookName):
    emptyFlag = False # 空内容标记
    filePath = bookDir + bookName + ".txt"
    if (os.path.exists(filePath)):
        os.remove(filePath) # 已经存在则先删除
    with open(filePath, "a", encoding = "utf-8") as f:
        while True:                            # 不断循环抓取章节列表页
            soup = getSoup(getHTML(domain + listPage))
            
            chapterTags = getChapterTags(soup) # 得到当前的章节列表
            if chapterTags == []: # 空列表
                emptyFlag = True
                break
            
            for chapterTag in chapterTags: # 处理每一个章节节点
                saveChapter(chapterTag["href"], chapterTag.string, f) # 存入到文件中
                
            listPage = getNextPage(soup)  # 得到下一章节列表页的链接
            if listPage == None:          # 没有下一页,到达章节列表的末页
                break
            
    if emptyFlag == True: # 小说内容为空
        os.remove(filePath) # 删除这本小说

# 输入章节页的链接、章节名、文件对象, 没有输出,写入文件中
def saveChapter(chapterPage, chapterName, file):
    soup = getSoup(getHTML(domain + chapterPage))
    pTags = soup.select("div.bd div.page-content.font-large p") # 得到两个p节点
    plen = len(pTags)
    try:
        file.write(chapterName + "\n") # 写入这一章的标题
        file.write(pTags[plen - 1].get_text()) # 将小说内容写到文件中
    except Exception as ex:
        print(type(ex))

# 输入一个字符串,输出一个合法的文件名
def filter(str):
   return re.sub(r'[\\/:*?"<>|]', '-', str) # 替换非法字符为'-'
    
if __name__ == "__main__":
    while True:                                     # 不断循环抓取小说列表页
        soup = getSoup(getHTML(domain + shukuPage))
        time.sleep(1)                               # 睡眠1s
        bookTags = getBookTags(soup)                # 得到当前的小说节点列表
        for bookTag in bookTags:                    # 处理每一个小说节点
            print(bookTag.string)                   # 打印小说名称
            newBookName = filter(bookTag.string)
            saveBook(bookTag["href"], newBookName)
        shukuPage = getNextPage(soup)               # 得到下一小说列表页的链接
        if shukuPage == None:                       # 没有下一页,到达小说列表的末页
            break
```
---
## 5. 几个问题
① 这个网站是根据**最近更新**排序小说的，因此一旦出现小说更新，就会排到前页（然而我们无法回头！），一些小说被挤到后面（我们可能重复爬取！）。这个问题如何解决？

② 我们是否能够使用一个数据库，存储小说的名称和URL链接吗？这样遇到新的小说和链接就存入，如果已经存在则不变。这里多出了一个访存数据库的过程，更加耗时。能有什么好处呢？
- 得到了一个小说的URL数据库？
- 可以通过比较最后一部分内容或者**字数**，判断小说是否更新，更新则写入更新内容到小说文件中，没有更新就避免了重复爬取小说？一想就很麻烦啊，这种功能不要也罢吧。

③ 发现有的**小说名**含有对于一个文件名来说非法的字符，必须过滤掉。
