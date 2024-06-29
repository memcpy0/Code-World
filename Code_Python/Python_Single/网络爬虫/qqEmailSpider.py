# -*- coding:utf-8 -*-
# -*- topic:      -*- 
# -*- arthor:张平   -*-

import requests

class QQMail(object):
    def __init__(self):
        self.url = ''

    def __getitem__(self, item):
        return