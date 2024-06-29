> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「玩转Git/GitHub」系列文章之一，将随着时间不断修改完善，目的在于为读者提供有关Git/GitHub使用的大部分参考信息。由于内容随时可能发生更新变动，欢迎关注和收藏[玩转Git/GitHub系列文章目录](https://memcpy0.blog.csdn.net/article/details/119684105)以作备忘。

今天Git提交时，报了这一串错误，给我看懵了，还以为是强行移动Git安装文件夹从D盘到C盘的问题（不是：
```bash
$ git push -u origin main
remote: Support for password authentication was removed on August 13, 2021. Please use a personal access token instead.
remote: Please see https://github.blog/2020-12-15-token-authentication-requirements-for-git-operations/ for more information.
fatal: unable to access 'https://github.com/memcpy0/LeetCode-Conquest.git/': The requested URL returned error: 403
```
此错误表示，当前正在使用密码验证（而非个人访问令牌）通过HTTPS访问GitHub。然而在2021/8/13以后，这种做法被禁止了，GitHub已经移除了对密码身份验证 `password authentication` 的支持，转而使用个人访问令牌 `personal access token` ——因为人们总会意外泄露密码，而个人访问令牌能通过严格限制权限以减少和控制风险与危害。因此，如果你试图通过用户名和密码访问Git，它将报错并阻止你使用。

一般来说，Git中第一次提交需要输入用户名和密码，不过之后就不用明确输入密码——原因在于，可能有一个凭证管理器 `Credential Manager` ，能够保存密码并在不提示用户的情况下发送密码。


不过首先，我们要给GitHub添加 `PAT (Personal Access Token)` ，即在GitHub中，按照 `Settings => Developer Settings => Personal Access Token => Generate New Token (Give your password) => Fillup the form => click Generate token => Copy the generated Token` ，结果是生成一串字符如 `ghp_sFhFsSHhTzMDreGRLjmks4Tzuzgthdvfsrta` ，将这串字符复制下来。

 在Windows上，要打开凭据管理器 `Credential Manager` ，它被用来存储凭据，例如网站登录和主机远程连接的用户名与密码。如果用户选择存储凭据，那么当用户再次使用对应的操作时，系统会自动填入凭据，实现自动登录。和Firefox的密码管理器有点像：
![在这里插入图片描述](https://img-blog.csdnimg.cn/a008500689bd46b7ad19cec56a4906a6.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


进入Windows凭据 `Windows Credentials` ，找到 `git:https://github.com` 并进行编辑，替换密码为GitHub的个人访问令牌。现在就可以自由使用Git了。
