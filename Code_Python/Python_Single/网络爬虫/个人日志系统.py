# -*- coding:utf-8 -*-
# -*- arthor:张平  -*-
from tornado import web, ioloop, httpserver

# 开发个人日志系统：
# 业务处理模块
class MainPageHandler(web.RequestHandler):
    def get(self, *args, **kwargs):
        self.write('欢迎来到Python世界')
        self.render("./index.html")

class CreateDiary(web.RequestHandler):
    def get(self, *args, **kwargs):
        #self.write('欢迎来到Python世界')
        self.render("./create.html")
# 路由
application = web.Application([
        (r'/index', MainPageHandler),
        (r'/create', CreateDiary),
    ])

# socket 套接字 系统
if __name__ == "__main__":
    http_server = httpserver.HTTPServer(application)
    http_server.listen(63342)
    ioloop.IOLoop.current().start()