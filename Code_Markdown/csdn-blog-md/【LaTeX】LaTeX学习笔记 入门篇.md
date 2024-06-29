@[toc]

 

食用本文的前提：首先看完这篇文章，[【VS Code】Windows10下VS Code配置LaTeX语言环境](https://memcpy0.blog.csdn.net/article/details/118217633) ，配置好语言学习环境。

---
# 1. $\TeX$ 和 $\LaTeX$ 入门简介
$\TeX$ 是由美国高德纳教授编写的一款功能强大的排版软件，也是一种计算机语言。后来出现了很多个不同的版本，现在经常提到的其实是 $\mathtt{Plain}\ \LaTeX$ ，它是 $\TeX$ 的一种改进。

由于 $\TeX$ 的命令比较简陋，为了方便其他人的使用，计算机科学家莱斯利·兰伯特在20世纪80年代初期，基于 $\TeX$ 并对 $\TeX$  进行扩展，开发了一个更加高级的排版系统——$\LaTeX$ 。$\LaTeX$ 是当前世界上最流行、使用最为广泛的 $\TeX$ 宏集，**真正做到了开箱即用**，它的使用者基本不需要自己设计命令、定义宏，因为 $\LaTeX$ 已经替你做好了。因此，即便是不太了解的初学者，也能够在短时间内通过修改 $\LaTeX$ 模板，生成高质量的文档。

某种意义上来说，$\LaTeX$ 之于 $\TeX$ ，就像汇编之于机器语言、C之于汇编、Java之于C，是更高层次的抽象。而 $\LaTeX$ 本身的语法则和HTML比较相似，其编辑和排版的工作方式更是类似于HTML与CSS、JavaScript之间的关系——**核心思想是，通过章、节等概念指定文档的逻辑结构，规定每一句话在文章中的层次，然后分别对每个层次进行格式渲染和实际布局**。从而将内容和布局相分离，极大提高了工作效率，同时 $\LaTeX$ 也允许用户在需要时自行调整排版和布局，具有高度的灵活性。

---
# 2. $\LaTeX$ 基本使用
## 2.1 第一个 $\TeX$ 文档
首先在个人代码库 `CodeWorld/Code_LaTeX` 中建立一个文件夹 `learn-latex` 然后打开该文件夹，新建一个 $\mathtt{tex}$ 文档，文件名为 `learn1.tex`——**尽量避免中文、特殊字符，只写英文字符**。随后将下列内容复制进去：
```css
\documentclass{article}
\begin{document}
    Hello LaTeX World!
\end{document} 
```
接着直接 `Ctrl+Alt+B` 构建 $\LaTeX$ 项目，顺利的话你就成功用 $\LaTeX$ 生成了第一个PDF！然后 `View in VSCode tab` ，效果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210703002530792.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

简单介绍一下这几个必须要用到的命令：
- `\documentclass{}` ，指定文档类型（用于模板），后面会有详细介绍，这里不做展开；
- `\begin{document}, \end{document}` ，这两个命令之间就是你的文档内容，前面的部分则是**导言区**。类似于HTML的 `<html>, </html>` 与 `<head>, </head>; <body>, </body>` 。


## 2.2 标签、作者、日期
建立一个新文档，或者直接修改前面的文档，将下列内容复制进去：
```css
\documentclass{article}
\author{memcpy0}
\title{learn2.tex}
\begin{document}
    \maketitle
    Hello LaTeX World!
\end{document}
```
`Ctrl+Alt+B` 构建一下，然后 `Ctrl+Alt+V` 外部查看PDF文档：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210703004108308.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

再简单介绍这些命令：
- `\author{}` ，指定文档作者的姓名；
- `\title{}` ，指定文章的标题；
- `\maketitle{}` ，排版标题、作者姓名和日期。一般情况下，会自动选取编辑这篇文章的当天，作为日期并排版上去；想要自己设置，可以在 `\title{}` 下加上命令 `\date{}` 来指定日期。



## 2.3 章节、段落
同样的，开一个新文档或者修改之前的文档：
```css
\documentclass{article}
\author{memcpy0}
\title{learn3.tex}
\begin{document}
    \maketitle

    \section{First}
    Hello LaTeX World!
    \subsection{second}
    Ha!Ha!Ha!
    \subsubsection{Third}
    \paragraph{HI}
    Hi!
    \subparagraph{AI}
    Ai!
\end{document}
```
构建并查看：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210703005144160.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
简单介绍这几个命令：
- `\section{}, \subsection{}, \subsubsection{}` 一级、二级、三级章节标题；
- `\paragraph{}, \subparagraph{}` 一级、二级段落
- 如果想要去掉章节前的数字编号，可以在命令中大括号前加上一个 `*` ，例如 `\section*{}` ，不过**去除编号后的章节标题，默认不会添加到目录中**。

## 2.4 目录
```css
\documentclass{article}
\author{memcpy0}
\title{learn4.tex}
\begin{document}
    \maketitle
    \tableofcontents
    
    \section{First}
    Hello LaTeX World!
    \subsection{Second}
    Ha!Ha!Ha!
    \subsubsection{Third}
    \paragraph{HI}
    Hi!
    \subparagraph{AI}
    Ai!
\end{document}
```
构建并查看：
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20210703011439107.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)



其中的 `\tableofcontents` ，将根据你的章节信息自动生成目录，不过 **$\LaTeX$ 处理目录需要编译两次**，具体来说指的是要使用 `Recipe: xelatex*2` 这样的构建选项。关于目录，之后还会有深入的讲解。

## 2.5 换行、注释
```css
\documentclass{article}
\author{memcpy0}
\title{learn5.tex}
\begin{document}
    \maketitle
    hahahaha
    hahaha

    haha
    ha

    12345 %后面是注释,不会被打印

    hihi\\
    hihihi 1234     1234\par
    hihihihihi
\end{document}
```
构建和查看：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210703114136111.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
$\LaTeX$ 的换行规则有些奇特：

