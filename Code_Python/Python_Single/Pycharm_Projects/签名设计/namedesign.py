# -*- coding:utf-8 -*-

import tkinter
from tkinter.constants import *
from tkinter.messagebox import *
import requests, urllib
import re

def get_img():
    name = nameent.get().encode("utf-8")
    if not name:
        showinfo(title="温馨提示", text="你还没有输入姓名")
        return
    r = requests.post("http://m.uustv.com/", data='word={}&sizes=60&fonts:jfcs.ttf&fontcolor:#000000'.format(name))
    reg = r'<div class ="tu"><img src="(.*?)"/></div>'
    html = r.text
    imgurl = "http://m.uustv.com/" + re.findall(reg, html)[0]
    print(imgurl)

window = tkinter.Tk()
window.title("Python个性签名设计~")
window.geometry("300x80+700+200")

label = tkinter.Label(window, text="姓名", font=('微软雅黑', 15))
label.grid()

nameent = tkinter.Entry(window, font=('微软雅黑', 15))
nameent.grid(row=0, column=1)

button = tkinter.Button(window, text="一键设计签名", font=('微软雅黑', 15), width=10, height=1, command=get_img)
button.grid(row=1, column=1, sticky=E)                                            # 绑定方法不能加括号，这样就是调用了

window.mainloop()
