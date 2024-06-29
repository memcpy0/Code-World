@[toc]


---
# 1. 前言
记得在写[简单爬虫抓取知乎专栏文章标题和链接、存储进CSV](https://memcpy0.blog.csdn.net/article/details/119787477)这篇文章的爬虫中，我突发奇想，想把 `headers` 的内容写全一点——因为是请求头，我就把当时F12检查得到的网页请求头信息全部写了进去：
![在这里插入图片描述](https://img-blog.csdnimg.cn/b9811b7b31084711874daabd505d2647.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
代码大概是这样的：
```py
headers = {
    "Accept": "*/*",
    "Accept-Encoding": "gzip, deflate, br",
    "Accept-Language": "zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2",
    "Cache-Control": "max-age=0",
    "Connection": "keep-alive",
    "Cookie": "_zap=ce2381cf-c65f-48ef-a6a0-160e902f2e82; Hm_lvt_98beee57fd2ef70ccdd5ca52b9740c49=1629295726,1629295957,1629296000,1629296248; d_c0=\"AeAQH7aGjROPTkcxEtgvqQwtycoUK3QE0oQ=|1628695178\";_xsrf=ED8twdQGwH2Gll5MfqzZdJSikbFjl5Xf; KLBRSID=cdfcc1d45d024a211bb7144f66bda2cf|1629299432|1629299085; Hm_lpvt_98beee57fd2ef70ccdd5ca52b9740c49=1629299437",
    "Host": "www.zhihu.com",
    "Origin": "https://zhuanlan.zhihu.com",
    "Referer": "https://zhuanlan.zhihu.com/c_1034016963944755200",    
    "Sec-Fetch-Dest": "empty",
    "Sec-Fetch-Mode": "cors",
    "Sec-Fetch-Site": "same-site",
    "TE": "trailers",
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:91.0) Gecko/20100101 Firefox/91.0", 
    "x-ab-param": "tp_contents=2;tp_topic_style=0;zr_slotpaidexp=1;pf_adjust=0;qap_question_visitor= 0;qap_question_author=0;pf_noti_entry_num=0;se_ffzx_jushen1=0;zr_expslotpaid=1;tp_dingyue_video=0;top_test_4_liguangyi=1;tp_zrec=0",
    "x-ab-pb": "CroBMwQyA44DUANXBIkMaQHHArULYAvzA0IE9AvCAoUEzwtgBGwE2AKEArkCoAOmAUMAmwtNBDQM6ANoBLQKUgsqAk8D1wJsA30CGwBHAEABagGLBLQAiQJtAvYCtwMOBHQBCgQLBMoC1wvsCtwLwQJPAXIDVgwqA0UEAQvgCzQEaQRqBIgEoQOrA10EeQRFA58CzAL0A/gDZASMAqIDKgTAAgkEVwPhAwgEDws3DI0BOwLqA0MEBww/AOQKEl0AAAAAAAAAAAMAAAAAAAALAAAAAAsAABgCAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQAAAAEBAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQAACwAAAAA=",
    "x-requested-with": "fetch",
    "x-zse-93": "101_3_2.0",
    "x-zse-96": "2.0_aTO0bHH0gBOfH928G0tye49yS8SYb820YTtqbQ9yS_tx",
    "x-zst-81": "3_2.0aR_sn77yn6O92wOB8hPZniUZrHYu6vNBzhV1xD_0QXFxQXt084kmWgLBpukRQ6f0YiFGkDQye8FL7AtqM6O1VDQyQ6nxrRPCHukMoCXBEgOsiRP0XL2ZUBXmDDV9qhnyTXFMnXcTF_ntRueThTpYOrgYzwt99uCqRUYqr8OCWwcYZhwCADeMpvgY0B2MzgOLBM3GXqXYBhYPvXHGEGpLIbVOOBtM0BS0kGVYQuxfEBe_kDxYkBYOCDo1QieXjB3O-gOyfJxOsBpsgqx1ZUCO3J9qoQxm39tBbDLBpce8hDVBuBw9NGtOybX0hCX1HJxm5qfzWCp9sUXMYhgLUwNf09HC6Gp8IqX94gS1kUeCjqO_qcSBQvCKLv99EDw1ugc0cCwOBU3OOcLV2_tfABXmF9SLpGCsiBCYEbe_Pb3fST3VHUoM5heL2JUC"
}
```
`r = requests.get(url, headers = headers)` 发出请求，结果一下就报错了：
```py
json.decoder.JSONDecodeError: Expecting value: line 1 column 1 (char 0)
```
为什么JSON解码会报错？别的不说，先看看文本信息：
```py
def getJSON(url):
    r = requests.get(url, headers = headers)
    print(r.text)
    ...
```
结果是一堆乱码：
![在这里插入图片描述](https://img-blog.csdnimg.cn/3cb5eb72f612407badbea071afbc6a04.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
……问题很简单了，就像网页分析工具中看到的请求头内容一样，我在 `requests.get` 的 `headers` 中设置了 `"Accept-Encoding": "gzip, deflate, br"` ， 以模拟浏览器去访问网页，导致了这种错误的出现。只要注释掉 `Accept-Encoding` 就能解决问题，成功爬取网页。只是，**为什么**？此处祭出吃蛋挞（千反田える，ちたんだえる）老师的经典名言：わたし、気になります！
![在这里插入图片描述](https://img-blog.csdnimg.cn/22ae51e52bba409f95d4ba94760b4364.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
此处的原因是：
- 浏览器添加这一请求头选项 `"Accept-Encoding": "gzip, deflate, br"` ，发送网页请求；服务器会选择其中一个提议，使用对应的压缩算法，并返回压缩后的网页作为响应，并在响应头的 `Content-Encoding` 项中通知客户端其选择。只要浏览器也使用同样的压缩算法，就可以正确解压和显示网页。
- 代码中如果也添加此头部信息，将得到压缩后的数据，却没有解码。而将压缩数据当做普通文本来解析为JOSN，自然就报错了。


---
请求头中有很多有意思的东西，说起来还是蛮复杂的，现在突然没有心情了，以后再慢慢写吧……感觉开了好多坑啊┓( ´∀` )┏……

![在这里插入图片描述](https://img-blog.csdnimg.cn/69469e3403a741ba8976b79ff797501a.gif#pic_center)
 


