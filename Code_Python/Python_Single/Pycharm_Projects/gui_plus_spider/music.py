# -*- coding:utf-8 -*-
import tkinter
from tkinter.messagebox import *
from tkinter.constants import *
# import requests
# from bs4 import BeautifulSoup
from selenium import webdriver
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities # 给phantomjs模拟一个头部

# from urllib.request import urlretrieve
import time

# 爬取网易云音乐
# 获取网易源代码
# 获取歌曲ID
# 下载歌曲
def get_music():
    url = "https://music.163.com/m/playlist?id=434809001" #entry.get() # .encode("utf-8")
    # print(url) # https://music.163.com/m/playlist?id=434809001
    if not url:
        showerror("警告", "请输入要抓取的歌单URL!")

    dcap = dict(DesiredCapabilities.PHANTOMJS)
    dcap["phantomjs.page.settings.userAgent"] = (
        "Mozilla/5.0(Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36(KHTML, like Gecko) Chrome/56.0.2924.90 Mobile Safari/537.36"
    )

    """
    headers = {
        "Host" : "music.163.com",
        "Referer" : "https://music.163.com/",
        "User-Agent" : "Mozilla/5.0(Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36(KHTML, like Gecko) Chrome/56.0.2924.90 Mobile Safari/537.36"
    }
    """
    # res = requests.get(url, headers=headers).text
    # print(r)
    # r = BeautifulSoup(res, "html.parser")
    """
    music_dict = {}
    result = r.find('ul', {'class', 'f-hide'}).find_all('a')
    print(result)
    for music in result:
        music_id = music.get('href').strip("/song/id?=")
        music_name = music.text
        music_dict[music_id] = music.text
    """

    driver = webdriver.PhantomJS(executable_path="D:/Python_Projects/Pycharm_Projects/phantomjs-2.1.1-windows/bin/phantomjs.exe", desired_capabilities=dcap)
    driver.get(url)
    time.sleep(3)
    sg_id = driver.find_elements_by_xpath("//span[@class='txt']/a/@href")
    for s_id in sg_id:
        print(s_id)
"""
    # url = "http://music.163.com/song/media/outer/url?id=28732740"
    for song_id in music_dict.keys():
        song_url = "http://music.163.com/song/media/outer/url?id=%s" % song_id
        # 下载的地址
        path = "D:\\Python_Projects\\Pycharm_Projects\\gui_plus_spider\%s.mp3" % music_dict[song_id]
        # 添加数据
        lst.insert(END, "正在下载: %s" % music_dict[song_id])
        # 文本框向下滚动
        lst.see(END)
        # 更新
        lst.update()
        # 下载地址 下载路径
        urlretrieve(song_url, path)
"""
# 搭建界面
root = tkinter.Tk()
root.title("网易云音乐抓取")
root.geometry("522x372+300+200")

label = tkinter.Label(root, text="请输入要抓取的网易云歌单URL(无#):", font=("华文行楷", 12), fg="black")
"""
# -anchor and -sticky
N='n' S='s' W='w' E='e'
NW='nw' SW='sw' NE='ne' SE='se'
NS='ns' EW='ew' NSEW='nsew' CENTER='center'
"""
# stickyness value must be a string containing n, e, s, and/or w
# gird 网格式 pack 包 place 位置
label.grid(row=0, column=0, sticky=W)

entry = tkinter.Entry(root, font=("微软雅黑", 20))
entry.grid(row=0, column=1, columnspan=3, sticky=E)

lst = tkinter.Listbox(root, font=("微软雅黑",15), width=43, height=10)
lst.grid(row=1, column=0, columnspan=3, sticky=W)

btn = tkinter.Button(root, text="开始下载", font=("微软雅黑", 15), command=get_music)
btn.grid(row=2, column=0, sticky=W)
btn2 = tkinter.Button(root, text="退出", font=("微软雅黑", 15), fg='red', command=quit)
btn2.grid(row=2, column=2, sticky=E)

root.mainloop()
