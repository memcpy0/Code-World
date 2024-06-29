## Python核心编程笔记

勉强学完了《笨办法学Python》这本书后，我又找了几本书来看，因为最终是要过渡到Python3的，所以有些Python2的书和笔记就只能扫过看一下了，以便了解一下Python的演进。

这本是《Python核心编程笔记》。

[TOC]

### 第二章 快速入门

- 代码缩进：四个空格，尽量不用`Tab`键(不同的OS，长度定义不同)。关于缩进的错误，见我的[Python出错日志](https://blog.csdn.net/myrealization/article/details/80028111)。

- 类中的`__init__()`方法：
当一个类被创建时，`__init__()`方法会**自动**被执行，类似构造器，它仅仅是**类对象创建后执行的第一个方法**。目的是完成对象的初始化工作。
> 当类对象被创建，也就是说通过赋值将类实例化后，该方法第一个自动地执行，以完成实例(对象)的初始化，也就是完成(代表着该实例自身引用的)self的属性的添加。

- self参数：
每个方法都有这个参数。**它是类实例自身的引用**。相当于其它面向对象编程语言中的this。
***
### 第三章 Python基础

- 特殊规则及特殊字符：
`#`号 : 注释
换行（\n） : 换行
反斜线（\） : 继续上一行（未来可能会被废弃，不用为好）
分号（;） : 两个语句连在一行，也就是将不同逻辑行的语句写在同一个物理行中；
冒号（:） : 将代码块的头和体分开

- 变量名必须由字母跟下划线组成，且区分大小写。对于一般的变量名**建议不要用下划线开头**。因此我们**在大多数时候只能用字母开头**。

- `__name__ `系统变量：
   - 模块被导入时，该模块的属性中`__name__`的值为模块的名字；
   -  模块被直接执行时，该模块的属性中`__name__`的值为`'__main__'`。

- 内存管理:
   - 变量无须事先申明；
   -  变量无须指定类型；
   -  程序员不用担心内存管理；
   -  变量名会被“自动回收”；
   - del 语句能够直接释放资源。

***
### 第四章 Python对象

- type()和 isinstance()：
  `type(obj)`：获取obj的类型；
  `isinstance(obj, obj_type)`：判断obj是否为`obj_type`类型，返回布尔值。

- repr(obj)或`obj`返回 obj 的字符串表示；
  str(obj) 返回`obj`适合可读性好的字符串表示。

- 可变类型：列表、字典
  不可变类型：数字、字符串、元组

- 不支持的类型：
  char或byte：无
  指针：无
***
### 第五章 数字

- 复数类型：
  - 虚数不能单独存在，必须加上0.0的实数部分；
  - 由实数+虚数两部分构成；
  - 实数跟虚数部分都是浮点型；
  - 虚数部分后缀是`j或J`。

- 复数属性：
  - `num.real` 该复数的实数部分；
  - `num.imag` 该复数的虚数部分；
  - `num.conjugate()` 返回该复数的共轭复数。

- 混合模式优先级(转化)： complex > float > *long* > int
> 注：Python3中长整数已经被删除了。

- 位操作符：只适用于整型
取反（~），安位与（&），或（|），异或（^），左移（<<），右移（>>）
**负数当做正数的2进制补码处理**。
> num <<（ 或 >> ）N = num * （ 或 / ）2 ** N

- `int(obj, base)` ： **返回obj数字的base进制数**，当然，也可以用作类型转换。

- 数值运算内建函数：
  `abs(num)` num 的绝对值；
  `coerce(num1, num2)` 转化为同一类型，返回一个元组；
  `divmod(num1, num2)` 返回元组（num1/num2, num1%num2）；
  `pow(num1, num2, mod=1)` 取num1的num2次方，**若有mod，则对其再取余**；
  `round(flt, ndig=1)` 对浮点型flt进行四舍五入，保留ndig位小数；

  `hex(num)` 将num转化为十六进制，并**以字符串返回**；
  `otc(num)` 将num转化为八进制，并**以字符串返回**；
  `chr(num)` 返回num的 ASCII 值，范围：0 <= num <= 255

### 第六章 序列：字符串、列表和元组
- ASCII 码：每个**英文字符**都是以7位二进制数的方式存放在计算机内，范围是32~126。**默认所有字面上的字符串都是 ASCII 编码**。

- 三引号（'''或"""）:
允许一个字符串跨多行，字符串中可以包含换行符，制表符及其他特殊字符。

- 字符串类型和元组是不可变的，当你要改变一个字符串的时候就必须通过创建一个新的同名的字符串来取代它。元组也是。

- Python 字符串不是通过`NULL或者'\0'`来结束的，它除了你定义的东西，没有别的。
> 这一点是提醒C程序员。

- 并非调用一个方法就会返回一个值。那些**可以改变对象值的可变对象的方法是没有返回值的**，或者说默认返回None。如：sort()，extend()等。

- 列表跟元组是两个非常相似的序列类型，之所以要保留二者是因为在某些情况下，其中一种类型要优于使用另一种类型。
***
### 第七章 映射和集合类型
#### 字典
- 字典是Python中唯一的映射类型：
- 映射类型内建函数：
  - `dict.clear()`  删除字典中的所有元素；
  -  `dict.fromkeys(seq, val=None)`  返回一个新字典，seq 为键，val 为值；
  - `dict.get(key, default=None)`  返回字典中key的值，若无此键则返default；
  - `dict.items() `  返回一个包括字典中键值对元组的视图；
  - `dict.keys()`      返回一个字典中的键的视图；
  - `dict.values()`  返回一个包含字典中所有值的视图；
  - `dict.update(dict2)`  将字典 dict2 的键值对添加到字典dict 中去。

- 不允许一个键对应多个值：键必须是**可哈希(hashable)**的对象。像列表和字典这样的可变类型，由于它们不是可哈希(unhashable)的，所以不能作为键。

#### 集合
- 现已成为Python的基本数据类型。
   - 两种类型：可变集合(set)和不可变集合(frozenset)；
   - 可用set()和frozenset()来**分别创建可变集合跟不可变集合**。

- 适用于所以集合类型的内建方法：
 - `s.issubset(t)`  判断 s 是否是 t 的子集，返回布尔值
 - `s.issuperset(t)` 判断 s 是否是 t 的超集，返回布尔值
 - `s.union(t)`  返回新集合，s 跟 t 的并集
 - `s.intersetion(t)` 返回新集合，s 跟 t 的交集
 - `s.difference(t)` 返回新集合，其成员是 s 的成员，但不是 t 的成员
***
### 第八章 条件和循环
#### if循环
```python
if expression1:
	expr1_true_suite
elif expression2:
	expr2_true_suite
elif expression3:
	expr3_true_suite
elif ……
else:
	None_of_the_above_suite
```
- Python 不支持switch/case语句，但完全可以用if/else的结构来代替；
- 三元操作符：`X if C else Y`（如果条件 C 成立，则结果为 X,否则为 Y）。

#### while循环
```python
while expression:
	suite_to_repeat
else:
	……
```
#### for语句
```python
for iter_var in interable:
	suite_to_repeat
else:
	……
```

- pass语句：Python中提供pass语句，表示不做任何事，NOP(No Operation)。

- 迭代器和 iter()函数：
 - 迭代器就是有一个`next()`方法的对象；
 - 对一个**可迭代对象调用**`iter()`方法就可以得到它的迭代器。
 
- 列表解析：列表解析来自函数式编程语言 Haskell。它是一个非常有用、简单而且灵活的工具，可以用来动态地创建列表。

- Python 早就支持函数式编程特性，例如`lambda表达式、map()和filter()、reduce()`等
其中，lambda允许用户快速地创建只有一行的函数对象。例如：map(lambda x:x ** 2, range(6))
***
### 第九章 文件和输入输出

- open()语法：
`file_object = open(file_name, access_mode=‘r’,buffering=-1)`

- 保留行分隔符：
使用`read()或者readlines()`从文件中读取行时，Python并不会删除行结束符，类似的，`write()或writelines()`也不会自动的加入行结束符，这得由程序员自己完成。这样可以保证读取的文件内容的格式稳定。

- 命令行参数：
  - `sys.argv` 是命令行参数的**列表**；
  - `len(sys.argv)`是命令行参数的个数(也就是argc)。

***
### 第十章 错误和异常
- Python 中常见的异常：
 - `NameError`： 尝试访问一个未申明的变量
 - `ZeroDivissionError`: 除数为零
 - `SyntaxError`： Python解释器语法错误
 - `IndexError`: 请求的索引超出序列范围
 - `KeyError`： 请求一个不存在的字典关键字
 - `IOError`： 输入\输出错误
 - `AttributeError`： 尝试访问未知的对象属性
 - `KeyboardInterrupt`： 中断异常
 - `BaseException`： 所有异常的基类

- 检测和处理异常：`try-except-else-finally`语句
处理多个异常的时候**可以用多个except，也可以用一个except，然后将异常放入一个元组里**。
 - 不推荐使用空except语句；
 - else子句：`try-except-else`：在try范围内没有异常被检测到时，执行else子句；
 - finally子句：`try-except(-else)-finally`：无论异常是否发生，是否捕捉，都会执行的一段代码。

- raise 语句：触发异常。
一般语法：
`raise [SomeException [,args [,traceback]]]`
  - 第一个参数：SomeException：触发异常的名字；
  - 第二个参数：args：可选，作为一个对象或者对象的元组传给异常；
  - 第三个参数：traceback：可选，很少用。
  
- 断言：**断言是一句必须等价于布尔真的判定**，否则将产生 AssertionError(断言错误)的异常，同时也说明断言是假的。建议跟try-except连用，将断言语句放在try中。
语法：
`assert expression[, arguments]`

- sys模块中的`exc_info()`函数，通过其提供的一个3元组信息同样可以捕捉异常信息：
sys.exc_info()得到的元组：`(type, value, traceback)`

***

### 第十一章  函数和函数式编程
- 创建函数：
```python
def function_name(arguments):
	“function_documentation_string”
	function_body_suite
```

- 前向引用：Python不允许在函数未声明之前，对其进行引用或调用，即不允许后向引用，只允许前向引用。

- Python支持**在外部函数的定义体内创建内嵌函数**。

- 函数中**使用默认参数**会使程序的健壮性上升到极高的级别。

- 函数式编程的内建函数：
  - `filter(func, seq)`：调用一个布尔函数func来迭代遍历每个seq中的元素；返回一个使func返回值为true的元素的序列；
  - `map(func, seq1[ ,sql2…])`： 将函数 func 作用于给定的序列 seq 中的每个元素，并用一个列表来提供返回值；
  - `reduce(func, seq)`： 将一个二元函数作用于seq序列的元素，每次携带一对（先前的结果以及下一个序列元素），连续地将现有的结果和下一个值作用，最后减少我们的序列为一个单一的返回值。

- 变量作用域：
全局变量除非被删除，否则它们存活到脚本运行结束，且对所有的函数都是可访问的。
***
### 第十二章 模块
- 与其他可以导入类（class）的语言不同，Python导入的是模块或模块属性。

- 语法： 
    - import module1[,module2[,…]]
    - from module import name1*,name2*,…++
    - from module import name as shortname
    - from module import *

- import 语句的模块顺序：
   - Python 标准库模块
   - Python 第三方模块
   - 应用程序自定义模块

- 导入(import)和加载(load):
一个模块只能被加载一次，无论它被导入多少次。

- reload()重新导入一个已经导入的模块：  reload(module) 使用 reload()时候必须是全部导入。
- 禁止模块的某个属性导入，可以**在该属性名称前加一个下划线**：import foo._bar。

- globals()和 locals()分别返回调用者全局和局部名称空间的字典。
***
### 第十三章  面向对象编程
在Python中，面向对象编程主要有两个主题，就是类和类实例。

- 语法：
```python
class MyNewObjectType(bases):  #bases 参数用于继承的父类
	‘define MyNewObjectType class’
	Class_suite
```
- object是"所有类之母"，若未指明父类，则object将作为默认的父类。

- 创建一个实例的语法：
```python
myFirstObject = MyNewObjectType()
```
- 添加类方法的语法：
```python
class MyDataWithMethod(object):  #定义类
	def printFoo(self):  #定义方法
	print ‘You invoked printFoo()’
```
self参数，它在**所有的方法声明中都存在，并且必须是第一个参数**。这个参数代表实例对象本身，即类实例自身的引用。当你调用方法的时候由解释器悄悄地传递给方法，不需要你自己传递self。

- 类、属性和方法的命名方式：
  - 类：通常大写字母打头，这是标准惯例。有助于识别类。
  - 属性：小写字母打头 + 驼峰，**使用名词**作为名字。
  - 方法：小写字母打头 + 驼峰，**使用动词**作为名字。

- Python不支持纯虚函数（如 C++）或抽象方法（如 Java）。
- Python严格要求，**类没有实例化的话，方法是不能被调用的**。方法必须绑定（到一个实例）才能被直接调用。

- `dir(class_name)`:  返回一个类对象的属性名列表；
`class_name.__dict__ `:  返回的是一个字典，key是属性名，value 是数据值。
`vars()`：内建函数与dir()相似，仅限于给定的对象有一个`__dict__`属性。返回一个字典，包含了对象存储于其`__dict__`中的属性(键)和值。

- 实例属性 VS 类属性：
类属性跟实例无关。这些值像静态成员那样被引用，即使在多次实例化中调用类，类属性的值都不会改变。

- 子类可以通过继承覆盖父类的方法。类似，如果在子类中覆盖了`__init__()`方法，那么父类的`__init__()`就不会被自动调用了。

***
### 第十四章 执行环境
Python 有三种不同类型的函数对象。分别是：
  - 内建函数（BIF）：C/C++写的，编译过后放到 Python 解释器当中的;
  - 用户定义的函数（UDF）;
  - lambda表达式。

***
**后面是高级部分，这些年来可能会有较大变动，因此仅做参考。**
***
### 第十五章 正则表达式
Python 通过标准库的re模块来支持正则表达式（RE）。

- **搜索and匹配**：“模式匹配（patten-mactching）”
  - 搜索(searching)：在字符串任意部分中搜索匹配的模式；
  - 匹配(matching)：判断一个字符串能否从起始处全部或部分的匹配某个模式。

- re模块：核心函数和方法
|函数/方法  | 描述
|:-------------|:-----------------------------------------------------------
|compile(pattern)  |对正则表达式模式pattern进行编译
|match(pattern,string)  |用pattern去匹配string ，成功返回对象，否则返回None
|search(pattern,string)  |在string中去搜索pattern第一次出现的位置，结果同上
|findall(pattern,string)  |在string中搜素pattern非重复出现，返回列表
|finditer(pattern,string) |功能同findall，但返回的是一个迭代器
|split(pattern,string) | 用pattern中的分隔符把string分割成一个列表
|sub(pattern,repl,string) |把string中所有 pattern 的地方替换为 repl
|匹配对象的方法
|group(num=0)  |返回全部匹配对象（或指定编号是num的子组）
|groups() | 返回一个包含全部匹配的子组的元组

- r'string：用于忽略string中的所有特殊字符。

- 正则表达式本身默认是贪心的，解决办法就是用非贪婪操作符“?”。这个操作符可以用在“*”、“+”或者“？”的后面，它的作用是要求正则表达式引擎匹配的字符越少越好。

***
### 第十六章  网络编程
#### 客户端/服务器网络编程
在完成服务之前，服务器必需要先完成一些设置。先要创建一个通讯端点，让服务器能够“监听”请求。
当服务器准备好之后，要通知客户端，否则客户端不会提出请求。
客户端比较简单，只要创建一个通信端点，建立到服务器的连接，然后提出请求。
一旦请求处理完成，客户端收到了结果，通信就完成了。

- 套接字：通信端点。
 - 套接字是一种**具有"通信端点"概念的计算机网络数据结构**。
 - 网络化的应用程序在开始任何通讯前都必需创建套接字。没有它将没办法通信。
 - 套接字有两种，分别是基于文件型的和基于网络型的。
   AF_UNIX：“地址家族：UNIX”，基于文件
   AF_INET：“地址家族：Internet”，基于网络
 - Python 只支持AF_UNIX，AF_INET和AF_NETLINK家族。
 
-  面向连接/面向无连接：
  - 面向连接："虚电路"或者"流套接字"，TCP，套接字类型为SOCK_STREAM
  - 面向无连接：UDP，套接字类型为SOCK_DGRAM (datagram 数据报)

- 创建套接字语法：
from socket import *
TCP套接字：`tcpSock = socket(AF_INET, SOCK_STREAM)`
UDP套接字：`udpSock = socket(AF_INET, SOCK_DGRAM)`

- 套接字对象的常用函数
|函数  | 描述
|:------------|:---------------------------------------------------
|**服务器端套接字函数**
|s.bind(addr)  |绑定地址addr（主机号，端口号）到套接字
|s.listen(num) | 开始TCP监听，最多允许num个连接进来
|s.accept()  |被动接受TCP客户端连接，（阻塞式）等待连接的到来
|**客户端套接字函数**
|s.connect()  |主动初始化TCP服务器连接
|s.connect_ex() | Connect()函数的扩展版本，出错时返回出错代码，而不是异常
|**公共用途的套接字函数**
|s.recv() | 接收TCP数据
|s.send() | 发送TCP数据
|s.sendall() | 完整发送TCP数据
|s.recvfrom() | 接收UDP数据
|s.sendto()  |发送UDP数据
|s.close()|  关闭套接字

***
### 第十七章 网络客户端编程
数据交换的参与者是一个服务提供者和一个服务的使用者，有人把它比作"生产者–消费者"，一般是一个生产者对多个消费者。

- **文件传输网际协议**：
  - FTP：文件传输协议（File Transfer Protocol）
  - UUCP：Unix-to-Unix 复制协议（Unix-to-Unix Copy Protocol）
  - HTTP：超文本传输协议（Hypertext Transfer Protocol）
  - rcp/scp/rsync： Unix下的远程文件复制指令

- **一般编程步骤**：
  - 连接到服务器
  - 登陆（如果需要的话）
  - 发出服务请求（有可能有返回信息）
  - 退出

- Python 和 FTP：`from ftplib import FTP`
  Python 和 NNTP：`from nntplib import NNTP`
  Python 和 SMTP：`from smtplib import SMTP`

***

### 第十八章 多线程编程
Python代码的执行由Python虚拟机（也叫解释器主循环）来控制。
虽然Python解释器中可以“运行”多个程序，但在任意时刻，只有一个线程在解释器中运行。与单个CPU的多线程原理是一样的。
对Python虚拟机的访问由全局解释器锁（GIL）来控制的，也正是这个锁保证了**同一时刻只有一个线程在运行**。

### 第十九章 图形用户界面编程
- GUI：`graphical user interface`。Python拥有大量的图形工具集。

- 创建GUI程序的五个基本步骤：
1. import tkinter （Python3中为tkinter）
2. 创建**顶层窗口对象**容纳你的GUI： `top = tkinter.Tk()`
3. 在top中创建所有的GUI模块
4. 将3中的GUI模块与底层代码相连接
5. 进入主事件循环

- Example:
```python
from tkinter import *  #导入 Tkinter
top = Tk()  # 顶层窗口
label = Label(top, text=’Hello world!’) #创建模块
label.pack() # 装载连接
mainloop()  #主事件循环
```
***
### 第二十章 Web编程

- Urllib模块：
Urllib模块提供了所有你需要的功能，它提供了一个高级的 Web 交流库。其特殊功能在于利用各种协议（HTTP、FTP 等）从网络上下载数据。

- CGI：帮助Web服务器处理客户端数据
Web服务器接收到表单反馈，与外部应用程序交互，收到并返回新的生成的HTML页面都发生在Web服务器的CGI（标准网关接口，Common Gateway Interface）接口上。
CGI其实只是一个适用于小型We 网站开发的工具。

***
### 第二十一章 数据库编程
- Python数据库API：Python 能够直接通过数据库接口，也可以通过ORM(需要自己书写 SQL)来访问关系数据库。

- DB-API：这是一个规范，它定义了一系列必需的对象和数据库存取方式，以便在各种各样的底层数据库系统和多种多样的数据库接口程序提供一致的访问接口。
- DB-API 模块属性
|属性名 | 描述
|:-------|:------------------------------------
|apilevel | 模块兼容的DB-API版本号
|threadsafety  |线程安全级别
|paramstyle  |该模块支持的SQL语句参考风格
|connect() | 连接函数

- 连接对象：要与数据库通信，必须先和数据库建立连接。连接对象**用于处理将命令送往服务器，以及从服务器接受数据**等基本功能。

- 游标对象：**用于执行数据库命令和得到查询结果**。
对于不支持游标的数据库来说，connect 对象的 cursor()方法仍然会返回一个尽量模仿游标对象的对象。
游标对象**最重要的属性**是`execute*()和fetch*()`，所有对数据库服务器的请求均由它们来完成。
Python到底支持哪些平台下的数据库？答案是几乎所有！

***
### 第二十二章  扩展 Python
- 一般来说，所有能被整合或者导入到其他Python脚本的代码，都可以称为扩展。

- 扩展的理由：
  - 添加额外的（非 Python）功能
  -  性能瓶颈的效率提升
  -  保持专有源代码私密

- 为 Python 创建扩展需要 3 个主要的步骤：
1. 创建应用程序代码：
  我们要建立的是一个“库”，一个将要在 Python 内运行的模块
  在 C 代码中放一个`main()`用于测试代码的正确性
2. 利用样板来包装代码
1) 包含Python头文件`#include "Python.h"`
2) 为每个模块的每一个函数增加一个形如`PyObject* Module_func()`的包
装函数
3) 为每个模块增加一个形如`PyMethod Def ModuleMethods[]`的数组
4) 增加模块初始化函数`void initModule()`
3. 编译与测试：`distutils`包被用来编译、安装和分发这些模块、扩展和包
1) 创建`setup.py`
2) 通过运行`setup.py`来编译和连接你的代码
3) 从Python中导入你的模块 : `$ python setup.py install`
4) 测试功能

- 在编译的时候，我们需要将代码跟Python库放在一起进行编译。

### 第二十三章  其他话题
略
