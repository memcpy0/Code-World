# -*- coding:utf-8 -*-

import tkinter
from tkinter.constants import *
from tkinter.messagebox import *
import requests

def get_weather():
    city = entry.get().encode("utf-8")
    if not city:
        showerror("警告", "请输入要查询的城市名称!")
    # print(city)
    else:

root = tkinter.Tk()
root.title("天气查询")
root.geometry("485x380+400+200") # 宽 高

label = tkinter.Label(root, text="输入要查询的城市名称:", font=("微软雅黑", 10), fg="black")
label.grid(row=0, column=0, sticky=W)
# 输入控件
entry = tkinter.Entry(root, font=("微软雅黑", 22))
entry.grid(row=0, column=1, sticky=E)
# 列表框控件
lis = tkinter.Listbox(root, font=("微软雅黑", 15), width=40, height=10)
lis.grid(row=1, columnspan=3, sticky=W)
# 按钮控件
btn = tkinter.Button(root, text="查询", font=("微软雅黑", 15), fg="black", width=10, height=1, command=get_weather)
btn.grid(row=2, column=0, sticky=W)

btn2 = tkinter.Button(root,  text="退出", font=("微软雅黑", 15), fg="black", width=10, height=1, command=exit)
btn2.grid(row=2, column=1, sticky=E)
root.mainloop()