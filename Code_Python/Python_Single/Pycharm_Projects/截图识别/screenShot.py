# -*- coding:utf-8 -*-
# -*- topic:截图文字识别 -*-
# -*- arthor:张平   -*-
#import win32clipboard as w
from PIL import ImageGrab
import keyboard
from time import sleep
#import sys

#from baiduApi import BaiduAPI
#from getText import GetText
"""
DESCRIPTION
    Take full control of your keyboard with this small Python library. 
    Hook global events, register hotkeys, simulate key presses and much more.
"""

def screenShot():
    """用于截图并保存"""
    # 截图开始
    if not keyboard.wait(hotkey='ctrl+f1'):
        # 截图结束的条件
        #if not keyboard.wait(hotkey='ctrl+c'):
        """
        * Blocks the program execution until the given hotkey is pressed or (if语句, 默认返回None)
        * If given no parameters, blocks forever. (else语句, 永远锁住)
        """
        sleep(1)       # 防止获得的是上一张截图
        im = ImageGrab.grabclipboard()
        im.save("Picture.jpg")  # 复制剪切板里的图片

if __name__ == "__main__":
    #baiduapi = BaiduAPI('password.ini')
    #for _ in range(sys.maxsize):
    while True:
        screenShot()
        #texts = baiduapi.picture2Text("Picture.jpg")
        #GetText.setText(texts)
        #GetText.getText()

