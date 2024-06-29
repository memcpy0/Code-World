# Python云端开发基础
本文基于中国大学MOOC嵩天老师的《Python云端开发基础》，其实蛮基础的，打算两三天看完，做一下示例。至于原来的学习《Python从入门到实践》还是先放一边吧。毕竟，作业重要。我还打算写个博客玩玩呢。
***
## 云端开发全栈技术简介
![这里写图片描述](//img-blog.csdn.net/20180503234735038?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这是老师给出的**技能图**，是全栈工程师升级的路。

### 云端技术特征
- 应用 = APP；
- 云端的作用在于**掌握应用逻辑、存储应用数据、控制应用权限、体现应用智能、提供应用交互体验**；
- 绝大部分应用情况**云端与应用之间使用HTTP协议，加密需求采用HTTPS协议，应用数据采用JSON格式传输，云端需要部署Web服务器，接口采用RESTful风格设计，终端采用浏览器或APP等多种形式**。可见下图：

![这里写图片描述](//img-blog.csdn.net/20180504073236841?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 终端技术趋势
![这里写图片描述](//img-blog.csdn.net/20180504073430848?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

从图中可知，Android、iOS、Windows是我们要适应的平台，而**HTML、CSS、JS、MINA是我们要学会的前端技术**。
### 数据交互
由云端到终端，掌握JSON、RESTful等：

![这里写图片描述](//img-blog.csdn.net/20180504073823311?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 云端全栈知识地图 (Python体系)
总结前面的，我们应该学的有：

![这里写图片描述](//img-blog.csdn.net/20180504074017197?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
***
## HTML技术理念及发展简介
> HTML(**Hyper Text Markup Language 超文本传输语言**)是WWW(World Wide Web)的信息组织方式，用来传输除了文本以外的声音、图像、视频等超文本信息。
> HTML通过预定义的`<>…</>`标签形式组织不同类型的信息。
> 2014年发布的HTML5侧重移动端，成为当前标准，它运行在浏览器上，不受限于操作系统。

- HTML本身是一段文本，由标签组成；
- 浏览器负责解析HTML，并展示成Web页面内容；
- HTML所使用的标签是预定义的，浏览器可解析；
- HTML符合XML规范，是XML的一种实例；
- HTML是组织不同类型数据的最好形式。

### HTML的框架标签及实例
```html
<!DOCTYPE html>
<html lang="en">
<head>
…
</head>
<body>
…
</body>
</html>
```
```html
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>Title</title>
</head>
<body>
	<ul>
		<li>Python网络爬虫</li>
		<li>Python云端系统开发</li>
		<li>Python全栈能力开发</li>
	</ul>
</body>
</html>
```
### HTML的常用标签
• 标题：h1, h2, h3, h4, h5 …
• 段落：p
• 换行：br
• 容器：div, span
• 表格：table, tr, td
• 列表：ul, ol, li
• 链接：a
• 图片：img
• 表单：input

其实现在网上到处都是活生生的实例，可以自己查看网页的HTML代码。

## CSS技术理念简介(Cascading Style Sheets 层叠样式表)
可以说HTML和CSS既有区别，又有很多联系。

区别在于：

- HTML负责数据组织，CSS负责样式表达。
- HTML以标签对的形式`<tag> </tag>`组织数据，而CSS以键值对形式`key:value`(如`color:red;font-size:20px;`)表达数据样式。

联系在于：

- 通过HTML和不同CSS的组合，可以展示不同风格；
- 通过CSS和不同HTML的组合，可以展示不同风格；
- CSS对HTML中某一“类”内容定义风格；
- CSS可以单独存储成文件，或书写在HTML中。

### CSS的3种写法及实例

- 行内模式：在标签的style属性中书写
- 页内模式：在HTML网页中使用独立的`<style>`标签书写
- 外部模式：单独在CSS文件中书写

#### 行内模式
```html
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>Title</title>
</head>
<body>
	<ul> # 把下面两行字改成红色
		<li style="color:red">Python网络爬虫</li> 
		<li style="color:red">Python云端开发</li>
		<li>Python全栈能力</li>
	</ul>
</body>
</html>
```
#### 页面模式
```html
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>Title</title>
	<style type="text/css">
		li {color: red} # 全部<li>标签的风格都被修改
	</style>
</head>
<body>
	<ul>
		<li>Python网络爬虫</li>
		<li>Python云端开发</li>
		<li>Python全栈能力</li>
	</ul>
</body>
</html>
```
#### 外部模式
```html
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>Title</title>
	<link rel="stylesheet" href="demo.css"> # 引入.css文件
</head>
<body>
	<ul>
		<li>Python网络爬虫</li>
		<li>Python云端开发</li>
		<li>Python全栈能力</li>
	</ul>
</body>
</html>
```
```css
# demo.css
li{
	color:red
}
```
## JavaScript技术理念简介
内容和布局有了，如何让网页动起来？这就是JS的任务了。
> JavaScript 简称JS，和Java没任何关系。

- 一种**轻量级的脚本型**编程语言；
- 为Web增加了**交互、动态效果和行为功能**；
- JS由浏览器执行，访问时**会被下载到本地**；
- JS可以写在HTML文件中，也可以单独为.js文件。

### JavaScript库
- 脚本性使得JS更容易**开放开源**；
- 大量的JS库可以使用；
- 希望什么效果，找个第三方库来使用；
- `echarts.js` （百度出品）http://echarts.baidu.com/

***
## HTTP协议与Web服务器简介
HTTP协议略过，看一下Web服务器。

-  Web服务器（Web Server）是软件工具；
- Web服务器用于**提供HTTP/HTTPS访问服务**；
- Web服务器很成熟，只需要配置，不需要开发；
- 常用的Web服务器软件：
```
Apache、Nginx、IIS、BEA Weblogic、Tomcat、Node.js
```