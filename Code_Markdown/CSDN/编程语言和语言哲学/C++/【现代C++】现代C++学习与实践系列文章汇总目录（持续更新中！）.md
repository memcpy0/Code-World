> 本文属于「现代C++学习实践」系列文章的汇总目录，这一系列正式开始于2021/09/04，着重于**现代C++**（即C++11、14、17、20、23等新标准）和**Linux C++服务端开发**的学习与实践。众所周知，「**C++难就难在：在C++中你找不到任何一件简单的事**」。因此，本系列将至少持续到作者本人「精通C++」为止（笑）。由于文章内容随时可能发生更新变动，欢迎关注和收藏本文以作备忘。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/Modern-Cxx-Learning-Path](https://github.com/memcpy0/Modern-Cxx-Learning-Path)。在这一仓库中，你可以看到本人学习C++的全过程，包括C++书籍源码、练习实现、小型项目等。
> <b></b> 
需要特别说明的是，为了透彻理解和全面掌握现代C++，本系列文章中参考了诸多博客、教程、文档、书籍等资料，限于时间精力有限，这里无法一一列出。部分重要资料的不完全参考目录如下所示，在后续学习整理中还会逐渐补充：
> - C++ Primer 中文版（第5版），Stanley B. Lippman、Barbara E. Moo等著，王刚、杨巨峰译，叶劲峰、李云、刘未鹏等审校，电子工业出版社； 
> - [Bjarne Stroustrup](https://www.stroustrup.com/)老爷子的个人网站。包括[Thriving in a Crowded and Changing World: C++ 2006–2020](https://www.stroustrup.com/hopl20main-p5-p-bfc9cd4--final.pdf)及其中文版——[在拥挤和变化的世界中茁壮成长：C++ 2006–2020](https://github.com/Cpp-Club/Cxx_HOPL4_zh)（一份了解标准化背后故事、以及C++未来发展方向的绝佳材料）
> - [Boost libraries Getting Started guide](https://www.boost.org/doc/libs/1_77_0/more/getting_started/index.html)
> - 侯捷老师的公开课；
>    - C++面向对象高级开发上、下：正确理解面向对象的精神和实现手法，涵盖对象模型、关键机制、编程风格、动态分配；
>    - STL标准库与范型编程：深入剖析STL标准库之六大部件、及其之间的体系结构，并分析其源码，引导高阶泛型编程。
>    - C++新标准C++11/14：在短时间内深刻理解C++2.0的诸多新特性，涵盖语言和标准库两层
>    - C++内存管理机制：学习由语言基本构件到高级分配器的设计与实作，并探及最低阶`malloc` 的内部实现。
>    - C++ Startup揭密：C++程序的生前和死后。认识Windows平台下的Startup Code(启动码)，完全通透C++程序的整个运行过程。


@[toc]

---
# 0. 贯穿始终的Linux Modern C++路线图
涵盖了Linux C++的完整学习路径及学习资料推荐。为了方便展示，把所有子主题、子子主题都缩了进去：
![在这里插入图片描述](https://img-blog.csdnimg.cn/b3614d7b14e44eda8ca273c4a60285d9.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16) 
## 0.1 计算机基础（正在学）
关于计算机基础知识，[这篇高赞回答 一毕业就能进阿里的人有多厉害？](https://www.zhihu.com/question/344204972/answer/1951813373)讲解并推荐了学习书籍和视频：
- ~~设计模式（大话设计模式，C#和C++）~~
- 计算机组成原理
- 计算机网络（小林Coding）
- 操作系统（小林Coding）
- 编译原理
- ~~[数据结构和算法]一周一篇~~
- ~~[CSAPP]~~
- [【MIT】MIT的计算机课程体系（附有官方教程链接）（持续优化和整理！）](https://memcpy0.blog.csdn.net/article/details/119253236)
- [MIT6.0001 Introduction to Computer Science and Programming in Python学习笔记](https://memcpy0.blog.csdn.net/article/details/115925705)
## 0.2 开发工具的选择（正在学）

~~[Vim/Emacs]~~
~~[VS Code]~~
![在这里插入图片描述](https://img-blog.csdnimg.cn/038ed9419dc44f1582eb82a0af6c2962.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
## 0.3 第一阶段：C++入门（正在学）
![在这里插入图片描述](https://img-blog.csdnimg.cn/68be185b20374f928fc697ca0dda019a.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
**强烈建议要跟着视频学**，C++本身就难，如果自己看书的话，会非常吃力。**在看视频的同时把书做为辅助**，有奇效。
- :tv:视频：~~C++面向对象高级开发上、下-兼谈对象模型；[源代码和课件1](https://github.com/19PDP/Bilibili-plus/tree/master/C%2B%2B-OOPBase1-HouJie)、[源代码和课件2](https://github.com/19PDP/Bilibili-plus/tree/master/C%2B%2B-OOPBase2-HouJie)~~
  C++新标准-C++11&14  [课件](https://github.com/19PDP/Bilibili-plus/tree/master/C%2B%2B-newC%2B%2B11%2614-HouJie)
  C++内存管理机制-从平地到万丈高楼 [课件](https://github.com/19PDP/Bilibili-plus/tree/master/C%2B%2B-Startup/slide)
   C++Startup揭秘-C++程序的生前死后  没有课件
- :book:书籍：
     - ~~C++ Primer第5版；~~
	 - Effective C++：改善程序与设计的55个具体做法，很多关于C++的面试问题的出处。
![在这里插入图片描述](https://img-blog.csdnimg.cn/303b02693d2a43b3b63b54ee6ec0a877.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

最后可以做一个电子词典小项目，以使用C++的基础知识。(电子词典的源码将和推荐的书籍、技术路图一同提供)


---
## 0.4 第二阶段：C++进阶（同步1学）
了解STL的使用，学会应用一些常用的设计，并且能够开发一个贪吃蛇项目，以学习类和设计模式的使用。

- :tv:视频：46集系列视频 C++ STL标准库和 C++ 泛型编程
；[源代码和课件](https://github.com/19PDP/Bilibili-plus/tree/master/C%2B%2B-STL-HouJie)
- :book:书籍：
   - 配套书籍推荐《**C++ 标准程序库**》：既适合初期入门，又适合当字典用，大致看一遍，以后用时再具体记；
  - STL源码剖析：**阅读、剖析名家代码乃是提高水平的捷径**。源码之前了无秘密。大师们的缜密思维、经验结晶、技术思路、独到风格，都原原本本体现在源码之中。在你仔细推敲之中，迷惑不解之时、恍然大悟之际，你的经验、思维、视野、知识乃至技术品位都会获得快速的成长。


## 0.5 第三阶段：征服Linux
![在这里插入图片描述](https://img-blog.csdnimg.cn/4b03f0e7b03b47e5a480d4a0b9d7437e.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
### Linux常用命令工具
系统命令工具：
基础命令工具：
网络参数工具：
磁盘参数工具：
日志监控工具：
参数监控工具：
![在这里插入图片描述](https://img-blog.csdnimg.cn/82d89df202b84622aa79ac5f742419d9.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_18,color_FFFFFF,t_70,g_se,x_16)
一般而言，Linux/Unix的书籍会覆盖**系统、网络、数据库**这三个方面的知识。所以，这里的书籍做一次统一推荐。

推荐书籍:book:
- 首推Linux编程经典书籍：**UNIX环境高级编程**，本书是被誉为UNIX编程“圣经”，这本书大而全，但也比较难啃，可以挑着看
- Linux高性能服务器编程：这本书对菜鸡比较友好，内容很庞大，而且基础讲解的比较详细；
- Linux多线程服务端编程：陈硕，C++开发的高水平作品
- MySQL入门：建议看《MySQL必知必会》，原理比较少，基本都是语法和使用细节，适合先过一遍。
- Redis入门：看《Redis设计与实现》这本书。非常详细的讲了Redis的底层数据结构，包括其集群部分也做了很好的解释。除此之外可以再去理解一下分布式的Raft算法，Gossip协议等等。
- 鸟哥的私房菜linux和:tv:[鸟哥的Linux](https://www.bilibili.com/video/BV184411z7q5)

然后，需要学习各种基于Linux的编程API，分别学会系统编程、网络编程、数据库编程。这些内容学完，已经能够自行实现软件开发了，找个工作问题不大。
![在这里插入图片描述](https://img-blog.csdnimg.cn/1035691936f04f3e887efeffa25b367e.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
很多同学在学习的同时，想要做些实战项目，把Github的优秀开源项目进行了整理：
![在这里插入图片描述](https://img-blog.csdnimg.cn/8959b074aa254e21bb649971872f424c.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

### 系统编程
Windows系统编程里有各种各样的Windows API，Linux系统API的使用就更加重要。
![在这里插入图片描述](https://img-blog.csdnimg.cn/6a13337120a54b5096fbef7724f150ad.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_16,color_FFFFFF,t_70,g_se,x_16)
### 网络编程
**后台开发离不开网络编程**，甚至简单来说，后台开发就是用电脑去处理大规模的网络请求。**作为一名合格的后端开发人员，对Linux网络编程的熟悉是必不可少的。**
![在这里插入图片描述](https://img-blog.csdnimg.cn/6e4644b4db854a52b5fab6c2b2634a33.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
### 数据库编程
如果只是会MySQL基础可能是远远不够的，像是Redis，MongoDB等非关系型数据库也是当前的热门。分布式数据库，分布式存储是当前每个大公司都会非常重视的核心，所以数据库基础非常的重要。
![在这里插入图片描述](https://img-blog.csdnimg.cn/cdc56345147646dcaa2e6b78ce15fb8c.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

这些内容学完以后，**基本上具有了工作两年的水平**，别说校招，就是社招找工作都是非常简单的。

## 0.6 第四阶段：高级进阶
**编程的学习并不是完全线性的**，并不一定要先学A才能学到B。比如，这里列到的监控的统计，大家在学习前面的编程书籍时，一般也都会涉及。

但我仍把它列到了高级进阶部分，因为我是基于对就业的必要性来划分的。很多同学看这个路线图的主要目的就是为了找份工作，所以，我为了减轻大家负担，把就业时可有可无的项放在了高级进阶，而把必会的编程能力放在了第三阶段：征服Linux。

这部分内容看启舰的知乎文章：耗时一个月，总结的Linux C++后台开发学习路线（已签腾讯sp，附学习资料）


---
# 1. C++入门学习
## 第零部分 开始
[【现代C++】第零部分 开始：初始输入输出、注释、控制流和类简介](https://memcpy0.blog.csdn.net/article/details/120303497)

第I部分的内容：C++基础。
## 第一部分 C++基础
[【现代C++】第一部分 C++基础(1) 基本类型、变量、复合类型、const限定符、自定义数据结构](https://editor.csdn.net/md?articleId=120561212)

[【现代C++】第一部分 C++基础(2) 字符串、向量、迭代器初步和数组](https://editor.csdn.net/md?articleId=120716404)

[【现代C++】第一部分 C++基础(3) 表达式（优先级、结合律、求值顺序）、运算符和类型转换](https://editor.csdn.net/md?articleId=120716414)

[【现代C++】第一部分 C++基础(4) 简单/复合语句、语句作用域、条件/迭代/跳转语句、try语句块和异常处理](https://editor.csdn.net/md?articleId=120716423)

[【现代C++】第一部分 C++基础(5) 函数基础、参数传递、返回类型和return语句、函数重载与匹配、函数指针](https://editor.csdn.net/md?articleId=120716435)

[【现代C++】第一部分 C++基础(6) 类定义、访问控制与封装、类特性、类作用域、构造函数、静态成员](https://editor.csdn.net/md?articleId=120716445)
```cpp
 
# 3. 字符串、向量和数组
## 3.1 命名空间的using声明
## 3.2 标准库类型string
## 3.3 标准库类型vector
## 3.4 迭代器介绍
## 3.5 数组
## 3.6 多维数组
# 4. 表达式
## 4.1 基础
## 4.2 算术运算符
## 4.3 逻辑和关系运算符
## 4.4 赋值运算符
## 4.5 递增和递减运算符
## 4.6 成员访问运算符
## 4.7 条件运算符
## 4.8 位运算符
## 4.9 sizeof运算符
## 4.10 逗号运算符
## 4.11 类型转换
## 4.12 运算符优先级表
# 5. 语句
## 5.1 简单语句
## 5.2 语句作用域
## 5.3 条件语句
## 5.4 迭代语句
## 5.5 跳转语句
## 5.6 try语句块和异常处理
# 6. 函数
## 6.1 函数基础
## 6.2 参数传递
## 6.3 返回类型和return语句
## 6.4 函数重载
## 6.5 特殊用途语言特性
## 6.6 函数匹配
## 6.7 函数指针

# 7. 类
## 7.1 定义抽象数据类型
## 7.2 访问控制与封装
## 7.3 类的其他特性
## 7.4 类的作用域
## 7.5 构造函数再探
## 7.6 类的静态成员
```
## 第二部分 C++标准库
[【现代C++】第二部分 C++标准库(1) IO类、文件输入输出、string流](https://memcpy0.blog.csdn.net/article/details/120716357) 第8章 IO库
[【现代C++】第二部分 C++标准库(2) 顺序容器概览、操作、vector对象的增长、额外的string操作、容器适配器](https://editor.csdn.net/md?articleId=120716365) 第9章 顺序容器
[【现代C++】第二部分 C++标准库(3) 泛型算法、定制操作、再探迭代器、泛型算法结构、特定容器算法](https://editor.csdn.net/md?articleId=120716374) 第10章 泛型算法
[【现代C++】第二部分 C++标准库(4) 关联容器概览、操作、无序容器](https://editor.csdn.net/md?articleId=120716379) 第11章 关联容器
[【现代C++】第二部分 C++标准库(5) 动态内存与智能指针、动态数组、文本查询程序](https://memcpy0.blog.csdn.net/article/details/120716393) 第12章 动态内存

[【现代C++】第二部分 高级主题(1) 标准库特殊设施：tuple、bitset、正则表达式和随机数](https://editor.csdn.net/md?articleId=120716503) 第18章 标准库特殊设施

```
# 8. IO库
## 8.1 IO类
## 8.2 文件输入输出
## 8.3 string流
# 9. 顺序容器
## 9.1 顺序容器概述
## 9.2 容器库概览
## 9.3 顺序容器操作
## 9.4 vector对象是如何增长的
## 9.5 额外的string操作
## 9.6 容器适配器

# 10. 泛型算法
## 10.1 概述
## 10.2 初始泛型算法
## 10.3 定制操作
## 10.4 再探迭代器
## 10.5 泛型算法结构
## 10.6 特定容器算法

# 11. 关联容器
## 11.1 使用关联容器
## 11.2 关联容器概述
## 11.3 关联容器操作
## 11.4 无序容器
# 12. 动态内存
## 12.1 动态内存与智能指针
## 12.2 动态数组
## 12.3 使用标准库：文本查询程序
```
## 第三部分 类设计工具
[【现代C++】第三部分 类设计工具(1) 拷贝、赋值与销毁、拷贝管理和资源管理、交换操作、动态内存管理类、对象移动和右值引用](https://editor.csdn.net/md?articleId=120716454) 第13章 拷贝控制

[【现代C++】第三部分 类设计工具(2) 输入输出、算术关系、赋值、下标、递增递减、成员访问、函数调用、类型转换等运算符重载](https://editor.csdn.net/md?articleId=120716462) 第14章 重载运算与类型转换

[【现代C++】第三部分 类设计工具(3) 基类/派生类定义、虚函数、抽象基类、访问控制与继承、构造函数与拷贝控制、容器与继承、文本查询程序再探](https://editor.csdn.net/md?articleId=120716468) 第15章 面向对象程序设计

[【现代C++】第三部分 类设计工具(4) 函数/类/成员模板定义、模板实参推断、重载与模板、可变参数模板、模板特例化](https://editor.csdn.net/md?articleId=120716478) 第16章 模板与泛型编程
```
# 13. 拷贝控制
## 13.1 拷贝、赋值与销毁
## 13.2 拷贝控制和资源管理
## 13.3 交换操作
## 13.4 拷贝控制示例
## 13.5 动态内存管理类
## 13.6 对象移动
# 14. 重载运算和类型转换
## 14.1 基本概念
## 14.2 输入和输出运算符
## 14.3 算术和关系运算符
## 14.4 赋值运算符
## 14.5 下标运算符
## 14.6 递增与递减运算符
## 14.7 成员访问运算符
## 14.8 函数调用运算符
## 14.9 重载、类型转换与运算符

# 15. 面向对象程序设计
## 15.1 OOP：概述
## 15.2 定义基类与派生类
## 15.3 虚函数
## 15.4 抽象基类
## 15.5 访问控制与继承
## 15.6 继承中的类作用域
## 15.7 构造函数与拷贝控制
## 15.8 容器与继承
## 15.9 文本查询程序再探
# 16. 模板与泛型编程
## 16.1 定义模板
## 16.2 模板实参推断
## 16.3 重载与模板
## 16.4 可变参数模板
## 16.5 模板特例化
```
## 第四部分 高级主题
[【现代C++】第四部分 高级主题(1) 标准库特殊设施：IO再探](https://editor.csdn.net/md?articleId=120716503) 第18章 标准库特殊设施

[【现代C++】第四部分 高级主题(2) 异常处理、命名空间、多重继承与虚继承](https://editor.csdn.net/md?articleId=120716516)  第18章 用于大型程序的工具

[【现代C++】第四部分 高级主题(3) 控制内存分配、运行时内存识别、枚举类型、类成员指针、嵌套内、union、局部类、固有不可移植的特性](https://editor.csdn.net/md?articleId=120716524) 第19章 特殊工具与技术

```
# 17. 标准库特殊设施
## 17.1 tuple类型
## 17.2 bitset类型
## 17.3 正则表达式
## 17.4 随机数
## 17.5 IO库再探
# 18. 用于大型程序的工具
## 18.1 异常处理
## 18.2 命名空间
## 18.3 多重继承与虚继承
# 19. 特殊工具与技术
## 19.1 控制内存分配
## 19.2 运行时类型识别
## 19.3 枚举类型
## 19.4 类成员指针
## 19.5 嵌套类
## 19.6 union：一种节省空间的类
## 19.7 局部类
## 19.8 固有的不可移植的特性
```

---
# 侯捷老师 C++面向对象高级设计 
P1.C++编程简介
15:50
P2.头文件与类的声明
22:54
P3.构造函数
27:42
P4.参数传递与返回值
38:54
P5.操作符重载与临时对象
56:23
P6.复习Complex类的实现过程
28:43
P7.三大函数：拷贝构造，拷贝复制，析构
38:36
P8.堆，栈与内存管理
47:18
P9.复习String类的实现过程
27:53
P10.扩展补充：类模板，函数模板，及其他
38:27
P11.组合与继承
44:44
P12.虚函数与多态
38:26
P13.委托相关设计


---
# 建立和改造GitHub仓库

本节将记录我对相关仓库的建立和日后的改造工作。先创建一个新的空仓库：
![在这里插入图片描述](https://img-blog.csdnimg.cn/66846631ddde4d1889534660d8cce833.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

由于是空仓库，所以什么都没有，白茫茫一片真干净……后来我决定做得更大些，不如改成现代C++学习资料的仓库得了。所以改名为 `Modern-Cxx-Learning-Path` 。目录结构如下所示：
```cpp
$ tree Modern-Cxx-Learning-Path
Modern-Cxx-Learning-Path
|-- Cxx-11&14-HouJie
|-- Cxx-Design-Pattern
|-- Cxx-Memory-Management-HouJie
|-- Cxx-OOP-HouJie
|-- Cxx-Primer
|   |-- Examples
|   `-- Solutions
|       |-- ch01
|       |-- ch02
|       |-- ch03
|       |-- ch04
|       |-- ch05
|       |-- ch06
|       |-- ch07
|       |-- ch08
|       |-- ch09
|       |-- ch10
|       |-- ch11
|       |-- ch12
|       |-- ch13
|       |-- ch14
|       |-- ch15
|       |-- ch16
|       |-- ch17
|       |-- ch18
|       `-- ch19
|-- Cxx-STL-HouJie
|-- Cxx-Startup-HouJie
`-- LICENSE

28 directories, 1 file
```
然后提交到远程仓库：
```cpp
$ git push -u origin main
Enumerating objects: 11, done.
Counting objects: 100% (11/11), done.
Delta compression using up to 4 threads
Compressing objects: 100% (8/8), done.
Writing objects: 100% (10/10), 8.63 KiB | 981.00 KiB/s, done.
Total 10 (delta 0), reused 0 (delta 0), pack-reused 0
To https://github.com/memcpy0/Modern-Cxx-Learning-Path.git
   5fcedec..07c50a6  main -> main
Branch 'main' set up to track remote branch 'main' from 'origin'.
```


# 《数据结构》课本源码与习题解析


## 项目介绍

本项目中的源码与教材《**数据结构-C语言版**》[严蔚敏,吴伟民版]以及《**数据结构题集-C语言版**》[严蔚敏,吴伟民,米宁版]配套。

|           数据结构教材           |           数据结构题集           |
| :----------------------------: | :----------------------------: |
| [外链图片转存失败,源站可能有防盗链机制,建议将图片保存下来直接上传(img-NB9V1kkn-1631639140425)(数据结构教材.png)] | [外链图片转存失败,源站可能有防盗链机制,建议将图片保存下来直接上传(img-5Aqmlg7x-1631639140432)(数据结构题集.png)] |


## 项目结构

本项目包含了**教材源码**跟**习题源码**，并分为4个版本，分别是：**CFree**、**Dev-C++**、**CLion**、**VisualC++**，其中：    

- **CFree** 版本是**早期**上传的完整版本，该版本在CFree这个IDE下测试通过。**此版本虽有瑕疵，但不会再实时维护**，因为新的更新会在下面三个分支版本中呈现。    
- **Dev-C++** 版本是指在Dev-C++这个IDE下测试通过的版本。    
- **CLion** 版本是指在CLion这个IDE下测试通过的版本。    
- **VisualC++** 版本是指在Microsoft Visual C++ 2010这个IDE下测试通过的版本。    

IDE的选择    
> CFree是一个优秀的国产软件，麻雀虽小五脏俱全，非常适合新手使用。不过该产品早已停更，在win10上有些兼容问题，需要调教。    
   
> Dev-C++是一个开源软件，同CFree一样小巧实用。最关键的是，可以兼容win10，推荐使用。    
   
> CLion需要掌握一点cmake知识，对笔记本性能要求也略高。不过JetBrains系列的产品，功能优秀没得说，强烈建议尝试。    
   
> Microsoft Visual C++是微软出品，该系列号称地表最强，不过复杂度也是很高，对于新手并不友好，需要耐心琢磨。如果将来不是走C/C++/C#等路线，可以先不使用。（注：从2018年开始，计算机二级C语言项目的考试中，已将VC++6换成了Microsoft Visual C++ 2010。所以如果有考级需求的同学，请自行熟悉该IDE）    

**习题解析**中存储了《数据结构题集》中非代码题的解析，对于需要写代码解决的问题，参见 **Dev-C++**、**CLion**、**VisualC++** 这三个版本中的源码。    

```
注：
1. "CFree"是完整版本。"Dev-C++"/"CLion"/"VisualC++"是新增的版本，这三个版本最终会取代"CFree"版本。    
2. "CFree"版本既可以用CFree直接打开，也支持用Dev-C++打开，所以当使用CFree遇到兼容问题时，可尝试用Dev-C++。    
3. 上述四个版的代码是同步更新的，但是各版本之间相互独立，没有任何依赖关系，允许单独运行/测试。    
4. 对所有版本的代码均未充分测试，尤其是很多代码没有完成的边界检查(原因是此处以实现算法正确性为目的，而较少考虑程序的健壮性)，所以如有BUG请到Issues反馈。    
```


## 更新目标

总的目标是保障正确性，提高可读性，降低学习难度，具体来说包含以下几点：    

1. ★★★项目工程化。    
2. 修复一些已知/潜在的BUG。    
3. 简化源码之间的引用关系，争取每个模块都可以单独运行测试。    
4. 修剪被引用源码中的次要内容，使得焦点更聚集，重点更突出。    
5. 增加注释与帮助信息，使源码展示更友好。    
6. 出自教材中的算法，会尽量使其代码与教材一致，如有改动，会在注释中提示。其它算法会视情形书写，不唯一。    
7. 修改部分被引入的结构，这一点需要视不同题目的要求而定；大多数被引入的结构会原封不动地保留下来。

## 附：教材源码目录

| 章             | 节                          | 内容                                | 包含算法                          | 备注                         |
| :------------- | :-------------------------- | :--------------------------------- | :------------------------------- | :-------------------------- |
| 01 绪论        | Status                      |                                    |                                  | 定义一些共享常量和函数        |
| 02 线性表      | SqList                      | 顺序表                             | 2.3、2.4、2.5、2.6                | 线性表的顺序存储结构          |
|                | Union                       | A=A∪B                              | 2.1                              |                             |
|                | MergeSqList                 | C=A+B                              | 2.2、2.7                         | 归并顺序表                   |
|                | LinkList                    | 链表                                | 2.8、2.9、2.10、2.11              | 线性表的链式存储结构          |
|                | MergeList                   | C=A+B                              | 2.12                             | 归并链表                     |
|                | SLinkList                   | 静态链表                            | 2.13、2.14、2.15、2.16            |                             |
|                | Difference                  | (A-B)∪(B-A)                        | 2.17                             |                             |
|                | DuLinkList                  | 双向循环链表                        | 2.18、2.19                       |                             |
|                | ELinkList                   | 扩展的线性链表                      | 2.20                             |                             |
|                | MergeEList                  | C=A+B                              | 2.21                             | 归并扩展的线性链表            |
|                | Polynomial                  | 一元多项式                          | 2.22、2.23                       |                             |
| 03 栈和队列     | SqStack                     | 栈                                  |                                  | 顺序存储结构                 |
|                | Conversion                  | 进制转换                            | 3.1                              | 栈的应用                     |
|                | LineEdit                    | 行编辑程序                          | 3.2                              | 栈的应用                     |
|                | Maze                        | 迷宫寻路                            | 3.3                              | 栈的应用                     |
|                | Expression                  | 表达式求值                          | 3.4                              | 栈的应用                     |
|                | Hanoi                       | 汉诺塔                             | 3.5                              | 递归                         |
|                | LinkQueue                   | 链列                                |                                  | 链式存储结构                 |
|                | SqQueue                     | 顺序队列                            |                                  | 循环队列，顺序存储结构        |
|                | BankQueuing                 | 模拟银行排队                        | 3.6、3.7                         | 队列的应用                   |
| 04 串          | SString                     | 顺序串                             | 4.1、4.2、4.3、4.5                | 顺序存储                     |
|                | HString                     | 堆串                                | 4.4                              | 顺序存储，动态分配内存        |
|                | LString                     | 块链串                             |                                  | 顺序存储+链式存储             |
|                | KMP                         | KMP算法                             | 4.6、4.7、4.8                    | 字符串匹配算法               |
|                | WordList                    | 关键词索引                          | 4.9、4.10、4.11、4.12、4.13、4.14 | 堆串和线性表的应用            |
| 05 数组和广义表 | Array                       | 多维数组                            |                                  |                             |
|                | TSMatrix                    | 稀疏矩阵                            | 5.1、5.2                         | 三元组顺序表存储方式          |
|                | RLSMatrix                   | 稀疏矩阵                            | 5.3                              | 行逻辑链接的顺序表存储方式     |
|                | CrossList                   | 稀疏矩阵                            | 5.4                              | 十字链表存储方式              |
|                | GList-HT                    | 广义表                             | 5.5、5.6、5.7、5.8                | 头尾链表存储表示              |
|                | GList-E                     | 广义表                             |                                  | 扩展线性链表存储表示          |
|                | MPList                      | m元多项式                           |                                  | 链式存储                     |
| 06 树和二叉树   | SqBiTree                    | 二叉树顺序存储结构                  |                                  |                             |
|                | BiTree                      | 二叉树的二叉链表存储结构             | 6.1、6.2、6.3、6.4                |                             |
|                | BiTriTree                   | 二叉树的三叉链表存储结构             |                                  |                             |
|                | BiThrTree                   | 线索二叉树                          | 6.5、6.6、6.7                    |                             |
|                | PTree                       | 树的双亲表存储表示                  |                                  |                             |
|                | CTree                       | 树的孩子链表(带双亲)的存储表示       |                                  |                             |
|                | CSTree                      | 树的二叉链表（孩子-兄弟）结构存储表示 |                                  |                             |
|                | MFSet                       | 集合                                | 6.8、6.9、6.10、6.11              |                             |
|                | HuffmanTree                 | 赫夫曼树                            | 6.12、6.13                       | 又称"哈夫曼树"               |
|                | PowerSet                    | 冪集                                | 6.14/6.15                        |                             |
|                | NQueens                     | N皇后问题                           | 6.16                             |                             |
| 07 图          | MGraph                      | 图的邻接矩阵存储                    | 7.1、7.2、7.4、7.5、7.6           | 有向图、有向网、无向图、无向网 |
|                | ALGraph                     | 图的邻接表存储                      |                                  | 有向图、有向网、无向图、无向网 |
|                | OLGraph                     | 图的十字链表存储                    | 7.3                              | 有向图、有向网、无向图、无向网 |
|                | AMLGraph                    | 图的邻接多重表存储                  |                                  | 无向图、无向网               |
|                | SpanningTree                | 无向图的生成树                      | 7.7、7.8                         | 深度优先生成树               |
|                | StronglyConnectedComponents | 有向图强连通分量                    |                                  | Kosaraju算法和Tarjan算法     |
|                | MinimumSpanningTree         | 无向网的最小生成树                  | 7.9                              | Prim算法和Kruskal算法        |
|                | ArticulationPoints          | 无向图的关节点                      | 7.10、7.11                       |                             |
|                | TopologicalSorting          | AOV-网的拓扑排序                    | 7.12                             | 有向图                       |
|                | CriticalPathMethod          | AOE-网的关键路径                    | 7.13、7.14                       | 有向网                       |
|                | ShortestPaths               | 最短路径算法                        | 7.15、7.16                       | Dijkstra算法和Floyd算法      |
| 08 动态存储管理 | BoundaryTagMethod           | 边界标识法                          | 8.1                              |                              |
|                | BuddySystem                 | 伙伴系统                            | 8.2                              |                              |
|                | GarbageCollection           | 无用单元收集                        | 8.3                              | 无栈遍历广义表                |

