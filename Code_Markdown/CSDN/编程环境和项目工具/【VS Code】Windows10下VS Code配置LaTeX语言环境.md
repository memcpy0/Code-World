@[toc]

 
> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。


$\LaTeX$ 是一个强大的排版系统，为了畅快舒心的写 $\LaTeX$ ，也为了少下几个软件（比如**Tex Studio**之类的S**T），我们还是自行折腾VS Code吧。

首先要区分一下 $\TeX$、$\LaTeX$ 、Tex Live、Tex Studio等一系列概念，由于比较复杂，这里只做简要比较与介绍：
- $\TeX$ ：一种类似于C的计算机语言，为简单的排版操作设计。$\TeX$ 有很多版本，现在经常提及的 $\TeX$ 其实是 $\text{Plain}\ \TeX$，$\text{Plain}\ \TeX$ 是 $\TeX$ 的一个改进；
- $\LaTeX$ ：是构建在 $\TeX$ 之上、并对 $\TeX$ 进行扩展的一系列宏定义的集合，也是一门基于 $\TeX$ 的语言。$\TeX$ 的命令相对而言比较简陋，为了方便作者写作，大佬为 $\TeX$ 加入了很多新的功能，这些功能跟 $\TeX$ 加在一起被称为 $\LaTeX$ 。除了 $\LaTeX$ 外还有很多种对 $\TeX$ 的扩展，比如CSDN使用的是 $\href{https://katex.org/}{\KaTeX}$ 的阉割版…… 
- TeX Live：是一个 $\TeX$ 发行版，用于Windows和Linux。所谓发行版，就是一组程序的集合/捆绑包，主要作用是将 $\TeX$ 代码进行解析排版、输出成PDF等。$\TeX$ 发行版相对于 $\TeX$ 语言，大致可以理解为C语言编译器(如GCC或Clang)相对于C语言的关系，不过TeX Live中还集合了一个名为TeXworks的编辑器，因此无法完全比作是编译器，由于TeXworks配置比较麻烦，本文不推荐下载这个编辑器。
- CTeX：国人建立的发行版，用于Windows，但是很久没有更新过了，不推荐使用。
- TeX Studio：大致上算是一个 $\TeX$ 的IDE，不过这里用不到（而且也没什么用），就不多说了。

---
# 1. 下载安装Tex Live
 进入[TeX Live官网下载地址](http://tug.org/texlive/acquire-netinstall.html)，点击红框处链接下载 `install-tl-windows.exe` ：
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20210607233903848.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
记得右键**以管理员身份运行** `install-tl-windows.exe` ，界面如下，点击 `Next >` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210607233306251.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
继续点击 `InStall` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210607233320803.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
中途会出现下图，无需理会，很快就会消失：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210607235051846.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

接着到了更改设置的时间。出现下图后，在 `GUI language` 中选择 `zh-CN` ；修改 `Installation root` 为 `C:\Program Files\texlive` ；由于Tex Live自带的TeXworks不好用，我们用VS Code作为 $\LaTeX$ 的编译器，所以不安装TeXworks前端：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210607233641454.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
如果需要更加个性化的设置，在 `Advanced` 中可以继续更改，其中**Adjust searchpath**选项一定要选中（默认已选中），可以自动添加环境变量。最后点击安装：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210607235008193.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

然后出现下图，慢慢等待安装所需的时间过去，一般可以开一盘帝国时代了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210608000400278.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

安装完成后如下图。由于之前我安装过CTex，所以提示不太一样。最后点击关闭即可：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021060922340857.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
安装完成后，可以在命令行中输入 `tex -version` ，返回的是 $\TeX$ 的版本信息，就说明安装成功了，具体如下图所示。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210702231855461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)




---
# 2. 安装 $\LaTeX$ 支持插件
点击拓展图标，输入 `latex workshop` ，选择搜索出来的插件，点击 `install` ，等待安装完成：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021060800203830.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

安装完成后，点击 `Ctrl + ,` 快捷键，进入UI形式的设置界面：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210626132118977.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
同时，点击下图的右侧图标，进入 `settings.json` 文件，即代码形式的设置界面，正常情况下 `settings.json` 文件中只有一对 `{}` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210626132518906.png)这两种设置界面的功能是一致的——只是前者的交互能力较强，但是某些设置需要去寻找，比较麻烦；后者虽然不太直观，但是可以直接用代码编写自己想要的功能，而且可以通过复制代码轻松克隆他人的设置，比较便捷。

---
# 3. 书写 `settings.json` 文件配置代码
复制下列 $\LaTeX$ 配置代码与注释（不包含**外部PDF查看器**设置）到 `settings.json` 文件中。此处可以是用户配置或者是工作区配置（即项目文件夹内的 `.vscode` 文件夹），由于相应配置代码比较长、用户配置不应与特定语言插件相关、不想时刻开启 `LaTeX Workshop` 插件等诸多原因，我将其放在了代码库 `CodeWorld` 下的 `Code_LaTeX/.vscode/settings.json` 工作区配置文件中，以后就在 `Code_LaTex` 工作区里愉快的写 $\LaTeX$ 了：
```json
{
    //设置扩展何时使用默认的(第一个)编译链(recipe),自动构建(build)LaTeX项目,
    //即什么时候自动进行代码的编译, 选项为3个:
    //- onSave: 当一个tex文件在vscode中被保存时,构建项目;
    //- onFileChange: 当检测到任何依赖项中的文件更改(即使被其他应用程序修改)时构建项目,即
    //检测到代码被更改时就自动编译tex文件以构建项目
    //- never: 从不自动编译,需要编写者手动构建 
    "latex-workshop.latex.autoBuild.run": "never", //新的LaTeX图标能够编译文档,将在下文提及

    //启用上下文LaTeX菜单,此菜单默认停用(因为可以通过新的LaTeX图标使用),即变量设置为false.
    //只需要将此变量设置为true,即可恢复菜单,将编译文档的两个选项显示在鼠标右键的菜单中,分别是
    //- Build LaTeX project: 进行tex文件的编译,构建LaTeX项目
    //- SyncTeX from cursor: 进行正向同步,从代码定位到编译出来的pdf文件相应位置
    "latex-workshop.showContextMenu": true,

    //是否从使用的宏包中自动提取命令和环境,补全正在编写的代码和环境 
    "latex-workshop.intellisense.package.enabled": true,

    //设置当文档编译错误时是否弹出显示警告和错误的弹窗,由于能从终端获取这些信息,所以设置为false
    "latex-workshop.message.error.show": false,
    "latex-workshop.message.warning.show": false,

    //定义编译链(recipe)中使用的LaTeX编译工具,每个工具通过name标记,name将用作下文
    //recipes的引用,command为该工具在扩展中的编译方式.
    //激活时,将通过定义的参数args、env中的环境变量来调用命令command.
    //占位符有%DOC%, %DOC_W32%, %DOC_EXT%, %DOC_EXT_W32%, %DOCFILE%, %DOCFILE_EXT%, 
    //%DIR%, %DIR_W32%, %TMPDIR%, %OUTDIR%, %OUTDIR_W32%
    //更多详情访问https://github.com/James-Yu/LaTeX-Workshop/wiki/Compile#placeholders.
    "latex-workshop.latex.tools": [
        {
            "name": "xelatex",
            "command": "xelatex",
            "args": [
                "-synctex=1",
                "-interaction=nonstopmode",
                "-file-line-error",
                "%DOCFILE%"
            ]
        },
        {
            "name": "pdflatex",
            "command": "pdflatex",
            "args": [
                "-synctex=1",
                "-interaction=nonstopmode",
                "-file-line-error",
                "%DOCFILE%"
            ]
        },
        {
            "name": "bibtex",
            "command": "bibtex",
            "args": [
                "%DOCFILE%"
            ]
        },
        {
            "name": "latexmk",
            "command": "latexmk",
            "args": [
                "-synctex=1",
                "-interaction=nonstopmode",
                "-file-line-error",
                "-pdf",
                "-outdir=%OUTDIR%",
                "%DOCFILE%"
            ]
        }
    ],
    
    //定义LaTeX使用的编译工具链,每个出现在列表中的工具链都是一个对象,包含它的标签name
    //和使用的编译工具tools(将被顺序调用).这些编译工具定义在latex-workshop.latex.tools.
    //默认的,第一个工具链将被用来编译项目.编译链的存在是为了方便编译,尤其是涉及到.bib文件时,
    //要进行多次不同命令的转换编译,整个过程比较麻烦,而编译链解决了这一问题.
    "latex-workshop.latex.recipes": [
        {
            "name": "XeLaTeX",
            "tools": [
                "xelatex"
            ]
        },
        {
            "name": "PDFLaTeX",
            "tools": [
                "pdflatex"
            ]
        },
        {
            "name": "BibTeX",
            "tools": [
                "bibtex"
            ]
        },
        {
            "name": "LaTeXmk",
            "tools": [
                "latexmk"
            ]
        },
        {
            "name": "xelatex -> bibtex -> xelatex*2",
            "tools": [
                "xelatex",
                "bibtex",
                "xelatex",
                "xelatex"
            ]
        },
        {
            "name": "pdflatex -> bibtex -> pdflatex*2",
            "tools": [
                "pdflatex",
                "bibtex",
                "pdflatex",
                "pdflatex"
            ]
        },
    ],

    //设置编译完成后要清除的辅助文件类型,无特殊需求的话就不用更改.它的属性必须是一个字符串数组.
    "latex-workshop.latex.clean.fileTypes": [
        "*.aux",
        "*.bbl",
        "*.blg",
        "*.idx",
        "*.ind",
        "*.lof",
        "*.lot",
        "*.out",
        "*.toc",
        "*.acn",
        "*.acr",
        "*.alg",
        "*.glg",
        "*.glo",
        "*.gls",
        "*.ist",
        "*.fls",
        "*.log",
        "*.fdb_latexmk"
    ],

    //设置什么时候对上文说明的辅助文件类型进行清除,要被清除的文件夹需要通过latex-workshop.latex.outDir来定义.
    //- onFailed: 当编译失败时,清除辅助文件
    //- onBuilt: 当编译结束后,无论成功或者失败,都要清除辅助文件
    //- never: 无论何时,都不清除辅助文件 
    //由于tex文档编译可能需要用到辅助文件(编译目录和参考文献时),使用onBuilt会导致编译不出完整结果甚至编译失败
    //如果要修改tex文件再编译,可能导致pdf文件没有正常更新,这时可能由于辅助文件没有及时更新,需要清除辅助文件,never做不到这一点
    "latex-workshop.latex.autoClean.run": "onFailed",

    //设置vscode编译tex文档时默认被latex-workshop.build使用的编译链,这同样被用到自动构建中.
    //编译链通过它们定义在latex-workshop.latex.recipes的name加以标识.有两个特定值:
    //- first: 使用latex-workshop.latex.recipes中的第一条编译链,可以根据自己的需要改变编译链顺序
    //- lastUsed: 使用最近一次编译所用的编译链 
    "latex-workshop.latex.recipe.default": "lastUsed",

    //用于反向同步(从编译出的pdf文件指定位置跳转到tex文件中相应代码所在位置)的内部查看器(internal viewer)
    //的快捷键绑定(keybinding).变量有两种:
    //- ctrl/cmd + click: 默认选项,使用ctrl/cmd+鼠标左键单击
    //- double-click: 使用鼠标左键双击
    "latex-workshop.view.pdf.internal.synctex.keybinding": "ctrl-click"
}
```
在定义完成了 `"latex-workshop.latex.recipes"` 编译链后，创建一个文件夹 `Code_LaTeX` ，打开该文件夹，然后在里面随便创建一个 `page.tex` 文件，发现左边出现了 $\TeX$ 插件图标，点击该图标能够看到定义的各种编译顺序；点击鼠标右键，还可以看到 `"latex-workshop.showContextMenu"` 设置为 `true` 后出现的两个选项：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210627003552771.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 4. $\TeX$ 文件编译构建、运行、调试
## 4.1 下载 $\TeX$ 测试文件
这一步也很重要，要测试一下现在的VS Code对 $\LaTeX$ 功能的支持程度——包括是否支持图片插入、进行引用、编译参考文献（bixtex文件）、编译目录、支持中英文混排等等。一个不错的测试文件在[这里](https://github.com/Ali-loner/Ali-loner.github.io/tree/master/The%20LaTeX%20testfile%20for%20vscode)。或者使用Git下载：
```shell
git clone https://github.com/Ali-loner/Ali-loner.github.io.git
```
## 4.2 编译 $\TeX$ 测试文件
下载完成之后，使用**文件->打开文件夹**，打开测试文件所在的文件夹 `The LaTeX testfile for vscode` ，然后选中tex文件查看内容：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210627120437909.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
TeX Live有多种编译器，比如 `PDFLaTeX, LaTeX, xeLaTeX, LaTeXmk, luaLaTeX` 等等，其中编译器 `PDFLaTeX` 与 `XeLaTeX` 的区别如下：
  1. `PDFLaTeX` 使用的是 $\TeX$ 的标准字体，所以生成PDF时，会将所有的非 $\TeX$ 标准字体进行替换，其生成的PDF文件默认嵌入所有字体；而使用 `XeLaTeX` 编译，如果论文中有很多图片或者其他元素没有嵌入字体，生成的PDF文件也会有些字体没有嵌入。
  2. `XeLaTeX` 对应的 `XeTeX` 对字体的支持更好，允许用户使用操作系统字体来代替 $\TeX$ 的标准字体，而且对非拉丁字体的支持更好。
  3. `PDFLaTeX` 进行编译的速度比 `XeLaTeX` 速度快。
 
像TeX Studio的默认编译器就是 `PDFLaTeX` ，不过我个人建议使用 `xeLaTeX` ，它对中文的支持更为成熟。此外，TeX Live还带有文献工具 `bibTeX` 、索引工具 `makeindex` 、词汇工具 `makeglossaries` 等等。由于此处**涉及到参考文献的引用**（**.bib**的编译），这里选择 `xelatex -> bibtex -> xelatex*2` 编译链。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210627120826794.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
当发现页面下方出现 $√$ (`recipe successed`)符号时，说明编译成功，如果出现了 $×$ ，说明编译失败：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210627121439359.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
或者使用配置代码里定义的右键菜单，即 `Build LaTeX project` 选项，又或者是 `Ctrl+Alt+B` 的快捷键：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210627123006392.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## 4.3 使用内部查看器、正向同步测试、反向同步测试
编译成功后，通过选择tex文件任意代码选中tex文件，然后点击 $\TeX$ 插件图标，点击 `View LaTeX PDF` 选项中的 `View in VSCode tab` ，在内部查看器中出现编译好的PDF页面（快捷键做法是，在选中tex文件任意代码后，按 `Ctrl+Alt+V` ，选择内部查看方式 `VSCode tab` ，就会出现PDF页面）。注意，现在正在使用**内部查看器**查看编译结果：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210627121858659.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

当一个PDF文件太长，我们不想滑动来翻找内容时，可以使用**正向同步测试**，即从代码定位到PDF页面相应位置。一种方法是使用 $\TeX$ 插件带来的侧边工具栏，先选中代码来选中tex文件，然后单击 $\TeX$ 插件图标，点击 `Navigate, select, and edit` 选项中的 `SyncTeX from cursor` ，即可跳转到对应行：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210627122815893.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
或者使用配置代码里定义的右键菜单，使用 `SyncTeX from cursor` 选项。又或者使用相对应的快捷键 `Ctrl+Alt+J` 。

`Ctrl+Alt+V` 和 `Ctrl+Alt+J` 这两个快捷键来自于：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210703003048260.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/20210703003152724.png)



接着进行反向同步测试，即从PDF页面定位到代码相应位置。一种方法是，在编译生成的PDF上选中跳转行，鼠标**左键双击**或者**Ctrl+鼠标左键单击**（上文设置），跳转到对应代码。
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/2021062714164852.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


## 4.4 添加和使用外部查看器SumatraPDF
由于内部查看器可能无法看到PDF文件的完整展现效果，此时可以选用外部查看器——优势在于能够在查看器中看到PDF文件的目录，可以实时进行跳转。这里选择SumatraPDF作为外部查看器，它是一个极度轻量的PDF阅读软件，安装包不到10MB，支持双向同步功能，能媲美甚至超越内部查看器的功能。

SumatraPDF下载和安装很简单。点击[此处](https://www.sumatrapdfreader.org/download-free-pdf-viewer)进入安装网站，选择 `SumatraPDF-3.2-64-install.exe` 下载，然后安装即可：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210627142454837.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
安装后打开SumatraPDF，UI如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021062916531524.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

记住SumatraPDF的安装路径，然后进行配置。当安装路径变更后，需要修改对应配置。然后在 `settings.json` 中添加配置代码和注释如下（其他设置见[https://github.com/James-Yu/LaTeX-Workshop/wiki/View#using-synctex-with-an-external-viewer](https://github.com/James-Yu/LaTeX-Workshop/wiki/View#using-synctex-with-an-external-viewer))：
```json
{
    //设置默认的PDF查看器,变量为:
    //- tab: 使用vscode内置PDF查看器
    //- browser: 使用电脑默认浏览器进行PDF查看
    //- external: 使用外部查看器进行查看 
    "latex-workshop.view.pdf.viewer": "external",
    
    //进行PDF内外部查看切换的关键参数
    //设置PDF查看器用于\ref命令上的[View on PDF]链接,此命令作用于\ref引用查看,参数如下:
    //- auto: 由编辑器根据情况自动设置
    //- tabOrBrowser: 使用VSCode内置PDF查看器或者电脑默认浏览器进行PDF查看
    //- external: 使用外部PDF查看器查看
    "latex-workshop.view.pdf.ref.viewer": "auto",

    //使用外部查看器时要执行的命令,这一函数不存在官方支持
    //设置外部查看器SumatraPDF.exe所在的位置,用于启动
    //此处需要自行修改
    "latex-workshop.view.pdf.external.viewer.command": "D:/Program Files/SumatraPDF/SumatraPDF.exe",

    //设置使用外部查看器时,"latex-workshop.view.pdf.external.viewer.command"的参数
    //%PDF%是用于生成PDF文件的绝对路径的占位符.
    "latex-workshop.view.pdf.external.viewer.args": [
        "%PDF%"
    ],

    //将生成的.synctex.gz辅助文件,转发到外部查看器时要执行的命令.
    //需要自行更改路径,即SumatraPDF.exe的路径.
    "latex-workshop.view.pdf.external.synctex.command": "D:/Program Files/SumatraPDF/SumatraPDF.exe", //注意修改路径
    
    //当转发.synctex.gz辅助文件到外部查看器时,需要设置的latex-workshop.view.pdf.external.synctex的参数.
    //%LINE%是行号,%PDF%是占位符(生成的PDF文件的绝对路径),%TEX%是触发syncTeX时扩展名为.tex的源LaTeX文件的路径.
    "latex-workshop.view.pdf.external.synctex.args": [
        "-forward-search",
        "%TEX%",
        "%LINE%",
        "-reuse-instance",
        "-inverse-search",
        //VSCode所在的.exe文件 //正向搜索设置
        "code \"D:/Program Files/Microsoft VS Code/Code.exe\" \"D:/Program Files/Microsoft VS Code/resources/app/out/cli.js\" -r -g \"%f:%l\"", //注意修改路径
        "%PDF%"
    ]
}
```
要想在外部查看器中使用反向搜索，需要打开SumatraPDF，进入**设置->选项->设置反向搜索命令行**，填写下面的命令：
```bash
"D:/Program Files/Microsoft VS Code/Code.exe" -g "%f":%l
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210629204616843.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)



此时已经可以使用SumatraPDF作为外部查看器。具体操作很简单，和使用内部查看器差不多。首先，通过选择tex文件任意代码选中tex文件，然后点击 $\TeX$ 插件图标，点击 `View LaTeX PDF` 选项中的 `View in external viewer` ，出现了编译好的PDF页面（快捷键做法是，在选中tex文件任意代码后，按 `Ctrl+Alt+V` ，**此时不用选择查看方式，默认使用外部查看器**）。可以发现，现在我们正在使用SumatraPDF查看原本内嵌输出的PDF，且书签出现在侧面：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021062919032175.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
正向同步和反向同步的过程也一样。正向同步要么使用 $\TeX$ 插件带来的侧边工具栏，点击 `Navigate, select, and edit` 选项中的 `SyncTeX from cursor` 跳转到对应行。或者使用配置代码里定义的右键菜单，使用 `SyncTeX from cursor` 选项。又或者使用相对应的快捷键 `Ctrl+Alt+J` 。反向同步则是，**可能需要先使用SumatraPDF独立打开文件**（个人经验，不一定准确），然后在编译生成的PDF上，鼠标**左键双击**跳转行从而跳转到对应代码。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210629191921279.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
 
 
## 4.5 内外部查看器切换
可以在**UI设置界面**或者 **`settings.json`** 中修改配置，一个示例是从外部查看转为内部查看：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210629205701493.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 5. VS Code与 $\LaTeX$ 的使用实例
可以看一下这篇文章：[使用VS Code+LaTeX编译剑桥大神所有数学课程笔记！妈妈再也不用担心我的数学！](https://memcpy0.blog.csdn.net/article/details/118344172)
