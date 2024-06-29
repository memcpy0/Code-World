@[toc]


# 0. 本文前言
在另外一篇[关于VS Code的文章](https://memcpy0.blog.csdn.net/article/details/117640795)中，我写过：
> 为什么使用VS Code？对我个人而言，**VS Code最突出的特性就是它强大的插件和配置功能**，可以根据需求和想法下载不同的插件、运用配置文件定义出**高度个性化的语言和项目环境**，定制出属于自己的编辑器。可以说，**多样化的插件和个性化的功能配置，是VS Code的灵魂所在**（建议仔细看完VS Code官方文档，深入了解VS Code的各种强大功能）。
> <b></b>
> 对VS Code中各种语言和项目环境的配置，无非是想要将VS Code打磨成个人趁手的兵器，**打造出一把无往而不利的瑞士军刀**——而非每学一门编程语言就下载一个专业IDE，如Python的Pycharm、Java的Intellij、前端的Webstorm等等（虽然JetBrains的IDE全家桶很强大），摆脱掉Eclipse、CodeBlocks等过时的IDE和VC6.0、DevC++等垃圾软件，同时为Haskell、Io、Erlang等开发工具支持力度较弱的语言（没听说过什么很好的IDE）创造良好的学习环境。
> <b></b>
> 这也是我最近的体会——VS Code插件系统的强大，**直接或间接地降低了很多新兴或小众语言开发的门槛**，让很多没有很好的IDE支持的语言可以舒适的写程序；而在大众语言如C/C++/Java/Python等方面，使用VS Code开发大型项目固然是短板，却也有利于中小型项目的敏捷开发。然而福兮祸所依、祸兮福所伏，在配置Graphviz环境时，我也发现，这些小众语言的插件怎么用怎么配都感觉是东拼西凑的，**各插件之间很难完美配合协同**，恨不得自己撸起袖子上阵造一个支持插件，把所有这些功能融合在一起。这也是VS Code固有的不足之处吧。

Vim、Emacs和VS Code虽然同为编辑器，但是三者之间有很大的不同，VS Code相对来说易用很多，而Vim和Emacs的学习曲线都很陡峭：
![在这里插入图片描述](https://img-blog.csdnimg.cn/84a581e0161f487286f6e188b6945146.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
Vim被称为「编辑器之神」，Emacs被称为「神的编辑器」，分别强调了它们在**编辑之内**或**编辑之外**的强大特性：
- Vim最厉害之处在于**Composability**，即编辑命令的组合能力。Vim引入了 `normal, insert, visual` 等多种编辑模式，如 `normal` 模式下按键将输入**命令或其组合序列**而非字符，`insert` 模式则是常规的编辑模式。通过**模式编辑**和**读写分离**，使编辑操作变得十分高效，**让人充分领略到编辑的快感**。可以说，Vim在「编辑」上做到了极致，除了学习曲线比较陡峭外没有明显的缺点。
Emacs虽然选择了使用**快捷键及其组合**来提高编辑效率，但是多次敲击Ctrl/Shift/Alt这些功能键相当痛苦；手指一直按住功能键导致按键序列不能过长；功能键不表意难以记忆。相较而言，个人还是比较喜欢使用命令序列来实现操作，（有含义的命令）也更容易记忆。
此外要说明的是，**Windows之外几乎所有的主流系统都默认安装了Vim**，开箱即用十分便捷，在远程登录的终端中远比Emacs好用，也省去了配置Emacs的时间。**Vim还能作为大部分命令行工具的输入输出界面**，对命令行模式具有更好的支持，是命令行生态圈的一个重要组成环节。**各种IDE和主流编辑器都有Vim模拟器**，还能一键开Vim（即模拟器无法完成的功能，可以打开Vim并跳转到同样位置，解决后保存退出返回原本的IDE或编辑器）。还可以用一个全局脚本，在任何文本输入框中使用Vim的编辑功能，加速文本操作。毫无疑问，**Vim真正做到了一次学习到处使用** `Learn Once, User Everywhere` 。
- Emacs最厉害之处则在于**Extensibility**，即Emacs本身和插件的可扩展性。由于Emacs本身就是由大量Emacs Lisp和少量C构建的软件，开放了几乎所有的API，我们甚至可以直接查看和修改Emacs的实现细节。使用几十行甚至几行Emacs Lisp代码就可以完成强大的插件，实现包括编程、编译乃至网络浏览、玩游戏、播放音乐等等扩展功能，而且**这样实现的插件往往可以和整个Emacs浑然一体、完美协调**。因此Emacs又有“Emacs操作系统”之称。
缺点是，Emacs插件有时可能过于个性化（插件也可以通用实现，还能开源共享给他人、为社区做贡献），使得任何两个人的Emacs配置都可能完全不同，因此**想在不同的Emacs配置间切换工作比较困难**；Emacs倾向于使用Emacs Lisp，然而**Elisp缺少流行语言那样充足的库**，而且运行效率比较慢，需要个人定制和优化，因此不太适合“伸手党”学习与使用。


综合来看，**VS Code基本上是Vim和Emacs的折中方案**，编辑操作混杂着GUI界面（鼠标操作）、各种快捷键组合以及插件定义的命令，**比不上Vim的高效编辑**；VS Code虽然有众多的插件，但对于某些小众语言，却难以将（不同作者实现的）各个插件的功能完美协调，自己写插件又很麻烦，**比不上Emacs的扩展能力**。因此，**在学习VS Code的同时，我选择在业余时间学一下Emacs+Evil Mode（Vim模拟插件）**，这样就相当于同时使用Emacs和Vim——为什么要互相排斥呢？我 全 都 要！值得一提的是，LeetCode就有Vim模式的键位绑定，刷题时可以顺便练习一下Vim：
<img src="https://img-blog.csdnimg.cn/2d502dc72b784e5484d1600cf5c95227.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="65%">


配置难度大、上手曲线陡峭是不假，只是使用Emacs和Vim无非是想要获得更高的编辑效率和更好的编程体验，所以上手Emacs和Vim，绝对不要做的事情就是**自己手动去配置**，先使用大佬给的开箱即用的配置即可。然后仔细阅读使用手册，把文档读好，就不会有什么大的问题。另外，这是陈斌大佬的[文章](https://github.com/redguardtoo/mastering-emacs-in-one-year-guide/blob/master/guide-zh.org)，**常读常新**。


---
# 1. 下载安装Emacs
Emacs有多个版本，我们先使用GUN Emacs，其[官网](https://www.gnu.org/savannah-checkouts/gnu/emacs/emacs.html)如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/a9764a6db2cf4abfbe844dfe862dbe02.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
GNU Emacs有如下特性：
- 对于多种文件类型，具有内容感知的编辑模式，包括语法着色；
- **完整的内置文档，包括针对新用户的教程**；
- 对于几乎所有源代码文件的完全的Unicode支持；
- **高度可定制**，使用Emacs Lisp代码或图形界面；
- 除了文本编辑之外，还有很多功能，包括项目策划、邮件和新闻阅读器、调试程序接口、日历、IRC客户端与更多； 
- 一个包管理系统，用于下载和安装扩展

点击相应版本的下载按钮，进入说明界面，然后点击 `nearby GNU mirror` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/82c810ecf46345cf9d984aa71cb63a27.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
毫无疑问，选择最新的 `emacs-27` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/6f00e7b380b0466e9f9b50cd9aa5ade7.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/fe8cfe04e8144db4a08d8c9e9f9b5fd2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

下载后运行安装器，一路 `next` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/ca9c9a48e597449aa56804e80cc51b89.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/b1944a46f2a44aafba489e88933f3347.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/f6c3ad6b77954a83a7f08ae89731b9e2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/276bada317434eb0848a6fb9f04be9f9.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
安装之后要创建快捷方式：
![在这里插入图片描述](https://img-blog.csdnimg.cn/a6141c59910442468a0561db6a2dabef.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


---
# 2. Emacs资料和学习路径
从GNU Emacs网站上，有这些资料可以阅读：
- [GNU Emacs文档和支持](https://www.gnu.org/software/emacs/documentation.html)，包括GNU Emacs在线手册、Elisp引导教程、Elisp语言手册、其他Emacs手册：
![在这里插入图片描述](https://img-blog.csdnimg.cn/95344bd2dbbd45119f6429bcb5fe51f7.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
- 一个Emacs的[引导教程](https://www.gnu.org/software/emacs/tour/index.html)，本文中会学习这篇教程：
![在这里插入图片描述](https://img-blog.csdnimg.cn/cb55813f0f984c14b112475c6617d7a2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

- [Emacs Wiki](http://www.emacswiki.org/)，以后可能经常参考上面的内容：
![在这里插入图片描述](https://img-blog.csdnimg.cn/d998dd62e2644f259d5b753eae2f7698.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

此外还有这些资料：
- [http://emacslife.com/](http://emacslife.com/)：
![在这里插入图片描述](https://img-blog.csdnimg.cn/9e5dff7f706d46589247826bed693628.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
- [http://ergoemacs.org/emacs/blog.html](http://ergoemacs.org/emacs/blog.html) 李杀大佬的博客，可以在Patreon上赞助一下。

学了本文（几乎是翻译过来）的引导教程和Emacs tutorial之后，就基本入门了Emacs；然后就是学习Elisp引导教程，入门Elisp。之后是EmacsWiki，作为Emacs和Elisp的入门和进阶资料，搭配上李杀大佬的几千篇博客，基本就进阶了。

---
# 3. Vim学习笔记
[【VIM】学习笔记一 Vim的六种基本模式和基本操作](https://memcpy0.blog.csdn.net/article/details/108459015)
[【Vim】学习笔记二 Vim文档编辑](https://memcpy0.blog.csdn.net/article/details/108462793)
[【Vim】学习笔记三 替换、撤销、快速缩进、查找操作](https://memcpy0.blog.csdn.net/article/details/108466065)
[【Vim】学习笔记四 多文件编辑、可视模式、视窗操作](https://memcpy0.blog.csdn.net/article/details/108472531)

---
# 4. Emacs引导教程学习
## 4.1 相关示例
[GNU Emacs Manual](https://www.gnu.org/software/emacs/manual/html_node/emacs/index.html)中描述Emacs为 `extensible, customizable, self-documenting and real-time display eidtor` ，下面是关于Emacs的一些简单示例，以展示Emacs的强大之处：
- 能够书写普通文本文件，支持多种不同的人类/编程/标记语言；
![在这里插入图片描述](https://img-blog.csdnimg.cn/39c4cb9e9f014f6aa5e4513617e3d619.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
- 也能用作编译、运行和测试程序的工具，比如Emacs集成了GDB `M-x gdb` :
![在这里插入图片描述](https://img-blog.csdnimg.cn/aae1a82c4e2a422e892d525fca205835.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
- 能比较两个文件并高亮显示不同之处 `M-x ediff` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/552618040f584c6bb41d1fc03c1daf55.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

- 也能作为一个文件管理器 `M-x dired` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/3a4dcedbcbde4fb4882d14b11d8bb631.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
- 甚至能用Emacs阅读新闻、邮件、RSS feeds `M-x gnus` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/efae580ae2804e919ad076e4ba70c31f.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
- 甚至能在其中玩俄罗斯方块 `M-x tetris` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/47987610713f410bb38c6c99f4973448.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
 
## 4.2 为什么是Emacs？
通过为许多不同种类的任务提供一个集成环境，Emacs能让你充满创造力：
- 不论你尝试做什么，所有基础的编辑命令均可使用：写代码，读手册、使用Shell、编辑邮件等等；
- 不论你尝试做什么，所有Emacs提供的工具均可使用：打开、保存、搜索、处理文本等等；

如果说Vim统一了编辑操作，Emacs则提供了更大领域的统一性——工作在Emacs中经常比学习使用某个单独的程序更轻松，**尤其是该程序拥有自己的一组编辑功能和快捷方式时**，比如VS Code。 

此外，如果Emacs没有按照你喜欢的方式工作，你可以使用Emacs Lisp来定制Emacs、自动化常规任务、添加新特性。Elisp易于掌握、力量强大，使用它能更改和扩展Emacs几乎所有的功能。通过编写Elisp让Emacs变成你的形状——一个证明是，上述提到的所有特性（以及之后描述的更多功能），都是用Elisp完成的。

Emacs还相当便携，能在多种平台如GNU/Linux、BSD和其他Unix衍生系统，以及一些专有操作系统如 Microsoft Windows上，使用同样的配置和同样的编辑器。


## 4.3 开始之前
安装Emacs之后，无论何时想要开始使用Emacs，都**应该打开Emacs tutorial**，这是一个交互式的实践教程，可以打开Emacs并键入 `C-h t` 即 `Ctrl-h t` ，包括如下内容：
- 打开和退出Emacs；
- 基础的文本移动和编辑命令；
- 打开和保存文件；
- Emacs的概念：`windows, frames, files, buffers` ；
- 通过快捷键和 `M-x` 调用命令

关键是有中文版：
![在这里插入图片描述](https://img-blog.csdnimg.cn/1a1fe73b9e78482094507abf7434ef16.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
如果提到了[(info "(emacs)Using Region")](https://www.gnu.org/software/emacs/manual/html_node/emacs/Using-Region.html)之类的内容，就是看Emacs Manual的时候了，[在线版本](https://www.gnu.org/software/emacs/manual/emacs.html)或在Emacs上用内置文档阅读器阅读 `C-h r` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/e5f19409c1e741f69b5153a75ab4c93f.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
还能通过 `C-h C-h` 查看所有的帮助特性（**Emacs中的快捷键在精不在多**，而且还要用Evil Mode，所以记忆那么二十来个Emacs快捷操作就够了……至少上述三个快捷键要记住）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/e598f9ef63c04988ae0f69d78ddb7de2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
## 4.4 文本操作的力量
本节介绍文本操作 `Text Manipulation` 

……
