> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「玩转Git/GitHub」系列文章之一，将随着时间不断修改完善，目的在于为读者提供有关Git/GitHub使用的大部分参考信息。由于内容随时可能发生更新变动，欢迎关注和收藏[玩转Git/GitHub系列文章目录](https://memcpy0.blog.csdn.net/article/details/119684105)以作备忘。

之前还好好的，结果今天在Git Bash和VS Code中，上传 `git push` 时均出现异常，两者的报错如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/372e7340bd0145a9a83719cc25e30b78.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/4f424adfc4e648f9af251eccebdb25b1.png)
为了解决这个问题，我搜集并整理了几种网上的解法：
- 原因：配置的用户名和邮箱可能输入错误。步骤如下： 
	```bash
	# 先查看自己的用户名和邮箱
	git config user.name
	git config user.email
	# 修改自己的用户名和邮箱
	git config --global user.name "xxx"
	git config --global user.email "xxx"
	# 移除并重新添加原有的仓库
	git remote rm origin
	git remote add origin https://github.com/XXX
	```
- 原因：服务器的SSL证书没有经过第三方机构的签署，所以报错，需要解除SSL认证。为此在Git Bash中输入以下命令：
    ```bash
    git config --global http.sslVerify "false"
	```
- 原因：上传的文件太大。在Git Bash中输入以下命令：
    ```bash
    git config http.postBuffer 5242880003
	```
 - 此外还可以尝试更新DNS缓存。在CMD中输入以下命令：
    ```bash
    ipconfig /flushdns
	```

结果都没用……我觉得还是网络的问题，挂着梯子可以推送，但是一断开就报错，而且反复 `push` 几次也没用。最后还是[这篇文章](https://blog.csdn.net/qq_29493173/article/details/113092656)解决了我的问题：
![在这里插入图片描述](https://img-blog.csdnimg.cn/b1da3085cba1407a8de83f97759bf9b7.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/a946bbfd9e21450194c6bfe3da3bcd2c.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
现在不挂梯子也能上传到GitHub了。

---
**2021/8/19**更新，即使已经修改了hosts文件，今天访问GitHub又被墙了。于是把hosts文件修改的部分去掉，然后 `ping` 一下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/762aa7ff2adf4515888b69e67c1adb4b.png)
过了十几分钟，重新把修改的部分添加回来，再 `ping` 结果连通了？？？不过这样的GitHub：
![在这里插入图片描述](https://img-blog.csdnimg.cn/cd325cc3436f4fd09b225ee5b47a33a1.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

事实上，GitHub总是在更新自己的IP地址，想要通过修改hosts文件访问GitHub，需要时常注意，一旦无法访问就要重新在[https://www.ipaddress.com/](https://www.ipaddress.com/)中查询IP地址，并再次修改hosts文件，是一件劳心劳力的体力活。还不如挂个梯子呢，决定了，先冲个三个月试试，看看具体编程体验如何。
