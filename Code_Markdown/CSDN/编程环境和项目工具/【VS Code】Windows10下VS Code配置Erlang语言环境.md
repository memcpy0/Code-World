@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。
 
[获取了VS Code编辑器之后](https://memcpy0.blog.csdn.net/article/details/117640795)，我们将在VS Code中配置Erlang的语言学习环境。同样的，我会在个人代码库 `CodeWorld` 中建立 `Code_Erlang` 文件夹，里面的文件夹是 `Erlang_Single` 和 `Erlang_Multiple` ，分别存放不同的分类代码文件夹，即程序运行目标分别是**单个源代码文件**或**少数几个源代码文件的组合**。

---
# 1. 下载安装Erlang
进入Erlang的[官网](https://www.erlang.org/)如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/15c60ab55a9f4af78c57a6a5ad9c38d4.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
[Erlang文档](https://erlang.org/doc/design_principles/users_guide.html)页面如下，里面包括了在线文档、Erlang书籍和论文：
![在这里插入图片描述](https://img-blog.csdnimg.cn/7f32b99e86124fc4b703e8cb810d4269.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


什么是OTP？**OTP是一组Erlang库和设计原则，用来给系统开发提供中间件** `middle-ware` ，它包括自己的分布式数据库、面向其他语言的接口、调试和发布工具，可以仔细看一下[OTP文档](https://erlang.org/doc/design_principles/users_guide.html)。目前最新发布的是OTP 24.0，我们进入下载界面，选择合适的版本：
![在这里插入图片描述](https://img-blog.csdnimg.cn/03e9ee3c3ef34d9985ed3b23c9177938.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
下载速度有点慢，耐心等待。之后运行安装程序：
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/c437086674884099829678f642019d65.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
可以选择安装路径：
![在这里插入图片描述](https://img-blog.csdnimg.cn/b99d3d3685a24597b067e6f02e2e3ed2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
不创建开始菜单中的快捷方式，最后点击安装：
![在这里插入图片描述](https://img-blog.csdnimg.cn/cfa9e83018ce423cb8d3c086d12e27da.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
安装完成后，将 `C:\Program Files\erl-24.0\bin` 添加到环境变量PATH中。这一目录下有这些工具：
![在这里插入图片描述](https://img-blog.csdnimg.cn/de2c0b7bd2804639b227a853ee81dfdd.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
在命令行中执行如下命令，不报错就说明Erlang安装成功了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/7eef1c8662c54c979aaa78d4056b0513.png)
随手创建一个 `hello.erl` 文件，输入以下代码：
```erlang
-module(hello).
-export([start/0]).
start() ->
	io:format("Hello World~n").
```
命令行中进入该目录，使用 `erlc` 编译 `hello.erl` 文件，并生成 `hello.beam` 文件：
```bash
C:\Users\21839>erlc hello.erl
C:\Users\21839>erl -noshell -s hello -s init stop
Hello World
```
也可以使用 `erl` 解释运行：
```bash
C:\Users\21839>erl
Eshell V12.0  (abort with ^G)
1> c(hello).
{ok,hello}
2> hello:start().
Hello World
ok
3> halt().
```

---
# 2. 下载安装构建工具Rebar3
部分类似于Haskell的Stack、Scala的SBT，**Rebar3是Erlang的官方构建工具**，可以用它来编写 `write` 、构建 `build` 、运行 `run` 、测试 `test` 、发布 `release` 和维护 `maintain` Erlang项目。我们进入[Rebar3官网](https://www.rebar3.org/)：
![在这里插入图片描述](https://img-blog.csdnimg.cn/d7a95494e1a747819cee087d3723adc6.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
[Rebar3的文档和教程](https://www.rebar3.org/docs/)如下所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/43ce0d24cd8a4c0e9740ba6f2eb278b3.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
先下载最新的[Rebar3](https://s3.amazonaws.com/rebar3/rebar3)，对于Windows来说，想要从Powershell或者CMD使用Rebar3，需要先把下载的Rebar3文件移动到Erlang的环境变量路径下，再新建一个 `rebar3.cmd` 文件，内容如下：
```bash
@echo off
setlocal
set rebarscript=%~f0
escript.exe "%rebarscript:.cmd=%" %*
```
为了验证Rebar3安装成功与否，运行 `rebar3 --version` 命令，显示版本号即成功：
![在这里插入图片描述](https://img-blog.csdnimg.cn/e2342cfd05af4580b4382538f5f41ea9.png)
现在可以尝试使用Rebar3新建Erlang项目，比如执行 `rebar3 new umbrella myproj` 新建一个 `myproj` 项目：
```bash
C:\Users\21839>rebar3 new umbrella myproj
===> Writing myproj/apps/myproj/src/myproj_app.erl
===> Writing myproj/apps/myproj/src/myproj_sup.erl
===> Writing myproj/apps/myproj/src/myproj.app.src
===> Writing myproj/rebar.config
===> Writing myproj/config/sys.config
===> Writing myproj/config/vm.args
===> Writing myproj/.gitignore
===> Writing myproj/LICENSE
===> Writing myproj/README.md
```
---
# 3. VS Code中安装支持插件
在VS Code中搜索如下插件并安装：
![在这里插入图片描述](https://img-blog.csdnimg.cn/aaf4003a8df949f7aa9bfa4fd3445e2d.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

这一插件的功能相当齐全，以下列出了大部分特性，更多内容可以查看插件的文档：
- 编辑支持：语法高亮，自动缩进，智能补全，动态展示错误和警告，跳转至定义/查看定义，鼠标悬停显示标准函数的帮助等等；
- 构建：以Rebar3为默认的构建工具，`rebar.config` 文件要放在项目根目录下；
- 调试：展示当前作用域下的变量列表，调用栈显示和控制，全断点支持等等；

---
# 4. 语言环境学习和插件配置
## 4.1 创建新项目
VSCode的Erlang插件，默认使用Rebar3生成的目录结构来进行调试，所以需要使用Rebar3来创建、构建、运行和调试项目。下面在VS Code终端运行 `rebar3 new app hello` 创建一个 `hello` 项目，包括一个 `rebar.config` 文件：
```bash
$ rebar3 new app hello
===> Writing hello/src/hello_app.erl
===> Writing hello/src/hello_sup.erl
===> Writing hello/src/hello.app.src
===> Writing hello/rebar.config
===> Writing hello/.gitignore
===> Writing hello/LICENSE
===> Writing hello/README.md
```
## 4.2 配置文件书写
首先完成任务文件 `tasks.json` 的书写，**终端->配置默认生成任务**，选择**使用模板创建 `tasks.json` 文件**：
![在这里插入图片描述](https://img-blog.csdnimg.cn/b1d26c383d48421ca8aef48c00445e7c.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/9bda5f7af74141f986affb823f2e8b1a.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

然后是**运行任意外部命令的示例**：
https://blog.csdn.net/witton/article/details/107980676

要完成launch.json erl arguments 

