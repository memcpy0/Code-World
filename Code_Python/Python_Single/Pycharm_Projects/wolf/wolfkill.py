# -*- coding:utf-8 -*-
# -*- topic:网页版狼人杀 -*-
# -*- arthor:张平   -*-

from tornado import web, ioloop, httpserver

# 业务处理模块 部门
class MainPageHandler(web.RequestHandler):
    def get(self, *args, **kwargs):
        #self.write('学习是积累的事情，一口吃不成个胖子')
        self.render("index.html")

    #def post(self, *args, **kwargs):
    #   pass

"""
class CreateDiary(web.RequestHandler):
    def get(self, *args, **kwargs):
        #self.write('欢迎来到Python世界')
        self.render("./create.html")
"""

# 配置
settings = {
    'template_path':'templates',  # 模板路径
    'static_path': 'statics'  # 静态文件路径
}

# 路由 分机系统
application = web.Application([
        (r'/index', MainPageHandler),
        #(r'/create', CreateDiary),
    ], **settings)

# http://localhost:8080/
# socket 套接字 系统 前台
if __name__ == "__main__":
    http_server = httpserver.HTTPServer(application)
    http_server.listen(8080)
    ioloop.IOLoop.current().start()