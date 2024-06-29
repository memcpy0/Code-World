@[toc]

---
# 1. CSS简介
学习CSS之前，必须基本了解HTML/XHTML。所谓的 *CSS, Cascading Style Sheets* 层叠样式表，是一种**描述样式的语言**。样式定义**如何显示HTML元素**，就像HTML中字体标签和颜色属性所起的作用那样，==多个样式定义可以**层叠**为一个样式，且样式通常存储在样式表中==。一个HTML文档可以显示不同的样式。

HTML 4.0和XHTML 1.0时，W3C联盟提出了一系列标准，==把静态页面分成了3个部分，即**结构层**（使用XHTML来表现，后被HTML5替代）、**表现层**（使用CSS层叠样式表来体现）、**行为层**（使用JavaScript来显现）==，标志着CSS的正式登场。这一分层的目的在于**解决内容、表现和行为分离的问题**——通过使用**外部样式表**并将其存储在**CSS文件**中，我们将页面内容和外观相互分离，==现在只用编辑一个简单的CSS文档，就可以改变所有页面的布局和外观、控制多重网页的样式和展现==，这极大提高了网页开发的工作效率。
## 1.1 CSS导入方式
CSS导入方式其实就是CSS代码和HTML代码的结合方式。CSS导入HTML有三种方式：
- 内联样式：在标签内部使用 `style` 属性，属性值是CSS属性键值对：
   ```css
   <div style="color: red">Hello CSS~</div>
   ```
	> 这个方式只能作用在这一个标签上，如果其他的标签也想使用同样的样式，那就需要在其他标签上写上相同的样式。复用性太差。
- 内部样式：定义 `<style>` 标签，在标签内部定义CSS样式：
   ```css
   <style text="text/css">
	   div { 
		   color : red;
	   }
   </style>
   ```
- 外部样式：定义 `link` 标签，导入外部的CSS文件：
   ```css
   <link rel="stylesheet" href="demo.css">
   ```
## 1.2 第1个CSS实例
遵循计算机界的惯例，我在 `CodeWorld/Code_FrontEnd/Code_CSS/Learn_CSS` 下新建了一个名为 `1.first-example.html` 的文件，然后配合[这篇文章](https://memcpy0.blog.csdn.net/article/details/118765401)中的**第1个HTML实例**，我写出了**第1个CSS实例**：
```html
<!DOCTYPE html>
<html lang="en">
	<head>
	    <meta charset="UTF-8">
	    <meta http-equiv="X-UA-Compatible" content="IE=edge">
	    <meta name="viewport" content="width=device-width, initial-scale=1.0">
	    <title>CSS3学习笔记,我的第一个CSS实例</title>
	    <style>
	        body {
	            background-color: #d0e4fe;
	        }
	        h1 {
	            color: orange;
	            text-align: center;
	        }
	        p {
	            font-family: "Times New Roman";
	            font-size: 20px;
	        }
	    </style>
	</head>
	<body>
	    <h1>我的第一个HTML标题</h1>
	    <p>我的第一个段落</p>
	    <p>Hello CSS World!</p>
	</body>
</html>
```
点击**启动调试**，运行结果如下，它将 `h1` 标签的内容的字体颜色定义为橙色：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715233654121.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
## 1.3 CSS语法：CSS=选择器+声明
CSS规则由两个主要部分构成：**选择器**，以及一条或多条**声明**。其中，**选择器通常是你希望改变样式的HTML元素**；每条声明则由**一个属性和一个值**组成，**属性 `property` 是你希望设置的样式属性 `style attribute`** ，每个属性都有一个值，属性和值之间以 `:` 分隔。选择器包括：
- 元素选择器：如 `元素名称 { color : red; }`
- ID选择器：如 `#id属性值 { color : red; }`
- 类选择器：如 `.class属性值 { color : red; }` 

CSS中声明总以 `;` 结束，多条声明之间以 `;` 分开，并被一个 `{}` 括起来。为了提高CSS的可读性，**建议每行只描述一个属性**。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210716135057165.png)
## 1.4 CSS注释
CSS中的注释，通常以 `/*` 开始，以 `*/` 结束。我们用注释来（向其他人和未来的自己）解释代码，并且可以随意编辑其中的内容，不过浏览器会忽略它。比如说：
```css
/* 注释在此 */
p {
	text-align: center; /* 居中对齐 */
	color: black; 		/* 颜色为黑 */
	font-family: arial; /* 定义字体 */
}
```

## 1.5 以第2、3个CSS实例结束本章
建立第2个CSS实例文件 `2.css-grammar.html` ，输入下面的代码：
```html
<!DOCTYPE html>
<html lang="en">
	<head>
	    <meta charset="UTF-8">
	    <meta http-equiv="X-UA-Compatible" content="IE=edge">
	    <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>第二个CSS实例,讲述CSS语法</title>
        <style>
            body {
                background-color: yellow;
            }
            h1 {
                font-size: 36pt;
                font-family: Arial;
            }
            h2 {
                color: blue;
            }
            p {
                margin-left: 50px;
            }
        </style>
    </head>
    <body>
        <h1>一级标题的大小为36pt, 字体为Arial.</h1>
        <h2>二级标题的颜色为蓝色：blue.</h2>
        <p>这个段落的左外边距为50像素：50px.</p>
    </body>
</html>
```
运行后页面显示如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210716140927532.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
接着建立第3个CSS实例文件，`3.complex_css.html` ，借用[这篇文章](https://memcpy0.blog.csdn.net/article/details/118765401)中的**第2个HTML实例**：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>第3个CSS实例,更多的CSS样式</title>
    <style>
        body {
            background-color: rgb(227, 230, 223);
        }
        h1 {
            color: maroon;
            font-size: 25pt;
            font-family: Georgia, 'Times New Roman', Times, serif;
        }
        hr { /* 被水平线分隔的标题和段落 */
            color: orangered;
        }
        p {
            font-size: 15pt;
            font-weight: bold;
            margin-left: 15px; /* 左外边距 */
            text-align: left;  /* 左对齐 */
        } 
        a:link {
            color: green;
        }
        a:visited { /* 访问过后的链接为黑色 */
            color: black;
        }
        a:hover {   /* 鼠标移动到链接上时为红色 */
            color: red;
        }
        a:active {
            color: blue;
        } 

    </style> 
</head>
<body>
    <h1>东方Project传教中！</h1>
    <hr />
    <p><a href="https://www.bilibili.com/video/BV1w4411b7ph/?spm_id_from=333.788.recommend_more_video.0">恋恋：我当世界第一，好吗？</a></p>
    <img loading="lazy" width="1200" height="765" src="https://gimg2.baidu.com/image_search/src=http%3A%2F%2Fi0.hdslb.com%2Fbfs%2Farticle%2F558fd2f62d6cfee6fac7f8a7077aab386d1cbfe2.jpg&refer=http%3A%2F%2Fi0.hdslb.com&app=2002&size=f9999,10000&q=a80&n=0&g=0n&fmt=jpeg?sec=1628940277&t=f9c3422abe1eea51c173c84a870ada29"/>
</body>
</html>
```
鼠标移动到链接上，链接变成红色，如下图所示：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302012238257.png)

---
# 2. CSS ID和CLASS选择器
先前我们说明，CSS很简单，只由两个主要部分构成：**选择器**+**声明**。其中，**选择器通常是你希望改变样式的HTML元素**，它告诉浏览器如何找到HTML文档中的相关元素。当然，这也离不开HTML的密切配合，**如果要在HTML元素中设置CSS样式，就必须在HTML元素中设置 `id` 和 `class` 属性**，用来给CSS的 `id` 和 `class` 选择器提供筛选的标准。==当多个选择器同时指向一个元素时，谁选择的范围越小就生效==。

## 2.1 `id` 选择器、第4个CSS实例
大多数HTML元素都可以设置 `id` 属性，不过由于 `id` 属性是在为HTML元素定义**唯一的ID**，对应的 `id` 选择器则为标有**特定ID**的HTML元素指定**特定的样式**。CSS中 `id` 选择器以 `#` 来定义。特别注意ID属性和选择器不要以数字开头，数字开头的ID在Mozilla/Firefox浏览器中不起作用。

下面建立第4个CSS实例文件 `4.id-selector.html` ，输入以下代码：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>我的第4个CSS实例,id选择器的使用</title>
    <style>
        #para1 {
            color: red;
            text-align: center;
        }
    </style>
</head>
<body>
    <p id="para1">Hello CSS Selector!</p>
    <p>这个段落不会受到样式的影响。</p>
</body>
</html>
```
运行程序，显示如下页面：![在这里插入图片描述](https://img-blog.csdnimg.cn/20210716234200439.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## 2.2 `class` 选择器、第5个CSS实例
几乎所有HTML元素都可以设置 `class` 属性，不过由于 `class` 属性为HTML元素定义**一个或多个类名** `classname` ，且没有唯一性要求，同一个 `class` 属性可以在多个HTML元素中使用，因此对应的 `class` 选择器有别于 `id` 选择器，它被用来**描述一组元素的样式**。在CSS中，`class` 选择器以 `.` 来定义。同样注意， 类名和类选择器不能以数字开头，它无法在Mozilla/Firefox中起作用。

下面的第5个CSS实例 `5. class-selector.html` 中，所有 `class` 属性为 `center` 或者说拥有 `center` 类名的HTML元素，均居中排布：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>我的第5个CSS实例,class选择器的使用</title>
    <style>
        .center {
            text-align: center;
        }
    </style>
</head>
<body>
    <h1 class="center">标题居中</h1>
    <p class="center">段落也居中</p>
</body>
</html>
```
页面显示如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210716234442230.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
`class` 选择器还可以**指定特定的HTML元素**来使用。比如修改第5个实例中的类选择器为 `p.center { text-align: center; }` ，然后所有的类名为 `center` 的 `p` 元素，其内部文本均居中排布：
![在这里插入图片描述](https://img-blog.csdnimg.cn/202107162350047.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


---
# 3. CSS属性