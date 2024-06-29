# -*- coding: utf-8 -*-
import os
import re
import time
import requests
from bs4 import BeautifulSoup as BS

"""
网站的结构是：
总网站——小说列表(要翻页)
            |——小说页1
            |——小说页2
            |——小说页3——-|
            |....       |——小说1
                        |——小说2        
                        |——小说3————章节列表(要翻页)       
                        |......         |——章节页1
                                        |——章节页2
                                        |——章节页3——|——章节1          
                                        |......     |——章节2
                                                    |——章节3——页表(要翻页)
                                                                |——书页1
                                                                |——书页2
                                                                |——书页3
"""

# 全局变量
bookDir = ""
domain =  "http://m.diyibanzhu.store/"
action = "wap.php?action="
shukuPage = "shuku&tid=8&pageno=1"
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

# 输入soup,输出小说列表下一页的链接
# 注意小说列表的末页没有“nexPage" 
# 注意章节列表的末页没有"nexPage"这一CSS类
# 注意小说翻页没有"nextPage"
def getNextPage(soup):
    nextTag = soup.find("a", class_="nextPage")
    return nextTag["href"] if nextTag != None else None

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

# 输入soup,输出当前章节页的各页节点列表,可能为空
def getPageTags(soup):
    tmpTag = soup.find("center", class_ = "chapterPages") # 找到这类节点
    if tmpTag == None:      # 这类节点为空,说明没有页
        return []
    pageTags = tmpTag.find_all("a") 
    return [pageTag["href"] for pageTag in pageTags]

# ------------------------------------------------------------------------------
# 输入一个字典,包含多个章节列表页的链接和书名,没有输出,存为小说
def saveBookList(rec):
    for key, val in rec.items():
        saveBook(key, val)

# 输入章节列表页的链接和书名,没有输出,存为小说
def saveBook(listPage, bookName):
    emptyFlag = False # 空内容标记
    # print(bookName)
    filePath = bookDir + bookName + ".txt"
    if (os.path.exists(filePath)):
        # os.remove(filePath) # 已经存在则先删除
        return
    cnt = 0 
    with open(filePath, "a", encoding = "utf-8") as f:
        while True:                            # 不断循环抓取章节列表页
            # print(domain + action + listPage, end='\n')
            soup = getSoup(getHTML(domain + action + listPage))
            
            chapterTags = getChapterTags(soup) # 得到当前的章节列表
            if chapterTags == []: # 空列表
                emptyFlag = True
                break
            
            for chapterTag in chapterTags: # 处理每一个章节节点
                cnt += 1
                print(cnt, chapterTag.string)
                saveChapter(chapterTag["href"], chapterTag.string, f) # 存入到文件中
                
            listPage = getNextPage(soup)  # 得到下一章节列表页的链接
            if listPage == None:          # 没有下一页,到达章节列表的末页
                break
            listPage = listPage.split('action=')[1]
            
    if emptyFlag == True: # 小说内容为空
        os.remove(filePath) # 删除这本小说

# 输入章节页的链接、章节名、文件对象, 没有输出,写入文件中
def saveChapter(chapterPage, chapterName, file):
    file.write(chapterName + "\n") # 写入这一章的标题
    savePage(chapterPage, file) # 写入第一页的内容
    
    soup = getSoup(getHTML(domain + chapterPage))     
    pages = getPageTags(soup)
    if pages == None: # 可能这一章没有页
        return
    for page in pages:
        savePage(page, file)
        
# 输入章节中每一页的链接、文件对象, 没有输出, 写入文件中
def savePage(page, file):
    soup = getSoup(getHTML(domain + page))    
    pTags = soup.select("div.bd div.page-content.font-large p") # 得到两个p节点
    plen = len(pTags)
    try:
        file.write(pTags[plen - 1].get_text()) # 将小说内容写到文件中
    except Exception as ex:
        print(type(ex))

# 输入一个字符串,输出一个合法的文件名
def filter(str):
    return re.sub(r'[\\/:*?"<>|]', '-', str) # 替换非法字符为'-'
    
if __name__ == "__main__":
    '''
    while True:                                     # 不断循环抓取小说列表页
        soup = getSoup(getHTML(domain + shukuPage))
        time.sleep(2)                               # 睡眠2s
        bookTags = getBookTags(soup)                # 得到当前的小说节点列表
        for bookTag in bookTags:                    # 处理每一个小说节点
            print(bookTag.string)                   # 打印小说名称
            newBookName = filter(bookTag.string)
            saveBook(bookTag["href"], newBookName)
        shukuPage = getNextPage(soup)               # 得到下一小说列表页的链接
        if shukuPage == None:                       # 没有下一页,到达小说列表的末页
            break
    '''
    saveBookList({
                "list&id=10351" : "长腿巨乳俏人妻",
                # "list&id=10367" : "爱与欲的升华"
                #   "list&id=3327" : "催眠玩偶雪之下雪乃",
                #   "list&id=11466" : "春物催眠之丝袜丽奴",
                #   "list&id=12854" : "春物语-我的雪之下：穿越成了大人物....",
                #   "list&id=12893" : "雪之下的秘密侍奉活动",
                #   "list&id=7111" : "位面猎奴之春物",
                #   "list&id=900" : "To Love-Ru NTR",
                #   "list&id=3979" : "淫魔在综漫",
                #   "list&id=2837" : "综漫催眠之佐山浩史的艳遇", #***
                #   "list&id=3336" : "综漫之大后宫系统",
                #   "list&id=786" : "综漫幻想传奇",
                #   "list&id=12679" : "综漫之调教系统",
                #   "list&id=1859" : "被绑架到魅魔学院成为肉棒教具",
                #   "list&id=2199" : "我被绑架到大小姐学校当肉棒样本",
                #   "list&id=953" : "位面猎奴之魔法禁书目录",
                #   "list&id=7008" : "位面猎奴之御坂美琴", 
                #   "list&id=2888" : "位面猎奴之狐妖小红娘",
                #   "list&id=1258" : "位面猎奴之我的妹妹哪有这么可爱",
                #   "list&id=7094" : "位面猎奴之埃罗芒阿老师",
                #   "list&id=6985" : "位面猎奴之亚丝娜",
                #   "list&id=7029" : "位面猎奴之从零",
                #   "list&id=5751" : "位面猎奴之回明",
                #   "list&id=7051" : "位面猎奴之Fate-Zero",
                #   "list&id=7510" : "禁止性骚扰（催眠）"
    })

    """
    list&id=9602", "双焰——圣女淫落")
    list&id=10603", "催眠系统")
    """
