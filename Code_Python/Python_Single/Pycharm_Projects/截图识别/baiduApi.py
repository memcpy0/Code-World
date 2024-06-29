# -*- coding:utf-8 -*-
# -*- topic:调用百度文字识别API -*-
# -*- arthor:张平   -*-

from aip import AipOcr
import configparser
import time

class BaiduAPI(object):
    def __init__(self, filepath):
        """你的APPID AK SK"""
        target = configparser.ConfigParser()
        target.read(filepath)
        app_id = target.get('工单密码', 'App_ID')
        api_key = target.get('工单密码', 'API_KEY')
        secret_key = target.get('工单密码', 'SECRET_KEY')
        #print(app_id, api_key, secret_key)
        """
        self.APP_ID = '11688486'
        self.API_KEY = 'EHsnhG7WBLRB4TyO60WyjV3O'
        self.SECRET_KEY = 'L7Ds0LFjBHkQD8gOaLFnyfLKxTrAC4Xz'
        """
        self.client = AipOcr(app_id, api_key, secret_key)

    def picture2Text(self, filepath):
        # 读取图片
        image = self.getPicture(filepath)
        texts = self.client.basicGeneral(image) # 通用文字识别

        allTexts = ''
        for word in texts['words_result']:
            allTexts = allTexts +word.get('words', '')
        return allTexts

    @staticmethod #
    def getPicture(filepath): # 不加self
        time.sleep(5)
        with open(filepath, 'rb') as fp:
            return fp.read()

baiduapi = BaiduAPI('password.ini')
text = baiduapi.picture2Text("Picture.jpg")
print(text)