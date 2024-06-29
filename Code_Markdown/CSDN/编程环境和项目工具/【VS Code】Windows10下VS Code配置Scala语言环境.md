@[toc]
 
> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。
 
[获取了VS Code编辑器之后](https://memcpy0.blog.csdn.net/article/details/117640795)，我们将在VS Code中配置Scala的语言学习环境。由于Scala运行在JVM上，所以我们先要完成Java的语言学习环境配置，具体见[这篇文章](https://memcpy0.blog.csdn.net/article/details/119280425f)。同样的，我会在个人代码库 `CodeWorld` 中建立 `Code_Scala` 文件夹，里面分别是 `Scala_Single` 和 `Scala_Multiple` ，分别存放不同的分类代码文件夹，即程序运行目标分别是**单个源代码文件**或**少数几个源代码文件的组合**。


---
# 1. 下载安装Scala
Scala的[官网](https://www.scala-lang.org/)如下，包括文档、库、下载等：
![在这里插入图片描述](https://img-blog.csdnimg.cn/c5e80a4bb584491b8fdbaf67d54ca540.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

官网上还展示了一系列Scala课程，指向Coursera和Edx，可以在上面进行学习：
![在这里插入图片描述](https://img-blog.csdnimg.cn/c843b5324d7240ebb30af1e87bd07b19.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


进入下载页面后，直接下载Scala 3：
![在这里插入图片描述](https://img-blog.csdnimg.cn/f89c8b539f6f44a0825be280e020b9c3.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


 
下载后解压 `iobin-win32-current.zip` ，得到几个文件并运行其中的 `.exe` 文件，它会询问解压到哪个位置，这和你希望安装Io的位置无关。解压后，进入文件夹 `IoLanguage-2013.11.05-win32/IoLanguage/bin` ，运行 `io.exe` ，如果报错，就将开始解压得到的 `libgcc_s_dw2-1.dll` 放在与 `io.exe` 相同的目录下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/021d4567172d4925a2ff6591bcb0bf28.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
然后重新运行 `io.exe` ，输入 `"Hello Io" print` 将打印 `Hello Io` ：![在这里插入图片描述](https://img-blog.csdnimg.cn/5557a7f7b5b1403bafa8b9c2986aca72.png)
现在将 `IOLanguage` 整个文件夹都移动到 `C:/Program Files` 中，然后将 `C:\Program Files\IoLanguage\bin` 这个文件夹加入到环境变量中，照葫芦画瓢在命令行中运行 `io` 解释器：
![在这里插入图片描述](https://img-blog.csdnimg.cn/4fbb8aeeb64c4b5cac3de4cc5e5c8dff.png)

---
# 2. 安装VS Code支持插件
打开VS Code的扩展商店，搜索 `Io` 安装如下插件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/07b84347dfc946eb87569a5f49761fa3.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这一插件的功能很简单，仅仅提供语法高亮功能。

---
# 3. VS Code中运行Io程序
创建一个名为 `Code_Io/Learn_Io` 的文件夹，然后用VS Code打开 `Code_Io` ，在里面建立一个 `temp.io` 文件，输入以下代码：
```cpp
"Hello Io World" print
```
然后在终端输入 `io temp.io` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/e23a3f281ad246d19913591654cd3f3c.png)




 vs code 安装Scala
首先本机要安装scala（官网肿么下不了，CSDN上面下的）；
配置scala到环境变量PATH中（Scala的根目录）；
VS中安装以下扩展：
1. Scala；
2. Sbt；
3. Code Runner；
代码中右键，选择“Run Code"，即可执行代码
 
无法Debug，VS Code现在对于Scala支持有限，可以看到扩展都是比较旧，没有什么更新。而且最坑的是scala代码页，右键没有run，只能通过Code runner这种通用的第三方执行插件来运行。

