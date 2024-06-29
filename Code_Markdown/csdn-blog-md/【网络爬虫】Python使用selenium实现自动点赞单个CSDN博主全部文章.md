@[toc]

---
# 1. 为什么写这个爬虫
为什么突然心血来潮写这个爬虫呢？因为我突然发现，自己写了这么多文章（尽管一大部分都是题解），也有了可怜的两百多个粉丝，被访问量超过13万+，但是收获的点赞却少得可怜，只有两三百个……而且不仅是我自己，很多粉丝众多的博主收获的点赞数也很少……这不禁让我陷入了沉思……

原因可能有哪些呢？
- 多数读者可能都是通过百度搜索各种技术问题，然后点进CSDN的，解决了自己的问题就会关掉网页，**不会过多停留**……如果没解决，可能在心底暗骂博主ZZ吧/(ㄒoㄒ)/~~
- 即使有一部分人觉得博主写得好，想要顺手点个赞，但由于CSDN的阴间设置——**点赞需要注册账号**，感到太麻烦而放弃点赞
- 这样一来，只有极少数有CSDN账号、并且觉得博主写得好的人，才会点赞
- 如果博主写的文章很多，就只有少数一部分文章会被集中点赞，大部分文章的赞数都很少——因为没有人有这个精力，点赞完所有的文章

因此我决定写一个爬虫，自动点赞一个博主的所有文章，省时省力，同时让被关注的博主感受到什么叫**洪荒之力**，什么叫**真爱粉**（笑


---
# 2. 实现思路
为了在CSDN中点赞，就**必须登录账号**，因此需要Selenium——这是一个Web自动化测试工具，[中文文档在这](https://python-selenium-zh.readthedocs.io/zh_CN/latest/1.%E5%AE%89%E8%A3%85/)，尽管这个文档写得不怎么样，但是够用了。

实现对一个博主的全文章点赞，分为以下几个步骤：
1. 登录个人账号
2. 获取、存储、读取、传入Cookies
3. 进入博主的博客页面，获取当页的所有文章地址
4. 对每一篇文章，进入文章内找到点赞按钮，如果已经点赞就直接返回；否则点赞
5. 获取下一页的链接，如果找到了下一页的链接，就返回3；否则退出程序

---
# 3. 事先准备
首先安装Selenium库：
```py
pip install selenium
```
与此同时，还要根据谷歌浏览器的版本下载一个Chrome驱动器 `chromedriver.exe` ，[下载地址在这里](https://sites.google.com/a/chromium.org/chromedriver/downloads)，可能需要翻墙。由于我的谷歌浏览器版本是：`87.0.4280.88（正式版本） （64 位）`，因此下了第二个：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201226213239157.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201226213445780.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

解压后，将 `chromedriver.exe` 和代码文件放在同一个文件夹中。

---
# 4. 代码实现
## (1) 登录个人账号
片段代码如下：
```py
import time
import json
import random
from selenium import webdriver
```
运行代码，看到下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201226215237843.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
由于我们使用的是**账号密码登录方式**，在这里需要做一个**切换**，点击**账号密码登录**：
```py
time.sleep(2) # 不要太过火
driver.find_element_by_xpath('//*[@id="app"]/div/div/div[1]/div[2]/div[5]/ul/li[2]/a').click()
time.sleep(2) # 免得被封号
```
运行后看到下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201226215822243.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

此时我们需要做的就是**清空账号和密码框**，然后**填写自己的CSDN账号密码**，最后**点击登录**：
```py
driver.find_element_by_xpath('//*[@id="all"]').clear() # 手机号/邮箱/用户名
driver.find_element_by_xpath('//*[@id="password-number"]').clear() # 密码
print("账号密码框清空完毕")
driver.find_element_by_xpath('//*[@id="all"]').send_keys(CSDN_Account) # 填入用户名
driver.find_element_by_xpath('//*[@id="password-number"]').send_keys(CSDN_Password) # 填入密码
time.sleep(3) # 等待对象被加载
driver.find_element_by_xpath('//*[@id="app"]/div/div/div[1]/div[2]/div[5]/div/div[6]/div/button').click() # 点击登录
```
---
## (2) 获取、存储、读取、传入Cookies
这里使用 `json` 库来存储和读取Cookies：
- `json.loads()` 将 `str` 转化成 `dict` 格式
- `json.dumps()` 是将 `dict` 转化成 `str` 格式
- `json.dump(dict, f)` 把 `dict` 转换成 `str` 类型，存储到 `f` 代表的文件中
- `json.load(f)` 把 `f` 指向的文件中的内容读取出来

使用 `webdriver` 提供的以下方法操作Cookies：
- `get_cookies()`  获得Cookies信息
- `add_cookie(cookie_dict)` 添加Cookies
- `delete_cookie(name)` 删除特定(部分)的Cookies
- `delete_all_cookies()` 删除所有的Cookies

```py
cookies = driver.get_cookies() # 获取并保存Cookies
with open("cookies.data", "w") as f: 
    json.dump(cookies, f) # 将dict序列化为str格式, 存入文件
```
接着读取  `cookies.data` 文件中的Cookies，添加到 `driver` 中：
```py
with open("cookies.data", "r") as f:
	cookies = json.load(f)
	for cookie in cookies:
		driver.add_cookie(cookie)
```
为什么要做这种吃力不讨好的事情呢？因为 `webdriver` 使用 `get(url)` 切换页面后，会**遗忘**之前的状态，因此必须使用Cookies表示**我们正在登陆自己的CSDN账号**。

---
## (3) 进入博主的博客页面
以我个人的博客页面为例：
```py
BlogAddr = "https://blog.csdn.net/myRealization?spm=1000.2115.3001.5343"
driver.get(BlogAddr)
driver = webdriver.Chrome(executable_path = "chromedriver.exe")
time.sleep(2)
```
如果不使用Cookies，切换页面后会看到这种结果：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201226222723759.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

