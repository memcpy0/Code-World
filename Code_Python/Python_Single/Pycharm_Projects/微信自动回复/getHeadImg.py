# -*- coding:utf-8 -*-
import itchat

# 获取微信好友头像
itchat.auto_login(hotReload=True)

friends = itchat.get_friends()[0:]

for i in friends:
    img = itchat.get_head_img(userName=i['UserName'])
    path = "D:\\Python_Projects\\Pycharm_Projects\\微信自动回复\\" + i['NickName'] + '.jpg'
    # print(path)
    print("正在下载:%s的头像" % i['NickName'])
    try:
        with open(path, 'wb') as f:
            f.write(img)
            f.close()
    except Exception as e:
        print(repr(e))

itchat.run()
