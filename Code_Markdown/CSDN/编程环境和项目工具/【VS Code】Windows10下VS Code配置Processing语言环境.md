@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。
 
[获取了VS Code编辑器之后](https://memcpy0.blog.csdn.net/article/details/117640795)，我们将在VS Code中配置Processing的语言学习环境。

---
# 1. 下载安装Processing
[Processing官网](https://processing.org/)界面如下，和其他编程语言一样，有教程 `Tutorials` 、示例 `Example` 、书籍 `Books` ，还有语言参考 `References`（用于解释Processing如何运行、如何为该语言编写有效源代码的文档，涵盖分支语句如何工作、有哪些常用类型、如何使用不同的运算符、如何书写注释等等）、库说明 `Libraries` 等资料：
![在这里插入图片描述](https://img-blog.csdnimg.cn/f0cb2dfe65ff4eeaaa6ed0750bcb3f65.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

点击左上角的 `Download` 链接，进入页面选择合适的软件版本，我下载的是Processing Windows 64 bit 3.5.4版。个人体验来看，从官网下载慢的出奇，动不动还下载失败，如果读者也出现这种状况，可以找个别的地方来下载安装包：
![在这里插入图片描述](https://img-blog.csdnimg.cn/dc703a676ba14db1beb85390e535f76b.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

之后解压包到C盘的 `Program Files` ，得到下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/1842ea9ecea84ff1992755bf432d72b1.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
其中的 `processing.exe` 是Processing的开发环境，或者说是 `Processing Development Environment (PDE) ` ，我们可以在文本编辑器中书写程序、使用运行按钮执行程序。**在Processing中，一个计算机程序被称为一个速写 `sketch` ，速写被存储在速写本 `sketchbook` 中**，实际上就是计算机上的一个文件夹。**Processing软件还有多种不同的模式 `modes`** ，方便部署 `sketches` 到不同的平台和程序上，默认模式是Java模式，其他模式可以自行从PDE右上角的下拉菜单栏下载： 
![在这里插入图片描述](https://img-blog.csdnimg.cn/29d80c7f9e4d4ee29afb7b5a8a89bb37.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

对于这一官方IDE，可以把常用配置都勾选了，根据自己的喜好更改下字体和字号。更多自定义配置可以在 `preferences.txt` 中设置，建议把默认搜素引擎改为百度 `search.format=https://baidu.com/search?q=%s` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2cf6705fdc774065922cc4abfd9cf64d.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
通过**文件->范例程序->Basics->Camera->MoveEye**，我们可以打开这一程序，然后**速写本->运行**，得到如下画面：
![在这里插入图片描述](https://img-blog.csdnimg.cn/88e41d1786cf4eed9f7e0478bc485fd2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
试用过后，我们要把Processing所在的路径加入到环境变量PATH中：
![在这里插入图片描述](https://img-blog.csdnimg.cn/91d94b1603d54897bc7a1963cfcdff5b.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
坦白来说，PDE的功能差强人意，不过有了VS Code，我们似乎可以做得更好（也不尽然）。

---
# 2. VS Code中安装支持插件、插件使用介绍
在VS Code扩展商店中，搜索Processing并下载第一个插件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/95d4846e356a4525a4adbe9e816c13ba.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这一插件的说明相当清晰，它的功能有：
- 语法高亮 `Syntax highlighting` ；
- 自定义代码片段 `Snippets` ；
- 快捷命令，在命令面板中可以执行如下命令：
	- `Create Task File` ：存在 `.pde` 源代码文件时，在项目文件夹中自动添加一个 `.vscode/tasks.json` 文件，内容如下，该文件拥有位于该项目根文件夹中的 `ProcessingTasks.json` 的内容。当使用快捷键 `Ctrl+Shift+B` 运行此任务时，将编译并运行你的项目。
	如果想要查看编译器的输出，只需要注释掉配置文件中的 ~~`"showOutput"："never"`~~（插件文档没有及时更新，`showOutput` 属性已被弃用，改为注释掉 `presentation` 属性内的 `"reveal": "always"` ）。 
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/833843813a974c31baa4d70131064340.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
	- `Run Processing Project` ：只是一个快捷方式，在 `Create Task File` 之后实际运行 `tasks.json` 中的任务——不如直接使用 `Ctrl+Shift+B` 。在Java模式下，会调用Processing编译器把PDE源文件编译为Java源文件，然后把Java源文件编译为Class文件，并实际执行程序。
	- `Open Extension Documentation` ：打开 `processing.org` 在线文档的语言参考 `Reference` 页面，相当于**PDE菜单栏->帮助->参考文档**；
	- `Open Documentation for Selection` ：对于当前选定内容，使用Google搜索 `processing.org` 在线文档，相当于**PDE菜单栏->帮助->在文档中查询**；
	- `Search Processing Website` ：输入搜索内容，默认使用Google搜索 `processing.org` 在线文档。
 - 可以设置项目文件夹下的 `settings.json` 文件，使用 `p5js.org` 文档（好像是国人建立的网站，有简体中文版，**可以和官网文档对照着看**）而非 `processing.org` 文档、使用DuckDuckGo而非谷歌搜索：
	```json
	{
	    "processing.docs": "p5js.org",
	    "processing.search": "DuckDuckGo"
	}
	```

使用这一插件还需要注意的是：
- 不能调试Java或者Processing项目；
- 必须先准备 `.vscode/tasks.json` 文件（命令 `Create Task File`）；
- 必须将Processing添加到PATH中，或者设置配置文件中的 `"Processing.path"` 属性；
- 要有一个 `.pde` 源代码文件，且文件名和项目文件夹名一致（`General Processing Requirement`）。

从目前来看，Processing自带编辑器PDE的优点有：
- **实时的错误提示，快速报告错误情况**；
- **引用、查看、下载和管理第三方库文件**；
- **模式众多，切换方便**；
- **工具丰富，内置的有字体创建、颜色选择器、速写本压缩输出、Movie Maker，可以添加和管理工具**；
- **有大量范例程序可供学习**；
-  **拥有调试器，可以调试程序**

VS Code编辑器和 `Processing Language` 插件的优点有：
- 更加智能的代码补全，折叠代码，自定义代码片段，编码体验相当舒适；
- 自动保存，文件管理等

好像不太行啊，VS Code和这个插件还不如PDE呢。可以卸载掉 `Processing Language` 插件，转而安装 `Processing VSCode` 插件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/3201e8b1210c4646a0648b55e4aba912.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这一插件其实是前者的一个分支，在前者的基础上做了以下变动：
- 更好的代码高亮；
- **鼠标悬停时显示文档** `Documentation on hover` ，这一点十分强力；
- **一个运行按钮**，包括 `Processing Java` 和 `Processing Python` ；
- 通过 `Processing-java CLI` ，进行简单的诊断，这一功能默认是关闭的；
- 自动关闭字符串 `Strings are auto closing and surrounding` ；
- 删除了命令 `Create Task File` 和 `Run Processing Project` ，不需要创建 `tasks.json` 文件，不用执行命令来运行程序，也没有了 `Ctrl+Shift+B` 快捷键；
- 增加 `Run` 命令，用于运行当前的Processing项目，自动检测是 `Processing Java` 还是 `Processing Python` ，对应的命令分别是 `RunJava` 和 `RunPy` 。 

对Python模式的增强这一点我恐怕是用不上了，[官方要求](https://py.processing.org/tutorials/command-line/)比较严格：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2942a3b3445e49b38af3f9865e2c1dc1.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

插件的配置步骤也比较麻烦：
![在这里插入图片描述](https://img-blog.csdnimg.cn/15fcddeb24084679afac5858d6bfa237.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

这两个插件谁好谁坏，各自心中有数就行，其他的好像没什么好说的了。VS Code用来**编写代码**、**查看文档**，PDE用来**调试程序**、**安装第三方库和工具**、**切换模式**和**实际运行**，两者相辅相成共同构建Processing的语言学习环境。
 
