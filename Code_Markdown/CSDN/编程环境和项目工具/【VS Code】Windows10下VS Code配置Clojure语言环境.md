@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。
 
[获取了VS Code编辑器之后](https://memcpy0.blog.csdn.net/article/details/117640795)，我们将在VS Clojure中配置Erlang的语言学习环境。同样的，我会在个人代码库 `CodeWorld` 中建立 `Code_Clojure` 文件夹，里面的文件夹是 `Clojure_Single` 和 `Clojure_Multiple` ，分别存放不同的分类代码文件夹，即程序运行目标分别是**单个源代码文件**或**少数几个源代码文件的组合**。

---
# 1. 下载安装Lein
[Clojure的官网](https://www.clojure.org/)对应页面如下，仍然有 `Reference` 、`Guides`（对应其他编程语言官网的 `Tutorials, Books` 等） 
![在这里插入图片描述](https://img-blog.csdnimg.cn/da58b96f56c849cf958f9cfac71232ec.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

由于Clojure是运行于JVM上的一种Lisp方言，「Clojure requires Java」
