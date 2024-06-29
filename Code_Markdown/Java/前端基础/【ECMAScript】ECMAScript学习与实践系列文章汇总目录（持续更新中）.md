> 本文属于「ECMAScript学习实践」系列文章的汇总目录，这一系列正式开始于2021/11/01，着重于ECMAScript的诸多实现（如**JavaScript**、**TypeScript**等）和运行环境（如**Node.js**等）的学习与实践。由于文章内容随时可能发生更新变动，欢迎关注和收藏本文以作备忘。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/)。在这一仓库中，你可以看到本人学习ECMAScript的全过程，包括ECMAScript书籍源码、练习实现、小型项目等。
> <b></b> 
> 需要特别说明的是，为了透彻理解和全面掌握ECMAScript，本系列文章中参考了诸多博客、教程、文档、书籍等资料，限于时间精力有限，这里无法一一列出。部分重要资料的不完全参考目录如下所示，在后续学习整理中还会逐渐补充：
> - *JavaScript: The Definitive Guide, Sixth Edition*，`David Flanagan` 著（即JavaScript权威指南第六版的英文版）


@[toc]

- 掌握 JavaScript 的**基础语法**
- 掌握 JavaScript 的**常用对象**（Array、String）
- 能根据需求灵活运用**定时器**及通过 js 代码进行**页面跳转**
- 能通过**DOM 对象**对标签进行常规操作
- 掌握常用的**事件**
- 能独立完成表单校验案例

---
# 1. JavaScript简介
**JavaScript是一门跨平台、面向对象的脚本语言**，而Java语言也是跨平台的、面向对象的语言，只不过Java是编译语言，是需要编译成字节码文件才能运行的；JavaScript是脚本语言，不需要编译，由浏览器直接解析并执行。

JavaScript和Java是完全不同的语言，不论是概念还是设计，只是名字比较像而已。但基础语法类似，所以有Java的学习经验，再学习JavaScript 语言就相对比较容易些。

JavaScript（简称JS）在1995年由 *Brendan Eich* 发明，并于1997年成为一部ECMA标准。ECMA规定了一套标准就叫ECMAScript，所有的客户端校验语言必须遵守这个标准，当然JavaScript也遵守了这个标准。ECMAScript 6（简称ES6）是最新的JavaScript版本（发布于2015年），这里基于最新的ES6进行讲解。
## 1.1 JavaScript功能介绍
**JavaScript是用来控制网页行为的，它能使网页可交互**；那么它可以做什么呢？如改变页面内容、修改指定元素的属性值、对表单进行校验等，下面是这些功能的效果展示：
- **改变页面内容**：当点击上面左图的 `点击我` 按钮，按钮上面的文本就改为上面右图内容，这就是JS改变页面内容的功能。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302012249278.png)
- **修改指定元素的属性值**：当我们点击上图的 `开灯` 按钮，效果就是上面右图效果；当我点击 `关灯` 按钮，效果就是上面左图效果。这个功能中有两张灯泡的图片（使用 `img` 标签进行展示），通过修改 `img` 标签的 `src` 属性值改变展示的图片来实现。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302012250327.png)
- **对表单进行校验**：在上面左图的输入框输入用户名，如果输入的用户名是不满足规则的就展示右图（上）的效果；如果输入的用户名是满足规则的就展示右图（下）的效果。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302012251646.png)

## 1.2 JavaScript引入方式
JavaScript引入方式就是HTML和JavaScript的结合方式。JavaScript引入方式有两种：
- 内部脚本：将JS代码定义在HTML页面中
- 外部脚本：将JS代码定义在外部JS文件中，然后引入到HTML页面中

### 1.2.1 内部脚本
**在HTML中，JavaScript代码必须位于 `<script>` 与 `</script>` 标签之间**。代码如下：
```javascript
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Title</title>
	</head>
	<body>
		<script>
			alert("hello js1");
		</script>
	</body>
</html>
```
`alert("data")` 是JavaScript的一个方法，作用是将参数数据以浏览器弹框的形式输出出来。效果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302012257903.png)

遵循语法的前提下，在HTML文档中可以在任意地方、放置任意数量的标签。如下：
```js
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Title</title>
		<script>
			alert("hello js1");
		</script>
	</head>
	<body>
		<script>
			alert("hello js1");
		</script>
	</body>
</html>
<script>
	alert("hello js1");
</script>
```
**一般把脚本置于 `body` 元素的底部，可改善显示速度**——因为浏览器在加载页面的时候会从上往下进行加载并解析。 我们应该让用户看到页面内容，然后再展示动态的效果。
### 1.2.2 外部脚本
**第一步：定义外部JS文件**。如定义名为 `demo.js` 的文件，项目结构如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302012301019.png)
`demo.js` 文件内容如下：
```js
alert("hello js");
```
**第二步：在页面中引入外部的JS文件**。在页面使用 `script` 标签中，并用 `src` 属性指定JS文件的URL路径。
```js
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Title</title>
	</head>
	<body>
		<script src="../js/demo.js"></script>
	</body>
</html>
```
> 注意：**外部脚本不能包含 `<script>` 标签**。在JS文件中直接写JS代码即可，不要在 `js`文件中写 `script` 标签。**`<script>` 标签不能自闭合**——在页面中引入外部JS文件时，不能写成 `<script src="../js/demo.js" />` 。

---
# 2. JavaScript基础语法
## 2.1 书写语法
JS有如下特征：
- 区分大小写：与Java一样，变量名、函数名以及其他一切东西都是区分大小写的；
- 每行结尾的分号可有可无；如果一行上写多个语句时，必须加分号用来区分多个语句。
- 注释（JavaScript没有文档注释）
	- 单行注释：`// 注释内容`
	- 多行注释：`/* 注释内容 */`
- **大括号表示代码块**

## 2.2 输出语句
JS可以通过以下方式进行内容的输出，只不过不同的语句输出到的位置不同。

可以**使用 `window.alert()` 写入警告框**：
```js
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Title</title>
	</head>
	<body>
		<script>
			window.alert("hello js"); //写入警告框
		</script>
	</body>
</html>
```
也可以**使用 `document.write()` 写入HTML输出**：
```js
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Title</title>
	</head>
	<body>
		<script>
			document.write("hello js 2~"); //写入html页面
		</script>
	</body>
</html>
```
上面代码通过浏览器打开，我们可以在页面上看到 `document.write(内容)` 输出的内容：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302012307374.png)

**使用 `console.log()` 写入浏览器控制台**：
```js
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Title</title>
	</head>
	<body>
		<script>
			console.log("hello js 3");//写入浏览器的控制台
		</script>
	</body>
</html>
```
上面代码通过浏览器打开，我们不能在页面上看到 `console.log(内容)` 输出的内容，它是输出在控制台了，而怎么在控制台查看输出的内容呢？在浏览器界面按F12就可以看到下图的控制台：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302012309852.png)

## 2.3 变量
JavaScript中用 `var` 关键字（`variable` 的缩写）来声明变量。格式 `var 变量名 = 数据值;` 。而**JavaScript是一门弱类型语言，变量可以存放不同类型的值**；如下在定义变量时赋值为数字数据，还可以将变量的值改为字符串：
```js
var test = 20;
test = "张三";
```
JS中的变量名命名也有如下规则，和Java语言基本都相同
- 组成字符可以是任何字母、数字、下划线 `_` 或美元符号 `$`
- 数字不能开头
- 建议使用驼峰命名

**JavaScript中 `var` 关键字有点特殊**，有以下地方和其他语言不一样
- **作用域是全局变量**：
   ```js
   {
       var age = 20;
   }
   alert(age); // 在代码块中定义的age变量，在代码块外边还可以使用
   ```
- 变量可以重复定义：
   ```js
   {
	   var age = 20;
	   var age = 30; //JavaScript会用30将之前age变量的20替换掉
   }
   alert(age); //打印的结果是30
   ```

针对如上的问题，**ECMAScript 6新增了 `let` 关键字来定义变量**。它的用法类似于 `var` ，但**所声明的变量只在 `let` 关键字所在的代码块内有效，且不允许重复声明**。例如：
```js
{
	let age = 20;
}
alert(age);
```
运行上面代码，浏览器并没有弹框输出结果，说明这段代码是有问题的。通过F12打开开发者模式可以看到如下错误信息：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302012330642.png)
而如果在代码块中定义两个同名的变量，IDEA开发工具就直接报错了：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302012330898.png)

**ECMAScript 6 新增了 `const` 关键字，用来声明一个只读的常量**。一旦声明，常量的值就不能改为其他值（不能重新赋值）。 通过下面的代码看一下就可以了：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302012331537.png)
 
我们可以看到给 `PI` 这个常量重新赋值时报错了。

## 2.4 数据类型
JavaScript中提供了两类数据类型：**原始类型** 和 **引用类型**。
> 使用 `typeof` 运算符可以获取数据类型；`alert(typeof age);` 以弹框的形式将 `age` 变量的数据类型输出。

原始数据类型包括 `number, string, boolean, null, undefined` 。

`number` ：数字（整数、小数、`NaN(Not a Number)` ），NaN是一个特殊的 `number` 类型的值，后面用到再说：
```js
var age = 20;
var price = 99.8;
alert(typeof age); // 结果是 ： number
alert(typeof price); // 结果是 ： number
```
`string` ：字符、字符串，单双引皆可，在JS中双引号和单引号都表示字符串类型的数据：
```js
var ch = 'a';
var name = '张三';
var addr = "北京";
alert(typeof ch); //结果是 string
alert(typeof name); //结果是 string
alert(typeof addr); //结果是 string
```
`boolean` ：布尔值，包括 `true, false` ：
```js
var flag = true;
var flag2 = false;
alert(typeof flag); //结果是 boolean
alert(typeof flag2); //结果是 boolean
```
`null` ：对象为空。
```js
var obj = null;
alert(typeof obj); //结果是 object
```
为什么打印上面的 `obj` 变量的数据类型，结果是 `object` ；这个官方给出了解释，下面是从官方文档截的图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302012340119.png)
`undefined` ：当声明的变量未初始化时，该变量的默认值是 `undefined` ：
```js
var a;
alert(typeof a); //结果是 undefined
```

## 2.5 运算符
JavaScript 提供了如下的运算符。大部分和 Java语言 都是一样的，不同的是 JS 关系运算符中的 == 和 === ，一会我们只演示这两个的区别，其他运算符将不做演示
- 一元运算符：`++, --` 
- 算术运算符：`+, -, *, /, %`
- 赋值运算符：`=, +=, -= ...`
- 关系运算符：`>, <, >=, <=, !=, ==, === ...`
- 逻辑运算符：`&&, ||, !`
- 三元运算符：条件表达式 `? true_value : false_value` 。

### 2.5.1 `==` 和 `===` 区别
`==` ：
1. 判断类型是否一样，如果不一样，则进行类型转换
2. 再去比较其值

`===` 是JS中的全等于：
1. 判断类型是否一样，如果不一样，直接返回 `false`
2. 再去比较其值
```js
var age1 = 20;
var age2 = "20";
alert(age1 == age2); // true
alert(age1 === age2); // false
```
### 2.5.2 类型转换
上述讲解 `==` 运算符时，发现会进行类型转换，所以接下来我们来详细的讲解一下JavaScript中的类型转换。

其他类型转为 `number` ：
- `string` 转换为 `number` 类型：**按照字符串的字面值转为数字**。如果字面值不是数字，则转为 `NaN` 。将 `string` 转换为 `number` 有两种方式：
	- 使用 `+` 正号运算符：
      ```js
	  var str = +"20";
	  alert(str + 1) //21 
	  ```
	- 使用 `parseInt()` 函数（方法），**建议使用**：
      ```js
	  var str = "20";
	  alert(parseInt(str) + 1);
	  ```
- `boolean` 转换为 `number` 类型：`true` 转为1，`false` 转为0。
   ```js
   var flag = +false;
   alert(flag); // 0
   ```

其他类型转为 `boolean` ：
- `number` 类型转换为 `boolean` 类型：`0` 和 `NaN` 转为 `false` ，其他的数字转为 `true` 
- `string` 类型转换为 `boolean` 类型：空字符串转为 `false` ，其他的字符串转为 `true`
- **`null` 类型转换为 `boolean` 类型是 `false`**
- `undefined` 转换为 `boolean` 类型是 `false`

代码如下：
```js
// var flag = 3;
// var flag = "";
var flag = undefined;
if(flag) {
	alert("转为true");
} else {
    alert("转为false");
}
```
**使用场景**：在Java中使用字符串前，一般都会先判断字符串不是 `null` ，并且不是空字符才会做其他的一些操作，JavaScript也有类型的操作，代码如下：
```js
var str = "abc";
// 健壮性判断
if (str != null && str.length > 0) {
	alert("转为true");
} else {
	alert("转为false");
}
```
但**由于JavaScript会自动进行类型转换，所以上述的判断可以进行简化**，代码如下：
```js
var str = "abc";
// 健壮性判断
if (str) {
	alert("转为true");
} else {
	alert("转为false");
}
```

## 2.6 流程控制语句
JavaScript中提供了和Java一样的流程控制语句，如下
- if
- switch
- for
- while
- dowhile

### 2.6.1 `if` 语句
```js
var count = 3;
if (count == 3) {
	alert(count);
}
```
### 2.6.2 `switch` 语句
```js
var num = 3;
switch (num) {
case 1:
	alert("星期一");
	break;
case 2:
	alert("星期二");
	break;
case 3:
	alert("星期三");
	break;
case 4:
	alert("星期四");
	break;
case 5:
	alert("星期五");
	break;
case 6:
	alert("星期六");
	break;
case 7:
	alert("星期日");
	break;
default:
	alert("输入的星期有误");
	break;
}
```
### 2.6.3 `for` 循环语句
```js
var sum = 0;
for (let i = 1; i <= 100; i++) { //建议for循环小括号中定义的变量使用let
	sum += i;
}
alert(sum);
```
### 2.6.4 `while` 循环语句
```js
var sum = 0;
var i = 1;
while (i <= 100) {
	sum += i;
	i++;
}
alert(sum);
```
## 2.6.5 `dowhile` 循环语句
```js
var sum = 0;
var i = 1;
do {
	sum += i;
	i++;
} while (i <= 100);
alert(sum);
```

## 2.7 函数
函数（就是Java中的方法）是被设计为**执行特定任务的代码块**；JavaScript 函数**通过 `function` 关键词进行定义**。
### 2.7.1 定义格式
函数定义格式有两种。方式1：
```js
function 函数名(参数1, 参数2..) {
	要执行的代码
}
```
方式2：
```js
var 函数名 = function (参数列表) {
	要执行的代码
}
```
注意：<b><font color="red">形式参数不需要类型，返回值也不需要定义类型</font></b>，可以在函数内部直接使用 `return` 返回即可，因为JavaScript是弱类型语言。如下函数的参数 `a` 和 `b` 不需要定义数据类型，在每个参数前加上 `var` 也没有任何意义：
```js
function add(a, b) {
	return a + b;
}
```
### 2.7.2 函数调用
函数调用很简单：`函数名称(实际参数列表);` 。例如：
```js
let result = add(10,20);
```
注意：JS中**函数调用可以传递任意个数参数**，函数调用只和函数名称有关系，和参数个数无关。例如 `let result = add(1, 2, 3);` ，它是将数据1传递给了变量 `a` ，将数据2传递给了变量 `b` ，而数据3没有变量接收。

---
# 3. JavaScript常用对象
JavaScript提供了很多对象供使用者来使用。这些对象总共分三类：
- 基本对象 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302020038611.png)
- BOM对象：![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302020038168.png)
- DOM对象：DOM中的对象就比较多了，下图只是截取部分：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302020047597.png)

先学习基本对象，而我们先学习 `Array` 数组对象和 `String` 字符串对象。
## 3.1 `Array` 对象
JavaScript Array对象用于定义数组
### 3.1.1 定义格式
数组的定义格式有两种：
```js
var 变量名 = new Array(元素列表);
var 变量名 = [元素列表];
```
例如：
```js
var arr = new Array(1,2,3); //1,2,3 是存储在数组中的数据（元素）
var arr = [1,2,3]; //1,2,3 是存储在数组中的数据（元素）
```
> 注意：Java中的数组静态初始化使用的是 `{}` 定义，而JavaScript中使用的是 `[]` 定义。

### 3.1.2 元素访问
访问数组中的元素和Java的一样，格式如下：
```js
arr[索引] = 值;
```
例如：
```js
// 方式一
var arr = new Array(1,2,3);
// alert(arr);
// 方式二
var arr2 = [1,2,3];
//alert(arr2);
// 访问
arr2[0] = 10;
alert(arr2);
```
### 3.1.3 特点
JavaScript中的数组相当于Java中集合。**数组的长度是可以变化的**，而 JavaScript是弱类型，所以**可以存储任意的类型的数据**。例如如下代码：
```js
// 变长
var arr3 = [1,2,3];
arr3[10] = 10;
alert(arr3[10]); // 10
alert(arr3[9]); //undefined
```
上面代码在定义数组中给了三个元素，又给索引是10的位置添加了数据10，那么索引3到索引9位置的元素是什么呢？之前就介绍了，**在JavaScript中没有赋值的话，默认就是 `undefined`** 。如果给 `arr3` 数组添加字符串的数据，也是可以添加成功的：
```js
arr3[5] = "hello";
alert(arr3[5]); // hello
```
### 3.1.4 属性
`Array` 对象提供了很多属性，如下图是官方文档截取的：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302020051261.png)
我们只讲解 `length` 属性，该数组可以动态的获取数组的长度。而有这个属性，我们就可以遍历数组了：
```js
var arr = [1,2,3];
for (let i = 0; i < arr.length; i++) {
	alert(arr[i]);
}
```
### 3.1.5 方法
`Array` 对象同样也提供了很多方法，如下图是官方文档截取的：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302020058583.png)
`push` 函数：给数组添加元素，也就是在数组的末尾添加元素。参数表示要添加的元素：
```js
// push:添加方法
var arr5 = [1,2,3];
arr5.push(10);
alert(arr5); //数组的元素是 {1,2,3,10}
```
`splice` 函数：删除元素。参数1：索引，表示从哪个索引位置删除。参数2：个数，表示删除几个元素。
```js
// splice:删除元素
var arr5 = [1,2,3];
arr5.splice(0,1); //从 0 索引位置开始删除，删除一个元素
alert(arr5); // {2,3}
```

## 3.2 `String` 对象
`String` 对象的创建方式有两种：
```js
var 变量名 = new String(s);
var 变量名 = "数组";
```
**属性**：`String` 对象提供了很多属性，列举了一个属性 `length` ，该属性是用于动态获取字符串的长度：

**函数**：`String` 对象提供了很多函数（方法），下面列举两个方法。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302020102003.png)
`String` 对象还有一个函数 `trim()` ，该方法在文档中没有体现，但是所有的浏览器都支持；它是用来去掉字符串两端的空格。
```js
var str4 = ' abc ';
alert(1 + str4 + 1);
```
上面代码会输出内容 `1 abc 1` ，很明显可以看到 `abc` 字符串左右两边是有空格的。接下来使用 `trim()` 函数，输出的内容是 `1abc1` 。这就是 `trim()` 函数的作用：
```js
var str4 = ' abc ';
alert(1 + str4.trim() + 1);
```
`trim()` 函数在以后开发中还是比较常用的，例如下图所示是登陆界面：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302020106049.png)
用户在输入用户名和密码时，可能会习惯的输入一些空格，这样在我们后端程序中判断用户名和密码是否正确，结果肯定是失败。所以一般都会对用户输入的字符串数据进行去除前后空格的操作。

## 3.3 自定义对象
在JavaScript中自定义对象特别简单，下面就是自定义对象的格式：
```js
var 对象名称 = {
	属性名称1:属性值1,
	属性名称2:属性值2,
	...,
	函数名称:function (形参列表){},
	...
};
```
调用属性的格式：`对象名.属性名` ，调用函数的格式：`对象名.函数名()` 。

接下来通过代码演示一下，体验一下JavaScript中自定义对象：
```js
var person = {
	name : "zhangsan",
	age : 23,
	eat: function (){
		alert("干饭~");
	}
};
alert(person.name); //zhangsan
alert(person.age); //23
person.eat(); //干饭~
```

---
# 4. BOM
*BOM, Browser Object Model* 浏览器对象模型，也就是**JavaScript将浏览器的各个组成部分封装为对象**，我们要操作浏览器的各个组成部分，就可以通过操作BOM中的对象来实现。比如现在想将浏览器地址栏的地址改为 `https://www.itheima.com` ，就可以通过使用BOM中定义的 `Location` 对象的 `href` 属性，代码：`location.href = "https://itheima.com";` 。

BOM中包含了如下对象：
- `Screen` ：屏幕对象
- `Navigator`：浏览器对象（前两个用得不多）
- `Window` ：浏览器窗口对象
- `History` ：历史记录对象
- `Location` ：地址栏对象

下图是BOM中的各个对象和浏览器的各个组成部分的对应关系：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302020111051.png)

BOM 中的 `Navigator` 对象和 `Screen` 对象基本不会使用，所以只对 `Window, History, Location` 对象讲解。
## 4.1 `Window` 对象
`window` 对象是JavaScript对浏览器的窗口进行封装的对象。
## 4.1.1 获取 `window` 对象
该对象不需要创建，**可直接使用 `window`** ，其中 `window.` 可以省略。比如之前使用的 `alert()` 函数，其实就是 `window` 对象的函数，在调用是可以写成如下两种
```js
// 显式使用 window 对象调用
window.alert("abc");
// 隐式调用
alert("abc")
```
### 4.1.2 `window` 对象属性
`window` 对象提供了用于**获取其他BOM组成对象**的属性：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302020114558.png)
也就是说，我们想使用 `Location` 对象的话，就可以使用 `window` 对象获取；写成 `window.location` ，而 `window.` 可以省略，**简化写成 `location` 来获取 `Location` 对象**。
### 4.1.3 `window` 对象函数
`window` 对象提供了很多函数供我们使用，而很多都不常用；下面给大家列举了一些比较常用的函数：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302020115512.png)
`confirm` 代码演示：
```js
// confirm()，点击确定按钮，返回true，点击取消按钮，返回false
var flag = confirm("确认删除？");
alert(flag);
```
下图是 `confirm()` 函数的效果。当我们点击确定按钮，`flag` 变量值记录的就是 `true` ；当我们点击取消按钮，`flag` 变量值记录的就是 `false` 。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302020120170.png)
而以后我们在页面删除数据时，如下图每一条数据后都有删除按钮，有可能是用户的一些误操作，所以对于删除操作需要用户进行再次确认，此时就需要用到 `confirm()` 函数：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302020120533.png)

定时器有两个函数：
- `setTimeout(function,毫秒值)` : 在一定的时间间隔后执行一个 `function` ，**只执行一次** 
- `setInterval(function,毫秒值)` :在一定的时间间隔后执行一个 `function` ，**循环执行**

定时器代码演示。当我们打开浏览器，3秒后才会弹框输出 `hehe` ，并且只会弹出一次：
```js
setTimeout(function (){
	alert("hehe");
}, 3000);
```
当我们打开浏览器，每隔2秒都会弹框输出 `hehe` ：
```js
setInterval(function (){
	alert("hehe");
}, 2000);
```
### 4.1.4 案例
需求说明：有如下页面效果，实现定时进行开灯、关灯功能。**需求：每隔1秒，灯泡切换一次状态**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302020123420.png)

注意，`<img>` 的 `border` 属性被废弃，以后要用CSS的 `border` 来替代：
```html
<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <title>JavaScript演示</title>
    </head>
    <body>
        <input type="button" onclick="on()" value="开灯">
        <img id="myImage" border="0" src="off.gif" style="text-align:center;">
        <input type="button" value="关灯" onclick="off()">
        <script>
            function on() {
                document.getElementById('myImage').src = "on.gif";
            }
            function off() {
                document.getElementById('myImage').src = "off.gif";
            }
            // 定义一个变量,记录灯的状态,偶数时开灯,奇数时关灯
            var x = 0;
            // 使用循环定时器
            setInterval(function() {
                if (x % 2 == 0) { // 偶数,开灯状态,调用on()
                    on();
                } else { // 奇数,关灯,调用off()
                    off();
                }
                ++x; // 改变变量的值
            }, 1000);  
        </script>
    </body>
</html> 
```

## 4.2 `History` 对象
`History` 对象是JavaScript对历史记录进行封装的对象。`History` 对象的获取
使用 `window.history` 获取，其中 `window.` 可以省略。`History` 对象的函数：
- `back()` 加载 `History` 列表中前一个URL
- `forward()` 加载 `History` 列表中下一个URL

这两个函数我们平时在访问其他一些网站时经常使用对应的效果。当我们点击向左的箭头，就跳转到前一个访问的页面，这就是 `back()` 函数的作用；当我们点击向右的箭头，就跳转到下一个访问的页面，这就是 `forward()` 函数的作用。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302040126829.png)

## 4.3 `Location` 对象
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302041325045.png)
`Location` 对象是JavaScript对地址栏封装的对象。**可以通过操作该对象，跳转到任意页面**。
### 4.3.1 获取 `Location` 对象
使用 `window.location` 获取，其中 `window.` 可以省略：
```js
window.location.方法();
location.方法();
```
### 4.3.2 `Location` 对象属性
`Location` 对象提供了很多属性。以后常用的只有一个属性 `href` ——可以设置或返回完整的URL。代码演示：
```js
alert("要跳转了");
location.href = "https://www.baidu.com";
```
在浏览器首先会弹框显示 `要跳转了` ，当我们点击了 `确定` 就会跳转到百度的首页。
### 4.3.3 案例：3秒跳转到百度首页
分析：
1. 3秒跳转，由此可以确定需要使用到定时器，而只跳转一次，所以使用 `setTimeOut()`
2. 要进行页面跳转，所以需要用到 `location` 对象的 `href` 属性实现

代码实现：
```js
document.write("3秒跳转到首页...");
setTimeout(function () {
	location.href = "https://www.baidu.com"
}, 3000);
```

---
# 5. DOM
## 5.1 概述
*DOM, Document Object Model* 文档对象模型。也就是**JavaScript将HTML文档的各个组成部分封装为对象**。

DOM其实我们并不陌生，之前在学习XML就接触过，只不过**XML文档中的标签需要我们写代码解析**，而**HTML文档是浏览器解析**。封装的对象分为：
- `Document` ：整个文档对象
- `Element` ：元素对象
- `Attribute` ：属性对象
- `Text` ：文本对象
- `Comment` ：注释对象

如下图，左边是HTML文档内容，右边是DOM树：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302041331324.png)
作用：JavaScript通过DOM， 就能够对HTML进行操作了。
- 改变HTML元素的**内容**
- 改变HTML元素的**样式**（CSS）
- **对HTML DOM事件作出反应**
- **添加和删除HTML元素**

**DOM相关概念**：
DOM是W3C（万维网联盟）定义的访问HTML和XML文档的标准。该标准被分为 3 个不同的部分：
1. 核心DOM：**针对任何结构化文档的标准模型**。XML和HTML通用的标准：
	- `Document` ：整个文档对象
	- `Element` ：元素对象
	- `Attribute` ：属性对象
	- `Text` ：文本对象
	- `Comment` ：注释对象
2. XML DOM： 针对XML文档的标准模型
3. HTML DOM： 针对HTML文档的标准模型。**该标准是在核心DOM基础上，对「HTML中的每个标签」都封装成了不同的对象**。
	- 例如： `<img>` 标签在浏览器加载到内存中时会被封装成 `Image` 对象，同时该对象也是 `Element` 对象。
	- 例如： `<input type='button'>` 标签在浏览器加载到内存中时，会被封装成 `Button` 对象，同时该对象也是 `Element` 对象。

## 5.2 获取 `Element` 对象
HTML中的 `Element` 对象可以通过 `Document` 对象获取，而 **`Document` 对象是通过 `window` 对象获取**。

`Document` 对象中提供了以下获取 `Element` 元素对象的函数：
- `getElementById()` ：根据 `id` 属性值（不能重复）获取，返回单个 `Element` 对象
- `getElementsByTagName()` ：根据标签名称获取，返回 `Element` 对象数组
- `getElementsByName()` ：根据 `name` 属性值获取，返回 `Element` 对象数组
- `getElementsByClassName()` ：根据 `class` 属性值获取，返回 `Element` 对象数组

代码演示：下面有提前准备好的页面。
```html
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Title</title>
	</head>
	<body>
		<img id="light" src="off.gif"> <br>
		<div class="cls">传智教育</div> <br>
		<div class="cls">黑马程序员</div> <br>
		<input type="checkbox" name="hobby"> 电影
		<input type="checkbox" name="hobby"> 旅游
		<input type="checkbox" name="hobby"> 游戏
		<br>
		<script>
			//在此处书写js代码
		</script>
	</body>
</html>
```
根据 `id` 属性值获取上面的 `img` 元素对象，返回单个对象：
```js
var img = document.getElementById("light");
alert(img);
```
结果如下，从弹框输出的内容，也可以看出是一个图片元素对象。：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302041339226.png)
根据标签名称获取所有的 `div` 元素对象：
```js
var divs = document.getElementsByTagName("div");// 返回一个数组，数组中存储的是div元素对象
// alert(divs.length); //输出 数组的长度
//遍历数组
for (let i = 0; i < divs.length; i++) {
	alert(divs[i]);
}
```
获取所有的满足 `name = 'hobby'` 条件的元素对象：
```js
// getElementsByName：根据name属性值获取，返回Element对象数组
var hobbys = document.getElementsByName("hobby");
for (let i = 0; i < hobbys.length; i++) {
	alert(hobbys[i]);
}
```
获取所有的满足 `class='cls'` 条件的元素对象：
```js
// getElementsByClassName：根据class属性值获取，返回Element对象数组
var clss = document.getElementsByClassName("cls");
for (let i = 0; i < clss.length; i++) {
	alert(clss[i]);
}
```

## 5.3 HTML Element对象使用
HTML 中的 `Element` 元素对象有很多，不可能全部记住，以后是根据具体需求查阅文档使用。下面通过具体案例演示文档的查询和对象的使用；提前准备好的页面jianjia

需求：点亮灯泡。此案例由于需要改变 `img` 标签的图片，所以我们查询文档，下图是查看文档的流程：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302041343973.png)
代码实现：
```js
//1. 根据 id='light' 获取 img 元素对象
var img = document.getElementById("light");
//2. 修改 img 对象的 src 属性来改变图片
img.src = "on.gif";
```

需求：将所有的 `div` 标签的标签体内容替换为 `呵呵`（`div` 对象没有属性，但继承了 `Element` 对象的各种属性）。
```js
//1. 获取所有的 div 元素对象
var divs = document.getElementsByTagName("div");
/* 
 * style:设置元素css样式
 * innerHTML：设置元素内容
 */
//2. 遍历数组，获取到每一个 div 元素对象，并修改元素内容
for (let i = 0; i < divs.length; i++) {
	// divs[i].style.color = 'red';
	divs[i].innerHTML = "呵呵";
}
```

需求：使所有的复选框呈现被选中的状态。此案例我们需要看 `复选框` 元素对象有什么属性或者函数是来操作「复选框的选中状态」。下图是文档查看：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302041347180.png)
代码实现：
```js
//1. 获取所有的 复选框 元素对象
var hobbys = document.getElementsByName("hobby");
//2. 遍历数组，通过将 复选框 元素对象的 checked 属性值设置为 true 来改变复选框的选中状态
for (let i = 0; i < hobbys.length; i++) {
	hobbys[i].checked = true;
}
```

---
# 6. 事件监听
要想知道什么是事件监听，首先先聊聊什么是事件？HTML事件是发生在HTML元素上的“事情”。比如：页面上的 **按钮被点击** 、 **鼠标移动到元素之上** 、 **按下键盘按键** 等都是事件。

<b><font color="red">事件监听是JavaScript可以在事件被侦测到时执行一段逻辑代码</font></b>。例如下图当我们点击开灯按钮，就需要通过JS代码实现替换图片：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302041349834.png)

再比如下图输入框，当我们输入了用户名后 `光标离开输入框` ，就需要通过JS代码对输入的内容进行校验，没通过校验就在输入框后提示用户名格式有误！
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302041350552.png)

## 6.1 事件绑定
JavaScript提供了两种事件绑定方式：**方式一是通过HTML标签中的事件属性进行绑定**（耦合在一起）。如下面代码，有一个按钮元素，我们是在该标签上定义 `事件属性` ，在事件属性中绑定函数。`onclick` 就是单击事件的事件属性。`onclick='on()'` 表示该点击事件绑定了一个名为 `on()` 的函数。`
```html
<input type="button" onclick='on()’>
```
下面是点击事件绑定的 `on()` 函数：
```js
function on(){
	alert("我被点了");
}
```
**方式二是通过DOM元素属性绑定**，推荐使用。如下面代码是按钮标签，在该标签上我们并没有使用事件属性，绑定事件的操作需要在JS代码中实现：
```html
<input type="button" id="btn">
```
下面JS代码是**获取了 `id='btn'` 的元素对象**，然后**将 `onclick` 作为该对象的属性，并且绑定匿名函数**。该函数是在事件触发后自动执行：
```js
document.getElementById("btn").onclick = function (){
	alert("我被点了");
}
```

## 6.2 常见事件
上面案例中使用到了 `onclick` 事件属性，那都有哪些事件属性供我们使用呢？下面就列举一些比较常用的事件属性：

| 事件属性名 | 说明| 
|:---|:---|
| `onclick` | 鼠标单击事件 |
| `onblur`  | 元素失去焦点 |
| `onfocus` | 元素获得焦点 | 
| `onload` |某个页面或图像被完成加载 |
| `onsubmit` | 当表单提交时触发该事件 |
| `onmouseover` |  鼠标被移到某元素之上 |
| `onmouseout` | 鼠标从某元素移开 |

`onfocus` 获得焦点事件。如下图，当点击了输入框后，输入框就获得了焦点。而下图示例是当获取焦点后会更改输入框的背景颜色。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302041402376.png)

`onblur` 失去焦点事件。如下图，当点击了输入框后，输入框就获得了焦点；再点击页面其他位置，那输入框就失去焦点了。下图示例是将输入的文本转换为大写。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302041406838.png)

`onmouseout` 鼠标移出事件；`onmouseover` 鼠标移入事件。如下图，当鼠标移入到苹果图片上时，苹果图片变大；当鼠标移出苹果图片时，苹果图片变小。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302041413916.png)

`onsubmit` 表单提交事件。如下是带有表单的页面：
```html
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Title</title>
	</head>
	<body>
		<form id="register" action="#" >
			<input type="text" name="username" />
			<input type="submit" value="提交">
		</form>
		<script>
		
		</script>
	</body>
</html>
```
如上代码的表单，当我们点击提交按钮后，表单就会提交，此处默认使用的是 `GET` 提交方式，会将提交的数据拼接到URL后。现需要**通过JS代码实现阻止表单提交（未通过验证）的功能**，代码实现如下：
1. 获取 `form` 表单元素对象。
2. 给 `form` 表单元素对象绑定 `onsubmit` 事件，并绑定匿名函数。
3. 该匿名函数如果返回的是 `true` ，提交表单；如果返回的是 `false` ，阻止表单提交。

```js
document.getElementById("register").onsubmit = function () {
	// onsubmit 返回true，则表单会被提交，返回false，则表单不提交
	return true;
}
```

---
# 7. 表单验证案例
## 7.1 需求
有如下注册页面，对表单进行校验，如果输入的用户名、密码、手机号符合规则，则允许提交；如果不符合规则，则不允许提交。
![img|350x400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302041426982.png)
完成以下需求：
1. **当输入框失去焦点时，验证输入内容是否符合要求**
2. 当点击注册按钮时，**判断所有输入框的内容是否都符合要求**，如果不符合则阻止表单提交

## 7.2 环境准备
下面是初始页面：
```html
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>欢迎注册</title>
		<link href="../css/register.css" rel="stylesheet">
	</head>
	<body>
		<div class="form-div">
			<div class="reg-content">
				<h1>欢迎注册</h1>
				<span>已有帐号？</span> <a href="#">登录</a>
			</div>
			<form id="reg-form" action="#" method="get">
				<table>
					<tr>
						<td>用户名</td>
						<td class="inputs">
							<input name="username" type="text" id="username">
							<br>
							<span id="username_err" class="err_msg" style="display: none">用户名不太受欢迎</span> <!-- 没有显示 -->
						</td>
					</tr>
					<tr>
						<td>密码</td>
						<td class="inputs">
							<input name="password" type="password" id="password">
							<br>
							<span id="password_err" class="err_msg" style="display: none">密码格式有误</span>
						</td>
					</tr>
					<tr>
						<td>手机号</td>
						<td class="inputs">
							<input name="tel" type="text" id="tel">
							<br>
							<span id="tel_err" class="err_msg" style="display: none">手机号格式有误</span>
						</td>
					</tr>
				</table>
				<div class="buttons">
					<input value="注 册" type="submit" id="reg_btn">
				</div>
				<br class="clear">
			</form>
		</div>
		<script>
		
		</script>
	</body>
</html>
```
## 7.3 验证输入框
此小节完成如下功能：
- 校验用户名。当用户名输入框失去焦点时，判断输入的内容是否符合 **长度是6-12位** 规则，不符合则让 `id='username_err'` 的 `span` 标签显示出来，给出用户提示。
- 校验密码。当密码输入框失去焦点时，判断输入的内容是否符合 **长度是6-12 位** 规则，不符合使 `id='password_err'` 的 `span` 标签显示出来，给出用户提示。
- 校验手机号。当手机号输入框失去焦点时，判断输入的内容是否符合 **长度是11位** 规则，不符合使 `id='tel_err'` 的 `span` 标签显示出来，给出用户提示。

代码如下：
```js
//1. 验证用户名是否符合规则
//1.1 获取用户名的输入框
var usernameInput = document.getElementById("username");
//1.2 绑定onblur事件 失去焦点
usernameInput.onblur = function () {
	//1.3 获取用户输入的用户名
	var username = usernameInput.value.trim();
	//1.4 判断用户名是否符合规则：长度 6~12
	if (username.length >= 6 && username.length <= 12) {
		//符合规则
		document.getElementById("username_err").style.display = 'none';
	} else {
		//不符合规则
		document.getElementById("username_err").style.display = '';
	}
}
//1. 验证密码是否符合规则
//1.1 获取密码的输入框
var passwordInput = document.getElementById("password");
//1.2 绑定onblur事件 失去焦点
passwordInput.onblur = function() {
	//1.3 获取用户输入的密码
	var password = passwordInput.value.trim();
	//1.4 判断密码是否符合规则：长度 6~12
	if (password.length >= 6 && password.length <= 12) {
		//符合规则
		document.getElementById("password_err").style.display = 'none';
	} else {
		//不符合规则
		document.getElementById("password_err").style.display = '';
	}    
}
//1. 验证手机号是否符合规则
//1.1 获取手机号的输入框
var telInput = document.getElementById("tel");
//1.2 绑定onblur事件 失去焦点
telInput.onblur = function() {
	//1.3 获取用户输入的手机号
	var tel = telInput.value.trim();
	//1.4 判断手机号是否符合规则：长度 11
	if (tel.length == 11) {
		//符合规则
		document.getElementById("tel_err").style.display = 'none';
	} else {
		//不符合规则
		document.getElementById("tel_err").style.display = '';
	}
}
```
## 7.3 验证表单
当用户点击注册按钮时，需要同时对输入的用户名、密码、手机号，如果都符合规则，则提交表单；如果有一个不符合规则，则不允许提交表单。实现该功能需要获取表单元素对象，并绑定 `onsubmit` 事件
```js
//1. 获取表单对象
var regForm = document.getElementById("reg-form");
//2. 绑定onsubmit事件
regForm.onsubmit = function () {

}
```
`onsubmit` 事件绑定的函数需要对输入的用户名、密码、手机号进行校验，这些校验我们之前都已经实现过了，这里我们还需要再校验一次吗？不需要，==只需要对之前校验的代码进行改造，把每个校验的代码专门抽象到有名字的函数中，方便调用==；并且每个函数都要返回结果、去决定是提交表单或阻止表单提交，代码如下：
```js
//1. 验证用户名是否符合规则
//1.1 获取用户名的输入框
var usernameInput = document.getElementById("username");
//1.2 绑定onblur事件 失去焦点
usernameInput.onblur = checkUsername;
function checkUsername() {
	//1.3 获取用户输入的用户名
	var username = usernameInput.value.trim();
	//1.4 判断用户名是否符合规则：长度 6~12
	var flag = username.length >= 6 && username.length <= 12;
	if (flag) {
		//符合规则
		document.getElementById("username_err").style.display = 'none';
	} else {
		//不符合规则
		document.getElementById("username_err").style.display = '';
	}
	return flag;
}
//1. 验证密码是否符合规则
//1.1 获取密码的输入框
var passwordInput = document.getElementById("password");
//1.2 绑定onblur事件 失去焦点
passwordInput.onblur = checkPassword;
function checkPassword() {
	//1.3 获取用户输入的密码
	var password = passwordInput.value.trim();
	//1.4 判断密码是否符合规则：长度 6~12
	var flag = password.length >= 6 && password.length <= 12;
	if (flag) {
		//符合规则
		document.getElementById("password_err").style.display = 'none';
	} else {
		//不符合规则
		document.getElementById("password_err").style.display = '';
	}
	return flag;
}
//1. 验证手机号是否符合规则
//1.1 获取手机号的输入框
var telInput = document.getElementById("tel");
//1.2 绑定onblur事件 失去焦点
telInput.onblur = checkTel;
function checkTel() {
	//1.3 获取用户输入的手机号
	var tel = telInput.value.trim();
	//1.4 判断手机号是否符合规则：长度 11
	var flag = tel.length == 11;
	if (flag) {
		//符合规则
		document.getElementById("tel_err").style.display = 'none';
	} else {
		//不符合规则
		document.getElementById("tel_err").style.display = '';
	}
	return flag;
}
```
而 `onsubmit` 绑定的函数需要调用 `checkUsername()` 函数、`checkPassword()` 函数、`checkTel()` 函数。
```js
//1. 获取表单对象
var regForm = document.getElementById("reg-form");
//2. 绑定onsubmit 事件
regForm.onsubmit = function () {
	//挨个判断每一个表单项是否都符合要求，如果有一个不符合，则返回false
	var flag = checkUsername() && checkPassword() && checkTel();
	return flag;
}
```
---
# 8. `RegExp` 对象
`RegExp` 是正则对象。正则对象是判断指定字符串是否符合规则。如下图是百度贴吧中的帖子：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302041429720.png)
可以通过爬虫技术去爬取该页面源代码，然后获取页面中所有的邮箱，后期可给这些邮箱地址发送推广邮件。那么问题来了，如何才能知道页面内容中哪些是邮箱地址呢？这里可使用正则表达式来匹配邮箱。

在JS中对正则表达式封装的对象就是正则对象。
## 8.1 正则对象使用
### 8.1.1 创建对象
正则对象有两种创建方式——直接方式，注意不要加引号：
```js
var reg = /正则表达式/;
var reg = /^\w{6,12}$/;
```
或创建 `RegExp` 对象：
```js
var reg = new RegExp("正则表达式");
var reg = new RegExp("^\\w{6,12}$");
```
### 8.1.2 函数
`test(str)` ：判断指定字符串是否符合规则，返回 `true` 或 `false` 。

## 8.2 正则表达式
从上面创建正则对象的格式中，可以看出不管哪种方式都需要正则表达式，那么什么是正则表达式呢？

**正则表达式定义了字符串组成的规则**。也就是判断指定的字符串是否符合指定的规则，如果符合返回 `true` ，如果不符合返回 `false` 。

正则表达式是和语言无关的。很多语言都支持正则表达式，Java语言也支持，只不过正则表达式在不同语言中的使用方式不同，JS中需要使用正则对象来使用正则表达式。

正则表达式常用的规则如下：
- `^` ：表示开始
- `$` ：表示结束
- `[]` ：代表某个范围内的单个字符，比如：`[0-9]` 单个数字字符
- `.` ：**代表任意单个字符，除了换行和行结束符**
- `\w` ：代表单词字符：字母、数字、下划线 `_` ，相当于 `[A-Za-z0-9]`
- `\d` ：代表数字字符： 相当于 `[0-9]`

量词：
- `+` ：至少一个
- `*` ：零个或多个
- `?` ：零个或一个
- `{x}` ：`x` 个
- `{m,}` ：至少 `m` 个
- `{m,n}` ：至少 `m` 个，最多 `n` 个

代码演示：
```js
// 规则：单词字符，6~12
//1. 创建正则对象，对正则表达式进行封装
var reg = /^\w{6,12}$/;
var str = "abcccc";
//2. 判断 str 字符串是否符合 reg 封装的正则表达式的规则
var flag = reg.test(str);
alert(flag);
```

## 8.3 改进表单校验案例
表单校验案例中的规则是通过一系列判断来实现的，现在学习了正则对象后，就可以使用正则对象来改进这个案例。
```html
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>欢迎注册</title>
		<link href="../css/register.css" rel="stylesheet">
	</head>
	<body>
		<div class="form-div">
			<div class="reg-content">
				<h1>欢迎注册</h1>
				<span>已有帐号？</span> <a href="#">登录</a>
			</div>
			<form id="reg-form" action="#" method="get">
				<table>
					<tr>
						<td>用户名</td>
						<td class="inputs">
							<input name="username" type="text" id="username">
							<br>
							<span id="username_err" class="err_msg" style="display: none">用户名不太受欢迎</span> <!-- 没有显示 -->
						</td>
					</tr>
					<tr>
						<td>密码</td>
						<td class="inputs">
							<input name="password" type="password" id="password">
							<br>
							<span id="password_err" class="err_msg" style="display: none">密码格式有误</span>
						</td>
					</tr>
					<tr>
						<td>手机号</td>
						<td class="inputs">
							<input name="tel" type="text" id="tel">
							<br>
							<span id="tel_err" class="err_msg" style="display: none">手机号格式有误</span>
						</td>
					</tr>
				</table>
				<div class="buttons">
					<input value="注 册" type="submit" id="reg_btn">
				</div>
				<br class="clear">
			</form>
		</div>
		<script>
			//1. 验证用户名是否符合规则
			//1.1 获取用户名的输入框
			var usernameInput = document.getElementById("username");
			//1.2 绑定onblur事件 失去焦点
			usernameInput.onblur = checkUsername;
			function checkUsername() {
				//1.3 获取用户输入的用户名
				var username = usernameInput.value.trim();
				//1.4 判断用户名是否符合规则：长度6~12,单词字符组成
				var reg = /^\w{6,12}$/;
				var flag = reg.test(username);
				//var flag = username.length >= 6 && username.length <= 12;
				if (flag) {
					//符合规则
					document.getElementById("username_err").style.display = 'none';
				} else {
					//不符合规则
					document.getElementById("username_err").style.display = '';
				}
				return flag;
			}
			//1. 验证密码是否符合规则
			//1.1 获取密码的输入框
			var passwordInput = document.getElementById("password");
			//1.2 绑定onblur事件 失去焦点
			passwordInput.onblur = checkPassword;
			function checkPassword() {
				//1.3 获取用户输入的密码
				var password = passwordInput.value.trim();
				//1.4 判断密码是否符合规则：长度 6~12
				var reg = /^\w{6,12}$/;
				var flag = reg.test(password);
				//var flag = password.length >= 6 && password.length <= 12;
				if (flag) {
					//符合规则
					document.getElementById("password_err").style.display = 'none';
				} else {
					//不符合规则
					document.getElementById("password_err").style.display = '';
				}
				return flag;
			}
			//1. 验证手机号是否符合规则
			//1.1 获取手机号的输入框
			var telInput = document.getElementById("tel");
			//1.2 绑定onblur事件 失去焦点
			telInput.onblur = checkTel;
			function checkTel() {
				//1.3 获取用户输入的手机号
				var tel = telInput.value.trim();
				//1.4 判断手机号是否符合规则：长度 11，数字组成，第一位是1
				//var flag = tel.length == 11;
				var reg = /^[1]\d{10}$/;
				var flag = reg.test(tel);
				if (flag) {
					//符合规则
					document.getElementById("tel_err").style.display = 'none';
				} else {
					//不符合规则
					document.getElementById("tel_err").style.display = '';
				}
				return flag;
			}
			//1. 获取表单对象
			var regForm = document.getElementById("reg-form");
			//2. 绑定onsubmit事件
			regForm.onsubmit = function() {
				//挨个判断每一个表单项是否都符合要求，如果有一个不符合，则返回false
				var flag = checkUsername() && checkPassword() && checkTel();
				return flag;
			}
		</script>
	</body>
</html>
```

---
# 第一部分 JavaScript基础
第一部分主要介绍JavaScript的基础知识，以及JavaScript如何与HTML和CSS协同工作。

[【ECMAScript】第一部分 JavaScript基础(1) JavaScript简介、使用方法、在Chrome中运行和输出](https://memcpy0.blog.csdn.net/article/details/121125488)
[【ECMAScript】第一部分 JavaScript基础(1) 语法、语句、注释]
[【ECMAScript】第一部分 JavaScript基础(1) 变量、数据类型、类型转换、对象
[【ECMAScript】第一部分 JavaScript基础(1) 函数、函数定义、函数参数、函数调用、闭包]
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript 作用域
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript 事件
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript 字符串
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript 运算符
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript 比较
[【ECMAScript】第一部分 JavaScript基础(1) 控制流（条件/switch语句、 for/while循环、break/continue 语句
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript typeof 
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript 正则表达式
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript 错误
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript 调试
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript 变量提升
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript 严格模式
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript 使用误区
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript 表单
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript 表单验证
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript 验证 API
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript 保留关键字
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript this
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript let 和 const
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript JSON
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript void
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript 异步编程
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript Promise
[【ECMAScript】第一部分 JavaScript基础(1) JavaScript 代码规范

 
# 第二部分 JavaScript与HTML DOM、浏览器DOM
## JS HTML DOM
DOM 简介
DOM HTML
DOM CSS
DOM 事件
DOM EventListener
DOM 元素
HTMLCollection 对象
NodeList 对象
## JS 浏览器BOM
JavaScript Window
JavaScript Window Screen
JavaScript Window Location
JavaScript Window History
JavaScript Navigator
JavaScript 弹窗
JavaScript 计时事件
JavaScript Cookie

# 第三部分 JavaScript进阶
## JS 高级教程
JavaScript 对象
JavaScript prototype
JavaScript Number 对象
JavaScript String
JavaScript Date（日期）
JavaScript Array（数组）
JavaScript Boolean（布尔）
JavaScript Math（算数）
JavaScript RegExp 对象
## JS 库
JavaScript 库
JavaScript 测试 jQuery
JavaScript 测试 Prototype

## JS 实例
JavaScript 实例
JavaScript 对象实例
JavaScript 浏览器对象实例
JavaScript HTML DOM 实例
JavaScript 总结

## JS 参考手册
JavaScript 对象
HTML DOM 对象
JavaScript 异步编程


# 第四部分 NodeJS

# 第五部分 TypeScript






 

---


