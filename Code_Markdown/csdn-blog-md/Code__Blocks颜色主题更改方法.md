我已经无法忍受DevC++了，于是下载了Code::Blocks，不过Code::Blocks默认的颜色主题太烂，所以摸索了一下如何更改颜色主题的方法。

首先，从官网下载颜色主题的做法好像不行了，总是打不开这个网址 `http://wiki.codeblocks.org/index.php?title=Syntax_highlighting_custom_colour_themes` 。所以这里将整个颜色主题配置文件的内容贴出来，里面有十几个配色主题，[点击此处](https://memcpy0.blog.csdn.net/article/details/117791300)。

创建一个名为 `color_themes.conf` 的文件，并将这些内容复制下来，粘贴进去。然后**以管理员身份运行** `CodeBlocks Share Config` （或者打开Code::Blocks安装目录下的 `cb_share_config.exe` 文件）：
<img src="https://img-blog.csdnimg.cn/20210610210005956.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="40%">


显示如下图：
<img src="https://img-blog.csdnimg.cn/20210610210219266.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="75%">

然后在 `Soures Configuration file` 选择上一步创建的 `color_themes.conf` ，在 `Destination Configuration file` 选择 `C:\Users\用户名\AppData\Roaming\CodeBlocks\default.conf` ，再将出现的主题全部勾选，然后依次点击 `Transfer >>, Save, Close` 三个按钮（此处不用备份默认主题，因为会自动保留）：
<img src="https://img-blog.csdnimg.cn/20210610210837630.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="75%">
`Transfer >>` 之后，能够明显看到右边出现的颜色主题：
<img src="https://img-blog.csdnimg.cn/20210610210910122.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="75%">
最后重启 `Code::Blocks` ，沿着 `Settings→Editor→Syntax highlighting` 就可以设置颜色主题了。


