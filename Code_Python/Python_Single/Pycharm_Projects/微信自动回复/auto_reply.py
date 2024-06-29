# -*- coding:utf-8 -*-
import itchat
import time
import requests

# 登录微信
itchat.auto_login(hotReload=True)

# 获取微信好友发的消息，根据发的消息回复
api_url = "http://www.tuling123.com/openapi/api"

def get_info(message):
    data = {
        'key' : 'e7858df3bfd145a282e5fe87b43f4260',
        'info' : message,
        'userid' : 'robot'
    }
    try:
        r = requests.post(api_url, data=data).json()
        info = r['text']
        print("robot reply:%s" % info)
        return info
    except:
        return

# 回复给微信好友
@itchat.msg_register(itchat.content.TEXT)
def auto_reply(msg):
    defaultReply = "我知道了"
    # 搜索微信好友
    real_friend = itchat.search_friends(name='许冕')
    real_friend_name = real_friend[0]['UserName']  # 真正标识符
    # print(real_friend_name)

    # 打印好友回复信息
    print("message:%s" % msg['Text'])

    # 调用图灵接口
    reply = get_info(msg['Text'])
    if msg['FromUserName'] == real_friend_name:
        itchat.send(reply, toUserName=real_friend_name)
itchat.run()

