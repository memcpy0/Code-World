# -*- coding:utf-8 -*-
from selenium import webdriver
import time
from selenium.webdriver.remote.webelement import WebElement
from selenium.common.exceptions import StaleElementReferenceException

def waitForload(driver):
    elem = driver.find_element_by_tag_name("html")
    count = 0
    while True:
        count += 1
        if count > 20:
            print("Timing out after 10 seconds and returning")
            return
        time.sleep(.5)
        try:
            elem == driver.find_element_by_tag_name("html")
        except StaleElementReferenceException:
            return

driver = webdriver.PhantomJS(executable_path="D:/Python_Projects/Pycharm_Projects/phantomjs/bin/phantomjs.exe")
driver.get("http://pythonscraping.com/pages/javascript/redirectDemo2.html")
waitForload(driver)
print(driver.page_source)

