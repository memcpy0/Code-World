# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-03 15:30
program     : 在函数中定义一次性类
"""

def format_string(string, formatter=None):
    '''Format a string using the formatter object,
    which is expected to have a format() method that accepts a string.'''
    class DefaultFormatter:
        '''Format a string in title case.'''
        def format(self, string):
            return str(string).title()

    if not formatter:
        formatter = DefaultFormatter()  # 没有提供格式化方法时，使用函数创建的默认的格式化方法
    return formatter.format(string)

hello_string = 'hello world, how are you today?'
print('Input: ' + hello_string)
print('Output: ' + format_string(hello_string))
# Input: hello world, how are you today?
# Output: Hello World, How Are You Today?