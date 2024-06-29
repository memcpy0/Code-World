# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-11-18 22:47
program     : 刷课
"""  # 

import time
from selenium import webdriver
from selenium.webdriver.common.by import  By
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait

video_list = []

def login(**params):
    if "username" in params and "password" in params:
        driver.find_element_by_class_name("loginSub").click()
        for field, value in params.items():
            time.sleep(1)
            element = driver.find_element_by_id(field)
            element.clear()
            element.send_keys(value)
    else:
        raise Exception("需要登录账号和密码!")

    driver.find_element_by_class_name("btn-login").click()
    time.sleep(1)

def enterCourseHomePage(driver):
    # 登录表单在页面的框架中，所以要切换到该框架
    driver.switch_to_frame("frame_content")

    try:
        element = WebDriverWait(driver, 5).until(
            EC.presence_of_element_located(
                (By.LINK_TEXT, "毛泽东思想和中国特色...")
            )
        )
    except Exception as e:
        print(e)
    else:
        element.click()
    finally:
        print(driver.find_element_by_xpath("/html/body/div/div[2]/div[2]/ul/li[1]/div[2]/p[1]").text)


def enterFirstVideo(driver):

    element = WebDriverWait(driver, 10).until(
        EC.visibility_of_element_located(
            (By.CSS_SELECTOR, "div.units:nth-child(1) > div:nth-child(2) > h3:nth-child(1) > span:nth-child(2) > a:nth-child(1)")
        )
    )
    element.click()

def testVideoEnd():
    pass

if __name__ == "__main__":
    location = r"C:\Program Files\Mozilla Firefox\firefox.exe"
    driver = webdriver.Firefox(firefox_binary=location)

    url = "http://www.mooc.whu.edu.cn/portal"

    driver.get(url)
    login(username="2017302360010", password="143579")

    enterCourseHomePage(driver)
    enterFirstVideo(driver)
    # getCourseVideoList(driver)
    time.sleep(5)
    # ele = driver.find_elements_by_xpath('/html/body/div[6]/div[1]/div[2]/div[3]/div[1]/div[3]/h3/span[2]/a')
    #
    # print(ele[0].text)

    # source = driver.page_source
    # print(source)


    #获取当前窗口的句柄
    # currentWin = browser.current_window_handle
    # #跳转到另一个新页面
    # tm.sleep(3)
    # #获取所有窗口的句柄
    # handles = browser.window_handles

    # for i in handles:
    #     if currentWin == i:
    #         continue
    # else:
    #     #将driver与新的页面绑定起来
    #     browser.switch_to_window(i)

    # tm.sleep(5)
    # a1 = browser.find_elements_by_class_name('speedPromote_btn')

    # print(a1[0].get_attribute('textContent'))

    # #切换下一页使标签可见
    # for i in range(classNumber//2): #翻页次数
    #
    #     browser.find_element_by_id('course_recruit_studying_next').click()
    #     tm.sleep(1)

    # tm.sleep(1.5)

    # #打开相应课程
    # print("打开课程页面")
    # browser.find_elements_by_class_name('speedPromote_btn')[classNumber].click()
    # #切换标签页
    # currentWin = browser.current_window_handle
    # handles = browser.window_handles
    # print(handles)
    # for i in handles:
    #     if currentWin == i:
    #         continue
    # else:
    #     #将driver与新的页面绑定起来
    #     browser.switch_to_window(i)
    #     #关闭弹出的提示
    # tm.sleep(10)

    # try:
    #     print("尝试关闭课程弹出警告")
    #     element = WebDriverWait(browser,10).until(EC.presence_of_element_located((By.CLASS_NAME, "popbtn_yes")))
    #     element.click()
    #     print("关闭成功")
    # except:
    #     print("关闭失败,请重启脚本或手动关闭")
    # '''
    # 以上为打开课程观看页面

    # 以下代码开始获取视频列表与当前播放视频及时间
    # '''
    # tm.sleep(2)
    # time = []
    # video = browser.find_elements_by_class_name('video')
    # video_now = browser.find_element_by_class_name('current_play')
    # time_un = browser.find_elements_by_class_name('time')
    # v_now = video.index(video_now)

    # print("当前播放视频为：",v_now)
    # print("尝试获取所有视频的播放时间")
    # for i in time_un:
    #     i = i.get_attribute('textContent').split(':')
    #     time.append(int(i[1])*60+int(i[2]))

    # print("获取完毕")
    # print("-------------------")
    # print("开始观看视频")

    # #开始看课
    # for v in range(0,len(video)):
    #     per = browser.find_element_by_class_name('progressbar').get_attribute('style')
    #     per = int(re.findall(r'[1-9]\d*',per)[0])
    #     print("当前视频播放进度",per)
    #     if v < v_now:
    #         print('当前属于已播放视频',v+1)
    #         continue
    #     if per >= 80 and v == v_now:
    #         print("当前首个播放视频进度已超过80%")
    #         continue
    #     try:
    #         print("开始播放第",v+1,"个视频")
    #         video[v].click()
    #     except:
    #         print("下一个视频播放失败,请重试",v+1)
    #         continue

    #     #开始等待问题弹窗的出现
    #     try:
    #         print("等待弹窗出现,可能时间稍长请耐心等待,时间:/s",time[v])
    #         element = WebDriverWait(browser,time[v]).until(EC.presence_of_element_located((By.CLASS_NAME, "popboxes_close")))
    #         element.click()
    #     except:
    #         print("弹窗关闭失败或无弹窗")
    #     #等待进度达到100%
    #     while(1):
    #         per = browser.find_element_by_class_name('progressbar').get_attribute('style')
    #         per = int(re.findall(r'[1-9]\d*',per)[0])
    #         print(per,end='\r')
    #         if per >= 80:
    #             break
