# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-16 21:15
program     : 使用unittest测试类方法；学会setUp方法
"""
import unittest
from employee_class import Employee

class EmployeeTestCase(unittest.TestCase):
    def setUp(self):
        first_name = 'Jahn'
        last_name = 'Warm'
        self.employee = Employee(last_name, first_name, 5000)

    def test_give_default_raise(self):
        self.employee.give_raise()
        self.assertEqual(self.employee.annual_salary, 10000)

    def test_give_custom_raise(self):
        self.employee.give_raise(4444)
        self.assertEqual(self.employee.annual_salary, 9444)

unittest.main()