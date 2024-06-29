# -*- coding:utf-8 -*-
from selenium import webdriver
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities # 给phantomjs模拟一个头部
import time

url = "https://music.163.com/playlist?id=434809001"
dcap = dict(DesiredCapabilities.PHANTOMJS)
dcap["phantomjs.page.settings.userAgent"] = (
    "Mozilla/5.0(Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36(KHTML, like Gecko) Chrome/56.0.2924.90 Mobile Safari/537.36"
    )
driver = webdriver.PhantomJS(executable_path="D:/Python_Projects/Pycharm_Projects/phantomjs-2.1.1-windows/bin/phantomjs.exe", desired_capabilities=dcap)
driver.get(url)
print(driver)
time.sleep(1)

sg_id = driver.find_elements_by_xpath("//*[@id='5495021061533463180297']/td[2]/div/div/div/span/a/@href")
print(sg_id)