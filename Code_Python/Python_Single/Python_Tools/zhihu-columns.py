# -*- coding: utf-8 -*-
import os
import time
import requests
import csv

zhihuColumn = "c_1339974226623885312" # 自行替换
startURL = "https://www.zhihu.com/api/v4/columns/" + zhihuColumn + "/items?limit=10&offset={}"
headers = {
    "Accept": "*/*",
    # "Accept-Encoding": "gzip, deflate, br",
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


# 输入URL,得到JSON数据
def getJSON(url):
    try:
        r = requests.get(url, headers = headers)
        r.raise_for_status() # 响应状态码,出错则抛出异常
        r.encoding = r.apparent_encoding
        return r.json()
    except Exception as ex:
        print(type(ex))
        time.sleep(10)
        return getJSON(url)

# 输入文章总数,输出所有文章标题和链接的CSV文件
def process(total):
    num = 0 # 文章编号
    if (os.path.exists("zhihu.csv")): # 已经存在时则删除
        os.remove("zhihu.csv") 
    with open("zhihu.csv", "a", encoding = "UTF-8", newline = '') as f:
        writer = csv.writer(f)
        writer.writerow(["编号", "标题", "链接"]) # csv表头部分
        for offset in range(0, total, 10):
            jsonData = getJSON(startURL.format(offset))
            items = jsonData["data"]
            for item in items:
                num = num + 1
                writer.writerow([num, item["title"], item["url"]])

if __name__ == "__main__":
    jsonData = getJSON(startURL.format(0))
    process(jsonData["paging"]["totals"])

