import web

urls = ('/', 'index')

app  = web.application(urls, globals())

render = web.template.render('D:/projects/gothonweb/templates/index.html')

class index(object):
    def GET(self):
        greeting  = "Hello World!"
        return render.index(greeting = greeting)
        
if __name__ == "__main__":
    app.run()
    