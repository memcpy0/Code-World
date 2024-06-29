# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-17 23:40
program     : 学习抽象类和接口类
"""
from abc import ABCMeta, abstractmethod, abstractproperty

"""
Python没有接口这个概念
* 接口类, 抽象类: 制定一个规范, 可以强制执行
"""

class Payment(metaclass=ABCMeta):  # 抽象类(接口类) # 制定了一个规范 

    # 强制执行, 凡是继承我的类中都必须有pay方法, 如果没有, 实例化对象的时候会报错
    @abstractmethod
    def pay(self, money):
        pass

    @abstractproperty
    def text(self):
        pass


class AliPay(Payment):
    name = "支付宝"

    def __init__(self, money):
        self.money = money

    def pay(self, money):
        print("使用%s支付了%s" % (self.name, money))

class JdPay(Payment):
    name = "京东支付"

    def __init__(self, money):
        self.money = money

    def pay(self, money):
        print("使用%s支付了%s" % (self.name, money))

class WeChatPay(Payment):
    name = "微信支付"

    def __init__(self, money):
        self.money = money

    def pay(self, money):
        print("使用%s支付了%s" % (self.name, money))

# 归一化设计
def pay(obj, num):
    obj.pay(num)

# print(AliPay.__name__)
a1 = AliPay(2000)
j1 = JdPay(3000)
w1 = WeChatPay(4000)
pay(a1, 200)
pay(j1, 200)
pay(w1, 2000)

"""
Python没有多态，它处处都是多态
它有鸭子类型，只有通用方法，这些类就是鸭子类型
"""