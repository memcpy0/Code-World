# -*- coding:utf-8 -*-

"""
some interesting little programming practices
"""

def rep(old_string, replaced_string, res_string): # the replace
    return str(res_string).join(str(old_string).split(str(replaced_string)))

