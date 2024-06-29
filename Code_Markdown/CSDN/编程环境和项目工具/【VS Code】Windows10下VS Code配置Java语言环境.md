@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。


[获取了VS Code编辑器之后](https://memcpy0.blog.csdn.net/article/details/117640795)，我们将在VS Code中配置Java的语言学习环境。同样的，我会在个人代码库 `CodeWorld` 中建立 `Code_Java` 文件夹，里面的文件夹是 `Java_Single` 和 `Java_Multiple` ，分别存放不同的分类代码文件夹，即程序运行目标分别是**单个源代码文件**或**少数几个源代码文件的组合**。


---
# 1. 下载安装Java
## (1) 下载安装JDK
「JDK」的全称是 `Java Development Kit` 即Java开发套件，是我们基于Java开发软件时必须的一个工具包（`This is the package of tools for developing Java-based software.`），最新版是[Java SE 16.0.2](https://www.oracle.com/java/technologies/javase-jdk16-downloads.html)，就下载这个好了。在页面中找到合适的版本进行下载：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20beb86cba5e4efc9c14970bc9b99bd8.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
之后运行安装包，点击下一步：
![在这里插入图片描述](https://img-blog.csdnimg.cn/6bfe8200e437490db3a8cbcdbc0eff56.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这个JDK包含专用的JRE，可以更改安装路径：
![在这里插入图片描述](https://img-blog.csdnimg.cn/1dfcdc4f871543bbb6ed7dc2c2883b52.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

之后就进行安装了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/1513ecef7e014198b688906fa188c4af.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
单击后续步骤，可以访问[JDK 16的API文档](https://docs.oracle.com/en/java/javase/16/index.html)、开发人员指南、发布说明等：
![在这里插入图片描述](https://img-blog.csdnimg.cn/0c8e5499e12f4c05ac0b834a94050f58.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

内容蛮丰富的，有时间可以看一下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/9a32a86a850046b79a66d461fb39b253.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## (2) 配置Java环境变量
现在开始配置Java的环境变量，发现安装时自动添加了一个变量：
![在这里插入图片描述](https://img-blog.csdnimg.cn/4bf7548b6d2345e997a2f96db0d1b55f.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

这个文件夹里面有什么呢？发现有 `java, javac, javaw, jshell` 四个软件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/886cfdf8761b424ebe512c670fec2f5c.png)

太少了，我们还是自己配置吧。首先在系统变量中建立一个变量 `JAVA_HOME` ，指向JDK 16.0.2的安装文件夹：
![在这里插入图片描述](https://img-blog.csdnimg.cn/516d8319c896464299db617c21c1edf6.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
然后在PATH变量中添加：
![在这里插入图片描述](https://img-blog.csdnimg.cn/7b174feaa114490caadcd18aebe18017.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
不过这里有个问题，新版的JDK安装路径下没有 `jre` 这个目录：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2d8447c5a91e427fbcb06cf8b1f13b08.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
我们需要手动生成，做法是：CMD进入JDK安装路径，输入命令 `bin\jlink.exe --module-path jmods --add-modules java.desktop --output jre` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/f9ecb3257ab744a3849e02b523ab6655.png)
现在有了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/748d11cbc50e46888b0efa49ab9a0b0c.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## (3) 验证是否安装成功
在CMD中，分别输入 `java, javac, java --version` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/02b3315db2ea4818bd9218b54baac2c4.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/3f556d886eca452e98e0ba785cd6c542.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/4989b8ef9d064b41b7df08462fe2cbc5.png)
接着使用Java运行程序，在桌面先创建一个 `HelloWorld.java` 文件，输入代码：

```java
public class HelloWorld {
	public static void main(String[] args) {
		System.out.println("Hello Java World!");
	}
}
```
运行如下命令，其中 `javac` 会得到一个 `HelloWorld.class` 文件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/ddfdc4f998144395b1025ba0f153ec24.png)
这些都说明，我们完成了Java的安装。

---
# 2. VS Code中安装Java支持插件
有很多和Java相关的插件，这里只需要下载「Java Extension Pack」即Java插件包：
![在这里插入图片描述](https://img-blog.csdnimg.cn/bfa601d05f0d4c0ca587b86b1793792d.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
它会自动为我们安装六个插件，也是VS Code帮我们整理的最常用的六个Java插件：
- Language Support for Java(TM) by Red Hat
- Debugger for Java
- Java Test Runner
- Maven for Java
- Project Manager for Java
- Visual Studio IntelliCode

可以看一下「Java Extension Pack」提供的「Java Overview」界面，里面讲述了使用这些插件的方法，「Configuration」中的第一项 `Configure Java Runtime` 也提供了下载JDK的链接：
![‘’](https://img-blog.csdnimg.cn/4e30b2b49f954227b5564c03366aee92.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/fa6df4fd7f1e405db56271eb81995f1d.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

从这里安装JDK，或者像我第一节的做法，成功后都可以在「Configure Java Runtime」界面的 `Installed JDKs` 选项中，看到已经安装的JDK的路径和版本：
![在这里插入图片描述](https://img-blog.csdnimg.cn/aa8f1ad35b6c40f7bfa97d7f6af399c8.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 3. 书写插件配置文件
打开用户配置 `settings.json` 文件，写下如下代码，其中 `java.home` 的值就是环境变量 `JAVA_HOME` 的值，也是上图中 `Path` 显示出的值：
```json
{
	"vsintellicode.modify.editor.suggestSelection": "automaticallyOverrodeDefaultValue",
    "notebook.cellToolbarLocation": {
        "default": "right",
        "jupyter-notebook": "left"
    },
    "java.completion.favoriteStaticMembers": [
        "org.junit.Assert.*",
        "org.junit.Assume.*",
        "org.junit.jupiter.api.Assertions.*",
        "org.junit.jupiter.api.Assumptions.*",
        "org.junit.jupiter.api.DynamicContainer.*",
        "org.junit.jupiter.api.DynamicTest.*",
        "org.mockito.Mockito.*",
        "org.mockito.ArgumentMatchers.*",
        "org.mockito.Answers.*"
    ], 
    "java.home": "C:\\Program Files\\Java\\jdk-16.0.2",
}
```
到目前为止，已经完成了Java的基本配置，更多配置待将来实际使用时更新。
