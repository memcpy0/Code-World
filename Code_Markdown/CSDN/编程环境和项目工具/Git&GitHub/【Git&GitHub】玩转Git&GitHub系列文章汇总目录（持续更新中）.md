@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「玩转Git/GitHub」系列文章的汇总目录，将随着时间不断修改完善，目的在于为读者提供有关Git/GitHub使用的大部分参考信息。由于内容随时可能发生更新变动，欢迎关注和收藏本文以作备忘。

版本管理工具一般具有如下特性：
1. 能够记录历史版本，回退历史版本
2. 团队开发，方便代码合并

版本管理发展简史（维基百科）如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161756001.png)
SVN是集中式版本控制系统，版本库是集中放在中央服务器的，其工作流程如下:
1. 从中央服务器远程仓库下载代码
2. 修改后将代码提交到中央服务器远程仓库
	![img|400x300](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161759624.png)

SVN优点是简单，易操作。缺点是所有代码必须放在中央服务器： 
1. 服务器一旦宕机无法提交代码，即容错性较差
2. 离线无法提交代码，无法及时记录我们的提交行为

Git是分布式版本控制系统（Distributed Version Control System，简称DVCS），分为两种类型的仓库：本地仓库和远程仓库。工作流程如下：
1. 从远程仓库中克隆或拉取代码到本地仓库 `clone/pull`
2. 从本地进行代码修改
3. 在提交前先将代码提交到暂存区
4. 提交到本地仓库。本地仓库中保存修改的各个历史版本（SVN不支持离线提交，Git支持离线提交代码）
5. 修改完成后，需要和团队成员共享代码时，将代码PUSH到远程仓库
	![img|400x380](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161802826.png)


---
# 1. Git/GitHub学习笔记
这一节的多篇文章，主要介绍Git/GitHub的基础入门和高级用法，**只涉及到Git/GitHub自身**。可能会反复重写，只要有新的感悟或学到了新的技巧。
[【Git/GitHub】学习笔记一 安装Git、GitHub/Git的初步配置与使用](https://memcpy0.blog.csdn.net/article/details/108431631)——这是本系列的第一篇文章，讲述Git/GitHub的初始用法。
 [【Git/GitHub】学习笔记二 Git的常用操作](https://memcpy0.blog.csdn.net/article/details/108456149)——讲解Git的大多数常用操作。
[【Git/GitHub】学习笔记三 熟悉GitHub基础功能](https://memcpy0.blog.csdn.net/article/details/108458577)——这一篇和下面的三篇都将介绍GitHub的功能，包括基础功能、Pull Request等等。
[【Git/GitHub】学习笔记四 发送Pull Request](https://memcpy0.blog.csdn.net/article/details/108478676)——这一篇讲述如何发送Pull Request、如何维护仓库。
[【Git/GitHub】学习笔记五 接收Pull Request]()——尚未完成的一篇文章，讲述如何接受Pull Request、合并分支、消除冲突。
[【Git/GitHub】学习笔记六 GitHub协作工具与服务、GUI客户端、开发流程]()——还未开始写，讲述与GitHub协作的工具、服务、GUI客户端，使用GitHub的开发流程等等

此外还有Git相关的一些工具，如GnuPG的使用。

【git 整理提交】git rebase -i 命令详解  链接：https://blog.csdn.net/the_power/article/details/104651772/  写得相当仔细，以后学一下。

---
# 2. Git/GitHub与其他软件协作
## 2.1 与VS Code协作
关于Git/GitHub和VS Code协同工作的，可以看这些文章：
[【Git/GitHub/VSCode】Git提交时使用EMOJI表情、在VS Code中使用emoji commit](https://memcpy0.blog.csdn.net/article/details/119713065)

## 2.2 与Vim/Emacs协作

## 2.3 与IDEA协作
[[【Git&GitHub】在IDEA中使用Git]]

---
# 3. Git/GitHub问题解决日志
[【Git/GitHub】解决Git提交时Support for password authentication was removed on August 13, 2021的错误](https://memcpy0.blog.csdn.net/article/details/119710528)
[【Git/GitHub】解决Git上传时OpenSSL SSL_read: Connection was reset, errno 10054的错误](https://memcpy0.blog.csdn.net/article/details/119737101)

---
# 4. Git/GitHub的使用路径
一般来说，Git和GitHub仓库的创建和使用，有几种选择：
1. **在GitHub上创建一个新的空仓库**，然后在Git Bash中创建一个新仓库，在本地添加修改然后提交到GitHub：
	```bash
	$ echo "# repositoryName" >> README.md
	$ git init
	$ git add README.md
	$ git commit -m "first commit"
	$ git branch -M main
	$ git remote add origin https://github.com/memcpy0/repositoryName.git
	$ git push -u origin main
    ```
2. **在GitHub上创建一个新的空仓库**，然后在Git Bash中将本地已经存在的仓库与GitHub远程仓库进行关联，最后 `push` 这个仓库：
	```bash
	$ git remote add origin https://github.com/memcpy0/repositoryName.git
	$ git branch -M main # 重命名当前分支master为main
	$ git push -u origin main
	```
3. **在GitHub上创建一个新的空仓库**，然后进入本地一个已经存在的文件夹，鼠标右键 `Git Bash Here` 打开Git Bash，`pwd` 命令查看文件夹位置。接着将文件夹设置为仓库，提交文件夹中的所有文件。最后遵循2的做法：	
	```bash
	$ git init # 将该文件夹变成Git可以管理的仓库
	$ ls # 查看文件夹中的内容
	$ git add . # 一个个提交太麻烦,git add .将所有文件都提交到暂存区
	$ git commit -m 'the initial edition' # 提交文件和说明到版本库
	```
4. 对于GitHub上的 **新建非空仓库（有 `README.md, LICENSE, .gitignore` 等文件）** 或者**已有仓库**，可以 `git clone` 下来，在本地添加修改然后提交到GitHub。
5. 对于GitHub上的 **新建非空仓库（有 `README.md, LICENSE, .gitignore` 等文件）** 或者**已有仓库**，除了4的做法外，在本地如果存在文件夹（先按照3的做法设置为仓库）或者相关仓库时，**还可以**先按照2的做法关联远程仓库（重命名分支可做可不做），然后就可以直接推送！不过在第一次进行推送时要注意的是，**GitHub远程仓库非空，因此需要先将两者进行合并**，最后再进行推送：
	```bash
	$ git pull --rebase origin master # 合并仓库
	$ git push -u origin master 
	# -u参数:将master分支的所有内容都提交,第一次推送后的修改可以不用带这个参数
	$ git push origin master
	```
6. **对别人建立的仓库**，可以 `fork` 过来，然后遵循4的做法，接着还可以提交Pull Request。
7. 最后，我们得到了一个属于个人的非空仓库，但是还需要定期维护。本地的修改大体上遵循4的做法，通过如下命令进行同步：
	```bash
	$ git add -A # 将所有文件的修改都提交
	$ git commit -m '说明' # 提交到本地仓库
	$ git push origin master # 推送到GitHub网站上  
	```

完全可以画一张流程图出来。


