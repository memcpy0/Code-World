@[toc]
> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。


CPython自带的IDLE太简陋，Pycharm又有点笨重，[现在获取了VS Code编辑器之后](https://memcpy0.blog.csdn.net/article/details/117640795)，我们将在VS Code中配置Prolog的语言学习环境。同样的，我会在个人代码库 `CodeWorld` 中建立 `Code_Python` 文件夹，里面的文件夹是 `Python_Single` 和 `Python_Multiple` ，分别存放不同的分类代码文件夹，即程序运行目标分别是**单个源代码文件**或**少数几个源代码文件的组合**。


---
# 1. 下载安装Python解释器
**共通的第一步已经完成，即获取编辑器**。对症下药，要配置Python语言学习环境，第二步就是下载Python解释器。我们打开Python的官网，现在Python已经更新到3.9.6了。进入[下载地址](https://www.python.org/downloads/)：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718223145698.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

点击Download后，在新页面选择64位安装程序：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718223417481.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

下载后打开安装程序，根据图示进行：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021071822431290.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

下一步中只需要勾选前两项，其中 `Documentation` 是Python文档，建议选上；`pip` 是Python的包管理工具，必须勾选；`tcl/tk and IDLE` 安装tkinter库和IDLE开发环境，tkinter库可以日后安装，IDLE太简陋了不要安装；`Python test suite` 是Python标准库测试套装，不是必须安装：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718224751645.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

下一步，可以自行调整安装路径：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718225133716.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
安装成功如图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718225310535.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

现在打开VS Code的Git Bash终端（或者打开CMD/PowerShell），输入 `python -V` ，将输出Python的版本信息，也标志着我们的安装成功了！
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718225602420.png)

---
# 2. 安装Python支持插件
在VS Code的扩展商店，搜索插件Python并安装：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718225815276.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这一插件功能众多，十分方便我们在VS Code中进行Python开发：
- 支持Python2.7和Python3.4+的Pyhon版本；
- 使用IntelliSense进行代码补全；
- 支持多种代码检查器；
- 对调试的支持；
- 对单元测试的支持；
- 代码段功能；
- 自动应用虚拟环境；
- 可在Jupyter环境和Jupyter Notebook中编辑代码

Python插件安装后，还要选择一个解释器。`Ctrl+Shift+P` 打开命令面板，输入 `Select Interpreter` ，然后使用我们手上唯一的一个解释器: )
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718233511886.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)这里我是先创建了一个Python文件，启动调试后，VS Code自动帮我选择了环境变量中的Python解释器：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021071823365057.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
如果你同时安装了多个版本的Python，还可以点击左下角的 `Python x.x.x 64-bit` 来切换解释器版本。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718233847427.png)
此时还可能弹出一个窗口，让你选择一个代码检查插件，可以直接点击安装来下载 `pylint` ，或者选择其他代码检查器：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718230414698.png)
选择后，Python会调用 `pip` 去安装代码检查器，如下图所示即说明安装成功：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718230634786.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
之后VS Code还可能弹出一个页面，是Python支持插件的使用教学：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718230847293.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
如果想要使用Jupypter笔记本的功能，还要安装这两个库：
```bash
$ pip install ipykernel
$ pip install notebook
```

---
# 3. 配置Python语言学习环境
我在 `CodeWorld` 中创建了一个 `Code_Python` 文件夹，然后打开 `Code_Python` 文件夹，在里面创建了 `Python_Single, Python_Multiple` 两个文件夹，然后在 `Python_Single` 中创建了 `Learn_Python` 文件夹。`Code_Python` 对应的 `.vscode` 文件夹中，项目配置 `settings.json` 代码如下：
```json
{
    "python.linting.pylintEnabled": true,
    "python.linting.enabled": true,
    "jupyter.jupyterServerType": "local",
    "python.defaultInterpreterPath": "C:\\Program Files\\Python39\\python.exe"
}
```
---
# 4. 尝试运行Python文件
随便创建一个 `temp.py` ，然后只写一句 `print("Hello World")` 。然后可以右键点击出Python插件提供的许多功能，比如**在终端中运行Python文件**。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210721225058457.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210721225051301.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

运行后效果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021072122522233.png)
如果安装了测试框架如 `pytest, unittest, nosetest` ，还可以使用**运行当前单元测试文件**。



