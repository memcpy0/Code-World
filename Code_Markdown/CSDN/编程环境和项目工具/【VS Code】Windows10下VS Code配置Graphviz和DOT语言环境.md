
> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。

@[toc]

在[获取了VS Code编辑器](https://memcpy0.blog.csdn.net/article/details/117640795)之后，我们将在VS Code中配置Graphviz和DOT的语言学习环境。此外，常用的Graphviz+DOT语法，可以见[【有用工具】画图工具之Graphviz学习笔记](https://memcpy0.blog.csdn.net/article/details/120430600)这篇文章。

---
# 0. Graphviz官方资料
**Graphviz是一款开源的图形可视化软件**，能够将结构化信息表示为抽象图和网络，在计算机网络、生物信息学、数据库、网页设计等诸多领域的可视化界面等都有重要的应用。就我个人而言，可以很方便地使用它来绘制数据结构和算法设计中的说明图。

**Graphviz布局程序用一种简单的文本语言Dot来描述图表**，为具体的图提供了很多特性，比如颜色、字体、表格节点布局、线条样式、超链接、自定义形状等，并用有意义的格式来制作图表——比如网页上的图像和SVG、其他文档包含的PDF或Postscript，或者在交互式图表浏览器中展示。

我们打开[Graphviz官网](http://www.graphviz.org/)，如下所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2240677e6657425bb247a142d4eb04bb.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
`Documentation` 链接指向相关教程和文档，`The DOT Language` 则介绍了DOT语言的抽象语法定义、词法和语法分析的一些细节，`Command Line` 则介绍了DOT命令行参数、各个环境变量的具体含义……建议把文档部分好好看一下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/8164cd8bed7747eca77276d846ead068.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 1. 下载和安装Graphviz
点击 `Download` 下载链接，进入下载页面，下载合适的版本：
![在这里插入图片描述](https://img-blog.csdnimg.cn/02b502854f324b72b23bdb916111bdd2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

下载后运行安装程序：
![在这里插入图片描述](https://img-blog.csdnimg.cn/c9f4ba3165b24a2c8b83cdb1be4b97b2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
同意许可证协议：
![在这里插入图片描述](https://img-blog.csdnimg.cn/b6abcd647f804524a594cfffe3902d34.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
自动添加系统变量：
![在这里插入图片描述](https://img-blog.csdnimg.cn/d3627ea0850f4ee6b8807236460d3f2c.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
选择安装位置：
![在这里插入图片描述](https://img-blog.csdnimg.cn/e4459231a964451db03694a942edc7b8.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
在开始菜单中创建快捷方式，再点击安装：
![在这里插入图片描述](https://img-blog.csdnimg.cn/f884364184674e708d634d9f0fa54a8f.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
最后完成安装：
![在这里插入图片描述](https://img-blog.csdnimg.cn/f96b1ac203eb4782805714218a777175.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

现在打开命令行，输入 `dot -v` 显示版本信息，说明安装成功：
```bash 
C:>dot -v
dot - graphviz version 2.48.0 (20210717.1556)
libdir = "C:\Program Files\Graphviz\bin"
Activated plugin library: gvplugin_dot_layout.dll
Using layout: dot:dot_layout
Activated plugin library: gvplugin_core.dll
Using render: dot:core
Using device: dot:dot:core
The plugin configuration file:
        C:\Program Files\Graphviz\bin\config6
                was successfully loaded.
    render      :  cairo dot dot_json fig gdiplus json json0 map mp pic pov ps svg tk vml xdot xdot_json
    layout      :  circo dot fdp neato nop nop1 nop2 osage patchwork sfdp twopi
    textlayout  :  textlayout
    device      :  bmp canon cmap cmapx cmapx_np dot dot_json emf emfplus eps fig gif gv imap imap_np ismap jpe jpeg jpg json json0 metafile mp pdf pic plain plain-ext png pov ps ps2 svg tif tiff tk vml xdot xdot1.2 xdot1.4 xdot_json
    loadimage   :  (lib) bmp eps gif jpe jpeg jpg png ps svg
```

---
# 2. VS Code中安装支持插件、介绍相应功能
在VS Code的扩展商店中，搜索Graphviz，相关的插件主要有5个：
![在这里插入图片描述](https://img-blog.csdnimg.cn/0505fbfd6c4048c8aa71f4229aeafea8.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
## 2.1 `Graphviz Interactive Preview` 和 `Graphviz Preview` 插件功能介绍
第一个和第二个插件可以分为一类，都提供预览功能，两者的功能大部分重合，但也有不重合的地方。第一个 `Graphviz Interactive Preview` 的功能有：
- 预览DOT/Graphviz源代码的图形，使用鼠标来拖动图形，滚轮可以放大或缩小图形；
- 输入时更新预览的图形；
- 预览界面中使用**交互式边跟踪，单击节点以高亮显示传入和传出边**，**还可以查找和定位结点**（不同之处）；
- 在预览界面，将图形导出为svg或dot；
- 可配置图形选项：例如 `transitionDelay, transitionDuration` 等等，不太重要的功能；
- 开发人员：**传递一个回调函数，在执行 `preview` 命令时接收 `webPanel` ，这允许你覆盖功能**——由webPanel提供的功能，类似于针对 `click/dblClick` 事件的处理程序（不同之处）。

创建一个 `temp.dot` 文件，输入以下代码：
```haskell
digraph G {
    main -> parse -> execute;
    main -> init;
    main -> cleanup;
    execute -> make_string;
    execute -> printf;
    init -> make_string;
    main -> printf;
    execute -> compare;
}
```
在命令面板中运行第一个插件提供的命令 `graphviz interactive: preview (beside)` ，会在右侧预览图形。至于传递回调函数、覆盖webPanel功能这种骚操作，现在也用不上，而且一时半会也搞不懂，这里就不加赘述了。可以注意到，由于没有安装语言支持插件，`.dot` 文件中的代码完全没有高亮：
![](https://img-blog.csdnimg.cn/80608cdb15a24b969e32bf95779698dd.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
我们可以用鼠标拖动图形，滚轮放大和缩小，也可以单击节点 `execute` 来高亮显示相关的边：
![在这里插入图片描述](https://img-blog.csdnimg.cn/4a5bef408ae24507ba1b35acc3e6923b.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
删除 `main -> cleanup` 这一句代码，立刻自动更新了预览的图形：
![在这里插入图片描述](https://img-blog.csdnimg.cn/e9998f24118f4caf9f4b4d6586d6988f.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
在预览界面点击齿轮图标，可以导出图形为SVG，在搜索栏可以查找和定位结点：![在这里插入图片描述](https://img-blog.csdnimg.cn/24bfdbe956ee4993b3ca5d724c024052.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
第二个插件的功能有：
- 运行命令或**点击预览按钮**，预览DOT/Graphviz源代码的图形，使用鼠标或方向键来拖动图形，滚轮或 `+, -` 键放大或缩小图形，还可以**选择缩放模式** `Fixed, Fit, Auto Fit` ，让图形居中显示……这一点上做得比第一个插件好一点，**只是没有交互式边追踪和查找定位结点的功能**；
- 输入时更新预览的图形，**如果出错会在预览时报告错误**，这一点上做得好一点； 
- 在预览界面，**将图形导出为PDF、PNG或SVG**，这一点做得好很多； 
- 允许用 `graphvizPreview.dotExtraArgs` 改变排版引擎 `layout engine`（不同之处）

仍以上面的代码为例，第二个插件的预览界面如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/b499f0f4810c4eb7a47a18b6bf16095f.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
对比下来，第二个插件的功能胜过第一个插件不少，只是还有不足之处，因此我还是都安装了。

## 2.2 `Graphviz Markdown Preview` 插件功能介绍
这一插件功能很简单，主要是为VS Code内置的Markdown插件添加Graphviz支持。具体来说，就是能在Markdown文件中使用 `graphviz` 代码块，因此装不装这个插件都行。随便创建一个 `.md` 文件，将示例代码输进去：
![在这里插入图片描述](https://img-blog.csdnimg.cn/6ea298e4cdb2438d92a3171b25553212.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

`Ctrl+Shift+V` 预览图如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/95873ac86c304ffc9e31f9123637dcf6.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## 2.3 `Graphviz (dot) language support for Visual Studio Code` 同名插件辨析
这两个插件的名字和摘要一模一样的，真让人摸不着头脑。仔细一看，作者是 `Stephanvs` 的插件提供的功能就只有代码高亮和几个代码片段：
![在这里插入图片描述](https://img-blog.csdnimg.cn/a383a9ab7a81481280edbb8eecfb8929.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

作者是 `João Pinto` 的这个插件，功能如下：
- 上面那个插件的代码片段功能 `graph, >, var, dir, prop, path, rank` ；
- 预览图形功能，缩放图形（1:1比例，按窗口高度或宽度缩放等）；
- 导出为SVG文件；
- 在浏览器中打开预览

嘛，也不是很丰富，因此我唯一没有安装的就是这个插件，其他插件都安装了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/488ce106b35f41d3a451808e7b56ed95.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 3. 本文总结
现在的VS Code对Graphviz和DOT的**重要支持**有：
- 语法高亮；
- 代码片段；
- 通过命令 `graphviz interactive: preview (beside)` 或者键盘快捷方式指定一个快捷键来预览DOT图形，使用交互式边跟踪，查找定位结点，覆写webPanel的事件处理；
- 通过命令 `graphviz: open preveiw to the side` 或者预览按钮或者 `Ctrl+K V` 来预览DOT图形，鼠标或键盘拖放、缩放图形，导出为PDF、PNG或SVG；
- 边输入DOT代码边更新预览的图形，在预览时会报错； 
- 在Markdown文件中支持 `graphviz` 代码块。

虽然不能算是尽善尽美，但是就这样吧，以后有更好的插件再补充。


