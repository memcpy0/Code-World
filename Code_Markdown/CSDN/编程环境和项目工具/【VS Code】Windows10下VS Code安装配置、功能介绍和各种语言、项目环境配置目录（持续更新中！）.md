> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章的汇总目录，内容随时可能发生更新变动，欢迎关注和收藏本文以作备忘。


@[toc]

为什么使用VS Code？对我个人而言，**VS Code最突出的特性就是它强大的插件和配置功能**，可以根据需求和想法下载不同的插件、运用配置文件定义出**高度个性化的语言和项目环境**，定制出属于自己的编辑器。可以说，**多样化的插件和个性化的功能配置，是VS Code的灵魂所在**（建议仔细看完[VS Code官方文档](https://code.visualstudio.com/docs)，深入了解VS Code的各种强大功能）。

对VS Code中各种语言和项目环境的配置，无非是想要将VS Code打磨成个人趁手的兵器，**打造出一把无往而不利的瑞士军刀**——而非每学一门编程语言就下载一个专业IDE，如Python的Pycharm、Java的Intellij、前端的Webstorm等等（虽然JetBrains的IDE全家桶很强大），摆脱掉Eclipse、CodeBlocks等过时的IDE和VC6.0、DevC++等垃圾软件，同时**为Haskell、Io、Erlang等开发工具支持力度较弱的语言（没听说过什么很好的IDE）创造良好的学习环境**。 

这也是我最近的体会——VS Code插件系统的强大，**直接或间接地降低了很多新兴或小众语言开发的门槛**，让很多没有很好的IDE支持的语言可以舒适的写程序；而在大众语言如C/C++/Java/Python等方面，使用VS Code开发大型项目固然是短板，却也**有利于中小型项目的敏捷开发**。然而福兮祸所依、祸兮福所伏，在配置Graphviz环境时，我也发现，这些小众语言的插件怎么用怎么配都感觉是东拼西凑的，**各插件之间很难完美配合协同**，恨不得自己撸起袖子上阵造一个支持插件，把所有这些功能融合在一起。这也是VS Code固有的不足之处吧。

---
# 0. 编辑器、IDE和环境搭建步骤
这里要区分的是 `Visual Studio Code` （VS Code）和 `Visual Studio`（VS）或JetBrains全家桶，前者是一个轻量级的**文本编辑器**，后者则是**集成开发环境IDE**。

我们都知道，编程过程中，首先要用**文本编辑器**写源代码，然后用**编译器**编译生成目标代码，接着使用**链接器**将目标代码和其他代码（如库函数、启动代码等）链接起来，生成可执行代码。其中，**编辑器**仅处理文本形式的源代码，可能还提供有智能提示、代码高亮等辅助功能，属于工具链必不可少的一环。而**集成开发环境IDE**则提供这一整套的程序开发环境，包括了代码编辑器、编译器、解释器、调试器、图形用户界面等整套工具链，集合了代码编写、编译、调试、运行、分析、上传发布等功能于一体。

因此，VS Code（或者Vim/Emacs）作为**代码编辑器**，与IDE完全不一样的一点是——**使用IDE时除非是它的深度用户，否则不需要知道很多细节**，例如为什么按一个按钮就能编译，为什么点一下行号就能加断点，为什么敲出 `import/#include/using` IDE就知道要导入什么符号等等诸如此类的问题。而使用VS Code做开发就要操心这些事情了，**VS Code（或者Vim/Emacs）要求用户首先知道如何脱离IDE来工作**。

由于VS Code不包含编译、调试等很多功能，因此需要**自行安装编译器、解释器、调试器等套件**，并将它们**有机结合起来**。将这一整套**工具链**整合到一起形成工作流的过程，就是所谓的**搭建环境**。总而言之，不要把VS Code和（传统）IDE相提并论，而要将VS Code 看做新时代的Emacs，其本质是：**当你知道如何用命令行做所有你想做的事情时，VS Code可以帮助你自动化和图形化**。对环境比较熟的用户，甚至可能觉得VS Code相比普通IDE来说，可扩展性更强、更轻量简洁、更好排查问题。 

因此，我们配置各种语言和项目环境的诸多流程，都可以归结为这几步——**先要获取VS Code（编辑器）**，然后**对不同的语言和项目，要获取相应编译/解释套装**（编译器、调试器、库文件等），最后还要**做好两者之间的沟通工作，即安装对应插件、书写相关配置文件等**。我们会在后文反复看到，VS Code中所有语言和项目的环境配置，都将在总体上遵循这一流程。

---
# 1. VS Code获取和基础配置
## 1.1 下载和安装VS Code
访问[官方网站](https://code.visualstudio.com/)，官网页面如下。`Code editing. Redefined` ，重新定义代码编写？口气有点大啊(⊙﹏⊙)，而且还是 `Free` 免费、`Built on open source` 构建在开源之上、`Runs everywhere` 到处运行：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210607014231557.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

如果点击左侧的红框 `Download for Windows` ，会进入下面的页面，同时默认下载User版本的VS Code：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210607014443739.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

这里我们下载System版本，需要点击右上角的 `Download` ，进入：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210607014722217.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


可以看到 `User Installer` 和 `System Installer` ，前者**只适用于当前用户下使用，更换用户后就无法使用**，后者**适用于所有用户使用**，因此选择 `System Installer 64 bit` 版本的软件下载并运行。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210607015547982.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

接受协议后，可以选择安装位置，安装到默认位置或者自定义位置：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210625130703837.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
 

按照自己想要的勾选，**创建桌面快捷方式**和**添加到PATH**这两者是必须的，也可以全部勾选上。接着点击下一步：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210607020023509.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

直接点击安装即可：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210607020056177.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

这里我不打算创建开始菜单文件夹：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210607015957786.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

系统安装中，等待VS Code安装结束。再点击完成即可：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210607020409970.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
## 1.2 VS Code中文/外文语言环境配置和切换
VS Code默认使用的还是英语，有时候对用户不太友好，可以设置为中文。不过这需要安装它的中文插件，以支持VS Code的中文环境。首先点击扩展图标，输入 `Chinese` ，选择 `Chinese (Simplified) Language Pack for Visual Studio Code` 插件，点击 `install` 进行安装。安装完成后，重启一波VS Code，显示如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210608001527874.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

如果选择安装其他语言（比如日语什么的），可以搜索相关插件并自行安装：
<img src="https://img-blog.csdnimg.cn/4c3082cb1e7b432988c3f77aec54b19a.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="65%">

如何在中英文或者安装的更多语言间切换呢？`Ctrl+Shift+P` 打开命令面板，输入 `Configure Display Language` ，选择`en` 或 `zh-cn` ，重启之后就显示对应语言：
![在这里插入图片描述](https://img-blog.csdnimg.cn/e8dede98a49f4da48eaefe092f837b16.png)

注意与 `Configure Language Specific Settings` 命令进行区分，此处是**配置（编程语言）特定设置**，而非**配置显示语言**。


## 1.3 VS Code文件图标配置
原有的文件图标太朴素了，我们需要漂亮一点的。在扩展商店搜索 `vscode icons` ，第一个就是目标，点击安装即可。这一插件的安装数量达到了惊人的8.3M，足以证明它的受欢迎程度：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210717152338929.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## 1.4 VS Code自动保存功能配置
注意，本文中对VS Code的功能配置，和配置 $\LaTeX$ 语言环境一样，都将书写在 `User Settings` 配置文件中，如果不明白什么是用户设置，可以先看这两篇文章：[文件夹（Folder）和工作区（Workspace）的使用](https://memcpy0.blog.csdn.net/article/details/118385489)，[settings.json的两种配置位置（全局用户设置和工作区设置）](https://memcpy0.blog.csdn.net/article/details/118362651)。

由于自动保存功能相当重要，本文额外花一个小节来介绍。我们可以打开UI设置界面，在常用设置中找到 `Auto Save` ，它有四个选项：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210701152835123.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

我们一一具体解读：
- `off` ：不开启自动保存，是默认选项，但是这样做很危险；
- `afterDelay` ：不编辑一段时间后自动保存，这段时间具体取决于 `Auto Save Delay` 设置：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210701153129390.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
- `onFocusChange` ：鼠标焦点离开当前文本编辑区时自动保存，**强烈推荐**；
- `onWindowChange` ：鼠标焦点离开VS Code窗口时自动保存，也可以选这个。

在此处选择后会写到用户设置 `settings.json` 文件中。我们也可以手动写到用户设置 `settings.json` 文件中：
```json
{
    "files.autoSave": "onFocusChange", //自动保存选项
}
```
## 1.5 VS Code开启或禁用自动更新
不知道为什么，每次一更新时，我重新打开的VS Code界面都会崩溃，就有了禁用自动更新的想法。具体操作是：文件->首选项->设置，搜索 `Update: Mode` 并将设置更改为**禁用更新** `none` 、**手动检查更新** `manual`（禁止自动后台更新检查）、**仅在启动时检查更新** `start`（禁止自动后台更新检查），默认的是**启用自动更新检查** `default` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/fd6170058bac4f4e83e911829ee694c6.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
相关的设置还有两个 `Update: Show Release Notes` **更新后显示发行说明**、`Update: Enable Windows Background Updates` **启用后台下载安装新的VS Code版本**。

嘛，最后权衡了一下，我还是没有选择禁用自动更新。原因在于：
- 如果禁用自动更新，恐怕就不会想着更新了，人的惰性使然；
- VS Code仍处于快速发展阶段，新功能层出不穷的涌现，**每次更新后及时查看发行说明，可以发现很多惊喜，也有利于增量式、渐进式地掌握VS Code的使用**，而不是过了很多个版本后发现VS Code大变样，又要从头开始学起。

## 1.6 VS Code官方文档和常用快捷键
开始使用前，可以看一下[VS Code 文档](https://code.visualstudio.com/docs)中的这些资料，了解一下VS Code的用户界面、设置和使用技巧。不要求一次性全部记住，日后可以**常看常新**：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210607015754401.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

下面列出一些比较有意思的、日常用的比较多的快捷键。以Windows为例：
- 对于文件中 **行** 的操作：
	- 重开一行：光标在行尾时，回车即可；光标在行中，`Ctrl+Enter` 向下重开一行，`Ctrl+Shift+Enter` 在上一行重开一行；
	- 删除一行：光标没有选择内容时，`Ctrl+X` 剪切一行，`Ctrl+Shift+K` 直接删除一行；
	- 移动一行：`Alt+UP` 将当前行向上移动一行，`Alt+Down` 将当前行向下移动一行；
	- 复制一行：`Shift+Alt+Down` 向下复制一行，`Shift+Alt+Up` 向上复制一行；
- 对于文件中 **词** 的操作：`Ctrl+D` 选中一个词；
- 跳转操作：`Home` 将光标跳转至行头，`End` 将光标跳转到行尾，`Ctrl+Home` 跳转到文件头，`Ctrl+End` 跳转到文件尾；
- 搜索或替换：`Ctrl+F` 搜索，`Ctrl+Shift+F` 在项目内替换，`Ctrl+Alt+F` 替换；
- 注释操作：`Ctrl+/` 添加或关闭行注释，`Shift+Alt+A` 添加或关闭块注释（为了方便使用，我换成了 `Ctrl+.` ）

前面的快捷键多少有点用处，建议记住，不过学了Vim就不用记了。下面的记不记均可，反正也没多少通用性：
- `Ctrl+Shift+P` 打开命令面板；
- `Ctrl+Shift+X` 跳转到扩展搜索栏；
- `Ctrl+K Ctrl+S` 查看所有的键盘快捷键
- `Ctrl+K Ctrl+T` 浏览颜色主题，在扩展搜索栏中输入 `category:themes` 可以查看颜色主题相关的插件；

## 1.7 VS Code用户设置相关代码
这里给出本人的用户设置 `settings.json` 文件中的部分配置代码，用于VS Code基础功能的设置，虽然不能说面面俱到、尽善尽美，但也可堪一用。如果要使用本人的配置代码，涉及到路径的可能需要稍加修改。有些配置代码的作用还会在后文加以介绍：
```json
{
    /*---------------------------------------VS Code相关设置-------------------------------------------------*/
    /*---------------------------------------文本编辑器设置-------------------------------*/
    "editor.codeActionsOnSave": { //在保存文件时运行的代码操作
        "source.organizeImports": true, //文件保存时运行"整理import语句"操作(让import语句以字典序排列)
    },  
    "editor.detectIndentation": false, //文件打开时不会基于文件内容检测#editor.tabSize#和 #editor.insertSpaces#
    "editor.lineNumbers": "on", //开启代码行号,显示为绝对行数 
    "editor.rulers": [80, 120], //在一定数量等宽字符后显示垂直标尺,多个值显示多个标尺
    "editor.tabCompletion": "on", //在按下Tab键时进行Tab补全,将插入最佳匹配至
    "editor.tabSize": 4, //设置一个tab符等于的空格数,用于Python
    "editor.wordWrap": "on", //控制折行的方式,on表示将在视区宽度处换行

    /* 字体设置 */
    "editor.fontSize": 14, //设置字体大小 
    "editor.fontWeight": "normal", //设置字体为正常还是粗体

    /* 格式化设置,某些编程语言可能还需要格式化插件和工具 */ 
    "editor.formatOnSave": false, //设置为true时,每次保存时自动格式化
    "editor.formatOnType": false, //设置为true,每次键入一行后自动格式化
    
    /* 缩略图设置 */
    "editor.minimap.enabled": false, //关闭右侧代码小地图  

    /* 建议设置 */
    "editor.acceptSuggestionOnEnter": "on", //Tab键以外的Enter键同样可以接受建议,减少插入新行和接受建议命令之间的歧义
    "editor.suggest.snippetsPreventQuickSuggestions": false, //活动代码段不阻止快速建议

    /* 文件设置 */
    "files.associations": { //配置语言的文件关联,关联的优先级高于已安装语言的默认关联
        "*.vue": "vue",
        "*.wpy": "vue",
        "*.wxml": "html",
        "*.wxss": "css"
    }, 
    "files.autoSave": "onFocusChange", //失去焦点后自动保存
    "files.exclude": { //这些文件和文件夹将不会显示在工作空间中
        "**/.git": true,
        "**/.svn": true,
        "**/.hg": true,
        "**/CVS": true,
        "**/.DS_Store": true,
        "**/*.js": {
            "when": "$(basename).ts" //ts编译后生成的js文件将不会显示在工作空间中
        },
        "**/node_modules": true
    },
    
    /*---------------------------------------工作台设置-------------------------------*/
    /* 外观设置 */
    "workbench.colorTheme": "Default Light+", //更改颜色主题 
    "workbench.iconTheme": "vscode-icons", //指定工作台中使用的文件图标主题;vscode-icons插件要下载
    "workbench.settings.editor": "ui", //指定默认使用的设置编辑器.设为json后打开设置默认进入settings.json文件

    /*---------------------------------------窗口设置-------------------------------*/
    "window.zoomLevel": 1.2, //调整窗口(包括图标/字体等同步缩放)的缩放级别 

    /*---------------------------------------功能设置-------------------------------*/
    /* 资源管理器设置 */
    "explorer.confirmDelete": true, //资源管理器在把文件删除时进行确认

    /* 搜索设置 */
    "search.exclude": { //在文本搜索时排除的文件和文件夹
        "**/node_modules": true,
        "**/bower_components": true,
        "**/target": true,
        "**/logs": true,
    },
    "search.smartCase": true, //搜索词全为小写则不区分大小写搜索,否则区分大小写
    "search.sortOrder": "modified", //控制搜索结果的排序顺序,modified表示按照文件的做法修改日期降序排列
    "search.useGlobalIgnoreFiles": true, //在搜索文件时使用全局.gitignore和.ignore文件
    "search.useIgnoreFiles": true, //在搜索文件时使用.gitignore和.ignore文件

    /* 终端设置 */
    "terminal.explorerKind": "integrated", //自定义要启动的终端类型.使用集成终端,只打开VS Code更方便,避免切换界面
    "terminal.integrated.profiles.windows": { //通过终端下拉列表创建新的终端时,要显示的Windows配置名和配置文件
        "PowerShell -NoProfile": { //可以定义多个配置名和配置文件
            "source": "PowerShell",
            "args": [
                "-NoProfile"
            ]
        },
        "Git-Bash": {
            "path": "D:\\Program Files\\Git\\bin\\bash.exe", //Git Bash的路径位置
            "args": []
        }
    },
    "terminal.integrated.defaultProfile.windows": "Git-Bash", //在Windows上使用的默认配置文件
    
    /*---------------------------------------安全性设置-------------------------------*/
    "security.workspace.trust.untrustedFiles": "open", //始终允许不受信任的文件引入受信任的工作区,不显示提示

    /*---------------------------------------扩展设置--------------------------------*/
    /*-----------------------------------------Emmet内置插件设置--------------------------------------------*/
    "emmet.includeLanguages": { //在默认不受支持的语言中启用Emmet缩写,在此语言和Emmet支持语言间添加映射
        "vue-html": "html",
        "vue": "html",
        "wpy": "html"
    },
    "emmet.showAbbreviationSuggestions": true, //将可能的Emmet缩写作为建议显示,在样式表中时不适应
    "emmet.showExpandedAbbreviation": "always", //以建议形式显示展开的Emmet缩写,适用于
    //html/haml/jade/slim/xml/xsl/css/scss/sass/less和stylus
    "emmet.triggerExpansionOnTab": true, //启用后,按下TAB键,将展开Emmet缩写

    /*-----------------------------------------Git内置插件设置--------------------------------------------*/
    "git.autofetch": true, //设置为true,则自动从当前Git存储库的默认远程库提取提交;设置为all,则从所有远程库进行提取 
    "git.confirmSync": false, //同步Git存储库前不用先进行确认
    "git.enabled": true, //启用Git
    "git.enableSmartCommit": true, //在没有暂存的更改时提交所有更改
    
    /*-----------------------------------------GitHub内置插件设置--------------------------------------------*/
    "github.gitAuthentication": true, //在VS Code中为git命令启用自动GitHub身份验证
}
```

---
# 2. VS Code功能介绍

## 2.1 VS Code常用主题和扩展插件
扩展插件也是VS Code功能的一部分，而且是很重要的一部分。这里另外写了一篇文章，用来搜集和整理有用的主题和插件：[Windows10下VS Code常用主题和扩展插件](https://memcpy0.blog.csdn.net/article/details/118370112)。注意，此处的扩展插件更多是一些修改VS Code样式、提高编程舒适度、用于划水摸鱼的有趣插件，而非语言学习（如C/C++/Java等语言）或项目开发（如Git/GitHub/CMake等）插件，此二者会在配置语言/项目环境的相应文章中进行说明。

## 2.2 VS Code自动格式化代码与插件 `Clang-Format` 

可以使用快捷键 `Shift+Alt+F` ，对HTML/CSS都有效，毕竟VS Code提供了内建的支持。其他编程语言就要自行安装代码格式化插件，在扩展商店中搜索 `category:formatters` + 编程语言。比如说C/C++/Java/Objective-C/Objective-C++/Protobuf等编程语言，就可以用Clang-Format这个插件来进行格式化。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715220503269.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
Clang-Format能够细致指定格式化的具体要求，看相关文档[http://clang.llvm.org/docs/ClangFormatStyleOptions.html](http://clang.llvm.org/docs/ClangFormatStyleOptions.html)进行了解，然后自行书写插件的配置文件。

安装Clang-Format工具和同名插件后，同样可以在文档内使用 `Shift+Alt+F` 格式化相对应的语言文件，或者右键选择**格式化文档**，如果有多种格式化方式，还会有**格式化文档的方式**这一选项。更自动化的方法是，在 `settings.json` 中添加以下设置，允许在键入一行代码后、在保存文件时自动格式化代码：
```json
{
    "editor.formatOnType": true,
    "editor.formatOnSave": true,
}
```

## 2.3 VS Code快速编写HTML/CSS与插件 `Emmet`
插件 `Emmet` 原为 `Zen Coding` ，由于VS Code内置了插件 `Emmet` ，写前端HTML和CSS代码时，掌握它的使用技巧会提高不少效率。关于这一部分，可见本人的这篇文章：[编写HTML/CSS代码的快捷技巧](https://memcpy0.blog.csdn.net/article/details/118767796)。更详细的说明，可以看它的[官方文档](https://docs.emmet.io/actions/select-item/)。
## 2.4 VS Code修改终端为Git Bash
关于这一部分，可见本人的这篇文章：[修改VS Code的终端为Git Bash、解决无法执行命令、调试错误的问题](https://memcpy0.blog.csdn.net/article/details/118875775)。

## 2.5 VS Code与Git版本控制
正在写……

## 2.6 VS Code与GitHub协同工作
正在写……
## 2.7 VS Code同步设置
关于这一部分，可见本人的这篇文章：[Windows10下VS Code同步和备份设置](https://memcpy0.blog.csdn.net/article/details/119793360)。

## 2.8 VS Code卸载和重装
如果将VS Code安装在其他位置，想要改变就需要卸载重装。不过如何卸载也是一件麻烦事：
- ~~首先，运用2.7中的方法，备份自己的设置文件；~~
- 然后，卸载VS Code，无论是系统版还是用户版；
- 再然后，删除 `C:\Users\yourname\.vscode` 和 `C:\Users\yourname\AppData\Roaming\Code` 中的 `cache, cacheData` 这两个文件夹，其中前者的 `extensions` 文件夹记录安装的扩展包，后者的两个文件夹包含了个人信息和缓存信息。
 
现在重装就可以选择安装位置了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/cac75a35153b447696271b7fa48834b2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这样删除似乎不用同步和备份配置，因为 `C:\Users\yourname\AppData\Roaming\Code` 有很多信息没有删除，我重装之后就又是原先配置好的VS Code，只是VS Code的安装位置发生了改变。不过如果你重装遇到了问题，可以试试彻底删除 `C:\Users\yourname\AppData\Roaming\Code` 这一文件夹，当然要记得做好备份。

---
# 3. VS Code配置各种语言环境
## 3.1 C/C++、Rust语言环境
[Windows10下VS Code配置C/C++语言环境](https://memcpy0.blog.csdn.net/article/details/118762568)
[Windows10下VS Code配置Rust语言环境]
## 3.2 Java、Kotlin、Scala语言环境
[Windows10下VS Code配置Java语言环境](https://memcpy0.blog.csdn.net/article/details/119280425) 
[Windows10下VS Code配置Scala语言环境]

## 3.3 Clojure、Common Lisp、Racket、Scheme、Emacs Lisp语言环境
[Windows10下VS Code配置Clojure语言环境]

## 3.4 Coq、Haskell、Idris、Latex、Mathematics(Walfram language)、Prolog语言环境
[Windows10下VS Code配置Haskell语言环境](https://memcpy0.blog.csdn.net/article/details/118878150)
[Windows10下VS Code配置LaTeX语言环境](https://memcpy0.blog.csdn.net/article/details/118217633)
[Windows10下VS Code配置Prolog语言环境](https://memcpy0.blog.csdn.net/article/details/119387437)
怎么评价 Idris 语言？ - Martin awodey的回答 - 知乎
https://www.zhihu.com/question/55342708/answer/453091774
 在Coq中实现算符优先文法分析器 - MisakaCenter的文章 - 知乎
https://zhuanlan.zhihu.com/p/380064642 

## 3.5 C#、F#、VB.NET语言环境
[Windows10下VS Code配置C#语言环境](https://memcpy0.blog.csdn.net/article/details/119490421)
[Windows10下VS Code配置F#语言环境](https://memcpy0.blog.csdn.net/article/details/119490421)

## 3.6 Python、Ruby语言环境
[Windows10下VS Code配置Python语言环境](https://memcpy0.blog.csdn.net/article/details/118878142)
[Windows10下VS Code配置Ruby语言环境](https://memcpy0.blog.csdn.net/article/details/118257444)

## 3.7 Golang、Swift、Objective-C语言环境
[Windows10下VS Code配置Golang语言环境]

## 3.8 HTML、CSS、JavaScript、TypeScript、PHP语言环境
[Windows10下VS Code配置HTML/CSS/JavaScript语言环境] 
相关插件：
√ Vim in Browser：在浏览器里预览网页必备，运行html文件
√ Open in Browser	右击选择浏览器打开html文件  
√ Auto Rename Tag：自动修改成对的HTML/XML标签，必备插件，和内置的E们met一起使用，爽快无比；
Beautify：美化JavaScript/CSS等代码；
√ Prettier - Code formatter：比Beautify更好用的代码格式化插件
√ Turbo Console Log：快捷添加 console.log，一键 注释 / 启用 / 删除 所有 console.log
√ CSS Peek：追踪至样式
√ Live Server：实时画面，推荐使用 

√ Path Intellisense：智能路径提示，可以在你输入文件路径时智能提示。
√ Markdown Preview Enhanced：实时预览 markdown。
√ GitLens：详细的 Git 提交日志，Git 重度使用者必备。
√ vscode-json：处理 JSON 文件
√ Regex Previewer：用于JS/TS/PHP/Vue中实时预览正则表达式的效果
√ Regex Workbench：一个工作台，用于设计、测试和实验正则表达式。

## 3.9 Erlang、Perl、Io语言环境
[Windows10下VS Code配置Erlang语言环境] 
[Windows10下VS Code配置Io语言环境](https://memcpy0.blog.csdn.net/article/details/119149575)

## 3.10 Graphviz和Dot、PlantUML（画图）、Processing语言环境
[Windows10下VS Code配置Graphviz和DOT语言环境](https://memcpy0.blog.csdn.net/article/details/119459490)
[Windows10下VS Code配置PlantUML使用环境](https://memcpy0.blog.csdn.net/article/details/120401758)
[Windows10下VS Code配置Processing语言环境](https://blog.csdn.net/myRealization/article/details/119421640)


