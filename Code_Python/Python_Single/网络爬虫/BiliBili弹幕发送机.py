# -*- coding:utf-8 -*-
# -*- topic: 弹幕发送机 2018 8 16 -*-
# -*- arthor:张平  -*-
import requests
import random
import time
import sys

class SendLiveRoll():
    def __init__(self, roomid=None):
        # 接受弹幕的网址
        self.url = 'https://api.live.bilibili.com/ajax/msg'
        self.url2 = 'https://api.live.bilibili.com/msg/send'
        self.roomid = roomid
        # post的数据
        self.headers = {
            "User-Agent" : "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"
        }
        self.cookies = {
        'Cookie':'l=v; finger=964b42c0; buvid3=46120021-235F-40B4-BB9E-EA85E1201D70163036infoc; LIVE_BUVID=AUTO4515343870741080; fts=1534387130; sid=abfuhrc4; DedeUserID=36309171; DedeUserID__ckMd5=05e671717d6eb7d1; SESSDATA=6295ce14%2C1536979150%2Cd0aadba8; bili_jct=984dce353b606d256abd3bd9b68b7171; rpdid=lmxomqiisdoskpkqqoxw; Hm_lvt_8a6e55dbd2870f0f5bc9194cddf32a02=1534388382,1534388473; Hm_lpvt_8a6e55dbd2870f0f5bc9194cddf32a02=1534407335; _dfcaptcha=70098216b5dde5fdb116e4c517788faf; UM_distinctid=16541c062800-0fdeda6b3a8aed-4c312b7b-144000-16541c0628164; bp_t_offset_36309171=152761743223719227'
        }
        self.data = {
            "csrf_token" : "984dce353b606d256abd3bd9b68b7171",
            "roomid" : "5004",
            "visit_id" : "769g15fqcdmo"
        }

    def getLiveRoll(self):
        '''获取弹幕'''
        r = requests.post(self.url, data=self.data)
        json_response = r.json()
        danmus = list(map(lambda i:json_response['data']['room'][i]['text'], range(10)))
        self.msg = random.choice(danmus)
        print(self.msg)

    def sendLiveRoll(self):
        '''发送弹幕'''
        data = {
            "color" : 16777215,
            "csrf_token" : "984dce353b606d256abd3bd9b68b7171",
            "fontsize" : 25,
            "mode" : 1,
            "msg" : self.msg,
            "rnd" : 1534409765,
            "roomid" : self.roomid
        }
        r = requests.post(self.url2, headers=self.headers, data=data, cookies=self.cookies)
        print(r)

if __name__ == "__main__":
    rmid = input("请输入roomid:")
    sendliveroll = SendLiveRoll(rmid)

    for _ in range(10): # sys.maxsize
        sendliveroll.getLiveRoll()
        sendliveroll.sendLiveRoll()
        time.sleep(random.randint(1, 4))