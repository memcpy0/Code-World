# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-16 20:31
program     : 使用unittest测试函数
"""
import unittest
from city_functions import get_formatted_city_country

class CityTestCase(unittest.TestCase):

    def test_city_country(self):
        city_country = get_formatted_city_country('santiago', 'chile')
        self.assertEqual(city_country, 'Santiago, Chile')

    def test_city_country_population(self):
        city_country_population = get_formatted_city_country('santiago', 'chile', population=5000000)
        self.assertEqual(city_country_population, 'Santiago, Chile - population 5000000')

unittest.main()