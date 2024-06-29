# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-16 21:10
program     : 学习《Python从入门到实践》十一章的类方法测试
"""
class Employee:
    def __init__(self, last_name, first_name, annual_salary):
        self.last_name = last_name
        self.first_name = first_name
        self.annual_salary = annual_salary

    def give_raise(self, got_raise=5000):
        self.annual_salary += got_raise



