@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。
 
[获取了VS Code编辑器之后](https://memcpy0.blog.csdn.net/article/details/117640795)，我们将在VS Code中配置PHP的语言学习环境。同样的，我会在个人代码库 `CodeWorld` 中建立 `Code_Php` 文件夹，里面的文件夹是 `Php_Single` 和 `Php_Multiple` ，分别存放不同的分类代码文件夹，即程序运行目标分别是**单个源代码文件**或**少数几个源代码文件的组合**。

---
# 1. 下载安装XAMPP
[PHP官网](https://www.php.net/)如下所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/8c39b4642bc644309b67e221a24f778f.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
在下载中，可以找到最近的Windows版本：
![在这里插入图片描述](https://img-blog.csdnimg.cn/c2936c9f3a48450ba78d5a8e997d0189.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
然后可以看到PHP的众多版本，如IIS（使用 `Non-Thread Safe` 版本的PHP）、Apache（使用 `Thread-Safe` 版本的PHP）等等：
![在这里插入图片描述](https://img-blog.csdnimg.cn/dad3f1eb73af44e2b3b0f02c0f0aa65a.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
如果在这里下载了PHP语言包（有 `php.exe` ），就还要下载Apache服务器、MySQL软件等等。不如直接[下载XAMPP](https://www.apachefriends.org/zh_cn/download.html)，**所谓的XAMPP是一个易于安装的Apache发行版**，其中包含了MariaDB、PHP和Perl，在官网下载[XAMPP](https://www.apachefriends.org/zh_cn/download.html)：![在这里插入图片描述](https://img-blog.csdnimg.cn/34ce22d570364dc3b507df28d34a90e7.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
下载完成后，启动安装程序，可能会弹出这种警告：
![在这里插入图片描述](https://img-blog.csdnimg.cn/844ea799254c4975b11e4d48e67452a9.png)




VS Code官方文档[PHP in Visual Studio Code](https://code.visualstudio.com/docs/languages/php)说明了如何在VS Code中使用PHP。VS Code使用官方的PHP linter（`php -l`）对PHP进行语言诊断，从而随着PHP linter的改进而保持更新。如果要使用XAMPP，就需要安装PHP的完整版来获取开发库。
