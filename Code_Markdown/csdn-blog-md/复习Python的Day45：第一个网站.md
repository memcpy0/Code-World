# 复习Python的第四十五天

## 习题50：你的第一个网站
在创建第一个网页应用程序之前，我们需要安装一个“Web 框架”，它的名字
叫`lpthw.web `。尽管好的web框架很多，如Django等，但是在这里我们根据作者所说的去做就行了。

> 所谓的“框架”通常是指“让某件事情做起来更容易的软件包”。
> 在网页应用的世界里，人们创建了各种各样的“网页框架”，用来解决他们在创建网站时碰到的问题，然后把这些解决方案用软件包的方式发布出来，这样你就可以利用它们引导创建你自己的项目了。

直接在cmd命令行中输入`pip install lpthw.web`就行了。

写一个简单的`Hello World`项目。
```python
import web
urls = (
'/', 'index'
)

app = web.application(urls, globals())
class index:
	def GET(self):
		greeting = "Hello World"
		return greeting

if __name__ == "__main__":
	app.run()
```
使用`python app.py`运行结果：
```python
http://0.0.0.0:8080/
```
然后python交互界面显示：`http://0.0.0.0:8080/`，使用浏览器打开`URL`：http://localhost:8080/，浏览器则会显示`"Hello World"`。

![](http://img.blog.csdn.net/20180419195026045?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](http://img.blog.csdn.net/20180419195142110?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这些是`lpthw.web`打印出的`log`信息，从这些信息你可以看出服务器有在运行，而且能了解到程序在浏览器背后做了些什么事情。这些信息还有助于你发现程序的问题。

### 发生了什么事情
> 在浏览器访问到你的网页应用程序时，发生了下面一些事情：
>1. 浏览器通过网络连接到你自己的电脑，它的名字叫做  localhost ，这是一个标准称谓，表示的就是网络中你自己的这台计算机，不管它实际名字是什么，你都可以使用`localhost`来访问。它使用到的网络端口是`5000`。
>2. 连接成功以后，浏览器对`bin/app.py`这个应用程序发出了HTTP请求(request)，要求访问`URL/`，这通常是一个网站的第一个`URL`。
>3. 在`bin/app.py`里，我们有一个列表，里边包含了`URL 和类的匹配关系`。我们这里只定义了一组匹配，那就是`'/', 'index'`的匹配。
>4. 它的含义是：如果有人使用浏览器访问`/`这一级目录，lpthw.web将找到并加载`class index`，从而用它处理这个浏览器请求。
>5. 现在lpthw.web找到了`class index`，然后针对这个类的一个实例调用了`index.GET`这个方法函数。该函数运行后返回了一个字符串，以供`lpthw.web` 将其传递给浏览器。
>6. 最后`lpthw.web`完成了对于浏览器请求的处理，将响应(response)回传给浏览器，于是你就看到了现在的页面。

### 修正错误
如同作者说的，我删去了greeting变量赋值，刷新结果如下：

![](http://img.blog.csdn.net/20180419211757203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> `lpthw.web` 的会把一些错误信息和堆栈跟踪(stack trace)信息显示在命令行终端，所以别忘了检查命令行终端的信息输出。

`lpthw.web`给我们的错误界面如下，检查每一段`Local vars`输出(用鼠标点击它们)，追踪里边提到的变量名称以及它们是在哪些代码文件中用到的：

![](http://img.blog.csdn.net/20180419212005263?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

###  创建基本的模板文件
暂时不做了。
