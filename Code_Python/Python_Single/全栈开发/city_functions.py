# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-16 20:31
program     : 学习《Python从入门到实践》十一章的函数测试
"""
def get_formatted_city_country(city, country, population=None):
    description = city + ', ' + country
    description = description.title()
    if population:
        description = description + ' - population ' + str(population)
    return description

if __name__ == '__main__':
    print(get_formatted_city_country('santiago', 'chile', 5000000))