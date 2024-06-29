# Unit PYG04 : Pygame Event Print v1

import pygame , sys

pygame.init()
screen = pygame.display.set_mode((600,400))
pygame.display.set_caption("Pygame事件处理")

while True :
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()

        elif event.type == pygame.KEYDOWN:
            if event.unicode == "":
                print("#",event.key, event.mod)
            else :
                print(event.unicode, event.key, event.mod)

        elif event.type == pygame.MOUSEMOTION:
            print("[MOUSEMOTTION]:", event.pos, event.rel, event.buttons)
        elif event.type == pygame.MOUSEBUTTONUP:
            print("[MOUSEBOTTONUP]:", event.pos, event.botton)
        elif event.type == pygame.MOUSEBUTTONDOWN:
            print("[MOUSEBUTTONDOWN]:", event.pos, event.button)

    pygame.display.update()