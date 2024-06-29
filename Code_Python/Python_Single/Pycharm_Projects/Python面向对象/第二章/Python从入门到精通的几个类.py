# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-16 16:02
program     : 餐馆、冰淇淋店 用户、管理员、权限
"""
class Restaurant:
    def __init__(self, restaurant_name, cuisine_type, number_served):
        self.restaurant_name = restaurant_name
        self.cuisine_type = cuisine_type
        self.number_served = number_served

    def describe_restaurant(self):
        print("This restaurant is %s and has cuisine type: %s" % (self.restaurant_name, self.cuisine_type))

    def open_restaurant(self):
        print("This restaurant %s is opening!" % self.restaurant_name)

    def set_number_served(self, value):
        self.number_served = value

    def increment_number_served(self, value):
        if value > self.number_served:
            self.number_served = value
        else:
            print("The number of people who eat at this restaurant can't be rolled back")


class IceCreamStand(Restaurant):
    def __init__(self, restaurant_name, cuisine_type, number_served, flavors):
        # Restaurant.__init__(self, restaurant_name, cuisine_type, number_served)
        super().__init__(restaurant_name, cuisine_type, number_served)
        self.flavors = flavors

    def show_favors(self):
        for flavor in self.flavors:
            print(flavor+' ice-cream')

flavors = ['apple', 'peach', 'pear']
ice_cream_stand = IceCreamStand('Alice"s', 'Ice-Cream', 200, flavors)
print(ice_cream_stand.restaurant_name, ice_cream_stand.cuisine_type, ice_cream_stand.number_served)
ice_cream_stand.show_favors()

#------------------------------------------------------------------------
class User:
    def __init__(self, first_name, last_name, login_attempts, **kwargs):
        self.first_name = first_name
        self.login_attempts = login_attempts
        self.last_name = last_name
        if kwargs:
            self.args = kwargs

    def describe_user(self):
        name = self.last_name+' '+self.first_name
        print("This user is "+name.title()+'.')
        for k, v in self.args.items():
            print(k.ljust(8), '=>', v)

    def greet_user(self):
        name = self.last_name + ' ' + self.first_name
        print('Hello,', name.title())

    def increment_login_attempts(self):
        self.login_attempts = self.login_attempts + 1

    def reset_login_attempts(self):
        self.login_attempts = 0

# us = User('qi', 'zhang', 3, age=22, height=177, weight=122)
# us.greet_user()
# us.describe_user()
# us.increment_login_attempts()
# print(us.login_attempts)
# us.reset_login_attempts()
# print(us.login_attempts)


class Privileges:
    def __init__(self, privileges):
        self.privileges = privileges

    def show_privileges(self):
        print("The Admin can do:")
        for privilege in self.privileges:
            print("=> ", privilege)

class Admin(User):
    def __init__(self, first_name, last_name, login_attempts, privileges, **kwargs):
        super().__init__(first_name, last_name, login_attempts, **kwargs)
        self.privileges = Privileges(privileges)

# privileges = [ "can add post", "can delete post", "can ban user"]
# admin = Admin('qi', 'zhang', 3, privileges)
# admin.privileges.show_privileges()

#-----------------------------------------------------------------------------------------
from random import randint

class Die:
    def __init__(self, sides=6):
        self.sides = sides

    def roll_die(self):
        roll_num = randint(1, self.sides)
        print("您扔出了%d点的骰子。" % roll_num)

# die = Die()
# for i in range(10):
#     die.roll_die()
# print('\n')
# die10 = Die(10)
# for i in range(10):
#     die10.roll_die()
# print('\n')
# die20 = Die(20)
# for i in range(10):
#     die20.roll_die()