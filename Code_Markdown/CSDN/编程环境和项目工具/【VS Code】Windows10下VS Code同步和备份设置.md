@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。

为什么要进行设置的同步与备份？答案很简单：在自己的电脑上对VS Code进行了不知道多少的配置，终于将它设置好了，然而一旦更换电脑或者重装VS Code，就又要将这些工作从头做起……简直像在经历西西弗斯的绝望。

---
# 1. `Settings Sync` 介绍
为了改变这种情况，我们可以安装一个名为 `Settings Sync` 的插件，同步 `Settings, Snippets, Themes, File Icons, Launch, Keybindings, Workspaces and Extensions` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/f4bd2b3619254ab692082d78561e5a77.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这一插件有如下关键特性：
- 使用个人的 `GitHub Account Token` 和 `Gist` ；
- 一次点击即可轻松上载和下载；
- 在末尾显示摘要页面，其中包含有关配置和扩展的详细信息；
- 启动时自动下载最新设置；
- 文件更改时自动上载设置；
- 与其他用户共享Gist，让他们下载你的设置（和Emacs共享 `emacs.d` 文件一样）；
- 支持 `GitHub Enterprise` ；
- 支持带有 `@sync` 关键字的 `pragmas` ：支持主机、操作系统和环境；
- 用于更改设置/登录的GUI；
- 允许跨计算机同步任何文件。

它将同步的有：
```bash
All extensions and complete User Folder that Contains
1. Settings File
2. Keybinding File
3. Launch File
4. Snippets Folder
5. VSCode Extensions & Extensions Configurations
6. Workspaces Folder
```

---
# 2. 初始化 `Settings Sync` 
按照文档进行配置，它弹出的引导界面如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/807a5bb04c134241bdafe984fd051301.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
 
具体来说，需要 `GitHub Token` 和 `GitHub Gist Id` 来 `setup` 。首先点击上述界面中的 `Login With GitHub` ，在浏览器中打开授权页面：
![在这里插入图片描述](https://img-blog.csdnimg.cn/e10a074050e448469f143ecfa694ad20.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
不过点击授权时，出现了错误：Firefox 无法建立到 localhost:54321 服务器的连接。Why？？？于是使用 `netstat ` 查看了一下54321端口是否处于打开状态、有无进程占用54321端口，结果是没有：
![在这里插入图片描述](https://img-blog.csdnimg.cn/732005ab0da4425da4707e315a245c21.png)后来发现可能是网络的问题，有那么一两个小时无法登上GitHub。等了一段时间后，重新打开该授权页面，确认授权：
![在这里插入图片描述](https://img-blog.csdnimg.cn/77d270701c2c47f0a5e69aa849ddb2eb.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
由于我是第一次使用 `Settings Sync` ，而且是第一次使用Gist，所以文档中说——当我们上载配置时，会自动创建Gist。如果已经有了GitHub Gist，就会打开新的窗口，允许**选择已有的Gist**或 **`Skip` 跳过并创建新的Gist**。


---
# 3. 使用 `Settings Sync`
在命令面板中输入 `Sync` ，打开相关命令：
![在这里插入图片描述](https://img-blog.csdnimg.cn/acd198a90f5a4a398e9d3067806a05a6.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
第一次下载或者上载配置时，引导界面会自动打开（不一定），可以配置 `Settings Sync` 。选择上载或下载后，将看到摘要信息——上载/下载的每个文件和扩展的列表。特别是在下载后，将打开新的弹出窗口，允许重新启动代码以应用设置。还有两个快捷键：`Shift+Alt+U` 用于上载配置，`Shift+Alt+D` 用于下载配置。

`Sync: 如何配置` 这个命令不用在意，也没什么用。如果选择了 `Sync: 重置扩展设置` 将重新设置 `Settings Sync` 。此外，在 `Sync: 高级选项` 中可以看到更多选择：
![在这里插入图片描述](https://img-blog.csdnimg.cn/93a484a958e9489abd80cfa7a85692a1.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

下面开始第一次上载，完成后输出一条信息：
![在这里插入图片描述](https://img-blog.csdnimg.cn/1fc4fa46dab3483d8d2d61b17e021254.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
还可在GitHub Gist上查看相应配置：
![在这里插入图片描述](https://img-blog.csdnimg.cn/80cbb2a5b28d48d28a00cbdba1bf8af6.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


不用特地记忆Gist ID，我们可以在 `Sync: 高级选项` 中，选择 `Sync: 打开设置` ，看到Gist ID、令牌和其他设置：
![在这里插入图片描述](https://img-blog.csdnimg.cn/55760b269dfa4f12a14caa28e5980cf8.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
如果要在其他机器的VS Code上下载配置，就需要下载这一插件，然后 `Ctrl+Shift+P` 打开命令框，输入 `Sync` ，找到 `Download settings` ，会跳转到Github的Token编辑界面，点Edit， `regenerate token` ，保存新生成的token，在命令框中输入此Token，回车，再输入之前的Gist ID，即可同步插件和设置。



 

