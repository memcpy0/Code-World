> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。

Visual Studio Code集成了源代码控制机制 `Source Control Management (SCM)` ，并且内嵌了对[Git](https://git-scm.com/)的支持。如果要使用其他的版本控制软件，也可以在扩展市场下载其支持插件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/366d972e734f4671a3290f29a86088f4.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
要在VS Code中使用Git，需要先下载安装Git（至少是 `2.0.0` 版本），并掌握它的初步使用。提交前还需要先在Git中配置用户名和电子邮件。具体可见这篇[文章目录](https://memcpy0.blog.csdn.net/article/details/119684105)。

---
# 1. Git支持
VS Code中左侧活动栏上的源代码管理图标，将始终显示**当前仓库中有多少更改**的概述 `overview` ，选择这一图标将进一步显示当前仓库中更改的详细信息：更改 `CHANGES` 、暂存的更改 `STAGED CHANGES` 、合并的更改 `MERGE CHANGES` 。以个人随手建立的一个图库为例，原先只有四张图片，现在新增了 `5.jpg, 6.jpg` ，同时在 `README.md` 中新增一行 `The jpg images are numbered from 1 to 6.` 。于是，源代码管理图标显示当前仓库有 `3` 个更改：
![在这里插入图片描述](https://img-blog.csdnimg.cn/1ad0ccc9549a46288a399b882f6ff8a9.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)由于示例简单，页面显示也比较简单，包括**源代码管理存储库、源代码管理**，不过我们还是可以做很多事。比如，**单击每项更改将详细显示每个文件中的文本更改**，对于尚未暂存的更改 `unstaged changes` ，**右边显示的编辑器仍然允许我们编辑文件**，新增一句 `The line is unstaged change.`（中间的**工作树**则是只读编辑器）；每项更改右边的三个按钮分别是**打开文件、放弃更改、暂存更改**：  
![在这里插入图片描述](https://img-blog.csdnimg.cn/39860d26ed5941969c700adb154c6980.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
对于图片文件，点击更改则只会在右边显示图片：
![在这里插入图片描述](https://img-blog.csdnimg.cn/abf960c89d1e44ceabd74837bee45f8e.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
在VS Code的左下角，可以找到仓库状态的指示符：**当前分支** `the current branch` 、**脏指示符** `dirty indicators` 、**当前分支签入签出的提交数** `the number of incoming and outgoing commits of the current branch` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/7ecf2bd449184834867ff1d49c0ef746.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 2. 提交

---
# 3. 复制一个仓库

---
# 4. 分支和标签


---
# 5. 远程


---
# 6. Git状态栏动作


---
# 7. Gutter指示符


---
# 8. 合并冲突

---
# 9. 查看更改

---
# 10. 查看时间线

---
# 11. Git输出窗口

---
# 12. 初始化一个仓库

---
# 13. 作为Git编辑器的VS Code

---
# 14. Pull Request

---
# 15. 下一步


