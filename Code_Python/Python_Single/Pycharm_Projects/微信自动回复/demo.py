# -*- coding:utf-8 -*-
import itchat
# 统计男女性别比例

# 登录微信
# itchat.login()
"""
Help on method auto_login in module itchat.components.register:
- auto_login(hotReload=False, statusStorageDir='itchat.pkl', enableCmdQR=False, 
- picDir=None, qrCallback=None, loginCallback=None, exitCallback=None) 
- method of itchat.core.Core instance
"""
itchat.auto_login(hotReload=True) # 10分钟有效性

# 获取微信好友列表（每一位好友均属字典类型）
friends = itchat.get_friends()[1:] # 排除自己

# 微信好友总数
male = 0
female = 0
other = 0
for f in friends:
    sex = f['Sex']
    if sex == 1:
        female += 1
    elif sex == 2:
        male += 1
    else:
        other += 1
total = len(friends[1:])

# 统计好友性别
print("男性好友:%.2f%%, 女性好友:%.2f%%, 其他:%.2f%%" % (male/total, female/total, other/total))
