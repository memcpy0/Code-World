# 复习Python的第四十六天
## 习题51：从浏览器中获得输入
本节我们将改进程序，让用户可以使用(form)表单来向网页程序提交文本，并将用户的相关信息保存到他们的会话(Session)中。

[TOC]

### Web的工作原理
下图展示了web的HTTP请(Request)求的各个不同部分，以及信息传递的大致流程：
![](https://img-blog.csdn.net/20180419132823250?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

1. 你在浏览器中输入网址  http://learnpythonthehardway.org/ ，然后浏览器会通过你的电脑的网络设备发出Request（线路 A）。
2. 你的Request被传送到互联网（线路 B），然后再抵达远端服务器（线路 C），然后我的服务器将接受这个Request。
3. 作者的服务器接受Request 后，作者的Web应用程序就去处理这个请求（线路 D），然后我的Python代码就会去运行index.GET这个“处理程序(handler)”。
4. 在代码return的时候，作者的Python服务器就会发出响应(Response)，这个响应
会再通过线路D传递到你的浏览器。
5. 这个网站所在的服务器将响应由线路D获取，然后通过线路C传至互联网。
6. 响应通过互联网由线路B传至你的计算机，计算机的网卡再通过线路A将响应传给你的浏览器。
7. 最后，你的浏览器显示了这个响应的内容。

### 相关术语
作者的解释很棒，仔细看一下。

- **浏览器(browser)**：它 的作用是接收你输入到地址栏网址 (例如http://learnpythonthehardway.org)，然后使用该信息向该网址对应的服务器提出求(request)。

- **地址(address) / URL(Uniform Resource Locator 统一资源定位器)**：
http指出了你要使用的协议(protocol)，这里用的是超文本传输协议(Hyper-Text Transport Protocol)，除此之外，还有FTP文件传输协议(File Transport Protocol)。
`learnpythonthehardway.org`这部分则是“主机名(hostname)”，也就是一个便于人阅读和记忆的字串，主机名会被匹配到一串叫作“IP 地址”的数字上面，这个“IP 地址”就相当于网络中一台计算机的电话号码，通过这个号码可以访问到这台计算机。
URL中还可以尾随一个“路径”，如`http://learnpythonthehardway.org/book/`中的`/book/`，它对应的是服务器上的某个文件或者某些资源，通过访问这样的网址，你可以向服务器发出请求，然后获得这些资源。
网站地址还有很多别的组成部分，不过这些是最主要的。

- **连接(connection)**
一旦浏览器知道了协议(http)、服务器(learnpythonthehardway.org)、以及要获得的资源，它就要去创建一个连接。
这个过程中，浏览器让操作系统(Operating System即OS)打开计算机的一个“`端口(port)`”（通常是80端口）。
端口准备好以后，操作系统会回传给你的程序一个类似文件的东西，它所做的就是通过网络传输和接收数据，让你的计算机和`learnpythonthehardway.org`这个网站所属的服务器之间实现数据交流。 
当你使用`http://localhost:8080/`访问你自己的站点时，发生的事情其实是一样的，只不过这次你告诉了浏览器要访问的是你自己的计算机(localhost)，要使用 的端口不是默认的 80，而是8080。 
你还可以直接访问`http://learnpythonthehardway.org:80/`， 这和不输入端口效果一样，因为HTTP的默认端口本来就是 80。

- **请求(request)**
你的浏览器通过你提供的地址建立了连接，现在它需要从远端服务器要到它（或你）想要的资源。
如果你在URL的结尾加了`/book/`，那你想要的就是`/book/`对应的文件或资源，大部分的服务器会直接为你调用`/book/index.html`这个文件，不过我们就假装不存在好了。
浏览器为了获得服务器上的资源，它需要向服务器发送一个“请求”。这里我就不讲细节了，为了得到服务器上的内容，你必须先向服务器发送一个请求才行。
有意思的是，“资源”不一定非要是文件。例如当浏览器向你的应用程序提出请求的时候，服务器返回的其实是你的Python代码生成的一些东西。

- **服务器(server)**
服务器指的是浏览器另一端连接的计算机，它知道如何回应浏览器请求的文件和资源。大部分的web服务器只要发送文件就可以了，这也是服务器流量的主要部分。
不过你学的是使用Python组建一个服务器，这个服务器知道如何接受请求，然后返回用Python处理过的字符串。
当你使用这种处理方式时，你其实是假装把文件发给了浏览器，其实你用的都只是代码而已。就像你在《习题 50》中看到的，要构建一个“响应”其实也不需要多少代码。

- **响应(response)**
这就是你的服务器回复你的请求，发回至浏览器的HTML，它里边可能有`css、javascript、或者图像`等内容。
以文件响应为例，服务器只要从磁盘读取文件，发送给浏览器就可以了，不过它还要将这些内容包在一个特别定义的“头部信息(header)”中，这样浏览器就会知道它获取的是什么类型的内容。
以你的 web 应用程序为例，你发送的其实还是一样的东西，包括header也一样，只不过这些数据是你用Python代码即时生成的。

> 有一个很好的方法让你更清楚，就是对照着上面的图示，将你在《习题 50》中创建的 web 程序中的内容分成几个部分，让其中的各部分对应到上面的图示。如果你可以正确地将程序的各部分对应到这个图示，你就大致开始明白它的工作原理了。

### 表单(form)的工作原理
```python
import web
urls = (
'/hello', 'Index'
)
app = web.application(urls, globals())
render = web.template.render('templates/')
class Index(object):
def GET(self):
form = web.input(name="Nobody")
greeting = "Hello, %s" % form.name
return render.index(greeting = greeting)
if __name__ == "__main__":
app.run()
```
还有后续，暂时不做了。