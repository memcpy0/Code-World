# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-24 10:49
program     : 听了一道面试题
"""
# 一个类
# 对象的属性：姓名 性别 年龄 部门
# 员工管理系统
# 内部转岗 Python开发 Go开发
# 姓名 性别 年龄 新的部门
# alex None 83 Python
# alex None 85 Luffy

# 1000名员工
# 如果员工的姓名和性别相同，认为他们是同一人
class Employee:

    def __init__(self, name, sex, age, partment):
        self.name = name
        self.sex = sex
        self.age = age
        self.partment = partment

    def __eq__(self, other):
        if self.name == other.name and self.sex == other.sex:
            return True

    def __hash__(self):
        return hash("%s%s" % (self.name, self.sex))

employee_lst = []
for i in range(200):
    employee_lst.append(Employee('alex', 'male', i, 'Python'))
for i in range(200):
    employee_lst.append(Employee('fox', 'female', i, 'Python'))
for i in range(200):
    employee_lst.append(Employee('bob', 'male', i, 'Java'))
for i in range(200):
    employee_lst.append(Employee('john', 'female', i, 'Go'))
for i in range(200):
    employee_lst.append(Employee('hash', 'male', i, 'C++'))

# set集合的去重机制
for person in set(employee_lst):
    print(person.__dict__)


