# Unit PYG04 : Pygame Event Post
import pygame,sys

pygame.init() # 初始化
screen = pygame.display.set_mode((600,400)) # 设置窗口展示大小
pygame.display.set_caption("Pygame事件处理")  # 设置窗口图标
fps = 1
fclock = pygame.time.Clock()
num = 1

while True:
    uevent = pygame.event.Event(pygame.KEYDOWN, {"unicode":123, "key":pygame.K_SPACE, "mod":pygame.KMOD_ALT})
    pygame.event.post(uevent)
    num = num +1
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            if event.unicode == "":
                print("[KEYDOWN {}]:".format(num),"#", event.key, event.mod)
            else:
                print("[KEYDOWN{}]:".format(num), event.unicode, event.key, event.mod)

    pygame.display.update()
    fclock.tick(fps)
