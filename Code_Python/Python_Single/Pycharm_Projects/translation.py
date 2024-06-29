# -*- coding:utf-8 -*-
import tkinter
from tkinter.constants import *
from tkinter.messagebox import * # messagebox not in *
import requests

def translate():
    lexicon = entry1.get().encode("utf-8")
    # print(lexicon)
    if not lexicon:
        showinfo("温馨提示", "请输入要翻译的文字!")

    headers = {
            "User - Agent": "Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.90 Mobile Safari/537.36"
        }
    url = "http://fanyi.youdao.com/translate?smartresult=dict&smartresult=rule"
    data = {
        "i" : lexicon,
        "from" : "AUTO",
        "to" : "AUTO",
        "smartresult" : "dict",
        "client" : "fanyideskweb",
        # 时间戳
        # "salt" : "1533362734935",
        # 签名 加密 变化
        # "sign" : "b3a6a4cf3809bdbe56617486eaf3fd0a",
        "doctype" : "json",
        "version": "2.1",
        "keyfrom" : "fanyi.web",
        "action" : "FY_BY_CLICKBUTTION",
        "typoResult" : "false"
    }
    # 端口 http 80 https 443 mysql 3306
    r = requests.post(url, data=data, headers=headers)
    # print(r.json())
    res = r.json()['translateResult'][0][0]['tgt']
    result.set(res)

root = tkinter.Tk()
root.title("中英互译")     # 窗口标题
root.geometry("340x100+250+300") # 窗口大小、位置

# 标签控件
label1 = tkinter.Label(root, text="输入要翻译的文字:", font=("微软雅黑", 12), fg='black')
# label1.pack(fill=X, expand=1) pack 包 place 位置
label1.grid(row=0, column=0, sticky=W)

label2 = tkinter.Label(root, text="翻译之后的结果:", font=("微软雅黑", 12), fg='black')
# label2.pack(fill=Y, expand=1)
label2.grid(row=1, column=0, sticky=W)

# 输入控件
entry1 = tkinter.Entry(root, font=("微软雅黑", 13))
entry1.grid(row=0, column=1, sticky=E)
# 翻译之后的结果
# 根据翻译内容变化的变量
result = tkinter.StringVar() ###
entry2 = tkinter.Entry(root, font=("微软雅黑", 13), textvariable=result)
entry2.grid(row=1, column=1, sticky=E)

# 按钮控件
button1 = tkinter.Button(root, text="翻译", font=("微软雅黑", 10), width=4, height=2, command=translate)
button1.grid(row=2, column=0, sticky=W) # sticky 对齐方式            # exit
button2 = tkinter.Button(root, text="退出", font=("微软雅黑", 10), width=4, height=2, command=root.quit)
button2.grid(row=2, column=1, sticky=E)

root.mainloop()
