# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-17 20:09
program     : 学习类的组合
"""
class GameRole:
    def __init__(self, hero_name, atk, hp):
        self.hero_name = hero_name
        self.atk = atk
        self.hp = hp

    def attack(self, another):
        another.hp -= self.atk
        print("%s攻击了%s, %s掉了%s点血， 还剩%s点血" %
              (self.hero_name, another.hero_name, another.hero_name, self.atk, another.hp))

    def arm_weapon(self, weapon):
        self.weapon = weapon

# person1 = GameRole('盖伦', 20, 500)
# person2 = GameRole('亚索', 50, 200)
# person1.attack(person2)
# print(person2.hp)

"""
类之间的关系不仅有继承和覆盖，还有组合

组合：给一个类的实例对象封装一个属性，该属性是另一个类的实例对象，
从而让对象和对象之间产生关系
如：
class Admin:
    def __init__(self, privileges):
        self.privileges = Privileges(privileges)

class Privileges:
    def __init__(self, privileges):
        self.privileges = privileges
"""
# 版本1: 应该是人物利用武器打别人，而不是武器，攻击行为发起者不合理
class Weapon:
    def __init__(self, weapon_name, atk):
        self.weapon_name = weapon_name
        self.atk = atk

    def fight(self, hero1, hero2):
        hero2.hp = hero2.hp - self.atk
        print("{0}用{1}打了{2}, {2}掉了{3}血, 还剩{4}血".format(hero1.hero_name, self.weapon_name, hero2.hero_name, self.atk, hero2.hp))

axe = Weapon('三板斧', 60)
broadsword = Weapon('屠龙宝刀', 100)

person3 = GameRole('泰坦', 200, 500)
person4 = GameRole('阿赖耶', 500, 200)

# axe.fight(person3, person4)
person3.arm_weapon(axe)
print(person3.weapon)
person3.weapon.fight(person3, person4)  # 还是有点不合理