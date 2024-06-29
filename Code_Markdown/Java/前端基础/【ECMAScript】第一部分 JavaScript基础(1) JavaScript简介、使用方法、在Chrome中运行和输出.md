> 本文属于「ECMAScript学习实践」系列文章之一，这一系列正式开始于2021/11/01，着重于ECMAScript的诸多实现（如**JavaScript**、**TypeScript**等）和运行环境（如**Node.js**等）的学习与实践。由于文章内容随时可能发生更新变动，欢迎关注和收藏[ECMAScript系列文章汇总目录](https://memcpy0.blog.csdn.net/article/details/118771714)一文以作备忘。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/)。在这一仓库中，你可以看到本人学习ECMAScript的全过程，包括ECMAScript书籍源码、练习实现、小型项目等。
> <b></b> 
需要特别说明的是，为了透彻理解和全面掌握ECMAScript，本系列文章中参考了诸多博客、教程、文档、书籍等资料，限于时间精力有限，这里无法一一列出。部分重要资料的不完全参考目录如下所示，在后续学习整理中还会逐渐补充：
> - *JavaScript: The Definitive Guide, Sixth Edition*，`David Flanagan` 著（即JavaScript权威指南第六版的英文版）


@[toc]

# 1. JavaScript简介
学习JavaScript之前，必须初步了解HTML和CSS。JavaScript是一种**轻量级的Web编程语言**（HTML和CSS可不算！），是一种**可插入HTML页面的程序脚本**，所有现代的Web页面和浏览器都在使用JavaScript，而且它很容易学习。甚至可以说，JavaScript是web开发人员必须学习的三门语言之一：
- HTML定义了网页的内容
 - CSS描述了网页的布局
 -  JavaScript控制了网页的行为

学习本文后，将了解JavaScript的使用方法，能够调试、运行JavaScript程序和输出数据。

## 1.1 第1个JavaScript实例：显示日期
遵循计算机界的惯例，我在 `CodeWorld/Code_JavaScript/Learn_JavaScript` 下新建了一个名为 `1.first-example.html` 的文件，然后写出了**第1个JS实例**：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>我的第一个JS实例</title>
    <script>
        function displayDate() {
            document.getElementById("demo").innerHTML = Date();
        }
        function displayHello() {
            document.getElementById("Hello").innerText = "Hello JavaScript World!";
        }
    </script>
</head>
<body>
    <h1>我的第一个JavaScript程序</h1>
    <p>点击下方显示日期的按钮，会显示当前时间</p>
    <button type="button" onclick="displayDate()">显示日期</button>
    <p id="demo"></p>
    <p>点击下方的按钮，会显示"Hello JavaScript World!"</p>
    <button type="button" onclick="displayHello()">显示宣言</button>
    <p id="Hello"></p>   
</body>
</html>
```
启动调试后，显示如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210716181847549.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
点击两个按钮，结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210716181914920.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
## 1.2 JavaScript用法 
### (1) `script` 标签和JavaScript脚本
**HTML文档中的JavaScript脚本必须处于标签对 `<script>, </script>` 之间**。所以想要在HTML页面插入JavaScript，就需要使用 `<script>` 标签——`<script>` 和 `</script>` 会告诉浏览器JavaScript在何处开始和结束，浏览器会**解释并执行** `<script>, </script>` 之间的JavaScript代码。某些旧实例可能在 `<script>` 标签中使用 `type="text/javascript"` ，不过现在不必了，**因为JS是所有现代浏览器和HTML 5中默认的脚本语言**：
```html
<script>
	alert("发起警告！");
</script>
```
### (2) JavaScript函数和事件的初遇

上述片段中的JavaScript语句，会在页面加载时执行。但这不一定是我们想要的行为效果。通常来说，**我们更加需要在某个事件发生时执行JS代码**，比如用户点击按钮时。这要求我们把JavaScript代码放进一个函数中，并且与某个能够产生事件的HTML元素绑定，就可以在事件发生时调用该函数——在1.1节的第1个JS实例中，我们在 `<head>` 部分写下的、用 `function` 声明的JavaScript语句，就是这样的**函数**。有两个这样的JavaScript函数，分别绑定到一个按钮上，在点击按钮这一**事件**发生时就会调用相应函数。

一般来说，我们能在HTML文档中放入**不限数量**的JavaScript脚本，里面包含JavaScript函数和语句。脚本可以位于HTML的 `<head>` 或 `<body>` 部分，或者同时存在两个部分中。**惯例的做法是把JS脚本放到 `<head>` 部分，或者放在页面底部，或者干脆放到外部JS文件中**——这样把它们放在同一位置，免得干扰页面的内容。 在下面三个小节中也可以看到，位置的不同不会影响到JS的执行效果。
### (3) `head` 中的JavaScript函数、第2个JavaScript实例
建立文件 `2.js-in-head.html` ，然后输入下面的代码：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>我的第2个JavaScript实例</title>
    <script>
        function myFunc() {
            document.getElementById("demo").innerHTML = "我的JavaScript函数";
        }
    </script>
</head>
<body>
    <h1>我的web页面</h1>
    <button type="button" onclick="myFunc()">尝试一下</button>
    <p id="demo">一个段落</p>
</body>
</html>
```
运行后界面如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/657c50ec2fca2899fd4800b6395a0670.png)
点击按钮：
![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/e0172f522ab56839b5446a54b1c63a7d.png)

### (4) `body` 中的JavaScript函数、第3个JavaScript实例
建立文件 `3.js-in-body.html` ，此处仅将(2)中代码 `<head>` 里面的JavaScript函数，放到了 `<body>` 中： 
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>我的第3个JavaScript实例</title>
</head>
<body>
    <h1>我的web页面</h1>
    <button type="button" onclick="myFunc()">尝试一下</button>
    <p id="demo">一个段落</p>
    <script>
        function myFunc() {
            document.getElementById("demo").innerHTML = "我的JavaScript函数";
        }
    </script>
</body>
</html>
```
运行后结果一模一样（除了页面标题外），这里就懒得放图了。


### (5) 外部的JavaScript、第4个JavaScript实例
我们还可以把脚本保存到外部JS文件中，**外部文件通常包含被多个网页反复重用的代码**。如要使用外部JS文件，可以在 `<script>` 标签的 `src` 属性中设置该 `.js` 文件的相对/绝对路径。同上的代码，我们建立一个 `4.js-in-outfile.html` 文件和一个 `4.js-in-outfile.js` 文件：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>我的第4个JavaScript实例</title>
    <script src="4.js-in-outfile.js"></script>
</head>
<body>
    <h1>我的web页面</h1>
    <button type="button" onclick="myFunc()">尝试一下</button>
    <p id="demo">一个段落</p>
</body>
</html>
```
外部JS文件中不使用 `<script>` 标签，而是直接写JavaScript代码：
```js
function myFunc() {
    document.getElementById("demo").innerHTML = "我的JavaScript函数";
}
```
外部引用的JS脚本和放在 `<script>` 标签中，运行效果完全一致：
![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/461fef5eaa00fbadca488b777651a482.png)


---
## 1.3 Chrome中调试和运行JavaScript
这节介绍如何在[Chrome](https://www.google.com/intl/zh-CN/chrome/)中调试和运行JS代码。首先在桌面上打开Chrome浏览器，或者执行第4个JavaScript实例，打开Chrome浏览器。然后可以按下**F12**或者右击页面选择**检查**，开启开发者模式：
![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/6da471184445e1394ff65848687a4c03.png)

有的网站会禁用F12按键和右键菜单，此时还可以通过右上角菜单栏选择**更多工具->开发者工具**来开启：
![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/aa956e0d2438995fb801d6f17cd7f216.png)


### (1) Console窗口运行JavaScript代码
打开开发者工具的Console窗口，我们可以在 `>` 后交互式执行JavaScript代码，输入时还会有自动补全和智能提示。比如输入 `console.log("Hello Console!")` （加不加分号均可）再按下回车执行，就会打印 `Hello Console!` 这句话：
![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/aadb86fda9cc462c8616a0de09bb506d.png)
还可以做很多事，比如说通过 `id` 查找HTML元素，输出和修改HTML元素的 `innnerText` ，这一修改如实地反映在网页上：
![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/3ea2315177cb3e31e8d07369831ca167.png)
### (2) Chrome snippets小脚本
一句句的输入JS代码虽然交互性强，但是一旦脚本稍微长一点，就很麻烦。Chrome浏览器在开发者模式的窗口中，为我们提供了一个有用的工具——点击 `Sources` 面板，可以看到网页使用的文档和资源，此处只有我们写的两个文件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/f3c0b3bfcbe68429bb476565cc55e423.png)
选择 `Snippets` 选项，点击 `+ New snippeet` 或者右键单击 `Snippets` 窗口卡并选择 `Create new snippet` ，将自动创建一个文件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/1d731e88ae175a359e1071c58367e86b.png)
在右侧的文件窗口输入以下代码，然后按 `Ctrl+S` 保存更改即可：
```js
document.write("HTML输出流中如果直接使用document.write,会在原有HTML代码中添加一串新的HTML代码。但如果在文档加载后使用（如用函数来执行这一语句），会覆盖整个文档！"); 
```
保存后，通过右击文件名，选择 `Run` 来执行代码，整个文档都被覆盖了，效果如下图左窗口。从这个意义上来说，一个Snippet文件就像一段外部函数，执行它是在文档加载后，因此 `document.write` 的效果和在HTML文档或外部JS文件中直接执行截然不同：
![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/9f2539a661d2c95d4d04d13c56c9a646.png)

---
## 1.4 JavaScript输出
JavaScript没有任何打印或者输出的函数，但它可以通过不同的方式来显示数据，这些方式我们在前文中基本都见过了，这里只是一次总结：
- 使用 `window.alert()` 弹出警告框；
- 使用 `document.write()` 方法将内容写到HTML文档中；
- 使用 `innerHTML` 写入到HTML元素；
- 使用 `console.log()` 写入到浏览器的控制台

### (1) 使用 `window.alert()`
创建 `5.js-window-alert.html` 文件，输入以下代码：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>我的第5个JavaScript实例,使用window.alert</title>
</head>
<body>
    <h1>我的页面</h1>
    <p>我的段落</p>
    <script>
        window.alert(5 + 6);
    </script>
</body>
</html>
```
运行后界面如下。先弹出警告框，点击确定后才会显示“我的页面”等文字内容。
![在这里插入图片描述](https://img-blog.csdnimg.cn/81c11ea411ef4d1cb058b63b04ee0f2a.png)
 

### (2) 写到HTML文档
出于测试目的，可以直接将数据写入HTML文档中，见 `6.js-doc-write.html` ：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>我的第6个JavaScript实例,使用document.write</title>
</head>
<body>
    <h1>我的页面</h1>
    <p>我的段落</p>
    <script>
        document.write(Date());
    </script>
</body>
</html>
```
运行后界面如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/c17942604e1f4c0ebf49f833754d6382.png)
注意，如果在文档已完成加载后执行 `document.write()` ，整个HTML页面将被覆盖。见 `7.js-doc-write-after.html` ：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>我的第7个JavaScript实例,使用function+document.write</title>
</head>
<body>
    <h1>我的页面</h1>
    <p id="demo">我的段落</p>
    <button onclick="myFunc()">点我</button>
    <script>
        function myFunc() {
            document.write(Date());
        }
    </script>
</body>
</html>
```
运行后界面如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/607fbb632b4047d8a97df4914a3624d3.png)
点击按钮后：
![在这里插入图片描述](https://img-blog.csdnimg.cn/d190eb8eaf61424cbf8029b6751f24a7.png)


### (3) 操作HTML元素
如需从JavaScript访问某个HTML元素，可用 `document.getElementById(id)` 方法，即使用 `id` 属性来查找HTML元素，并使用 `innerHTML` 来获取或修改元素的HTML内容。建立文件 `8.js-innerhtml-use.html` ：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>我的第8个JavaScript实例,使用getElementById和innerHTML</title>
</head>
<body>
    <h1>我的页面</h1>
    <p id="demo">我的段落</p>
    <script>
        document.getElementById("demo").innerHTML = "段落已修改";
    </script>
</body>
</html>
```
运行后界面如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/b61195a9a51e424ca2ea001892525529.png)

### (4) 写到控制台
如果浏览器支持调试（使用F12来启用调试模式，并点击 `Console` 菜单），可以使用 `console.log()` 方法在浏览器中显示值。创建 `9.js-console-log.html` ：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>我的第9个JavaScript实例,使用console.log</title>
</head>
<body>
    <h1>我的页面</h1> 
    <script>
        a = 5;
        b = 6;
        c = a + b;
        console.log(c);
    </script>
</body>
</html>
```
实例界面及Console截图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/ab732b41db6a49799c20165a5a15e0f5.png)

