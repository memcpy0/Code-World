# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-15 10:33
program     : 打开并一键连接到校园网
"""
# import pyperclip
# import re, string
# import time, datetime
# import json, csv
# import requests, urllib.request
# from lxml import etree
# from bs4 import BeautifulSoup
# import os, glob, sys, subprocess, threading
# from multiprocessing.dummy import Pool
# from PIL import Image
# import PyPDF2, reportlab, csv, json
# import smtplip, imapclient, pyzmail
# from collections import *
import webbrowser
import requests, re

def surfOnline(url):
    webbrowser.open(url)

def loginSchoolSite(url, account='', password=''):
    if not (account or password): surfOnline(url)

"""
{"userIndex":"30376533386632333233663333306364356666636333613230336136333130305f31302e3133322e3136352e3232315f32303137333032333630303130",

"result":"success","message":"获取用户信息成功","keepaliveInterval":null,"maxLeavingTime":"33 天 13 小时 29 分钟 37 秒",
"maxFlow":null,"userName":"张平","userId":"2017302360010","password":null,"userIp":"10.132.165.221",
"userMac":"aa6b3612710a","webGatePort":null,"webGateIp":null,"service":"校园网",
"realServiceName":"Internet","apMac":null,"vlanId":null,"ssid":null,"accountInfo":null,
"loginType":"3","utrustUrl":"","userUrl":"http://www.msftconnecttest.com/redirect",
"pubMessage":null,"userGroup":"学生组","accountFee":"22.58","wlanAcName":null,
"hasMabInfo":true,"isAlowMab":true,"userPackage":"学生计天",
"selfUrl":"http://172.19.1.10:8080/selfservice//module/scgroup/web/login_judge.jsf?channel=cG9ydGFs&name=6653190520f496115e0363861cc0f792
&password=f65b8cb19f08a12b&ip=172.19.1.9&callBack=loginself",

"ballInfo":"[{\"alertContent\":\"\",\"alertContentMobile\":\"\",\"alertUrl\":\"\",
\"displayName\":\"套餐&余额\",\"displayNameOnMoveIn\":\"\",\"id\":\"packageandfee\",\"jsonValue\":\"\",
\"needAlert\":\"false\",\"type\":\"money\",\"typeOnMoveIn\":\"\",

\"url\":\"http://172.19.1.10:8080/selfservice//module/userself/web/portal_business_detail.jsf?channel=

cG9ydGFs&name=6653190520f496115e0363861cc0f792&password=f65b8cb19f08a12b&userIp=10.132.165.221&ip=172.19.1.9&callBack=portal_business_detail&index=1\",
\"value\":\"22.58\",\"valueOnMoveIn\":\"\"},{\"alertContent\":\"\",\"alertContentMobile\":\"\",

\"alertUrl\":\"\",\"displayName\":\"剩余天数\",

\"displayNameOnMoveIn\":\"剩余天数\",\"id\":\"flow\",\"jsonValue\":\"\",

\"needAlert\":\"false\",\"type\":\"leftday\",\"typeOnMoveIn\":\"leftday\",

\"url\":\"http://172.19.1.10:8080/selfservice//module/userself/web/portal_business_detail.jsf?channel=cG9ydGFs&name=6653190520f496115e0363861cc0f792&password=f65b8cb19f08a12b&userIp=10.132.165.221&ip=172.19.1.9&callBack=due_time_chart_of_month\",

\"value\":\"34\",\"valueOnMoveIn\":\"34\"},{\"alertContent\":\"\",\"alertContentMobile\":\"\",\"alertUrl\":\"\",\"displayName\":\"在线设备\",\"displayNameOnMoveIn\":\"\",\"id\":\"onlinedevice\",\"jsonValue\":\"\",\"needAlert\":\"false\",\"type\":\"deviceNum\",\"typeOnMoveIn\":\"\",\"url\":\"http://172.19.1.10:8080/selfservice//module/userself/web/portal_business_detail.jsf?channel=cG9ydGFs&name=6653190520f496115e0363861cc0f792&password=f65b8cb19f08a12b&userIp=10.132.165.221&ip=172.19.1.9&callBack=portal_business_detail&index=3\",\"value\":\"1\",\"valueOnMoveIn\":\"\"}]","notify":"[]","resource":"[]","offlineurl":"","redirectUrl":"http://123.123.123.123/","announcement":"<a href=\"http://user-serv.whu.edu.cn:8080/selfservice\" target=\"_blank\"><span style=\"FONT-SIZE: 24px\"><span microsoft=\"\" yahei=\"\" background-color:=\"\" rgb=\"\" 51=\"\" 102=\"\" 255=\"\"><span style=\"color:#000000;\"><span style=\"BACKGROUND-COLOR: rgb(255,255,255)\"><strong>网络自助</strong></span></span></span></span></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <a href=\"http://10.100.0.33:8001/\"><span style=\"font-size:24px;color:#000000;\"><strong>新用户注册</strong></span></a>","userNotice":"","systemNotice":"","portalIp":"172.19.1.9","portalUrl":"","rghallUrl":"","welcomeTip":"早上好","serviceList":"<div style='display:none' id='selectDisname'>校园网</div><li id='bch_service_0'onclick='selectService(\"dianxin\",\"电信\",\"internet\",\"0\")'onmouseover='overLi(this,2)' onmouseout='outLi(this,2)'><a href='#' id='_service_0'>电信</a></li> <li  style='background-color:#4285ca;'id='bch_service_1'onclick='selectService(\"Internet\",\"校园网\",\"internet\",\"1\")'onmouseover='overLi(this,1)' onmouseout='outLi(this,1)'><a href='#' id='_service_1' style='color:#4285ca;background-color:#FFF;'>校园网</a></li>
 
 <li id='bch_service_2'onclick='selectService(\"yidong\",\"移动\",\"internet\",\"2\")'onmouseover='overLi(this,2)' onmouseout='outLi(this,2)'><a href='#' id='_service_2' style='border:1px solid #D2D2D2;border-width:1px 1px 1px 1px;'>移动</a></li> <div style='display:none' id='serviceCountId'>3</div>","isSuccessService":"true","isCloseWinAllowed":"true","checkUserLogout":"1","samEdition":"enterprise","isFaq":"false","pcClient":"false","pcClientUrl":"","phoneClient":"false","isAutoLogin":"false","domianName":"false","netFlowKey":null,"errorflowurl":"http://172.19.1.10:8080/selfservice//module/userself/web/portal_business_detail.jsf?channel=cG9ydGFs&name=6653190520f496115e0363861cc0f792&password=f65b8cb19f08a12b&userIp=10.132.165.221&ip=172.19.1.9&callBack=show_error_flow_detail&netflowkey=","isErrorMsg":"false","successUrl":"[]","mabInfo":"[{\"client_hostname\":\"\",\"createHost\":\"172.19.1.1\",\"createTime\":\"2018-09-15\",\"deviceName\":\"\",\"deviceType\":\"电脑\",\"fgr\":\"\",\"lastUpdateHost\":\"172.19.1.1\",\"lastUpdateTime\":\"2018-09-15\",\"mabRegister\":\"system\",\"macExpireTime\":\"2018-09-15\",\"password\":\"f65b8cb19f08a12b\",\"showMacExpireTime\":\"2018-09-15\",

\"systemType\":\"Windows 7\",\"userId\":\"2017302360010\",
\"userMabInfoUuid\":\"4028168165d461a80165db109b38247c\",\"userMac\":\"AA6B3612710A\"}]","mabInfoMaxCount":3}
"""



