@[toc]

近来，玩《baba is you》这款游戏的时候，我注意到游戏的逻辑几乎都是用Lua写的：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191028155000349.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
就像当年我玩《命运石之门》的游戏时，发现代码是用Python2写的时候一样...有点想学...所以就下了SciTE编辑器和Lua的交互程序。写下了第一行Lua语句(CSDN的编辑器连Lua的格式都没有?)：

```python
print("Hello Lua World!")
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191028155624476.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
## Lua介绍
Lua作为一种轻量级的脚本语言，往往与C/C++集成，用于游戏的开发、Web脚本的开发和独立应用的扩展和定制。

 **Lua 特性**：
   - 轻量级: 它用标准C语言编写并以源代码形式开放，编译后**仅仅一百余K**，可以很方便的**嵌入别的程序**里。
   - 可扩展: Lua提供了非常易于使用的扩展接口和机制：由宿主语言(通常是C或C++)提供这些功能，Lua可以使用它们，就像是本来就内置的功能一样。
  - 其它特性：
       支持**面向过程**(procedure-oriented)编程和**函数式编程**(functional programming)；
       **自动内存管理**；只提供了**一种通用类型的表**（table），用它可以实现数组，哈希表，集合，对象；
       语言内置模式匹配；闭包(closure)；函数也可以看做一个值；提供多线程（协同进程，并非操作系统所支持的线程）支持；
       通过闭包和table可以很方便地支持面向对象编程所需要的一些关键机制，比如数据抽象，虚函数，继承和重载等。

Lua是有限的，除了表(关联数组)只有table一种外，它的数字类型也只有double一种...等等，因此，它是很轻量的。

和python一样，lua可以交互式编程，也可以写.lua脚本。
## Lua基本语法
- 注释：单行注释为`--`，多行注释为`--[[  --]]`。
- 标识符：用于定义变量或者函数，`[_A-Za-z][_A-Za-z0-9]*`，区分大小写，不允许用特殊字符如$/%等定义标识符。
- 关键字和保留字：不能作为常量、变量或其他自定义标识符，很少，和C艹无法比。
	```cpp
	and        break do     else 
	elseif     end   false  for 
	function   if    in     local 
	nil        not   or     repeat 
	return     then  true   until 
	while 
	```
- Lua内部变量一般形式为下划线+一串大写字母，别用。
- 变量默认为全局变量，无需声明，和python类似，**赋值即创建这个全局变量**，用local申明局部变量。**访问一个不存在的全局变量不会出错**，只会得到`nil`；**删除一个全局变量，赋值为nil**即可。
