# -*- coding: utf-8 -*-
import time
import json
import random
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

CSDN_Account = "myRealization"
CSDN_Password = "!wdcsdnmm123"
# BlogAddr = "https://blog.csdn.net/myRealization?spm=1000.2115.3001.5343"
# driver = webdriver.Chrome(executable_path = "chromedriver.exe")
# driver.get(BlogAddr)

# 输入账号密码,登录CSDN
def loginCSDN(driver):
    driver.get("https://passport.csdn.net/account/login")
    time.sleep(2)
    driver.find_element_by_xpath('//*[@id="app"]/div/div/div[1]/div[2]/div[5]/ul/li[2]/a').click()
    time.sleep(2)
    # 切换到账号密码输入界面
    driver.find_element_by_xpath('//*[@id="all"]').clear() # 手机号/邮箱/用户名
    driver.find_element_by_xpath('//*[@id="password-number"]').clear() # 密码
    print("账号密码框清空完毕")
    driver.find_element_by_xpath('//*[@id="all"]').send_keys(CSDN_Account) # 填入用户名
    driver.find_element_by_xpath('//*[@id="password-number"]').send_keys(CSDN_Password) # 填入密码
    time.sleep(2) # 必须等待
    driver.find_element_by_xpath('//*[@id="app"]/div/div/div[1]/div[2]/div[5]/div/div[6]/div/button').click() # 点击登录
    print("登录CSDN")
    time.sleep(3) # 必须等待
    cookies = driver.get_cookies() # 获取并保存Cookies
    with open("cookies.data", "w") as f: 
        json.dump(cookies, f) # 将dict序列化为str格式, 存入文件

# 进入自己的博客
def getIntoSelfBlog(driver):
    driver.find_element_by_xpath('//*[@id="csdn-toolbar"]/div/div/div[3]/div/div[1]/a').click() # 点击博主博客页面的链接
    print("进入自己的博客")
    time.sleep(2)

# 进入下一博客文章列表页
def getIntoNextPage(driver):
    pass

def findArticles(): 
    articles = driver.find_elements_by_xpath('//*[@id="articleMeList-blog"]/div[2]/div/h4/a')
    for article in articles:
        article.click()

if __name__ == '__main__':
    driver = webdriver.Chrome(executable_path = "chromedriver.exe")
    loginCSDN(driver)
    getIntoSelfBlog(driver)
#    findArticles(driver)
    driver.find_element_by_xpath('/html/body/div[6]/main/div[2]/div[2]/div[1]/h4').click()
    time.sleep(2)
 
#    html = driver.execute_script("return document.documentElement.outerHTML") # 获取整个文档的HTML
#    with open("n.txt", "w", encoding='utf-8') as f:
#        f.write(html)


    tmp = driver.find_element_by_xpath('//*[@id="is-like-imgactive-animation-like"]')
    if tmp.text == "点赞":
        print("已赞")
    tmp.click()
    # driver.find_element_by_xpath('//*[@id="is-like"]').click()
    driver.close()
    
#with open("cookies.data", "r") as f:
#    cookies = json.load(f)
#    for cookie in cookies:
#        driver.add_cookie(cookie)
# time.sleep(2)
# driver.close()
# driver.get("https://blog.csdn.net/qq_38251616/article/details/82963395")
# time.sleep(3)
# driver.find_element_by_xpath("//button[@class=' low-height hover-box btn-like ']").click()
# print("点赞完成！")
# time.sleep(3)
