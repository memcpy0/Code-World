今日在VS Code集成终端（我使用的是Git Bash）中工作，像下面一样工作在 `python` 解释器中：
![在这里插入图片描述](https://img-blog.csdnimg.cn/a3bb024fd1024952ad51ab46735a9de0.png)
突然就崩溃了，不仅是终端，还有VS Code。我就尝试着打开了外部的Git Bash，输入 `python` ，结果半天没有响应，连续按了几次 `Ctrl+Z` 想要退出，结果报了一系列错误：
![在这里插入图片描述](https://img-blog.csdnimg.cn/dfcb4dc166db48519de0e3aac5d3aee4.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
无奈之下只能强行退出。去网上搜索了一下，发现早有类似记录：
![在这里插入图片描述](https://img-blog.csdnimg.cn/69f50a7364c84afb9f9706befd11ec4c.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
[FAQ git-for-windows](https://github.com/git-for-windows/git/wiki/FAQ#some-native-console-programs-dont-work-when-run-from-git-bash-how-to-fix-it)的答疑区解释得更详细：
![在这里插入图片描述](https://img-blog.csdnimg.cn/1f0e4a0431184136bcf4a66066228184.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

不过，最后不知道怎么的，现在我又能在VS Code集成终端（Git Bash）下输入 `python` 进入解释器了，`winpty python` 也行：
![在这里插入图片描述](https://img-blog.csdnimg.cn/bc3a1fce0324411892533290abc8f902.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

