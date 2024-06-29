> 参考资料：W3CSchool，https://developer.mozilla.org/zh-CN/docs/Web/HTML/

@[toc]

---
# 1. HTML简介
HTML *HyperText Markup Language* 超文本标记语言，是一种**用于创建网页的标记语言**，所有的网页都是用HTML这门语言编写出来的。==它不是一种编程语言，而是由一套**标记标签** *markup tag* 组成的，运用标记标签和其中的文本内容来**描述网页**==（之前学习的XML就是标记语言，由一个个的标签组成）。HTML文档（也称web页面）**运行在浏览器上**，由浏览器解析并作为网页显示。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301271323151.png)
可以看得如下内容，就是由一个个标签组成的：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301271325825.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301271325618.png)
这些标签不像XML那样可以自定义，<font color="red"><b>HTML中的标签都是预定义好的，运行在浏览器上并由浏览器解析</b></font>，然后展示出对应的效果。例如想在浏览器上展示出图片就需要使用预定义的 `img` 标签；想展示可点击的链接，就使用预定义的 `a` 标签等。

HTML预定义了很多标签，由于我们是Java工程师、是做后端开发，所以不会每个都学习，页面开发是有专门的前端工程来开发。那为什么还要学习呢？因为在公司中或多或少大家也会涉及到前端开发。

简单聊一下开发流程：以后我们是通过Java程序从数据库中查询出来数据，然后交给页面进行展示，这样用户就能通过在浏览器通过页面看到数据。

<font color="red"><b>W3C标准</b></font>：W3C是万维网联盟，这个组织是来定义标准的。他们规定了一个网页是由三部分组成，分别是：
- 结构：对应的是HTML语言
- 表现：对应的是CSS语言
- 行为：对应的是JavaScript语言

HTML定义页面的整体结构；CSS是用来美化页面，让页面看起来更加美观；JavaScript可以使网页动起来，比如轮播图也就是多张图片自动进行切换等效果。为了更好的给大家表述这三种语言的作用。我们通过具体的页面给大家说明。如下只是使用HTML语言编写的页面的结构：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301271328637.png)
可以看到页面是比较丑的、只是图片和文字的堆积，但每一部分其实都已经包含了。接下来咱们加上CSS进行美化看到的效果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301271329948.png)
瞬间感觉好看多了，这就是CSS的作用，用来美化页面的。接下来再加上JavaScript试试：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301271333729.png)
在上图中可以看到多了轮播图，在浏览器上它是会自动切换图片的，并且切换的动态效果是很不错的。

## 1.1 HTML标签和元素
HTML标记标签简称HTML标签 *HTML tag* ，是由**尖括号**包围的关键词，如 `<html>` 。HTML标签往往**成对**出现，如 `<html></html` 和 `<b></b>` 。标签对中的第一个标签是**开始标签**或**开放标签**，第二个标签是**结束标签**或**闭合标签**。

HTML标签通常和HTML元素表示同样的意思，不过狭义上来说，一个HTML元素是一个标签对，包含了开始标签和结束标签，如 `<p></p>` 。
 
## 1.2 第1个HTML实例
遵循计算机界的惯例，我在 `CodeWorld/Code_HTML/Learn_HTML` 下新建了一个名为 `1.first-example.html` 的文件（后缀名可以是 `html` 或者 `htm` ，没有区别都可使用，但建议统一用前者），输入了下面的内容：
```html
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>HTML5学习笔记,我的第一个HTML实例</title>
    </head>
    <body>
        <h1>我的第一个HTML标题</h1>
        <p>我的第一个段落</p>
        <p>Hello HTML World!</p>
    </body>
</html>
```
VS Code中点击**运行->启动调试**，弹出运行环境选项，好像包括 `Chrome` 和 `Edge: Launch` 两种，我选择了 `Chrome` ，于是Chrome浏览器被打开，出现了如下界面。由此可见，**浏览器并不是直接显示HTML标签，我们可以运用标签决定如何展现HTML页面的内容**：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715180417220.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
此时按下F12，即可进入调试模式，看到源代码：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715181729695.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
## 1.3 HTML页面框架
一个完整的HTML页面，必须包括几个必不可少的要素：
- `<!DOCTYPE html>` 声明为HTML5文档；
- `<html>` 标签对，里面囊括了整个HTML文档的内容，`<html>` 元素是HTML页面的**根元素**，下面有 `<head>` 标签和 `<body>` 标签这两个子标签：
	- `<head>` 标签对（或称头部元素）内包括了**文档的元 `meta` 数据**。其中的 `<meta charset="UTF-8">` 定义网页的编码格式为UTF-8；`<title>` 标签对描述了文档的标题，它定义的内容会展示在浏览器的标题位置；
	- `<body>` 标签对内则包括了**用户可见的页面内容**，一般来说也是HTML文档的大部分内容。**只有 `<body>` 区域的内容才会在浏览器中显示出来**。其中的 `<h1>` 标签对定义了一个一级大标题，HTML标题 `Heading` 通过 `<h1> ~ <h6>` 来定义；`<p>` 标签对定义了一个段落。
	![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715181747700.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## 1.4 HTML版本变迁
从网络的诞生开始，HTML版本经过了多次的变迁：

| 版本 | 发布时间
|:--|:--|
| HTML | 1991
| HTML+ | 1993
| HTML 2.0 | 1995
| HTML 3.2 | 1997
| HTML 4.01 | 1999
| XHTML 1.0 | 2000
| HTML5 | 2012
| XHTML5 | 2013

## 1.5 `!DOCTYPE html` 声明
如果能在文档中准确声明HTML的版本，浏览器就可以正确显示网页内容。`doctype` 声明不区分大小写，用于告知浏览器以哪一种HTML版本显示页面，以下方式均可以：
```html
<!DOCTYPE html>
<!DOCTYPE HTML>
<!doctype html>
<!Doctype Html>
```
HTML版本的变化也影响到了 `doctype` 声明：
- HTML 4.01中声明如下，需要引用DTD（**文档类型声明**），因为HTML 4.01基于SGML `Standard Generalized Markup Language`（标准通用标记语言）。HTML 4.01规定了三种不同的 `<!DOCTYPE>` 声明，分别是 `Strict, Transitional, Frameset` ：
	```html
	<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd"> 
	```
- XHTML 1.0中声明为：
	```html
	<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
	```
- HTML5不基于SGML，因此不要求引用DTD，声明也很简单：
	```html
	<!DOCTYPE html>
	```
查看完整网页声明类型：[DOCTYPE 参考手册](https://www.runoob.com/tags/tag-doctype.html)。

## 1.6 中文编码
为了避免输出中文时出现乱码，就需要在HTML文档头部将字符编码声明为UTF-8或者GBK。即使用 `<meta charset="UTF-8">` 。部分浏览器如360浏览器会设置GBK为字符编码，即使用 `<meta charset="GBK">` 。

## 1.7 HTML链接和图像、以第2个HTML实例结束这一章！
HTML链接通过标签 `<a>` 来定义，其中 `href` 属性指示链接的**网页地址**；HTML图像通过标签 `<img>` 来定义，其中 `src` 属性指示**图片源文件地址**，`width, height` 分别指示图片的宽度和高度。

建立第二个HTML文件 `2.link-and-image.html` ，输入以下代码：
```html
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>我的第二个HTML实例</title>
    </head>
    <body>
        <h1>东方Project传教中！</h1>
        <a href="https://www.bilibili.com/video/BV1w4411b7ph/?spm_id_from=333.788.recommend_more_video.0"><p>恋恋：我当世界第一，好吗？</p></a>
        <img loading="lazy" width="1200" height="765" src="https://gimg2.baidu.com/image_search/src=http%3A%2F%2Fi0.hdslb.com%2Fbfs%2Farticle%2F558fd2f62d6cfee6fac7f8a7077aab386d1cbfe2.jpg&refer=http%3A%2F%2Fi0.hdslb.com&app=2002&size=f9999,10000&q=a80&n=0&g=0n&fmt=jpeg?sec=1628940277&t=f9c3422abe1eea51c173c84a870ada29"/>
    </body>
</html>
```
运行后Chrome页面如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715193052984.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

点击链接，跳转到B站MinusT大佬的视频 **【3D东方】Brambly Boundaries** ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715193236334.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 2. HTML元素和属性
HTML文档由**HTML标记标签对**组成，或者说是由**HTML元素**来定义。

## 2.1 HTML元素语法和元素嵌套
==HTML元素是**一组标记标签对**，以**开始标签**起始，以**结束标签**终止==。当然，即使有时候忘记使用结束标签，很多浏览器也会正确显示HTML，比如 `<p>段落1<p>段落2` 会被正常显示为两个段落，**因为关闭标签有时是可选的**。本人不推荐依赖这种做法，==**正确使用结束标签能够避免许多不可预料的结果或错误**。特殊的是，**空元素**在开始标签中进行**关闭**，即以开始标签的结束为结束==。

==**元素内容**指的是开始标签和结束标签之间的内容，HTML空元素则拥有**空内容** *empty content*==。此外，大多数HTML元素还拥有**属性**。另外需要注意的是，HTML标签对/元素对大小写不敏感，`<p>` 等同于 `<P>` ，有部分网站使用大写的HTML元素。不过本人习惯小写标签，可以少敲一次Shift键，而且W3C联盟早在HTML 4中就**推荐使用小写**。

**大多数HTML元素都可以进行嵌套**，即HTML元素中包含其他HTML元素，HTML文档本身就是由互相嵌套的HTML元素构成——比如 `<html></html>` 中一般嵌套了 `<head></head>` 和 `<body></body>` ，`<body></body>` 中也可以嵌套其他HTML元素。

## 2.2 HTML空元素和正确关闭方法
没有内容的HTML元素就是**空元素**，在开始标签中关闭。例如 `<br>` 标签定义换行，就是没有关闭标签的空元素，它在所有浏览器中都是有效的。不过在XHTML、XML和未来的HTML中，**所有元素都必须被关闭**——==关闭空元素的正确方法是，**在开始标签中添加斜杠**==，例如 `<br />, <img />` ，HTML、XHTML、XML都接受这种方式，这也是更长远的保障。

## 2.3 HTML文档示例及具体解析
下面给出一个十分简单的HTML文档：
```html
<!DOCTYPE html>
<html>
	<body>
		<p>这是第一个段落。</p>
	</body>
</html>
```
上述示例包含了三个HTML元素：
- `<p>` 元素：`<p>这是第一个段落。</p>` ，定义了HTML文档中的一个段落，拥有一个开始标签 `<p>` 和一个结束标签 `</p>` ，中间的元素内容是**这是一个段落**。
- `<body>` 元素：`<body><p>这是第一个段落。</p></body>` ，定义了HTML文档的主体部分，拥有一个开始标签 `<body>` 和一个结束标签 `</body>` ，元素内容是另一个HTML元素 `<p>` 。
- `<html>` 元素：它定义了整个HTML文档，拥有一个开始标签 `<html>` 和一个结束标签 `</html>` ，元素内容是另一个HTML元素 `<body>` 。

查看完整的HTML元素列表：[HTML 参考手册- (HTML5 标准) HTML标签列表](https://www.runoob.com/tags/html-reference.html)

## 2.4 HTML属性语法
HTML属性是HTML元素提供的**附加信息**，一般描述于**开始标签**，总是以**名称/值对**的形式出现，如 `name="value"` 。一个示例是，HTML的链接由 `<a>` 标签定义，链接地址在 `href`（超文本引用 `hypertext reference`）属性中指定：
```html
<a href="https://www.baidu.com/">这是一个指向度娘的链接</a>
```
下面列出的**属性**适用于大多数HTML元素：

| 属性 | 描述|
|:---|:---
| `class` | 为HTML元素定义**一个或多个类名** `classname`（**类名从CSS样式文件引入**），即 `class=" "` 引号内可以填入多个以空格分隔的 `class` 属性
| `id` | 为HTML元素定义**唯一ID**，即 `id=" "` 引号内只能填一个，填多个是无效的
| `style` | 规定HTML元素的**行内样式** `inline style` 
| `title` | 描述了HTML元素的额外信息，作为工具条使用

需要注意的有两点：
- HTML属性值应该**始终被包括在单/双引号内**。属性值本身就含有双引号时，就必须使用单引号，比如 `name='Trump "ShotGun" Donald'` 。反之如果本身就含有单引号，就必须使用双引号 。
- **属性值对不是大小写敏感的**。不过W3C联盟仍然**推荐使用小写的属性值对**。

查看完整的HTML属性列表：[HTML全局属性参考手册](https://www.runoob.com/tags/ref-standardattributes.html)。

## 2.5 HTML注释
注释是相当重要的，我们将注释插入HTML代码中，可以提高代码的可读性，使之更容易被人理解。注意，**浏览器会忽略注释，不会在页面上显示它们**。

HTML中的注释很简单，以一个左尖括号开始，紧跟一个叹号和两个连字符，以两个连字符和一个右尖括号结束，具体写法如下：
```html
<!-- 这是一个注释 -->
```

---
# 3. HTML元素介绍
## 3.1 基础标签和特殊字符
基础标签就是一些和文字相关的标签，如下（ `font, center`  标签都不建议使用，改用CSS控制样式）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011711125.png)
对于一些特殊字符，需要使用转义字符。有如下转义字符：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011743898.png)

## 3.2 HTML标题、水平线、第3个HTML实例
HTML文档的 `<body>` 部分中，标题很重要。原因在于：
- 搜索引擎可能使用标题，为网页的结构和内容编制索引；
- 用户能够通过标题，快速浏览网页；

因此，**将标题用来呈现文档结构**是很重要的，要确保HTML标题标签只用于标题，**不能为了生成粗体或大号文本而使用标题**。

标题 `Heading` 通过 `<h1>, <h2>, <h3>, <h4>, <h5>, <h6>` 标签进行定义，`<h1>` 是最大的标题，`<h6>` 是最小的标题，1级至6级标题与6号至1号字体相对应。**应将 `<h1>` 用作最重要的主标题**，而后依次类推。注意，**浏览器会自动在标题前后添加空行**。水平线在HTML页面中由 `<hr />` 标签定义，可以用于分隔内容。

`<font>` 标签可用来定义字体 `face` 、大小 `size` 和颜色 `color` 。注意，**`<font>` 标签是一个过时的标签，已经不建议使用了，要用CSS来控制文本的样式**。
- `face` 属性：用来设置字体。如 "楷体"、"宋体"等
- `color` 属性：设置文字颜色。颜色有三种表示方式
	- 英文单词：`red, pink, blue...` 。这种方式表示的颜色特别有限，所以一般不用。
	- `rgb(值1,值2,值3)` ：值的取值范围 $0\sim 255$ 。此种方式也就是三原色（红绿蓝）设置方式。 例如：`rgb(255,0,0)` 。这种书写起来比较麻烦，一般不用。
	- `#值1值2值3` ：值的范围 $00\sim \textrm{FF}$ 。这种方式是RGB方式的简化写法，**以后基本都用此方式**。值1表示红色范围，值2表示绿色范围，值3表示蓝色范围。例如 `#ff0000` 。
- `size` 属性：设置文字大小

建立 `3.heading-and-horizon.html` ，输入以下代码：
```html
<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>我的第3个HTML实例</title>
    </head>
    <body>
        <!-- 学习HTML标题和水平线的使用, 这个注释不会显示在浏览器界面 -->
        <p>这是一个段落</p>
        <p>hr标签定义水平线。</p>
        <hr />

        <h1>这是1级标题</h1>
        <font size="6">这是6号字体文本</font>
        <hr />

        <h2>这是2级标题</h2>
        <font size="5">这是5号字体文本</font>
        <hr />

        <h3>这是3级标题</h3>
        <font size="4">这是4号字体文本</font>
        <hr />

        <h4>这是4级标题</h4>
        <font size="3">这是3号字体文本</font>
        <hr />

        <h5>这是5级标题</h5>
        <font size="2">这是2号字体文本</font>
        <hr />

        <h6>这是6级标题</h6>
        <font size="1">这是1号字体文本</font>
        
    </body>
</html>
```
运行后结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/6968d383d75c414799c8734861d40cce.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## 3.2 HTML段落、折行
在页面文件中书写如下内容：
```html
刚察草原绿草如茵，沙柳河水流淌入湖。藏族牧民索南才让家中，茶几上摆着馓子、麻花和水果，炉子上刚煮开的奶茶香气四溢……

6月8日下午，习近平总书记来到青海省海北藏族自治州刚察县沙柳河镇果洛藏贡麻村，走进牧民索南才让家中，看望慰问藏族群众。
```
在浏览器展示的效果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011736665.png)
我们可以看到并没有换行。如果要实现换行效果，需要使用换行标签 `br` 。修改页面文件内容如下：
```html
刚察草原绿草如茵，沙柳河水流淌入湖。藏族牧民索南才让家中，茶几上摆着馓子、麻花和水果，炉子上刚煮开的奶茶香气四溢……
<br>
6月8日下午，习近平总书记来到青海省海北藏族自治州刚察县沙柳河镇果洛藏贡麻村，走进牧民索南才让家中，看望慰问藏族群众。
```
浏览器打开效果如下，现在就有换行效果了：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011739892.png)

上面文字展示的效果还是不太好，**我们想让每一段上下都加空行**。此时就需要使用段落标签 `p` 标签：
```html
<p>
刚察草原绿草如茵，沙柳河水流淌入湖。藏族牧民索南才让家中，茶几上摆着馓子、麻花和水果，炉子上刚煮开的奶茶香气四溢……
</p>
<p>
6月8日下午，习近平总书记来到青海省海北藏族自治州刚察县沙柳河镇果洛藏贡麻村，走进牧民索南才让家中，看望慰问藏族群众。
</p>
```
在浏览器展示的效果如下，这种效果就会比之前的效果好一些，呈现出段落的效果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011740311.png)

## 3.3 加粗、斜体、下划线、居中标签
代码如下：
```html
<b>沙柳河水流淌</b><br>
<i>沙柳河水流淌</i><br>
<u>沙柳河水流淌</u><br>
```
在浏览器展示的效果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011742701.png)

代码如下：
```html
<hr>
<center>
	<b>沙柳河水流淌</b>
</center>
```
在浏览器效果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011742975.png)

## 3.4 图片、音频、视频标签
`<img>` ：定义图片
	- `src` ：规定显示图像的URL（统一资源定位符）
	- `height` ：定义图像的高度
	- `width` ：定义图像的宽度

`<audio>` ：定义音频。当前HTML支持的音频格式只有：MP3、WAV、OGG。
	- `src` ：规定音频的 URL
	- `controls` ：显示播放控件（ `controls="controls"` ，但只写 `controls` 也行）

- `<video>` ：定义视频。支持的音频格式：MP4、WebM、OGG
	- `src` ：规定视频的 URL
	- `controls` ：显示播放控件（才显示播放按钮）
	- 宽度和高度……

**尺寸单位**：`height` 属性和 `width` 属性有两种设置方式：
- 像素：单位是 `px` ，屏幕上一个点。
- 百分比：**占父标签的百分比**。例如宽度设置为50%，意思就是占它的父标签宽度的一般（50%）

**资源路径**：图片、音频和视频标签都有 `src` 属性，是用来指定对应的图片、音频、视频文件的路径。此处的图片、音频、视频就称为资源。资源路径有如下两种设置方式：
- 绝对路径：完整路径。这里的绝对路径是网络中的绝对路径。格式为：`协议://ip地址:端口号/资源名称` 。如：
   ```html
<img src="https://th.bing.com/th/id/R33674725d9ae34f86e3835ae30b20afe?rik=Pb3C9e5%2b%2b3a9Vw&riu=http%3a%2f%2fwww.desktx.com%2fd%2ffile%2fwallpaper%2fscenery%2f20180626%2f4c8157d07c14a30fd76f9bc110b1314e.jpg&ehk=9tpmnrrRNi0eBGq3CnhwvuU8PPmKuy1Yma0zL%2ba14T0%3d&risl=&pid=ImgRaw" width="300" height="400">
   ```
- 相对路径：相对位置关系。找页面和其他资源的相对路径。`./` 表示当前路径，`../` 表示上一级路径，`../../` 表示上两级路径。如模块目录结构如下：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011751650.png)
	则在代码 `01-基础标签.html` 中找不同的图片，路径写法不同：
   ```html
    <!--在该页面找a.jpg，就需要先回到上一级目录，该级目录有img目录，进入该目录就可以找到 a.jpg图片-->
	<img src="../img/a.jpg" width="300" height="400">
	<!--该页面和aa.jpg 是在同一级下，所以可以直接写 图片的名称，也可以写成 ./aa.jpg-->
	<img src="aa.jpg" width="300" height="400">
   ```

使用这些标签的代码为 `img-audio-video.html` ：
```html
<!DOCTYPE html> 
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>图片、音频、视频</title>
    </head>
    <body>
        <img src="a.jpg" width="300" height="400">
        <audio src="b.mp3" controls></audio>
        <video src="c.mp4" controls width="500" height="300"></video>
    </body>
</html>
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011817297.png)

## 3.5 超链接标签
在网页中可以看到很多超链接标签，当我们点击这些超链接时会跳转到其他的页面或者资源。而超链接使用的是 `a` 标签。
- `ref` ：指定访问资源的URL；
- `target` ：指定打开资源的方式。
	- `_self` ：默认值，在当前页面打开
	- `_blank` ：在空白页面打开

代码演示：
```html
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Title</title>
	</head>
	<body>
		<a href="https://www.itcast.cn" target="_self">点我有惊喜</a>
	</body>
</html>
```
效果图示：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011826818.png)
当我们将 `target` 属性值设置为 `_blank` ，效果图示：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011826650.png)

## 3.6 列表标签
HTML 中列表分为以下两种，都使用 `<li>` 定义列表项：
- 有序列表。如下图，页面效果中有标号对每一项进行标记。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011837382.png)
- 无序列表。如下图，页面效果中没有标号对每一项进行标记，而是使用点进行标记。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011837217.png)
- 有序列表中的 `type` 属性用来「指定标记的标号」的类型（数字、字母、罗马数字等），无序列表中的 `type` 属性用来指定标记的形状。不过==这种列表项的标识符，将来一般是不要的、不赞成使用（见W3CSchool），用CSS取代和控制它们==。

## 3.7 表格标签
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011909046.png)
如上图就是一个表格，表格可以使用如下标签定义
- `table` ：定义整体的表格
	- `border` ：规定表格边框的宽度，不加这个属性就没有单元格边框
	- `width` ：规定表格的宽度
	- `cellspacing` ：规定单元格之间的空白。单元格和单元格之间有两条边框，加这个来使两条边框合二为一。
- `tr` ：定义行
	- `align` ：定义表格行的内容对齐方式
- `td` ：定义单元格
	- `rowspan` ：规定单元格可横跨的行数，该单元格属于其开始于的那行。
	- `colspan` ：规定单元格可横跨的列数，该单元格属于其开始于的那列。
- `th` ：定义表头单元格

对应上图的代码是：
```html
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Title</title>
	</head>
	<body>
		<table border="1" cellspacing="0" width="500">
			<tr>
				<th>序号</th>
				<th>品牌logo</th>
				<th>品牌名称</th>
				<th>企业名称</th>
			</tr>
			<tr align="center">
				<td>010</td>
				<td><img src="../img/三只松鼠.png" width="60" height="50"></td>
				<td>三只松鼠</td>
				<td>三只松鼠</td>
			</tr>
			<tr align="center">
				<td>009</td>
				<td><img src="../img/优衣库.png" width="60" height="50"></td>
				<td>优衣库</td>
				<td>优衣库</td>
			</tr>
			<tr align="center">
				<td>008</td>
				<td><img src="../img/小米.png" width="60" height="50"></td>
				<td>小米</td>
				<td>小米科技有限公司</td>
			</tr>
		</table>
	</body>
</html>
```

## 3.8 布局标签
下面两个标签**一般都是和CSS结合到一块使用、来实现页面的布局**。**`div` 标签在浏览器上会有换行的效果**（因为它是块内标签，占据一行），而 `span` 标签在浏览器上没有换行效果（因为它是行内标签）。
- `div` ：定义HTML文档中的一个区域部分，经常与CSS一起使用，用来布局网页
- `span` ：用来组合行内元素

```html
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Title</title>
	</head>
	<body>
		<div>我是div</div>
		<div>我是div</div>
		<span>我是span</span>
		<span>我是span</span>
	</body>
</html>
```
浏览器效果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011926451.png)

## 3.9 表单标签
表单标签效果大家其实都不陌生，像登陆页面、注册页面等都是表单。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011928339.png)
像这样的表单就是用来采集用户输入的数据，然后将数据发送到服务端，服务端会对数据库进行操作，比如注册就是将数据保存到数据库中，而登陆就是根据用户名和密码进行数据库的查询操作。

**表单是很重要的标签**，需要大家重点来学习。
### 3.9.1 表单标签概述
表单：在网页中主要负责数据采集功能，使用 `<form>` 标签定义表单。
表单项（元素）：不同类型的 `input` 元素、下拉列表、文本域等。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011929849.png)
`form` 是表单标签，它在页面上没有任何展示效果。**需要借助于表单项标签来展示不同的效果**。如下图就是不同的表单项标签展示出来的效果。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011931901.png)
### 3.9.2 `form` 标签属性
- `action` ：**规定当提交表单时向何处发送表单数据，该属性值就是URL**。
 以后会将数据提交到服务端，该属性需要书写服务端的URL。我们可以书写 `#` ，表示提交到当前页面来看效果。
- `method` ：规定用于发送表单数据的方式，取值有如下两种：
	- `get` ：默认值。如果不设置 `method` 属性则默认就是该值，请求参数会拼接在URL后边，URL的长度有限制（4KB），因此参数有个数限制
	- `post` ：**浏览器会将数据放到HTTP请求消息体中**，请求参数无限制个数

由于表单标签在页面上没有任何展示的效果，所以演示过程会先使用 `input` 这个表单项标签展示输入框效果。代码如下：
```html
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Title</title>
	</head>
	<body>
		<form>
			<input type="text">
			<input type="submit">
		</form>
	</body>
</html>
```
浏览器展示效果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011936101.png)
从效果可以看到页面有一个输入框，用户可以在数据框中输入自己想输入的内容，点击提交按钮以后会将数据发送到服务端，当然现在肯定不能实现。现在可以将 `form` 标签的 `action` 属性值设置为 `#` ，将其将数据提交到当前页面。还需要注意一点，**要想提交数据，`input` 输入框必须设置 `name` 属性**。代码如下：
```html
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Title</title>
	</head>
	<body>
		<form action="#">
		<input type="text" name="username">
		<input type="submit">
		</form>
	</body>
</html>
```
浏览器展示效果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011938484.png)
在输入框输入 `hehe` ，然后点击提交按钮，就能看到如下效果——在浏览器地址栏的URL后拼接了我们提交的数据。`username` 就是输入框 `name` 属性值，而 `hehe` 就是我们在输入框输入的内容：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011941166.png)

接下来聊 `method` 属性，默认是 `method = 'get'` ，所以该取值就会将数据拼接到URL的后面。那我们将 `method` 属性值设置为 `post` ，浏览器的效果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011944715.png)
从上图可以看出数据并没有拼接到 URL 后，那怎么看提交的数据呢？我们可以使用浏览器的开发者工具来查看：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011945328.png)

### 3.10 表单项标签
表单项标签有很多，不同的表单项标签有不同的展示效果。表单项标签可以分为以下三个：
- `<input>` ：表单项，**通过 `type` 属性控制输入形式**，`type` 属性的取值不同，展示的效果也不一样—— `type="submit"` 会提交该 `input` 标签前面没提交的信息，可以用 `value` 属性定义该按钮上的字。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011949000.png)
- `<select>` ：定义下拉列表，**`<option>` 定义列表项**。如下图就是下拉列表的效果：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011954068.png)
- `<textarea>` ：文本域。如下图就是文本域效果。它可以输入多行文本，而 `input` 数据框只能输入一行文本。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302011954281.png)

> 注意：
> - 以上标签项的内容要想提交，必须得定义 `name` 属性。
> - 每一个标签都有 `id` 属性，`id` 属性值是唯一的标识。

**可以定义这样一种效果**：如点击“用户名”这一标签，则鼠标指针自动进入对应输入框——用 `<label>` 标签包住“用户名”，然后使用 `for` 属性指定要聚焦的输入框（通过其 `id` 属性）；**用 `<label>` 包裹住对应标签也可以达到这种效果**。

**单选框 `radio` 、复选框 `checkbox`、下拉列表 `select` 需要使用 `value` 属性指定实际提交的值**。单选框还要定义相同的 `name` 属性来关联那些选项，使得只能选择其一。
```html
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Title</title>
	</head>
	<body>
		<form action="#" method="post">
			<input type="hidden" name="id" value="123">
			
			<label for="username">用户名：</label>
			<input type="text" name="username" id="username"><br>
			
			<label for="password">密码：</label>
			<input type="password" name="password" id="password"><br>
			性别：
			<input type="radio" name="gender" value="1" id="male"> <label for="male">男</label>
			<input type="radio" name="gender" value="2" id="female"> <label for="female">女
			</label>
			<br>
			爱好：
			<input type="checkbox" name="hobby" value="1"> 旅游
			<input type="checkbox" name="hobby" value="2"> 电影
			<input type="checkbox" name="hobby" value="3"> 游戏
			<br>
			头像：
			<input type="file"><br>
			城市:
			<select name="city">
				<option>北京</option>
				<option value="shanghai">上海</option>
				<option>广州</option>
			</select>
			<br>
			个人描述：
			<textarea cols="20" rows="5" name="desc"></textarea>
			<br>
			<br>
			<input type="submit" value="免费注册">
			<input type="reset" value="重置">
			<input type="button" value="一个按钮">
		</form>
	</body>
</html>
```
在浏览器的效果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302012005179.png)
