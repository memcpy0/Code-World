# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-17 10:17
program     : 存储alien_invasion的设置
"""
class Settings():
    """存储《外星人入侵》的所有设置的类"""
    def __init__(self):
        """初始化游戏的设置"""
        # 屏幕设置
        self.screen_width = 1200
        self.screen_height = 700
        self.bg_color = (230, 230, 230)  # 浅灰色
        self.ship_speed_factor = 1.5     # 飞船的速度
