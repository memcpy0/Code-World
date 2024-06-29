@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的【Windows10上配置VS Code】系列文章之一，内容随时可能更新变动，欢迎关注和收藏[系列总目录](https://memcpy0.blog.csdn.net/article/details/117640795)以作备忘。

---
# 0. 各种颜色主题和主题切换
浅色主题有：
- `Atom One Light` ：基于Atom的One Light主题；
- `Github Light Theme`：基于GitHub的外观和视觉，有纯白色背景和灰色背景两种形式；
-  `Brackets Light Pro` ：外观简洁护眼，有 `Brackets Light Pro` 和 `Brackets Light Pro Bold` 两种样式；
-  `Hop Light` ：对比度鲜明，易于阅读；
-  `Snazzy Light` ：在明亮的环境光条件下，色彩仍然清晰鲜明；
-  `Primer Light` ：从Sublime Text移植的主题；
-  `Monokai Light` ：相当于Visual Studio Marketplace上各种深色Monokai主题的浅色版；
-  `Bluloco Light` ：奇特而又复杂的主题，充分考虑了美观、对比和可读性；
-  `Blue Light` ：一种大多数为蓝色的主题，为了消除干扰，只使用了几种颜色；
-  `Solarized Light` ：可以尝尝鲜。

深色主题有：
- `One Dark Pro` ：基于Atom的One Dark主题；
- `Night Owl` ：对应的还有 `Light Owl ` 浅色主题；


可以通过**文件->首选项->颜色主题**，切换使用安装的不同颜色主题：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715164521900.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


---
# 1. 背景图插件 `background` 及自定义配置
使用 `Ctrl+Shift+X` 打开扩展界面，搜索 `background` ，点击安装这个插件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630213704638.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

安装后需要修改插件的默认配置。`Ctrl+,` 打开UI设置界面，搜索 `background` 找到插件设置，然后点击**在 settings.json 中编辑**，进入用户设置 `settings.json` 中修改配置：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630214156616.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这一个背景图插件有7个设置项：
- `background.customImages` ：自定义背景图路径，是一个数组，即可以定义多张图片，随着你打开不同的文件轮流出现；
- `background.enabled` ：是否启用插件，`true` 或者 `false` ；
- `background.loop` ：是否循环使用图片，`true` 或者 `false` ；
- `background.style` ：自定义每个背景图的公有样式；
- `background.styles` ：自定义每个背景图的独有样式；
- `background.useDefault` ：是否使用默认图片，`true` 或者 `false` ；
- `background.useFront` ：是否使用前景图，让图片放到代码的顶部，`true` 或者 `false` ；

样式有这些属性，和CSS很像，可以自行调整：
- `background-position` 决定你的图片位置。
- `opacity` 推荐设置为 `0.1` 左右，否则会看不见代码。 
- `background-size`


将下列配置代码复制到用户设置 `settings.json` 中：
```json
    /*-----------------------------------------背景图插件 `background` 及自定义配置------------------------------------------*/
    "background.enabled": true,     //是否启用插件
    "background.useDefault": false, //是否使用默认图片
    "background.useFront": false,   //是否使用前景图
    "background.loop": true,        //是否循环使用图片
    "background.customImages": [    //自定义背景图路径,最多3张,最好用和电脑屏幕一样大的图 
        //"C:/Users/21839/Pictures/vscode/2.jpg", 
    ],
    "background.style": {           //自定义每个背景图的公有样式
        "content": "''",
        "pointer-events": "none",
        "position": "absolute",
        "z-index": "99999",
        "width": "100%",
        "height": "100%",
        "background-position": "0% 0%", //决定你的图片位置(x,y)
        "background-repeat": "no-repeat",
        "opacity": 0.1,
        "background-size": "contain",
    },
```
图片效果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630232614503.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

注意此时可能弹出Code安装似乎损坏的提示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210701125148675.png)
此时可以下载一个名为 `Fix VSCode Checksums` 的插件， 然后 `Ctrl+Shift+P` 打开命令面板，输入 `Fix Checksums: Apply` ，接着重启VS Code即可：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021070112535924.png)

---
# 2. 更好用的背景图插件 `background-cover` 
上面那个背景图插件总是会出现一些问题，所以我把它卸载了，然后下载了这个国人出品的插件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210713193826667.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这一扩展插件的参数和命令如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210713194036607.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

使用此扩展需要**以管理员身份运行VS Code**，即在Windows环境中，右键单击VS Code图标，选择【以管理员身份运行】。插件作者还建议使用者通过**插件配置命令**或者**底部按钮**进行配置，而不是手动修改配置参数，不过由于参数很少，手动配置完全可行。我个人的 `User  Settings` 配置如下：
```json
{
    /*-----------------------------背景图插件 `background-cover` 及自定义配置------------------------------------*/ 
    //设置背景透明度(0-1)
    "backgroundCover.opacity": 0.3, 
    //可以使用图片目录而不是傻乎乎的写多个图片路径
    "backgroundCover.randomImageFolder": "c:\\Users\\21839\\Pictures\\vscode", 
    //每次启动时随机自动更换背景图(从图片目录中选择,因此要先添加目录后开启)
    "backgroundCover.autoStatus": true,
    //当前使用的背景图路径
    "backgroundCover.imagePath": "c:\\Users\\21839\\Pictures\\vscode\\35.jpg",
}
```
也可以使用配置命令来设置。具体来说，需要点击底部的切换背景图按钮：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210713202424352.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
然后命令面板弹出这些配置命令，进行配置即可。需要注意的是，**关闭背景图**后如要重新开启，可以**选择一张背景图**。一个快捷键是 `Ctrl+Shift+F7` ，可随机更新背景图并重启 `Random update background and restart` 。 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210713202538823.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
完成配置后，效果如下图：![](https://img-blog.csdnimg.cn/20210713203721995.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
：


---
# 3. 缩进染色插件 `indent-rainbow`
搜索并安装 `indent-rainbow` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210701133809207.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
让我们的缩进五光十色，在写Python代码的时候说不定会有用呢？
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210701134012394.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)



---
# 4. 括号染色插件 `Bracket Pair Colorizer 2`
搜索并安装 `Bracket Pair Colorizer 2` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210701134232944.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

对类C/C++代码很有用：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210701134323593.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


---

# 5. 迷你浏览器插件 `Browser Preview` 
搜索并安装 `Browser Preview` ，然后电脑中必须有Chrome浏览器：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210701132619652.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


安装完插件后，左侧菜单栏会出现一个小图标，点击它会自动出现微型浏览器：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210701133441472.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

由于这个插件记不住密码，刷洛谷或者其他OJ的题目时登录不了网站，因此**平时上网还是要在普通的浏览器中**，顶多刷题的时候复制题目链接过来方便看题，或者工作累了想看一下小说偷会懒，或者遇到问题时随手搜索一下……

这个插件的真实用途嘛，其实是用来调试JavaScript、HTML的。


---
# 6. 文件图标插件 `vscode-icons` 和 `Material Icon Theme` 
原生VS Code的文件图标是相当简陋的，我们可以安装文件图标插件，添加好看的文件图标。安装后，可以点击插件页面的**设置文件图标主题**按钮，以使用当前插件作为文件图标主题。
![在这里插入图片描述](https://img-blog.csdnimg.cn/b02be17e4e684fe894fbceb777a38da2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/497aeb2011984b598d40d345fb305bf9.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

或者在**文件->首选项->文件图标主题**中选择不同的文件图标主题：
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/c030cea5c4324d9890fd1a790a9a79da.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/b0c6c929858543b782c2b962995ba481.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

如果有时间，还可以自己用SVG文件定制文件图标主题，比如在[https://materialdesignicons.com/](https://materialdesignicons.com/)中下载合适的文件图标：
![在这里插入图片描述](https://img-blog.csdnimg.cn/1445d5ff386b4b0caa5339daa8d5a29d.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 7. 显示当前文件大小的插件 `filesize`

安装这个插件后，会在状态栏中显示当前文件的大小：
![在这里插入图片描述](https://img-blog.csdnimg.cn/544d6f2277d44dac9339afe8c91b1921.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 8. 酷炫特效插件 `Power Mode`
安装下列插件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/c1dc4f732319451fbc56fed5533b6672.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
安装后，在用户设置 `settings.json` 中添加这几行JSON代码：
```json
{
    "powermode.enabled": true,  			//开启Power Mode
	"powermode.shakeIntensity": 0, 			//关闭抖动（喜欢的也可以选择不关闭)
    "powermode.presets": "exploding-rift",  //特效预设.还有"fireworks","flames","magic","clippy","simple-rift","exploding-rift"
    "powermode.explosionOrder": "random", 	//或者"sequential" 
}
```
`exploding-rift` 的效果比较炫酷，但是可能会（绝对会）遮挡视线，为此按照说明中配置：
![在这里插入图片描述](https://img-blog.csdnimg.cn/a8dfd9c4e9104be9b9e948fd41b1d611.png)

可以在这里 `https://github.com/hoovercj/vscode-power-mode/issues/1` 分享和寻找轻量的、绚丽的GIF特效图：： ![在这里插入图片描述](https://img-blog.csdnimg.cn/8b3c16db2b7b4f28adba372c9d1646a2.gif#pic_center)

