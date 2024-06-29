# -*- coding:utf-8 -*-
"""
in order to master tkinter to do GUI
this is a tool to impress your love
"""

# from tkinter import *  # __all__ = [a, b]
import tkinter
from tkinter.constants import *
from tkinter.messagebox import * # messagebox not in *

def Love():
    # 独立组件，顶级窗口
    love = tkinter.Toplevel(tk)
    love.geometry("300x100+500+200")
    love.title("好巧，我也是！")

    label = tkinter.Label(love, text="好巧，我也是！", font=("微软雅黑", 20))
    label.pack()

    # entry = tkinter.Entry(love, font=("微软雅黑", 16))
    # entry.pack()
    btn = tkinter.Button(love, text="确定", width=10, height=2, command=close_all_window)
    btn.pack()

def noLove():
    no_love = tkinter.Toplevel(tk)
    no_love.geometry("300x100+500+200")
    no_love.title("再考虑考虑呗？！")
    label = tkinter.Label(no_love, text="再考虑考虑呗？！", font=("微软雅黑", 20))
    label.pack()

    btn = tkinter.Button(no_love, text="好的", width=10, height=2, command=no_love.destroy)
    btn.pack()
    no_love.protocol("WM_DELETE_WINDOW", closenolove)  # 删除窗口，触发函数方法

def closenolove():
    # showinfo(title="再考虑一下", text="再考虑考虑呗")
    noLove()

def closeWindow():
    showerror(title="警告", message="不许关闭，好好回答！") # 显示警告
    # showinfo 显示提示信息
    return

def close_all_window():
    # destroy 根窗口
    tk.destroy()
    # return

tk = tkinter.Tk()               # 创建根窗口
tk.title("你喜欢我吗？")         # 窗口标题
tk.geometry("422x480+540+240")  # 窗口大小(宽x高)
# tk.geometry("+540+240")       # 窗口位置(+x+y)

# protocol 用户关闭窗口触发的事件
tk.protocol("WM_DELETE_WINDOW", closeWindow) # 删除窗口，触发的函数

# 标签控件
label = tkinter.Label(tk, text="Hey，小姐姐", font=("微软雅黑", 17), fg="red") # font + 颜色
label.grid(row=0, column=0, sticky=W)     # 网格式布局，最少用, 默认从左上角0行0列开始 # label.pack()

label1 = tkinter.Label(tk, text="喜欢我吗？", font=("微软雅黑", 30))
label1.grid(row=1, column=1, sticky=E)    # 对齐方式 E S W N（右 下 左 上）

# 显示图片 但PhotoImage()方法不支持jpg格式
photo = tkinter.PhotoImage(file = "D:\\Python_Projects\\Pycharm_Projects\\表白界面\\cc3.png")
imagelabel = tkinter.Label(tk, image = photo)
imagelabel.grid(row=2, columnspan=2) # 跨列处理

# 按钮控件                                  # px像素         # command点击触发的方法
button = tkinter.Button(tk, text="喜欢！", width = 15, height = 2, command=Love)
button.grid(row=3, column=0, sticky=W)

button1 = tkinter.Button(tk, text="不喜欢", command=noLove)
button1.grid(row=3, column=1, sticky=E)

# 显示窗口，消息循环
tk.mainloop()


