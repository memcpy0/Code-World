
@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。

很多时候，在书写前端代码时，尤其是HTML时效率十分低下，因为存在很多尖括号、很多标签需要闭合。于是 `Emmet` 应运而生，它提供了一种简练的语法规则，能够用来生成对应的HTML结构和CSS代码，此外它还有多种实用的功能，掌握它的使用可以大幅提高前端开发的效率。

本文只会略微介绍编写HTML代码时的快捷技巧，更多更丰富的功能见[Emmet的文档](https://docs.emmet.io/)。

---
# 1. HTML语法规则总结
## (1) 元素 `Elements`
可以使用元素的名称如 `div, p` 来生成HTML标签对。对于Emmet来说，只要知道元素的缩写，就会自动转换为对应标签：
```html
html        <html></html>
html:5      和!一样生成HTML5标准、空body的基本dom
html:xml    <html xmlns="http://www.w3.org/1999/xhtml"></html>

a:mail      <a href="mailto:"></a>
a:link      <a href="http://"></a>

base        <base href="">
br          <br>

link        <link rel="stylesheet" href="">
script      <script></script>
script:src  <script src=""></script>

div         <div></div>
foo         <footer></footer>
label       <label for=""></label>

input       <input type="text">
inp         <input type="text" name="" id="">
input:btn   <input type="button" value="">
input:email <input type="email" name="" id="">
input:pas   <input type="password" name="" id="">
input:check <input type="checkbox" name="" id="">
input:radio <input type="radio" name="" id="">
btn         <button></button>
btn:s       <button type="submit"></button>
btn:r       <bun:r></bun:r>

select      <select name="" id=""></select>
option      <option value=""></option>
bq          <blockquote></blockquote>
```
## (2) 文本内容 `Text`
在生成标签对的同时，添加文本内容可以使用 `{}` ：
```html
div{文本内容} 
<div>文本内容</div>
a{点击此处}
<a href="">点击此处</a>
```
## (3) 属性操作 `Attribute`
Id属性用 `E#id` ，Class属性用 `E.class`
```html
div.test    <div class="test"></div>
div#unique  <div id="unique"></div>
```
绑定多个类名，用 `.` 符号连接起来即可：
```html
div.test1.test2.test3 
<div class="test1 test2 test3"></div>
```
自定义属性使用 `[attr1 = '', attr2 = '']` ：

```html
a[href='#' date-title='mask' target='_blank']
<a href="#" date-title="mask" target="_blank"></a>
```
当标签名隐式时，会根据配置规则自动联想生成对应元素，不太建议这样使用：
```html
.class 
<div class="class"></div>

em>.class 
<em><span class="class"></span></em>

table>.row>.col 
<table>
    <tr class="row">
        <td class="col"></td>
    </tr>
</table>
```
## (4) 嵌套 `Nesting`
嵌套操作符用于将缩写元素放置在生成的树中，可以配合元素、属性和文本：
- 父子关系 `>` ：用于生成嵌套子级元素，从 `>` 右边最近的子元素所在位置开始，查找 `>` 左边最近的元素，嵌套在其中：
	```html
	div#pageId>ul>li
	<div id="pageId">
	    <ul>
	        <li></li>
	    </ul>
	</div>
	```
- 兄弟关系 `+` ：用于生成兄弟元素，从 `+` 右边最近的元素所在位置开始，查找 `+` 左边最近的元素，和它同级并列：
	```html
	div#pageId+div.child 
	<div id="pageId"></div>
	<div class="child"></div>
	```
-  父亲兄弟关系 `^` ：用于生成父级元素的兄弟元素，从 `^` 右边最近的元素所在位置开始，查找 `^` 左边最近的元素的父级元素并生成其兄弟元素：
	```html
	div>p.parent>span.child^ul.brother>li
	<div>
	    <p class="parent"><span class="child"></span></p>
	    <ul class="brother">
	        <li></li>
	    </ul>
	</div>
	```

## (5) 分组 `Grouping`
分组实现缩写的分离，下面的例子不加括号，那么 `a` 将作为 `span` 的子元素生成：
```html 
div>(ul>li+span)>a
<div>
    <ul>
        <li></li>
        <span></span>
    </ul>
    <a href=""></a>
</div>
```
当然，由于嵌套层级不深，这个示例还可以有其他写法：
```html
div>ul>li+span^a
<div>
    <ul>
        <li></li>
        <span></span>
    </ul>
    <a href=""></a>
</div>
```
## (6) 重复 `Multiplication`
使用 `*N` 即可重复生成标签对：
```html
ul>li*3
<ul>
    <li></li>
    <li></li>
    <li></li>
</ul>
```
## (7) 自动计数符号 `$`
使用 `$` 来自动计数，`$` 这个符号可以放在Id和Class属性的值中、文本中、自定义属性的属性名和值中，用于增加一个数字序号。如要生成多位数，可以使用多个连续的 `$$` 。
```html

<div id="mod1">
    <ul>
        <li class="item1">item number:1</li>
        <li class="item2">item number:2</li>
        <li class="item3">item number:3</li>
    </ul>
</div>
<div id="mod2">
    <ul>
        <li class="item1">item number:1</li>
        <li class="item2">item number:2</li>
        <li class="item3">item number:3</li>
    </ul>
</div>
```
## (8) 更改序号方向和起始值 `@`
和 `$` 配合使用，`@` 可以让序号升序 `$@+` 或者降序 `$@-` 排列，可以让序号从某个值开始 `$@N` ——如果配合升序降序使用，`N` 要放到 `+/-` 之后。
```html
ul>li.item$@-*3
<ul>
    <li class="item3"></li>
    <li class="item2"></li>
    <li class="item1"></li>
</ul>

ul>li#item$@-6*3
<ul>
    <li id="item8"></li>
    <li id="item7"></li>
    <li id="item6"></li>
</ul>
```
## (9) 填充随机文本
Emmet内置了Lorem Ipsum功能，使用 `loremN` 随机生成 `N` 个单词，正整数 `N` 表示生成的单词数，可以不写。
```html
lorem
Lorem ipsum dolor sit amet consectetur adipisicing elit. Laudantium exercitationem veritatis reiciendis natus, doloremque illo esse aut excepturi perspiciatis illum, iure dolor eum. Est praesentium consectetur nostrum. Error, at est.

p>lorem3
<p>Lorem, ipsum dolor.</p>

(p>lorem2)*3
<p>Lorem, ipsum.</p>
<p>Deserunt, reiciendis!</p>
<p>Quos, enim.</p>
```
## (10) 使用缩写进行包装 `wrap with Abbreviation` 
这一功能比较复杂难用，它可以把一段指定的文本包装成想要的HTML结构。比如文本和想要实现的结构如下：
```html
公司概况
产品介绍
应用案例
支持服务
联系我们

<div>
    <ul>
        <li>公司概况</li>
        <li>产品介绍</li>
        <li>应用案例</li>
        <li>支持服务</li>
        <li>联系我们</li>
    </ul>
</div>
```
需要选择文本，`Ctrl+Shift+P` 打开命令面板，输入 `ewrap` ，选择**Emmet: 使用缩写进行包装**选项，然后输入缩写字符 `div>ul>li*` ，按下回车键看到效果： 
![WD4buT.gif](https://img-blog.csdnimg.cn/img_convert/476d997ddbc732d97fda0aee7fd8c491.gif) 
如果给出的文本有序号，可以使用缩写 `|t` 来处理，例如 `div>ul>li*|t` ：
```html
1.公司概况
2.产品介绍
3.应用案例
4.支持服务
5.联系我们
```
--- 
# 2. VS Code中的具体示例

`Emmet` 中HTML语法基本规则总结如下：
```css
! 			生成HTML文档框架
E 			代表HTML元素,生成标签对

E.class 	生成带有class属性的标签对
E#id 		生成带有ID属性的标签对
E[attr=foo] 生成带有特定属性的标签对
E{foo}		生成含有内容foo的标签对

/* 嵌套操作Nesting Operators */
E>N			代表N是E的子元素
E+N			代表N是E的同级元素
E^N		    代表N是E的上级元素

/* 分组操作Grouping */
(E)			括号中的多个标签对处于同级

/* 重复操作Multiplication */
E*n			生成n个同级别的标签对 

/* 自动计数符号 */
$
```
下面给出几个具体实例。
## (1) 快速生成HTML基本框架
在VS Code中新建一个 `.html` 空白文件，然后输入 `!` ，弹出提示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715174742472.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
此时按下tab键或enter键，即可生成HTML的基本框架：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715174723226.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
## (2) 快速输入成对标签
输入标签名：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715175001819.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
然后按下tab键或enter键即可生成成对标签：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715175054438.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## (3) 快速输入多个重复成对标签
输入标签名加上 `*` 和所需标签对数：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715175158651.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
然后按下tab键或enter键，生成想要的结果：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715175239226.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
## (4) 快速输入父子关系标签
输入父标签名加上 `>` 和子标签名，**子标签名后也可以乘以数字**：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715200155577.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

按下tab或者enter键，结果如下所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715200252962.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

**父标签名后面也可以乘以数字**，比如 `ul*3>li` 的效果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715200402728.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## (5) 快速输入兄弟关系标签
输入哥哥标签名加上 `+` 和弟弟标签名，比如 `h3+p` ，生成的结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021071520060375.png)
当然，表达式中还可以嵌套父子关系，还可以有多个加号，标签名后面也可以乘以数字，比如 `body>h3*3+p*2+img+a>p` ，生成结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715201021536.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## (6) 快速输入标签及有序自增的类名
如 `div.gary$*3` 会生成：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715214647930.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)只要一个标签及其类名时，写成 `div.gary` 。其他的例子如 `p.one$*3` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715214800284.png)

## (7) 快速输入多标签内默认文字
示例如 `div{恋恋：马上就到你家门口！}*3` 的效果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715215414545.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
或者 `h3{That will be the end of civilisation as we know it!}*5` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715215700138.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 3. 小结
以下面的网页源代码结构为例：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715225424407.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

可以写出这段来生成：
```html
html>head+body.activity-stream>div.root>div.key>span+div.outer-wrapper>main>div.non-collapsible-section+div.body-wrapper
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715225352844.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

运用之妙存乎一心啊！借用Sir汉弗瑞的一句话作为结尾吧——`The possibilities are endless. Listen and learn!`
