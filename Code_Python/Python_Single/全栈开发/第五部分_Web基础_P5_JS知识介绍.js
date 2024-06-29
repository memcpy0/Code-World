// 单行注释
/*
** 多行注释
*/

/* http://www.cnblogs.com/liwenzhou/p/8004649.html
** 用分号作为语句的结束符
** JS代码的引入
** 1. 将代码写在script标签中
** 2. 通过script的src属性引入
*/

/*
1. JavaScript语言基础
变量声明
  可以使用_, 数字, 字母, $组成，不能以数字开头
  声明变量用var 变量名的格式来进行声明
  变量名是区分大小写的 保留字不能用做变量名
  推荐使用驼峰式命名
*/
var firstName="Alex";
alert(firstName);
const PI=3.1415;
alert(PI)

/*
2. JavaScript数据类型
JavaScript拥有动态类型
    1. 数值类型(Number) JavaScript不区分整型和浮点型
    2. NaN --> Not a Number 也是数值类型
    3. 字符串类型(String) "" 或''
       模板字符串   `${}...`
       将字符型的数值转换为数值类型
       parseInt("100") --> 100  但会截断浮点型数值
       parseFloat("100.11") --> 100.11

    5. 布尔值 true false <-- 0 NaN "" undefined null
    6. 对象
        1. 数组 [1, 2, 3, 4, 5]
            .length     .push(num) .pop() .unshift(num) .shift()
            .slice(from, ro)  .reverse()
            .join(seq) 用seq连接前面的数组  .concat()
            .sort() sort有坑
            .splice(start, stop, replace) 删除返回替换
            .map()  返回一个数组元素调用函数处理后的新数组
            遍历一个数组
            .forEach()
        2. null
    7. typeof

*/
var x; // x是undefined
var x=1; // 此时是数字
var x="Alex"; // 此时是字符串

/*
3. 常用字符串方法
    .anchor
    .big
    .blink
    .bold
    .charAt(n)                      返回第n个字符
    .charCodeAt()
    .concat(value,...)              拼接 一般用+
    .constructor()

    .endsWith()
    .fixed
    .fontcolor
    .fontsize

    .includes
    .indexOf(substring, start)      从start找子序列的位置
    .italics
    .lastIndexOf
    .link
    .localeCompare
    .length                         返回长度

    .match
    .normalize
    .padEnd
    .padStart
    .repeat
    .replace
    .search
    .slice(start, end)              切片
    .small
    .split(delimiter, limit)        分割
    .startsWith()
    .strike()
    .sub
    .substr
    .substring(from, to)            根据索引获取子序列
    .sup

    .toLocaleLowerCase
    .toLocaleUpperCase
    .toLowerCase()                  小写
    .toUpperCase()                  大写
    .toString
    .toLocaleString()

    .trim()                         清除左右空白
    .trimEnd()
    .trimLeft()                     清除左空白
    .trimRight()                    清除右空白
    .trimStart()

    .valueof
    .hasOwnProperty
    .isPrototypeOf
    .propertyIsEnumerable
*/

/*
4. 运算符
算术运算符 - + * / % ++ --
比较运算符 == 弱等于
         === 强等于
         !=  弱不等于
         !== 强不等于
    > >= < <=
逻辑运算符 &&  与  || 或  ! 非
赋值运算符 = -= += *= /=

5. 流程控制
条件语句和C语言一样
if ( ) {

} else if {

} else {

}

switch(){
case 1:  do-something; break;
case 2:  do-something; break;
case 3:  do-something; break;
default:
}

for(var i=0; i<10; i++){
    do-something;
}

while(){
    do-something;
}

三元运算
c = a > b ? a : b
*/

/*
6. 函数
function foo(a, b){
    return a-b;
}

匿名函数
var f=function(){console.log(123);}

自执行函数 一次性的
(function(a, b){return a+b;})(10, 20);

箭头函数 =>

函数嵌套 -> 函数闭包

arguments

激活对象AO 词法分析 Active Object AO
1. 函数参数 AO.func_argument = undefined
2. 局部变量 AO.func_local_argument = undefined
3. 函数定义 AO_local_func = undefined
函数内部无论使用函数参数还是局部变量都是从AO上面找
*/

/*
7. 自定义对象和面向对象介绍
let s = new String("111"); new Object(); Map()
< undefined
s
< String {"111"}
typeof s
< "object"

自定义对象 只有字符串可以做key
let ol = {"a":100, "b":200};
for(var i in ol){console.log(i);}
用key来取值 ol.a  ol["a"]

定义类
# 1.构造函数
var Person = function(dream){
    this.dream = dream;
};
var p1 = new Person("OK");
p1.dream
< "OK"

# 2. 方法定义
Person.prototype.makeDream = function(){
    console.log("做梦");
}
var p2 = new Person("OJBK");
p2.makeDream();
< "做梦"

# 3.继承
# 创造一个子类构造函数
var Yellow=function(dream){
    Person.call(this, dream);
}
# 继承父类
Yellow.prototype = Object.create(Person.prototype);
# 修复子类的构造函数
Yellow.prototype.constructor = Yellow;
# 定义子类独有的方法
Yellow.prototype.sing = function(){
    console.log("龙的传人");
}

*/

/*
8. 内置对象
    1. Date对象
       var d = new Date();
       Date对象方法: (获取Date对象创建的时候的一些时间)
      * getDate()          获取日
      * getDay()           获取星期
      * getMonth()         获取月(0-11)
      * getFullYear()      获取年
      * getYear()          获取年(1900)
      * getHours()         获取时间
      * getMinutes()       获取分钟
      * getSeconds()       获取秒
      * getMilliseconds()  获取毫秒
      * getTime()          获取累计毫秒数(1970)

     2. Json对象
        JSON.stringify(o1); --> 序列化 把js的对象转换为JSON格式的字符串
        JSON.parse(s1);     --> 反序列化 把JSON格式的字符串转换为JS中的对象

     3. RegExp对象
     两种创建方式
        1. var r1 = new RegExp('^1[3-9]\\d{9}$');
           r1.test("18812341234");

        2. 简写 /^1[3-9]\\d{9}$/.test("18812341234")

     .test(字符串) test()中不传值相当于传入了一个"undefined"
     JS的正则表达式中不要加空格
     匹配模式 /regexp/i 忽略大小写
             /regexp/g 全局 多个 注意有一个lastIndex属性记录
     4. Math对象
     Math.abs()
     Math.floor()
     Math.max()
     Math.pow()
     Math.random()
     Math.round()
     Math.sin(x)
     Math.sqrt()
     Math.tan(x)
*/

/*
// http://cnblogs.com/liwenzhou/
9. BOM对象操作 Browser Object Model

   1. window对象时客户端JavaScript的最高层对象之一，可以省略
   如window.document.write() --> document.write()
      所有的浏览器都支持window，它表示浏览器窗口
      所有的JavaScript全局对象、函数以及变量均自动成为window对象的成员
      全局变量是window对象的属性，全局函数是window对象的方法

      window.innerHeight 当前窗口高度
      window.innerWidth  当前窗口宽度
      window.open()      开启新窗口
      window.close()     关闭当前窗口
      window.onload      文档加载完成后执行

   2. navigator对象
   浏览器对象，通过这个对象可以判定用户使用的浏览器，包含了浏览器相关信息
      navigator.appName          web浏览器全称
      navigator.appVersion       web浏览器厂商和版本的详细字符串
      navigator.userAgent        客户端绝大部分信息
      navigator.platform         浏览器运行所在的操作系统

   3. screen对象 屏幕对象，不常用
      screen.availWidth      可用的屏幕宽度
      screen.availHeight     可用的屏幕高度

   4. history对象 window.history对象包含浏览器的历史
      history.forward()  前进一页
      history.back()     后退一页

   5. location对象
   window.location对象可用于获取当前页面的URL地址，
   并可用来将浏览器重定向到新的页面
      location.href       获取URL
      location.href="URL" 调转到指定页面
      location.reload()   重新加载页面

   6. 弹出框
   可用在JavaScript中创建三种消息框: 警告框 确认框 提示框
      alert(msg)
      confirm(msg) --> true false
      prompt(msg1, msg2) --> 返回新的信息, 不然返回msg2，取消返回null

   7.计时相关
   在一段时间间隔后执行
      setTimeout(function(){alert(123);}, milliseconds);
      可以传进匿名函数或函数对象 当指定的毫秒到的时候执行
      清除 clearTimeout(定时器对象);
   每隔一段时间执行
      setInterval(function, milliseconds);
      清除 clearInterval(定时器对象);

10. DOM对象操作 查找标签 插入标签
    Document Object Model
DOM标准规定HTML文档中的每一个成分都是一个节点node
    document对象  文档节点 代表整个文档
    element对象   元素节点 代表一个元素标签
    text对象      文本节点 代表元素中的文本
    attribute对象 属性节点 代表一个属性(元素标签才有属性)

    1. 查找标签
        document.getElementById() 通过ID找到标签
            * .innerText
        document.getElementsByClassName() 根据class属性获取
        document.getElementsBYTagName()   根据标签名获取标签合集

        parentElement           父标签元素
        children                所有子标签元素
        firstElementChild       第一个儿子标签
        lastElementChild        最后一个儿子标签
        previousElementSibling  上一个兄弟元素

    2. 节点标签
        创建节点
            createElement(标签名)
            eg: var divEle = document.createElement("div");
        添加节点
            .appendChile()  向一个父节点中最后添加一个子节点
            .insertBefore(newNode, 某个节点) 向一个父标签内部某个标签前面添加一个标签
        删除节点
            .removeChild(要删除的节点)
        替换节点
            .replaceChild(newNode, 要替换的节点)

    3. 属性节点
        .innerTest 获取文本节点的值  设置文本节点的值
        .innerHTML 可以获取内部HTML 也可以赋值

        .setAttribute()
        .getAttribute(attr, value)
        .removeAttribute(attr)

    4. 获取值(input/select/textarea)操作
        .value  获取值
        .value-"要设置的值"  设置值

样式操作
    5. class的操作
        .className 获取所有样式类值(字符串)
        .classList 获取所有样式类值

        classList.remove(cls) 删除指定类值
        classList.add(cls)    添加类值
        classList.contains(cls) 存在着返回true，不然返回false
        classList.toggle(cls)   存在则删除返回false，不存在则添加为true

    6. 指定CSS操作
        object.style.CSStyle (遇到CSS中有中横线的去掉并将后面的单词首字母大写)
        eg. object.style.marginTop
            object.style.borderLeftWidth
            object.style.backgroundColor

    7. 事件
    * 通常会把给页面绑定事件的JS代码写在body最后面
    给按钮单击绑定事件
    onclick  ondbclick
    方法1:
        <input id="d1" value="点我" onclick="change();">
        <script>
        function change(){
            var c1Ele = document.getElementById("d1");
            c1Ele.classList.toggle("c1");
        }
        </script>
    方法2: 通过js给标签绑定事件
        <script>
        var c1Ele = document.getElementById("d1");
        c1Ele.onclick = function(){
            console.log(this); //this表示触发事件的标签本身
            var c1Ele = document.getElementById("d1");
            d1Ele.classList.toggle("c1");
        }
        </script>
     获取焦点和失去焦点
     onfocus onblur
        <script>
        var i1Ele = document.getElementById("i1")'
        i1Ele.onfocus = function(ev){
            this.value = "";
        }
        i1Ele.onblur = function(ev){
            this.value = "第三十";
        }
        </script>
*/