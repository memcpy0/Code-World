# -*- coding:utf-8 -*-
"""
>>> dir(pywifi)
['Profile', 'PyWiFi', '__builtins__', '__cached__', '__doc__', '__file__', '__loader__',
'__name__', '__package__', '__path__', '__spec__',
'_wifiutil_win', 'const', 'iface', 'logging', 'profile', 'set_loglevel', 'wifi']
"""
import pywifi
from pywifi import const # 引用一些常量

# 判断是否已经连接到WiFi
def gic():
    # 创建一个无线对象
    wifi = pywifi.PyWiFi()
    # 获取到第一个无线网卡 # interfaces()返回列表list object
    ifaces = wifi.interfaces()[0] # [<pywifi.iface.Interface object at 0x000001C1A5364A58>]

    # print(ifaces)
    # 打印网卡的名称
    # print(ifaces.name())

    # 打印网卡的连接状态 未连接到WiFi环境 0 连接到 4
    # print(ifaces.status())
    # IFACE_CONNECTED 连接状态 connect 连接
    if ifaces.status() == const.IFACE_CONNECTED:
        """ 
        # Define interface status.
        IFACE_DISCONNECTED = 0
        IFACE_SCANNING = 1
        IFACE_INACTIVE = 2
        IFACE_CONNECTING = 3
        IFACE_CONNECTED = 4
        """
        print("已连接")
    else:
        print("未连接")

# 扫描周围的WiFi，都有什么无线网卡
def bies():
    wifi = pywifi.PyWiFi() # PyWiFi对象
    ifaces = wifi.interfaces()[0] # 一般笔记本只有一个网卡
    # 扫描WiFi
    ifaces.scan()
    # 获取扫描结果
    result = ifaces.scan_results()
    # print(result)
    for name in result:
        # 打印扫描结果和ssid WiFi的名称
        print(name, name.ssid)
bies()