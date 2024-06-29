@[toc]

---



### 编程作业 Hello, World
可选的练习，保证你能写简单的Java程序、使用 `algs4.jar` 包、提交作业。注意仔细阅读程序的规格说明书[Specification](https://coursera.cs.princeton.edu/algs4/assignments/hello/specification.php)、提交说明 `Web Submission` 和评价报告指南[Assessment Guide](https://www.coursera.org/learn/algorithms-part1/programming/37IH3/hello-world/instructions)。

安装环境的网站：https://lift.cs.princeton.edu/java/windows/
![在这里插入图片描述](https://img-blog.csdnimg.cn/6fc65da5cf544c5a9c3cc183881a1dcd.png)

#### `algs4.jar` 包有什么？
在规格说明中没有写出，我们只知道它是 `textbook libraries` 即本书的工具库。我们从[https://algs4.cs.princeton.edu/code/](https://algs4.cs.princeton.edu/code/)即本书网站得到更详细的说明。

`algs4.jar` 包含本书使用的[标准输入输出库](https://introcs.cs.princeton.edu/java/stdlib/)，用于**从标准输入读取数字、文本、二进制数**，**向标准输出写入数字、文本、二进制数**，**从URLs和文件读取数字、文本、二进制数，向文件写入数字和文本**，绘制几何形状，操作音频，生成随机数，计算数据，读/写一维和二维数组，处理色彩图像和灰度图像等等：
![在这里插入图片描述](https://img-blog.csdnimg.cn/1dc7721aeb7e48f692f92b616bea75dd.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
在命令行里输入“jar xf”。这个是Jar文件的提取命令。

    “x”是“提取”的缩写。
    “f”是“文件”的缩写。
    放在一起，“xf”代表你想要在命令行里提取特定的文件。

提取整个Jar文件。这会反向编译Jar文件里存储的所有文件。提取后的文件出现在同一个目录下。 

#### 如何使用它？（通过推荐环境、命令行和Java程序）
使用本书工具库和**推荐环境**（预配置了 `algs4.jar` ）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/e2f60c935b1744f9b5161137c518cb32.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)从命令行暂时或永久使用 `algs4.jar` 的方法：
![在这里插入图片描述](https://img-blog.csdnimg.cn/204b943323484d4c8ba624568ec78301.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
在Java程序中使用工具库，下载测试数据集：
![在这里插入图片描述](https://img-blog.csdnimg.cn/90c6fde106ee4c95821eab51ddebbe33.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
algs4.jar, which contains Java classes for I/O and all of the algorithms in the textbook.

使用标准输入输出库[https://introcs.cs.princeton.edu/java/stdlib/](https://introcs.cs.princeton.edu/java/stdlib/)
Q. What is the easiest way to execute the main() method in classes that are contained in algs4.jar?

A. If you used our autoinstaller, you can execute with a command like

    % java-algs4 edu.princeton.cs.algs4.StdDraw

#### 安装推荐环境
遵守规格说明书[Specification](https://coursera.cs.princeton.edu/algs4/assignments/hello/specification.php)，一步步安装和使用推荐环境。

运行 `lift-java-installer.exe` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/a2a3149c835347f3a8d50ff06b2fc81d.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/ca85a8aa6b2649edabb19c11650c1da7.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/23ad3ef47467425186af70309732f686.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/9235c45f267f4e80a7b84b753dc6dab0.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/91113264a6c84bebb1e08a102e5de6bf.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/97951059086f4cc6826eed5fd3b54028.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)


提交说明 `Web Submission` 和评价报告指南[Assessment Guide](https://www.coursera.org/learn/algorithms-part1/programming/37IH3/hello-world/instructions)


如果您以前使用过IntelliJ 2021.1，请仅在需要重新安装时运行此安装程序。安装程序将使用我们的新手友好设置覆盖任何现有IntelliJ 2021.1设置。

安装程序还执行Git Bash的干净重新安装，这将覆盖以前的任何用户设置，包括.bashrc。

提交时命名为 `hello.zip` ，包含三个源文件 `HelloWorld.java, HelloGoodbye.java, RandomWord.java` 。

遵循[Specification]，下载 `hello.zip` ：
- your code conforms to the prescribed APIs: each program must be in the "default" package (i.e., no package statements) 
- include **only the public methods and constructors** specified (extra private methods are fine)
- algs4.jar uses a "named" package, so you must **use an import statement** to access a class in algs4.jar

IntelliJ organizes Java programs into projects. In our context, each project **corresponds to one programming assignment**.
**Download the project** for your programming assignment to **a convenient location**

**creates a project folder with the name** of the corresponding programming assignment 

To open a project from the Welcome to IntelliJ IDEA screen, click **Open** and select the project folder.  **see an assignment logo** (in the main editor window) and a list of project files

When you launch IntelliJ **for the first time**, it may take a minute or two to index your files; some features (such as auto importing) will be unavailable until this process completes. 

**Do not click New Project**; this option is intended for advanced programmers. Also, **always use Open with a project folder**, not an individual file. 

Select the menu option **LIFT → New Java Class**. When prompted, type **HelloWorld** for the Name and click **OK**. 

In the main editor window, complete the Java program HelloWorld.java exactly **as it appears below**.

To save the file, select the menu option File → Save All (Ctrl + S). When you save the file, IntelliJ re-formats it (**as necessary**). 

**IntelliJ is configured to automatically save changes** that you make to your files upon various events (such as compiling, executing, closing a file or project, or quitting the IDE). We still **recommend using File → Save All (Ctrl + S)** to re-format your code. 


#### Assessment Report![在这里插入图片描述](https://img-blog.csdnimg.cn/302e8de860134f47b6bb1a0caca96fad.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

……

![在这里插入图片描述](https://img-blog.csdnimg.cn/36dfea61639b488eaebd90a08064cece.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/d2e9587b028e45a29a166adc602f7558.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/df1036817a6945cfbcb3e9d2bcc8c6c2.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

![在这里插入图片描述](https://img-blog.csdnimg.cn/1ebcfd47231647c484a299e30fe5afc9.png)https://checkstyle.sourceforge.io/checks.html
![在这里插入图片描述](https://img-blog.csdnimg.cn/ab26cc995b6548939b6975af0bae5b49.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/7b864ac6567a497980ce8c9f37de9716.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)


---


