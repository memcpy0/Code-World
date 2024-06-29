# UNit PYG02 : Pygame Hello World Game
import pygame,sys       #引入pygame和sys

pygame.init()            #初始化init()及设置
screen = pygame.display.set_mode((600,400))    #初始化显示窗口
pygame.display.set_caption("Pygame游戏之旅")  #设置显示窗口的标题内容

while True:             #无限循环
    for event in pygame.event.get():   #从Pygame的事件队列中取出事件并从队列中删除该事件
        if event.type == pygame.QUIT:  #获取事件类型并逐类响应
            sys.exit()     #结束游戏并退出
    pygame.display.update()            #对显示窗口进行更新，默认窗口全部重绘