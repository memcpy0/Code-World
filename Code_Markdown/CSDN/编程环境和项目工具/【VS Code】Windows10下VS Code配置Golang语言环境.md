@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。
 
[获取了VS Code编辑器之后](https://memcpy0.blog.csdn.net/article/details/117640795)，我们将在VS Code中配置Golang的语言学习环境。同样的，我会在个人代码库 `CodeWorld` 中建立 `Code_Golang` 文件夹，里面的文件夹是 `Go_Single` 和 `Go_Multiple` ，分别存放不同的分类代码文件夹，即程序运行目标分别是**单个源代码文件**或**少数几个源代码文件的组合**。

---
# 0. Golang官方资料
[Golang官网](https://golang.google.cn/)在此：![在这里插入图片描述](https://img-blog.csdnimg.cn/dab393dcc43c4364b34a4ec27851d984.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
[Documentation页面](https://www.rust-lang.org/learn)是Golang的文档，包括一些基础教程和进阶教程、语言参考、[标准库文档](https://golang.google.cn/pkg/)、[命令文档](https://golang.google.cn/doc/cmd)等等，值得初学者仔细阅读：
![在这里插入图片描述](https://img-blog.csdnimg.cn/4217f81c5f62423892e4ea6f68d86384.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 1. 下载安装Golang
点击官网页面的 `Download Go` ，进入如下页面，安装合适的版本：
![在这里插入图片描述](https://img-blog.csdnimg.cn/96efa54ecdda4f39a23376b732c0de7e.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

运行安装程序，一路 `next` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/1e0531cd559947d484f041aa70f64ab3.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/1ee83579b6df4c2c9394cfea904e442a.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/a7d08284acf445d59c9223b05ebcd504.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/a0dbceb5d44b429ebe0e9d2e61e7b73e.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/e11fb81f96d146c18d18ea575637afe0.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

