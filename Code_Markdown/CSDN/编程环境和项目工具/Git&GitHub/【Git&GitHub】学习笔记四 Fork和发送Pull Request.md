@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「玩转Git/GitHub」系列文章之一，将随着时间不断修改完善，目的在于为读者提供有关Git/GitHub使用的大部分参考信息。由于内容随时可能发生更新变动，欢迎关注和收藏[玩转Git/GitHub系列文章目录](https://memcpy0.blog.csdn.net/article/details/119684105)以作备忘。

**只是按步就班地创建GitHub账号，并公开自己的源代码**，这不算难事。不过，刚刚使用GitHub的人，往往不会也不敢用Pull Request功能。

Fork和Pull Request是GitHub的核心功能，也是社会化编程的象征，**不会使用这一功能，就等于不会用GitHub**。不过掌握它的难度确实比较高，刚刚用Pull Request的人在发送Pull Request的时候，往往找不到对方的项目或者不知道如何发送。

---
# 1. Pull Request的概要
什么是Pull Request？Pull Request是**自己修改源代码，请求对方仓库接纳该修改的一种行为**。

假设在使用GitHub的一款开源软件时，发现了Bug，为了继续使用它而手动修复了这个Bug。然后，想要将这段修改的代码提交到该软件的开发仓库中去，让其他使用这款软件的人不会遇到这个Bug。为此，需要第一时间发送Pull Request。

在GitHub上发送Pull Request后，接收方的仓库会创建一个附带源代码的Issue，**我们在这个Issue中记录详细内容，就是Pull Request**。

发送过去的Pull Request是否被接纳，要由接收方仓库的管理者进行判断。一般只要代码没有问题，对方都会接纳。有问题，我们就会收到评论。

只要我们的Pull Request被接纳，我们就会成为这个项目的Contributor，这段代码也将被全世界的人使用。

---
# 2. 发送Pull Request前的准备
整体的过程如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200908230014827.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)


## (1) 查看要修改的源代码
登录[网站](https://ituring.github.io/first-pr/) ，这个网站的源码被公开在[GitHub](https://github.com/ituring/first-pr)上。将自己的感想写入源代码（需要修改 `index.html` 文件），然后**发送Pull Request**。这个网站通过GitHub的GitHub Pages功能发布。GitHub Pages的网站的源代码位于仓库的gh-pages分支。访问仓库页面，就可以看到源代码。

## (2) Fork
访问仓库页面后，点击Fork按钮创建自己的仓库。新建的仓库名为 `自己的账户名/first-pr` 。这里命名为 `memcpy0/first-pr ` 。

## (3) clone
将我们Fork的这个仓库clone到当前的开发环境中：
```bash
$ git clone https://github.com/memcpy0/first-pr.git
Cloning into 'first-pr'...
remote: Enumerating objects: 112, done.
remote: Counting objects: 100% (112/112), done.
remote: Compressing objects: 100% (74/74), done.
remote: Total 9801 (delta 62), reused 69 (delta 38), pack-reused 9689Receiving o
Receiving objects: 100% (9801/9801), 2.87 MiB | 54.00 KiB/s, done.

Resolving deltas: 100% (5913/5913), done.
$ cd first-pr
```
`first-pr` 目录下会生成Git仓库。这个仓库与我们GitHub账户下的 `first-pr` 仓库状态相同，现在只要在这个本地仓库中修改源代码，然后 `push` ，GitHub账户中的仓库就会被修改。

## (4) branch
我们需要在特性分支中进行作业。**当前Git的主流开发模式都会使用特性分支**，要养成：**创建特性分支后再修改代码的好习惯**。在GitHub上发送Pull Request时，**一般都是发送特性分支**。这样，Pull Request的特性就更加明确，有助于仓库拥有者了解我们修改代码的意图。

先查看clone的仓库的分支，其中开头加了 `remotes/origin/` 的是GitHub端仓库的分支，本地开发环境中只有 `gh-pages` 分支。网站中显示的HTML位于 `origin/gh-pages` 分支：
```bash
$ git branch -a
* gh-pages
  remotes/origin/HEAD -> origin/gh-pages
  remotes/origin/feature/move-jquery-from-cdn-to-local
  remotes/origin/gh-pages
```
接着创建一个名为 `work` 的分支，用来发送Pull Request。这个 `work` 就是本次的特性分支：
```bash
$ git checkout -b work
Switched to a new branch 'work'
$ git branch -a
  gh-pages
* work
  remotes/origin/HEAD -> origin/gh-pages
  remotes/origin/feature/move-jquery-from-cdn-to-local
  remotes/origin/gh-pages
```
查看文件列表，可以看到网站中显示的 `index.html` 文件：
```bash
$ ls
images/  index.html  javascripts/  params.json  README.md  stylesheets/
```

## (5) 修改代码
用编辑器打开 `index.html` 文件，以HTML形式添加感想，然后关闭编辑器：
```html
<p class="impression">...</p>
```
比如，我添加的就是 `2020/9/8` 的一条感想：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021070213560124.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
## (6) 提交修改
用 `git diff` 命令查看，修改是否已经正确进行：
```bash
$ git diff
diff --git a/index.html b/index.html
index a629945..3fe726d 100755
--- a/index.html
+++ b/index.html
@@ -75,7 +75,7 @@
...... 省略
```
然后用浏览器打开，看显示是否正确，最后确认添加的代码，提交到本地仓库：
```bash
$ git status
On branch work
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git restore <file>..." to discard changes in working directory)
        modified:   index.html

no changes added to commit (use "git add" and/or "git commit -a")
$ git add index.html
$ git commit -m "Add my impression"
[work bc642d0] Add my impression
 1 file changed, 1 insertion(+), 1 deletion(-)
```
## (7) 创建远程分支
想要在GitHub上发送Pull Request，则自己在GitHub端的仓库中必须有一个包含了修改后代码的分支。现在以同名形式将 `work` 分支 `push` 到远程仓库，创建本地 `work` 分支对应的远程分支：
```bash
$ git push -u origin work
Enumerating objects: 3, done.
Counting objects: 100% (3/3), done.
Writing objects: 100% (3/3), 47.56 KiB | 2.50 MiB/s, done.
Total 3 (delta 0), reused 0 (delta 0), pack-reused 0
remote:
remote: Create a pull request for 'work' on GitHub by visiting:
remote:      https://github.com/memcpy0/first-pr/pull/new/work
remote:
To https://github.com/memcpy0/first-pr.git
 * [new branch]      work -> work
Branch 'work' set up to track remote branch 'work' from 'origin'.
```
查看分支，`/origin/work` 已经创建：
```bash
$ git branch -a
  gh-pages
* work
  remotes/origin/HEAD -> origin/gh-pages
  remotes/origin/feature/move-jquery-from-cdn-to-local
  remotes/origin/gh-pages
  remotes/origin/work
```
打开GitHub的 `用户名/first-pr` 页，`work` 分支已经被创建，且包含我们添加的代码：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200909000619399.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

---
# 3. 发送Pull Request
登录GitHub，切换到 `work` 分支，点击Pull Request右侧的绿色按钮 `New Pull Request` ，跳转到查看分支间差别的页面，看看刚才进行的更改是否正确：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200909001357687.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
上面显示的就是这次Pull Request中包含的提交！确认后内容差别无误后，点击 `Create pull request` 。之后显示表单，用于**填写请求对方采纳的评论**。在评论栏中描述本次进行Pull Request的理由，没有问题后发送即可。随后Pull Request的目标仓库中，会新建Pull Request和Issue，同时该仓库的管理员会接到通知。此时发送的代码还没有被采纳，目标仓库也不会有变化，所以网页也是原样。

如果要查看已发送Pull Request的状态，登录GitHub打开控制面板，查看Pull Request标签页。点击自己发送的Pull Request后，就进入下图的界面。管理者对Pull Request的评论会发到这里。 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200909001855921.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
现在回头看，发现对方接纳了我的Pull Request，我也是该仓库的Contributor了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210702135849283.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


---
# 4. 让Pull Request更有效
如何在开发过程中更加有效地运用Pull Request？
## (1) 在开发过程中发送Pull Request
虽然可以像这里一样，等代码完成后，再发送Pull Request。但是实际开发中，**这样做可能导致一个功能完成后才收到设计或实现方面的指正**，从而使得代码需要大幅度更改或者重新实现。

为此，可以在GitHub上尽早创建Pull Request，从审查中获得反馈，让设计和实现的思路一致，从而提高代码质量。**只要在想发起讨论时发送Pull Request即可，不必等代码最终完成**。

甚至，==可以在Pull Request中附带一段简单代码，再加入直观易懂的Tasklist==，就可以反映出哪些功能已经实现，将来要做哪些工作。这不但能**加快审查者的工作效率**，还能**作为自己的备忘录来使用**。

从反馈中，不但能够获得对自己所提议的新功能的支持和相关改善意见，有时还会被指出没注意到的失误，这样最终完成的代码质量一定高得多。另外，**向发送过Pull Request的分支添加提交时，该提交会自动添加到已发送的Pull Request中**。

总而言之，<font color="red">**这一方法要求尽早发送Pull Request，越早效果越好。**</font>这种在代码库中边讨论边开发的作业流程，比起以往在完成后再审查再反馈的流程高效得多。

## (2) 明确标出"正在开发过程中"
为了防止开发到一半的Pull Request被误合并，一般会在标题前加上 `[WIP]` 字样，表示 `Work In Progress` 仍在开发中。等所有功能都实现之后，再消去这个前缀。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200909110919679.png#pic_center)

## (3) 不进行Fork直接从分支发送Pull Request
一般来说，在GitHub上修改别人的代码时，需要先将仓库Fork到本地，再修改代码，发送Pull Request。但是如果用户对该仓库有编辑权限，则可以直接创建分支，从分支发送Pull Request。

这样就免去了Fork仓库的麻烦。成员在需要时就可以创建自己的分支，然后直接向 `master` 分支等发送Pull Request。

---
# 5. 仓库的维护
Fork或者clone的仓库，如果不管就会离最新的源代码越来越远。如果不用最新的源代码为基础进行开发，就可能白费力气。这里需要学习**如何让仓库保持最新状态**。

clone来的仓库实际上和原仓库没有任何关系，所以需要把原仓库设置为远程仓库，从该仓库获取 `fetch` 数据（这里只修改了远程分支的状态），然后和本地仓库进行合并 `merge` ，让本地仓库的源代码保持最新的状态。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200909111835252.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
## (1) 仓库的Fork与clone
举一个例子说明。我们Fork[ octocat / Spoon-Knife](https://github.com/octocat/Spoon-Knife)这个仓库[^1]，然后对Fork的仓库进行clone，得到本地仓库：
```bash
$ git clone https://github.com/memcpy0/Spoon-Knife.git
Cloning into 'Spoon-Knife'...
remote: Enumerating objects: 16, done.
remote: Total 16 (delta 0), reused 0 (delta 0), pack-reused 16
Unpacking objects: 100% (16/16), 2.30 KiB | 18.00 KiB/s, done.
$ cd Spoon-Knife/
```

## (2) 给原仓库设置名称
给原仓库设置 `upstream` 的名称，将其作为远程仓库。
```bash
$ git remote add upstream https://github.com/memcpy0/Spoon-Knife.git
```
此后，我们的本地仓库将以 `upstream` 作为原仓库的标识符，这个环境下只需设置一次即可。

## (3) 从远程仓库获取最新数据
下面从远程仓库 `upstream` 实际获取 `fetch` 最新源代码，与自己仓库的本地分支进行合并。**要让仓库维持最新状态，只需要重复这一工作即可**。
```bash
$ git fetch upstream
From https://github.com/memcpy0/Spoon-Knife
 * [new branch]      change-the-title -> upstream/change-the-title
 * [new branch]      master           -> upstream/master
 * [new branch]      test-branch      -> upstream/test-branch 
$ git merge upstream/master
```
这里通过 `git fetch` 命令获取最新的数据更新远程分支，然后将 `upstream/master` 远程分支和本地当前分支 `master` 合并，此时当前分支 `master` 就获取了最新的源代码。虽然这里没有可以合并的内容，但是这一操作**确实可以把最新的源代码合并至当前分支**。

我们在创建特性分支、编辑源代码之前，**建议将仓库更新到这一状态**。不过一般来说 `master` 分支的代码都是最新的，很少需要Fork的开发者自己修正。




 
[^1]: This repository is meant to provide an example for forking a repository on GitHub.


 
