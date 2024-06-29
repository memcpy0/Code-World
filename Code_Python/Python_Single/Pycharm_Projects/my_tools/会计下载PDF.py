# -*- coding:utf-8 -*-

import os
import requests
import bs4

r = requests.get("http://www.cninfo.com.cn/cninfo-new/disclosure/szse_sme/bulletin_detail/true/1204546410?announceTime=2018-03-30")

with open("C:/Users/dell/Desktop/ger",'wb') as f:
    f.write(r.content)
    f.close()


# http://www.cninfo.com.cn/information/companyinfo_n.html?fulltext?szsme002241
# http://www.cninfo.com.cn/information/companyinfo_n.html?fulltext?szsme002241

# http://www.cninfo.com.cn/cninfo-new/disclosure/szse_sme/bulletin_detail/true/1204546410?announceTime=2018-03-30
# http://www.cninfo.com.cn/cninfo-new/disclosure/szse_sme/download/1204639909?announceTime=2018-04-17


# http://www.cninfo.com.cn/cninfo-new/disclosure/szse_sme/bulletin_detail/true/1204639909?announceTime=2018-04-17