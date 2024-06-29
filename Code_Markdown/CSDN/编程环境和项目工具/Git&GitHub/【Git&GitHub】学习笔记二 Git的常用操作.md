@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「玩转Git/GitHub」系列文章之一，将随着时间不断修改完善，目的在于为读者提供有关Git/GitHub使用的大部分参考信息。由于内容随时可能发生更新变动，欢迎关注和收藏[玩转Git/GitHub系列文章目录](https://memcpy0.blog.csdn.net/article/details/119684105)以作备忘。

这里将介绍Git相关的基本知识与操作方法，介绍一些必不可少的Git操作。对于太长的指令，可以在 `.bashrc` 文件中写一条 `alias 短命令名='长命令'` 。~~Git Bash中使用 `source ~/.bashrc` 打开该文件~~。

[git回滚reset、revert、四种模式，超级详细_git revert_Liangyi_J的博客-CSDN博客](https://blog.csdn.net/qq_36125138/article/details/118606548)

---
# 0. Git基础认知
![](https://img-blog.csdnimg.cn/20210709143127423.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MTI1MTM4,size_16,color_FFFFFF,t_70#pic_center)
![img|550x400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161804031.png)
假设我们已经将本地代码提交到远程分分支上，但是我们想撤回某一个commit或者是对某一个文件的修改进行撤回等操作，我们可以使用以下这几种方式

一、git reset
git reset ：回滚到某次提交。
git reset --mixed：此次提交之后的修改会被退回到工作区。不删除工作区改动的代码，撤销commit，撤销git add .【暂存区】
git reset --soft：此次提交之后的修改会被退回到暂存区。不删除工作空间的改动代码 ，撤销commit，不撤销git add . 【用于只想删除该提交本身而不影响其他更改】
git reset --hard：此次提交之后的更改都会被删除，git status 查看工作区是没有记录的(删除工作空间的改动代码，撤销commit且撤销add)

reset命令会更改 Git 存储库的历史记录，这意味着您的提交历史记录将会被修改
reset 命令只能回滚某次的提交，无法满足保留最后一次提交只回滚之前的某次提交。

回滚代码
git log // 查看所有commit的信息
git reset --hard commit_id // 回滚到指定提交
git push origin HEAD --force // 将本地分支代码强制推送到远处分支
1
2
3
或者

git reset --soft HEAD^  
1
命令解释：

HEAD^ 表示上一个版本，即上一次的commit，几个^ 代表几次提交，如果回滚两次就是HEAD^^。
也可以写成HEAD~ 1，如果进行两次的commit，想要都撤回，可以使用HEAD~2。
tips:
假设我们有些时候commit的备注写错了，我们可以运行
git commit --amend 进入vim编辑模式，修改完保存即可

撤回回滚
假设我们整错了commit_id或者误删了某次commit
git reflog //显示可引用的历史版本记录. 找到要撤回的commit的前面的
git reset --hard commit_id // 恢复到这次修改
git push origin HEAD --force // 将本地分支代码强制推送到远处分支
1
2
3
补充：
使用git log命令只可以查看到HEAD指针及其之前的版本信息，如果版本发生过回退操作，则可能会出现，HEAD指针之后仍存在历史提交版本的情况，而这些提交版本信息通过git log命令是看不到的。

二、git revert
git revert：放弃某次提交。之前的提交仍会保留在 git log 中，而此次撤销会做为一次新的提交。
git revert -m：用于对 merge 节点的操作，-m 指定具体某个提交点。
1）撤销提交

要撤销中间某次提交时，使用 git revert 也是一个很好的选择：

git log
git revert commit_id // 这时候代码会发生冲突 解决冲突 重新提交即可 相当于一次新的commit
git push
1
2
3
三、git rebase
git rebase：您想彻底删除 Git 中的某次提交的内容，可以使用 git rebase 命令并将该提交删除。

也不会在git log中保留， 可以撤销中间某次提交
1）撤销提交

git log
git rebase -i commit_id // 进入vim编辑模式，把要删除的commit的前面的pick改为drop 
git push origin HEAD --force // 推到远程分支上
1
2
3
使用 git reset 命令可以将当前分支的 HEAD 指针移到任意提交，从而使您能够删除 Git 存储库中的提交。
使用 git revert 命令可以撤销指定提交的更改，并创建一个新的提交来保留已撤销的更改的历史记录。
使用 git rebase 命令来删除提交可以永久删除提交并更改 Git 历史记录。

补充：
在工作区撤回某个文件 【也就是在git add .之前】

//这2个命令都不会撤回新建的文件，新建的文件只能手动删除
git checkout -- filepath //放弃该相对路径下的文件的修改
git checkout . //放弃所有文件的修改
1
2
3
在暂存区撤回某个文件 【也就是在git add之后，git commit之前】

git reset HEAD filename //放弃该文件的commit，回退到工作区
git reset HEAD // 放弃所有文件的commit，回退到工作区
————————————————
版权声明：本文为CSDN博主「xiaaaa.z」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_43957384/article/details/131394867

在我们日常工作中，通常由于某种原因，需要更新历史提交的commit信息。一般会有如下几种场 景： • 修改当前分支最新 commit 的提交信息 • 修改当前分支某历史commit的提交信息 • 修改当前分支所有提交的commit信息 对于当次提交来说，我们可以显示指定提交者信息。 git commit -m "Initial commit" --author="mn " 通过 git commit 命令将暂存区内容添加到本地仓库后，git会生成相应的commit id。后续我们就 可以针对指定的commit id 进行操作，比如，回退到某个commit id，拉取指定commit id的代码 等。 下面我们针对上面的三种情况进行讲述。 修改上次提交的commit信息 git commit --amend --author="果冻" 输入 git commit --amend 之后，进入编辑模式，修改提交信息，然后按wq保存退出。 如果不想修改提交信息，则添加 --no-edit ，如下所示： git commit --amend --author="果冻不吃皮 " --no-edit git commit --amend 命令只会修改最后一次commit的信息，之前的commit需要使用 git rebase 修改上几次提交的commit信息 git rebase -i 说明： git rebase -i 命令可以压缩合并多次提交。 格式：git rebase -i [startpoint] [endpoint] 其中， -i 的意思是–interactive，即弹出交互式的界面让用户编辑完成合并操作， [startpoint] [endpoint] 则指定了一个编辑区间，如果不指定 [endpoint] ，则该区间的终点 默认是当前分支HEAD所指向的commit(该区间指定的是一个前开后闭的区间)。 首先，我们通过 git rebase -i 选择将哪些提交获得重定位。 例如，合并最近的三次提交： git rebase -i HEAD~3 或者，合并从当前head到15f745b(commit id)的提交： git rebase -i 15f745b 然后，在列表中将开头的pick修改为edit，然后重复执行以下命令直到完成： git commit --amend --author="mn " 收起 次提交的commit信息 几次提交的commit信息 前某一个特定的commit… 有提交的commit信息 重置本项目用户的信息 ：git rebase ：git filter-branch 档 首发于 项目管理与运维实践 赞同 7 1 条评论 分享 喜欢 收藏 申请转载 登录即可查看 超5亿 专业优质内容 超 5 千万创作者的优质提问、专业回答、 深度文章和精彩视频尽在知乎。 立即登录/注册 之后，通过continue命令回到正常状态。 git rebase --continue 修改之前某一个特定的commit信息 查看log，找到上一次提交的commit id git log 然后，通过 git rebase 到要修改的commit之前那个commit。 git rebase 928fc8a3686bf5fcf4527873e075703a9998c127 --interactive（与上面 git rebase -i 类似） 然后，在vi中修改pick为edit，wq保存退出，接着进行内容修改， git add 后 git commit -- amend 。 修改所有提交的commit信息 前提：重置本项目用户的信息 git config user.name '果冻' git config user.email 'liguodongiot@163.com' 方案一：git rebase 首先回到当前分支第一个commit。 git rebase -i --root 然后，弹出编辑器，在需要修改的commit处，将picked改变为edit，然后wq，退出vi。 git commit --amend --reset-author 之后，通过continue命令回到正常状态。 git rebase --continue 查看日志，确定是否修改成功 git log 最后，强制push到远程仓库 git push origin master -f 方案二：git filter-branch 使用 git rebase 方式，步骤比较多，可以直接使用 git filter-branch 快速方便。 例如，将提交者 liguodongiot@163.com 修改为 liguodongiot@foxmail.com # !/bin/sh git filter-branch --env-filter ' OLD_EMAIL="liguodongiot@163.com" CORRECT_NAME="吃果冻不吐果冻皮" CORRECT_EMAIL="liguodongiot@foxmail.com" 首发于 项目管理与运维实践 登录即可查看 超5亿 专业优质内容 超 5 千万创作者的优质提问、专业回答、 深度文章和精彩视频尽在知乎。 编辑于 2022-02-09 09:41 if [ "$GIT_COMMITTER_EMAIL" = "$OLD_EMAIL" ] then export GIT_COMMITTER_NAME="$CORRECT_NAME" export GIT_COMMITTER_EMAIL="$CORRECT_EMAIL" fi if [ "$GIT_AUTHOR_EMAIL" = "$OLD_EMAIL" ] then export GIT_AUTHOR_NAME="$CORRECT_NAME" export GIT_AUTHOR_EMAIL="$CORRECT_EMAIL" fi ' --tag-name-filter cat -- --branches --tags 总结 注意： 如需将修改信息同步到远端仓库，可使用 git push -f 命令进行强制同步，该操作会覆盖远端 分支的提交历史，请自行确认操作风险。

基础概念：
- Clone：克隆，就是将远程仓库复制到本地仓库
- Push：推送，就是将本地仓库代码上传到远程仓库
- Pull：拉取，就是**将远程仓库代码下载到本地仓库，并将代码克隆到本地工作区**
- 本地仓库：是在开发人员自己电脑上的Git仓库，存放我们的代码（ **`.git` 隐藏文件夹就是我们的本地仓库**)     
- 远程仓库：是在远程服务器上的Git仓库，存放代码（可以是github.com或者gitee.com 上的仓库，或者自己该公司的服务器）
- 版本库：`.git` 隐藏文件夹就是版本库，版本库中存储了很多配置信息、日志信息和文件版本信息等
- **工作区：我们自己写代码（文档）的地方**，包含 `.git` 的目录就是工作区，也称为工作目录，主要用于存放开发的代码
- 暂存区：**本地仓库中的一个特殊的文件 `index` 叫做暂存区**（也可叫stage），临时存储我们即将要提交的文件（临时保存修改文件的地方）
	![img|740x650](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161809739.png)

Git工作目录下的文件存在两种状态，这些文件的状态会随着我们执行Git的命令发生变化：
1. `untracked` 未跟踪（未被纳入版本控制），比如新建的文件
2. `tracked` 已跟踪（被纳入版本控制）     
	- `Staged` 已暂存状态：添加到暂存区，但未提交状态（`git add` 工作区->暂存区）
	- `Unmodified` 未修改状态：已提交（`git commit` 暂存区->本地仓库）
	- `Modified` 已修改状态：修改了，但是还没有提交 
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161816167.png)
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161819873.png)





--- 
# 1. 基本操作
## ① `git init` 初始化仓库
要用Git进行版本管理，**必须先初始化Git仓库**。我们需要实际建立一个新文件夹，之后用 `git init` 命令初始化以创建新的Git仓库：
```bash
$ mkdir git-tutorial
$ cd git-tutorial
$ git init
Initialized empty Git repository in D:/MyDownloads/git-tutorial/.git/
```
初始化成功的标识是，成功执行 `git init` 命令的目录下会生成 `.git` 文件夹。其中存储着管理当前目录内容所需的仓库数据。在Git中，这个目录的内容被称作：**附属于该仓库的工作树**。文件的编辑等操作在工作树中进行，然后记录到仓库中，以此**管理文件的历史快照**。如果要恢复文件到之前的状态，可以从仓库中调取之前的快照，在工作树中打开。
```bash
# 初始化仓库不带工作区
$ git init --bare
```
我们的本地仓库由Git维护的三棵树组成：
- 第一个是我们的 ***工作目录*** ，它持有实际文件；
- 第二个是 ***暂存区（Index）*** ，它像一个缓存区域，临时保存个人的改动；
- 第三个是 ***HEAD*** ，它指向我们最后一次提交的结果。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200918001647764.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

---
## ② `git status` 查看仓库中的状态
`git status` 命令用于显示Git仓库的状态。**工作树和仓库的状态，会在操作过程中不断变化**。在Git操作中就常常用 `git status` 查看当前状态。下面实际运行一下：
```bash
$ git status
On branch master

No commits yet

nothing to commit (create/copy files and use "git add" to track)
```
结果显示，我们当前正处于 `master` 分支下，还显示：没有可提交的内容。所谓**提交** `Commit` ，指的是：**记录工作树中所有文件的当前状态**。没有可提交的内容说明，当前我们建立的仓库中没有记录任何文件的任何状态。

我们建立 `README.md` 文件作为管理对象，对第一次提交做准备：
```bash
$ touch README.md
$ git status
On branch master

No commits yet

Untracked files:
  (use "git add <file>..." to include in what will be committed)
        README.md

nothing added to commit but untracked files present (use "git add" to track)
```
可以看到，`Untracked files` 中显示了未提交的 `README.md` 文件。可见，**只要对Git的工作树或者仓库进行操作，`git status` 命令的显示结果就会发生变化**。

---
## ③ `git add` 向暂存区中添加文件
如果仅使用Git仓库的工作树创建文件，那么该文件不会被记入Git仓库的版本管理对象中，用 `git status` 能发现它们被显示在 `Untracked files` 之中。要想让文件变成Git仓库的管理对象，需要用 `git add <filename>` 或者 `git add *` 命令提出更改，将它们添加到暂存区 (`Stage/Index`，**提交之前的一个临时区域**) 当中：
```bash
$ git add README.md
$ git status
On branch master

No commits yet

Changes to be committed:
  (use "git rm --cached <file>..." to unstage)
        new file:   README.md
```
将 `README.md` 加入暂存区后，`git status` 命令的显示结果发生变化，`README.md` 文件显示在 `Changes to be committed` 当中。**这是Git基本工作流程的第一步**。还可将暂存区的文件取消暂存：
```bash
$ git reset <文件名>
```

---
## ④ `git commit` 保存仓库的历史记录
`git commit -m "代码提交信息"` 命令实际提交改动，将当前暂存区中的文件实际保存到仓库的历史记录中，通过这些记录可以在工作树中回滚和恢复文件。另外，使用这一命令，可以记述提交信息，`-m` 参数后的字符串就是提交信息，用来对这个提交进行概述：
```bash
$ git commit -m "First commit"
[master (root-commit) d938084] First commit
 1 file changed, 0 insertions(+), 0 deletions(-)
 create mode 100644 README.md
```
现在，我们的改动已经提交到了 ***HEAD*** ，但是还没有到我们的远端仓库。如果要记录得更加详细，就不加 `-m` ，直接执行 `git commit` 命令。执行后编辑器自动启动，显示如下：
```bash
# Please enter the commit message for your changes. Lines starting
# with '#' will be ignored, and an empty message aborts the commit
# On branch master
# 
# Initial commit
#
# Changes to be committed:
#  (use "git rm --cached <file>..." to unstage)
# 
#      new file:   README.md
# 
```
在 `#` 标为注释的 `Changes to be committed` 要提交的更改一栏中，可以看到本次提交中包含的文件。需要注意的是：**如果在编辑器启动后，将提交信息留空并直接关闭编辑器，提交就会被中止**。

在编辑器中记录提交信息的格式如下：
- 第一行：用一行文字简述提交的更改内容；
- 第二行：空行
- 第三行以后：记录更改的原因和详细内容

将提交信息按照格式记录完毕后，保存并关闭编辑器，以 `#` 为注释的行不用删除，之后，这些记录的提交信息就会被提交。只要按照这样的格式输入和提交，以后就可以通过确认日志的命令看到这些记录。

查看提交后的状态，发现当前工作树处于刚刚完成提交的最新状态，没有内容进行提交：
```bash
$ git status
On branch master
nothing to commit, working tree clean
```
有时候我们一些文件是不需要提交的，比如代码自动生成的一些配置文件。此时我们可修改根目录下的 `.gitignore` 文件：
```python
# 所有以.a结尾的文件讲被忽略(递归)
*.a
# 不管其他规则怎样,强制不忽略lib.a
!lib.a
# 只忽略文件TODO(注意这里是文件)
/TODO
# 忽略build文件夹下所有内容(递归),这里是文件夹
build/
# 忽略doc目录下以*.txt结尾的文件(不递归)
doc/*.txt
# 忽略doc目录下以*.pdf结尾的文件(递归)
doc/**/*.pdf
```

---
## ⑤ `git log` 查看提交日志
使用 `git log` 可以查看仓库以往的提交日志：
```bash
$ git log
commit d938084c76d2a56a672b75cd91797fcdb3a8a078 (HEAD -> master)
Author: memcpy0 <2183927003@qq.com>
Date:   Sun Sep 6 22:27:59 2020 +0800

    First commit

```
可以发现，这是刚刚的提交操作，`commit` 一栏后跟着的是指向这个提交的哈希值。在Git的其他命令中，指向提交时将用到这个哈希值。`Author` 栏则显示给Git设置的用户名和邮箱地址。`Date` 栏中显示提交执行的日期和时间。接着就是这个提交的提交信息。

在 `git log` 后面加上 `--all` 会显示所有分支；`--pretty=oneline` 会将提示信息显示为一行，太长可加上 `--abbrev-commit` ，`--graph` 以图形方式显示。

**在 `git log` 命令后加上 `--pretty=short` ，就可以让程序显示第一行简述信息**，从而方便查看多个提交的日志：
```bash
$ git log --pretty=short
commit d938084c76d2a56a672b75cd91797fcdb3a8a078 (HEAD -> master)
Author: memcpy0 <2183927003@qq.com>

    First commit
```
如果在 `git log` 之后加上目录名，就只会显示该目录下的日志；**如果添加文件名，就只会显示和该文件相关的日志**：
```bash
$ git log README.md
commit d938084c76d2a56a672b75cd91797fcdb3a8a078 (HEAD -> master)
Author: memcpy0 <2183927003@qq.com>
Date:   Sun Sep 6 22:27:59 2020 +0800

    First commit
```
更进一步，**如果想查看提交带来的改动，可以加上 `-p` 参数，文件的前后差别就会显示在提交信息之后**。执行如下命令，就可以只查看 `README.md` 文件的提交日志、提交前后的差别：
```bash
$ git log -p README.md
commit d938084c76d2a56a672b75cd91797fcdb3a8a078 (HEAD -> master)
Author: memcpy0 <2183927003@qq.com>
Date:   Sun Sep 6 22:27:59 2020 +0800

    First commit

diff --git a/README.md b/README.md
new file mode 100644
index 0000000..e69de29
```
从这里可以看出，`git log` 可以组合多种参数，帮助开发者查看以往提交的内容。我们也不必一次记下所有的参数，需要时查看就可以了。

## ⑥ `git diff` 查看更改前后的差别
`git diff` 可以查看**工作树**、**暂存区**、**最新提交**之间的差别。为了理解，以之前提交的 `README.md` 为例，写一点内容进去：
```cpp
# Git学习笔记
```
接着执行 `git diff` 命令，查看当前工作树和暂存区的区别——由于我们还没有 `git add` 向暂存区中添加文件，所以**程序只会显示工作树和最新提交状态之间的差别**：`"+"` 号显示的是新添加的行，被删除的行用 `"-"` 号标出，这里只添加了一行：
```bash
$ git diff
diff --git a/README.md b/README.md
index e69de29..70c68ac 100644
--- a/README.md
+++ b/README.md
@@ -0,0 +1 @@
+# Git学习笔记
\ No newline at end of file
```
用 `git add README.md` 命令将 `README.md` 加入暂存区。如果之后立刻执行 `git diff` ，由于此时工作树和暂存区的状态没有区别，结果什么都不会显示。**要查看与最新提交的差别**，执行以下命令：
```bash
$ git diff HEAD
diff --git a/README.md b/README.md
index e69de29..70c68ac 100644
--- a/README.md
+++ b/README.md
@@ -0,0 +1 @@
+# Git学习笔记
\ No newline at end of file
```
在执行 `git commit` 命令之前，先执行 `git diff HEAD` 命令查看本次提交和上次提交之间有什么区别，确认后再提交，是一个好习惯。注意：这里的 `HEAD` 是指向当前分支中最新一次提交的指针。由于确认了两个提交之间的差别，直接运行 `git commit` 即可：
```bash
$ git commit README.md
[master a23d4ca] Add index
                           1 file changed, 1 insertion(+)
```
之后查看提交日志，确认提交是否成功。发现能够查到第二个提交：
```bash
$ git log
commit a23d4ca4adf7c50ded2011debe3965e9d6343dc7 (HEAD -> master)
Author: memcpy0 <2183927003@qq.com>
Date:   Sun Sep 6 23:37:59 2020 +0800

    Add index

commit d938084c76d2a56a672b75cd91797fcdb3a8a078
Author: memcpy0 <2183927003@qq.com>
Date:   Sun Sep 6 22:27:59 2020 +0800

    First commit
```

## ⑧ 删除文件
可以用如下命令：
```bash
# 从本地工作区 删除文件
$ git rm <文件名>  
# 如果本工作区误删,想要回退
$ git checkout head <文件名>
```
## ⑨ 删除本地Git仓库
删除本地Git仓库的根本原理是，**删除Git本地仓库根目录下的 `.git` 隐藏文件夹**。因此可以手动删除 `.git` 文件夹，或在本地仓库的根目录下，调用Git Bash删除 `.git` 文件夹，输入：
```bash
find . -name ".git" | xargs rm -Rf
```
如果要校验是否成功删除了本地仓库，可以在Git Bash中进入仓库根目录，如果目录末尾没有 `(master)` ，说明成功删除了本地仓库：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210702100526270.png)


---
# 2. 分支操作
在多人并行开发的过程中，往往同时存在多个最新的代码状态。`master` 分支是Git默认创建的分支，所有开发都是以这个分支为中心进行的；每个分支中都拥有自己的最新代码。

不同分支之间，可以同时进行完全不同的任务。某个分支的任务完成后，再将它合并到 `master` 分支上。通过灵活运用分支和合并，能够进行大规模的并行开发。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200918002928545.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

## 2.1 `git branch` 显示分支表和创建、删除和移动分支
`git branch` 的参数如下：
```bash
$ git branch -h
usage: git branch [<options>] [-r | -a] [--merged] [--no-merged]
   or: git branch [<options>] [-l] [-f] <branch-name> [<start-point>]
   or: git branch [<options>] [-r] (-d | -D) <branch-name>...
   or: git branch [<options>] (-m | -M) [<old-branch>] <new-branch>
   or: git branch [<options>] (-c | -C) [<old-branch>] <new-branch>
   or: git branch [<options>] [-r | -a] [--points-at]
   or: git branch [<options>] [-r | -a] [--format]

Generic options
    -v, --verbose         show hash and subject, give twice for upstream branch
    -q, --quiet           suppress informational messages
    -t, --track           set up tracking mode (see git-pull(1))
    -u, --set-upstream-to <upstream>
                          change the upstream info
    --unset-upstream      unset the upstream info
    --color[=<when>]      use colored output
    -r, --remotes         act on remote-tracking branches
    --contains <commit>   print only branches that contain the commit
    --no-contains <commit>
                          print only branches that don't contain the commit
    --abbrev[=<n>]        use <n> digits to display object names

Specific git-branch actions:
    -a, --all             list both remote-tracking and local branches
    -d, --delete          delete fully merged branch
    -D                    delete branch (even if not merged)
    -m, --move            move/rename a branch and its reflog
    -M                    move/rename a branch, even if target exists
    -c, --copy            copy a branch and its reflog
    -C                    copy a branch, even if target exists
    -l, --list            list branch names
    --show-current        show current branch name
    --create-reflog       create the branch's reflog
    --edit-description    edit the description for the branch
    -f, --force           force creation, move/rename, deletion
    --merged <commit>     print only branches that are merged
    --no-merged <commit>  print only branches that are not merged
    --column[=<style>]    list branches in columns
    --sort <key>          field name to sort on
    --points-at <object>  print only branches of the object
    -i, --ignore-case     sorting and filtering are case insensitive
    --format <format>     format to use for the output
```
`git branch` 命令可以将分支名列表显示，同时确认当前所在的分支：
```bash
$ git branch # 列出所有本地分支
* master
```
结果中没有显示其他分支名，表示本地仓库只有 `master` 一个分支；`master` 分支左侧标有 `"*"` ，表示这是当前分支，我们正在 `master` 分支下进行开发。
```bash
# 列出所有远程分支
git branch -r
# 列出所有本地分支和远程分支
git branch -a

# 创建分支
git branch <分支名>
# 切换分支 
git checkout <分支名>
# 删除分支(如果分支已经修改过,还没合并,则不允许删除;如果完全合并了就可删除)
git branch -d  <分支名>
# 强制删除分支
git branch -D  <分支名>
# 创建并切换到分支
git checkout -b <分支名>
```
## 2.2 `git checkout -b` 创建、切换分支
要以当前的 `master` 分支为基础创建新的分支，需要用到 `git checkout -b` 命令。执行下述命令，会创建名为 `feature-A` 的分支，并切换过去：
```bash
$ git checkout -b feature-A
Switched to a new branch 'feature-A'
```
注意，这条命令等价于下面的连续两条命令：
```bash
$ git branch feature-A   //创建feature-A分支
$ git checkout feature-A //切换当前分支到feature-A分支
```
注意，除非我们把分支推送到远端仓库，不然该分支就是不为他人所见的：`git push origin <branch>` 。之后查看分支列表，显示处于 `feature-A` 分支下，因为 `feature-A` 分支左侧标有 `"*"` ：
```bash
$ git branch
* feature-A
  master
```
在这一状态下，可以正常开发、修改代码、执行 `git add` 命令并进行提交，不过代码会被提交到 `feature-A` 分支。不断对一个分支进行提交的操作，被称为**培育分支**。

此时如果在 `README.md` 中实际添加一行：
```
# Git学习笔记
- this is feature-A
```
添加到暂存区，进行提交，之后这一行就添加到 `feature-A` 分支中：
```bash
$ git add README.md
$ git diff HEAD
diff --git a/README.md b/README.md
index 70c68ac..f9a2349 100644
--- a/README.md
+++ b/README.md
@@ -1 +1,2 @@
-# Git学习笔记
\ No newline at end of file
+# Git学习笔记
+- this is feature-A
\ No newline at end of file
$ git commit -m "Add feature-A"
[feature-A 8bdbe0d] Add feature-A
 1 file changed, 2 insertions(+), 1 deletion(-)
```
现在回头看一下 `master` 分支有没有受到影响。切换到 `master` 分支：
```bash
$ git checkout master
Switched to branch 'master'
```
然后查看 `README.md` 文件，发现它仍然保持之前的状态，没有被添加文字：
```bash
$ cat README.md
# Git学习笔记
```
可见，`feature-A` 分支的更改，不会影响到 `master` 分支，这正是在开发中创建分支的优点。==只要创建多个分支，就可以在彼此隔离、互不影响的情况下，同时进行多个功能的开发==。

**如果想要把新建的分支删除掉（不能删除掉当前分支，必须先切换出去），使用如下命令**，这里不实际执行：
```bash
$ git branch -d feature-A //删除feature-A分支
```
现在切换回 `feature-A` 分支：
```bash
$ git checkout -
Switched to branch 'feature-A'
```
**这样用 `"-"` 连字符代表分支名，就可以切换到上一个分支**。或者将 `"-"` 替换成 `feature-A` 也可以。

## 2.3 特性分支与主干分支
特性分支，是集中实现单一特性或者主题 `Topic` ，除此之外不进行任何作业的分支。日常开发中，往往创建多个特性分支，并保留一个随时可以发布软件的稳定分支，稳定分支通常由 `master` 分支来担当。
- 之前创建的 `feature-A` 主要实现 `feature-A` ，除此之外不进行任何作业。即使开发中发现了BUG，也需要创建新的分支，在新分支中进行修正。
- 基于特定主题的作业，在特定分支中进行，完成后再与 `master` 分支合并。只要基于这一开发流程，就能确保 `masterr` 分支随时可以供人查看，其他开发者也可以放心地从 `master` 分支创建新的特性分支。

**主干分支是特性分支的原点，也是合并的终点**。通常 `master` 分支是主干分支，==主干分支中没有开发中途的代码，可随时供他人查看。有时候需要管理多个版本发布，则需要有多个主干分支==。

## 2.4 `git merge` 合并分支
假设 `feature-A` 已经实现完成，将它合并到主干分支 `master` 中。**首先切换**到 `master` 分支使其成为当前分支：
```bash
$ git checkout master
Switched to branch 'master'
```
接着进行合并，为了在历史记录中明确记录下本次分支合并，需要创建合并提交。需要在合并时加上 `--no-ff` 参数：
```bash
$ git merge --no-ff feature-A
Merge made by the 'recursive' strategy.
 README.md | 3 ++-
 1 file changed, 2 insertions(+), 1 deletion(-)
```
用 `git log` 查看，发现 `feature-A` 分支的内容已经被合并到 `master` 分支中了：
```bash
$ git log
commit f5521b1559418e0f784e067a340356bd2d21e640 (HEAD -> master)
Merge: a23d4ca 8bdbe0d
Author: memcpy0 <2183927003@qq.com>
Date:   Mon Sep 7 10:11:32 2020 +0800

    Merge branch 'feature-A' into master

commit 8bdbe0de8eb9b2f2d40cc0fcbb04e630dbd1d254 (feature-A)
Author: memcpy0 <2183927003@qq.com>
Date:   Mon Sep 7 01:32:55 2020 +0800

    Add feature-A

commit a23d4ca4adf7c50ded2011debe3965e9d6343dc7
Author: memcpy0 <2183927003@qq.com>
Date:   Sun Sep 6 23:37:59 2020 +0800

    Add index

commit d938084c76d2a56a672b75cd91797fcdb3a8a078
Author: memcpy0 <2183927003@qq.com>
Date:   Sun Sep 6 22:27:59 2020 +0800

    First commit
```
 
## 2.5 `git log --graph` 图表形式查看分支
如果用 `git log --graph` 查看，就能够用图表形式输出提交日志，非常直观。可以看到特性分支 `feature-A` 提交的内容已被合并：
```bash
$ git log --graph
*   commit f5521b1559418e0f784e067a340356bd2d21e640 (HEAD -> master)
|\  Merge: a23d4ca 8bdbe0d
| | Author: memcpy0 <2183927003@qq.com>
| | Date:   Mon Sep 7 10:11:32 2020 +0800
| |
| |     Merge branch 'feature-A' into master
| |
| * commit 8bdbe0de8eb9b2f2d40cc0fcbb04e630dbd1d254 (feature-A)
|/  Author: memcpy0 <2183927003@qq.com>
|   Date:   Mon Sep 7 01:32:55 2020 +0800
|
|       Add feature-A
|
* commit a23d4ca4adf7c50ded2011debe3965e9d6343dc7
| Author: memcpy0 <2183927003@qq.com>
| Date:   Sun Sep 6 23:37:59 2020 +0800
|
|     Add index
|
* commit d938084c76d2a56a672b75cd91797fcdb3a8a078
  Author: memcpy0 <2183927003@qq.com>
  Date:   Sun Sep 6 22:27:59 2020 +0800

      First commit
```
---
# 3. 更改提交的操作
## 3.1 `git reset` 回溯历史版本
前面学会了如何在实现功能后进行合并提交、累积提交日志作为历史记录。但是这仅仅是Git功能的冰山一角。**Git的真正强大之处，在于可以灵活操作历史版本**——分散型仓库的优势可以让Git在不影响其他仓库的前提下，对历史版本进行操作。

为此，我们先回溯历史版本，创建一个名为 `fix-B` 的特性分支：
```bash
		master
		 / |
		/  |
	   /   |
	  /	   |
feature-A  |    fix-B
	  \    |    /
	   \   |   /
	  	\  |  /
 	  	 \ | /
	  	  node
	  	   |
	  	   |
	  	   |
	  	  node
```
### a. 回溯到创建 `feature-A` 分支前
我们回溯到上一次 `feature-A` 分支创建之前，创建一个名为 `fix-B` 的特性分支。要让仓库的 `HEAD` 、暂存区、当前工作树，回到指定状态，需要用 `git reset --hard` 命令。**只要提供目标时间点的哈希值，就可以完全恢复到该时间点的状态。** 执行下面的命令：
```bash
$ git reset --hard a23d4ca4adf7c50ded2011debe3965e9d6343dc7
HEAD is now at a23d4ca Add index
```
### b. 创建 `fix-B` 分支
我们成功回到了特性分支 `feature-A` 创建之前的状态。因为**所有文件都回溯到了指定哈希值对应的时间点上**，`README.md` 文件的内容也恢复到了当时的状态。接着创建特性分支 `fix-B` ：
```bash
$ git checkout -b fix-B
Switched to a new branch 'fix-B'
```
在这个主题中，我们在 `README.md` 中添加一行文字：
```bash
# Git学习笔记
- this is fix-B
```
然后直接添加并提交 `README.md` 文件：
```bash
$ git add README.md
$ git commit -m "Fix B"
[fix-B d4914ee] Fix B
 1 file changed, 2 insertions(+), 1 deletion(-)
```
现在的状态，或者说当前 `fix-B` 分支的状态，如下所示：
```bash
$ git log --graph
* commit d4914ee6e016a759393be87e517804d2061ee5ad (HEAD -> fix-B)
| Author: memcpy0 <2183927003@qq.com>
| Date:   Mon Sep 7 11:17:40 2020 +0800
|
|     Fix B
|
* commit a23d4ca4adf7c50ded2011debe3965e9d6343dc7 (master)
| Author: memcpy0 <2183927003@qq.com>
| Date:   Sun Sep 6 23:37:59 2020 +0800
|
|     Add index
|
* commit d938084c76d2a56a672b75cd91797fcdb3a8a078
  Author: memcpy0 <2183927003@qq.com>
  Date:   Sun Sep 6 22:27:59 2020 +0800

      First commit
```
接着，`fix-B` 分支的目标是如下所示的状态，即主干分支合并 `feature-A` 分支的修改后，又合并 `fix-B` 的修改：
```bash  
		master
		   | \
		   |  \
           |   \
           |    \
           |     \
		  node   | 
		 / |     |
		/  |     |
	   /   |     |
	  /	   |     |
feature-A  |   fix-B
	  \    |    /
	   \   |   /
	  	\  |  /
 	  	 \ | /
	  	  node
	  	   |
	  	   |
	  	   |
	  	  node
```

### c. 推进至 `feature-A` 分支合并后的状态
因此，我们需要恢复到 `feature-A` 分支被合并后的状态，这一操作被称为**推进历史**。`git log` 只能够**查看以当前状态为终点的历史日志**，这里需要**使用 `git reflog` 查看当前仓库的操作日志**，在其中找到回溯历史之前的哈希值，通过 `git -reset --hard` 恢复到回溯历史前的状态：
```bash
$ git reflog
d4914ee (HEAD -> fix-B) HEAD@{0}: commit: Fix B
a23d4ca (master) HEAD@{1}: checkout: moving from master to fix-B
a23d4ca (master) HEAD@{2}: checkout: moving from fix-B to master
a23d4ca (master) HEAD@{3}: checkout: moving from master to fix-B
a23d4ca (master) HEAD@{4}: reset: moving to a23d4ca4adf7c50ded2011debe3965e9d6343dc7
f5521b1 HEAD@{5}: merge feature-A: Merge made by the 'recursive' strategy.
a23d4ca (master) HEAD@{6}: checkout: moving from feature-A to master
8bdbe0d (feature-A) HEAD@{7}: checkout: moving from master to feature-A
a23d4ca (master) HEAD@{8}: checkout: moving from feature-A to master
8bdbe0d (feature-A) HEAD@{9}: checkout: moving from master to feature-A
a23d4ca (master) HEAD@{10}: checkout: moving from feature-A to master
8bdbe0d (feature-A) HEAD@{11}: checkout: moving from master to feature-A
a23d4ca (master) HEAD@{12}: checkout: moving from feature-A to master
8bdbe0d (feature-A) HEAD@{13}: commit: Add feature-A
a23d4ca (master) HEAD@{14}: checkout: moving from master to feature-A
a23d4ca (master) HEAD@{15}: commit: Add index
d938084 HEAD@{16}: commit (initial): First commit
```
我们在日志中看到 `commit, checkout, merge, reset` 等Git命令的执行记录。只要不对Git进行GC，就可以通过日志随意调取近期的历史状态。即使我们错误执行了Git操作，也基本都可以通过 `git reflog` 命令恢复到原先的状态。

`feature-A` 特性分支合并后的状态，对应的哈希值是 `f5521b1` ，我们将 `HEAD` 、暂存区、工作树恢复到这个时间点的状态：
```bash
$ git checkout master
Switched to branch 'master'
$ git reset --hard f5521b1
HEAD is now at f5521b1 Merge branch 'feature-A' into master
```
现在回到了回溯前的历史状态，此时的状态如下：
```bash
		master
		 / |
		/  |
	   /   |
	  /	   |
feature-A  |    fix-B
	  \    |    /
	   \   |   /
	  	\  |  /
 	  	 \ | /
	  	  node
	  	   |
	  	   |
	  	   |
	  	  node
```
## 3.2 消除冲突
现在只要合并 `fix-B` 分支，就可以得到我们的目标状态：
```bash
$ git merge --no-ff fix-B
Auto-merging README.md
CONFLICT (content): Merge conflict in README.md
Automatic merge failed; fix conflicts and then commit the result.
```
发生什么状况了？原来是 `README.md` 文件发生了冲突 `Conflict` ，系统在合并 `README.md` 时，`feature-A` 分支更改的部分和本次想要合并的 `fix-B` 分支更改的部分发生了冲突。不解决冲突，就无法合并。

在编辑器中打开 `README.md` ，会发现其内容变成下述状态：
```
# Git学习笔记
<<<<<<< HEAD
- this is feature-A
=======
- this is fix-B
>>>>>>> fix-B
```
`=======` 以上的部分是当前 `HEAD` 的内容，以下的部分是要合并的 `fix-B` 分支中的内容，在编辑器中修改成想要的样子：
```
# Git学习笔记
- this is feature-A
- this is fix-B
```
这里的修正，将 `feature-A` 与 `fix-B` 的内容并存于文件中。但是实际的开发中，往往需要删除其中一部分，所以发生冲突时需要仔细分析冲突部分的内容，再进行修改。

冲突解决后，执行添加和提交命令，由于本次解决了冲突，所以记录的提交信息为 `"Fix conflict"` ：
```bash
$ git add README.md
$ git commit -m "Fix conflict"
[master 81d143c] Fix conflict
```
查看日志图表，这次合并已经被记录下来：
```bash
$ git log --graph
*   commit 81d143c79a1a67d6a477115cd8c2dff48245bc6a (HEAD -> master)
|\  Merge: f5521b1 d4914ee
| | Author: memcpy0 <2183927003@qq.com>
| | Date:   Mon Sep 7 12:49:18 2020 +0800
| |
| |     Fix conflict
| |
| * commit d4914ee6e016a759393be87e517804d2061ee5ad (fix-B)
| | Author: memcpy0 <2183927003@qq.com>
| | Date:   Mon Sep 7 11:17:40 2020 +0800
| |
| |     Fix B
| |
* |   commit f5521b1559418e0f784e067a340356bd2d21e640
|\ \  Merge: a23d4ca 8bdbe0d
| |/  Author: memcpy0 <2183927003@qq.com>
|/|   Date:   Mon Sep 7 10:11:32 2020 +0800
| |
| |       Merge branch 'feature-A' into master
| |
| * commit 8bdbe0de8eb9b2f2d40cc0fcbb04e630dbd1d254 (feature-A)
|/  Author: memcpy0 <2183927003@qq.com>
|   Date:   Mon Sep 7 01:32:55 2020 +0800
|
|       Add feature-A
|
* commit a23d4ca4adf7c50ded2011debe3965e9d6343dc7
| Author: memcpy0 <2183927003@qq.com>
| Date:   Sun Sep 6 23:37:59 2020 +0800
|
|     Add index
|
* commit d938084c76d2a56a672b75cd91797fcdb3a8a078
  Author: memcpy0 <2183927003@qq.com>
  Date:   Sun Sep 6 22:27:59 2020 +0800

      First commit
```
---
## 3.3 `git commit --amend` 修改提交信息
使用 `git commit --amend` 可以修改上一条提交信息。这里把上一条提交信息记为 `"Fix conflict"` ，但这实际上是 `fix-B` 分支的合并，解决合并时发生的冲突只是过程之一，这样标记不太恰当。所以进行修改：
```bash
$ git commit --amend
Fix conflict

# Please enter the commit message for your changes. Lines starting
# with '#' will be ignored, and an empty message aborts the commit.
#
# Date:      Mon Sep 7 12:49:18 2020 +0800
#
# On branch master
# Changes to be committed:
#       modified:   README.md
#
```
执行命令后，编辑器启动，显示的内容包含之前的提交信息，把它修改为 `Merge branch 'fix-B'` ，然后保存文件，关闭编辑器：
```bash
[master 9faf393] Merge branch 'fix-B'
        Date: Mon Sep 7 12:49:18 2020 +0800
```
之后显示上面的结果。然后，执行 `git log --graph` 命令，可以看到提交日志中的内容也被修改：
```bash
$ git log --graph
*   commit 9faf393fe36699141f0727f09149716f6ca1b630 (HEAD -> master)
|\  Merge: f5521b1 d4914ee
| | Author: memcpy0 <2183927003@qq.com>
| | Date:   Mon Sep 7 12:49:18 2020 +0800
| |
| |     Merge branch 'fix-B'
| |
| * commit d4914ee6e016a759393be87e517804d2061ee5ad (fix-B)
| | Author: memcpy0 <2183927003@qq.com>
| | Date:   Mon Sep 7 11:17:40 2020 +0800
| |
| |     Fix B
| |
* |   commit f5521b1559418e0f784e067a340356bd2d21e640
|\ \  Merge: a23d4ca 8bdbe0d
| |/  Author: memcpy0 <2183927003@qq.com>
|/|   Date:   Mon Sep 7 10:11:32 2020 +0800
| |
| |       Merge branch 'feature-A' into master
| |
| * commit 8bdbe0de8eb9b2f2d40cc0fcbb04e630dbd1d254 (feature-A)
|/  Author: memcpy0 <2183927003@qq.com>
|   Date:   Mon Sep 7 01:32:55 2020 +0800
|
|       Add feature-A
|
* commit a23d4ca4adf7c50ded2011debe3965e9d6343dc7
| Author: memcpy0 <2183927003@qq.com>
| Date:   Sun Sep 6 23:37:59 2020 +0800
|
|     Add index
|
* commit d938084c76d2a56a672b75cd91797fcdb3a8a078
  Author: memcpy0 <2183927003@qq.com>
  Date:   Sun Sep 6 22:27:59 2020 +0800

      First commit
```
---
## 3.4 `git commit -am` 一次性完成添加和提交操作
先创建一个 `feature-C` 特性分支：
```bash
$ git checkout -b feature-C
Switched to a new branch 'feature-C'
```
在 `feature-C` 的实现 `README.md` 中，添加一行含有拼写错误的文字，以便之后修正：
```bash
# Git学习笔记
- this is feature-A
- this is fix-B
- this is faeture-C
```
这是个小变更，提交时没必要先执行 `git add` 再执行 `git commit` ，用 `git commit -am` 一次完成这两步操作：
```bash
$ git commit -am "Add feature-C"
[feature-C 3814a73] Add feature-C
 1 file changed, 2 insertions(+), 1 deletion(-)
```

---
## 3.5 `git rebase -i` 压缩历史，抹去提交记录
在合并特性分支前，如果发现已提交的内容中有一些拼写错误，可以**提交一个修改**，然后**把这个修改包含在前一个提交中，压缩成一个历史记录**。这是一个很有用的技巧。

承接④中的特性分支（添加错字），现在自行修正刚才提交内容中的错误。修改后用 `git diff` 进行比较，差别如下：
```bash
$ git diff
diff --git a/README.md b/README.md
index b07bb4a..76679b1 100644
--- a/README.md
+++ b/README.md
@@ -1,4 +1,4 @@
 # Git学习笔记
 - this is feature-A
 - this is fix-B
-- this is faeture-C
\ No newline at end of file
+- this is feature-C
\ No newline at end of file
```
之后进行提交，修正错字漏字 `typo` ：
```bash
$ git commit -am "Fix typo"
[feature-C fb5281a] Fix typo
 1 file changed, 1 insertion(+), 1 deletion(-)
```
事实上不应该在历史记录中看到这些提交，因为健全的历史记录不需要它们。我们来更改历史，把 `"Fix typo"` 修改的内容**和前一次提交合并**，在历史记录中合并为一次完美的提交：
```bash
$ git rebase -i HEAD~2
```
这样执行 `git rebase` 命令，将**选定当前分支中包含 `HEAD` (最新提交) 在内的两个最新历史记录为对象**，并在编辑器中打开：
```bash
pick 3814a73 Add feature-C
pick fb5281a Fix typo

# Rebase 9faf393..fb5281a onto 9faf393 (2 commands)
#
# Commands:
# p, pick <commit> = use commit
# r, reword <commit> = use commit, but edit the commit message
# e, edit <commit> = use commit, but stop for amending
# s, squash <commit> = use commit, but meld into previous commit
# f, fixup <commit> = like "squash", but discard this commit's log message
# x, exec <command> = run command (the rest of the line) using shell
# b, break = stop here (continue rebase later with 'git rebase --continue')
# d, drop <commit> = remove commit
# l, label <label> = label current HEAD with a name
# t, reset <label> = reset HEAD to a label
# m, merge [-C <commit> | -c <commit>] <label> [# <oneline>]
# .       create a merge commit using the original merge commit's
# .       message (or the oneline, if no original merge commit was
# .       specified). Use -c <commit> to reword the commit message.
#
# These lines can be re-ordered; they are executed from top to bottom.
#
# If you remove a line here THAT COMMIT WILL BE LOST.
#
# However, if you remove everything, the rebase will be aborted.
```
我们将 `fb5281a` 的 `Fix typo` 的历史记录，压缩到 `3814a73` 的 `Add feature-C` 中。如下所示，将 `fb5281a` 左侧的 `pick` 部分删除，修改为 `fixup` ：
```bash
pick 3814a73 Add feature-C
fixup fb5281a Fix typo
```
随后，保存编辑器中的内容，关闭编辑器。显示 `rebase` 成功，也就是将上面的两个提交作为对象，将 `"Fix typo"` 的内容合并到上一个提交 `"Add feature-C"` 中，改写成一个新的提交：
```bash
Successfully rebased and updated refs/heads/feature-C.
```
现在查看提交日志，发现 `"Add feature-C"` 的哈希值已经变化，说明提交被修改：
```bash
$ git log --graph
* commit b472ba0a73b25fb4691381171a4b2f8fc9c7008f (HEAD -> feature-C)
| Author: memcpy0 <2183927003@qq.com>
| Date:   Mon Sep 7 15:20:27 2020 +0800
|
|     Add feature-C
|
*   commit 9faf393fe36699141f0727f09149716f6ca1b630 (master)
|\  Merge: f5521b1 d4914ee
| | Author: memcpy0 <2183927003@qq.com>
| | Date:   Mon Sep 7 12:49:18 2020 +0800
| |
| |     Merge branch 'fix-B'
| |
| * commit d4914ee6e016a759393be87e517804d2061ee5ad (fix-B)
| | Author: memcpy0 <2183927003@qq.com>
| | Date:   Mon Sep 7 11:17:40 2020 +0800
| |
| |     Fix B
| |
省略
```
这样 `"Fix typo"` 就被抹去，相当于 `"Add feature-C"` 中从来没有出现过拼写错误，是一种良性的历史改写。`feature-C` 分支的使命结束，将它和 `master` 分支合并：
```bash
$ git checkout master
Switched to branch 'master'
$ git merge --no-ff feature-C
Merge made by the 'recursive' strategy.
 README.md | 3 ++-
 1 file changed, 2 insertions(+), 1 deletion(-)
```
---
## 3.6 `git rebase -i` 压缩历史，修改已推送记录
如果很不幸地，错误的提交信息已经被推送到远程仓库，整个修改步骤就会麻烦许多。具体参考[已经push的commit如何修改message](https://blog.csdn.net/weixin_33856370/article/details/92024153)这篇文章，有以下步骤：
- `git rebase -i` 列出 `commit` 列表，具体来说，`git rebase -i <commit id>` 会列出不包括 `<commit id>` 的列表，`git rebase -i HEAD~n` 会列出最近的 `n` 条，`git rebase -i` 会列出本地仓库没有 `push` 到远程仓库的 `commit` 信息；
- 找到需要修改的 `commit` 记录，把最前面的 `pick` 改为 `edit` 或 `e` ，`:wq` 退出；
- 使用 `git commit --amend` ，修改 `commit` 的具体信息，然后保存并 `git rebase --continue` ，直到出现 `Successfully rebased and updated refs/heads/main.` 这条消息；
- 修改途中，也可跳过某些记录或退出 `git rebase (--skip | --abort)` ；
- 最后使用 `git push -f` 强制更新远程仓库，一定要有 `-f` ，否则会将 `edit` 的 `commit` 添加到 `commit` 后面，而不是更新原 `commit` 信息。

---
## 3.7 `git checkout --<filename>` 替换本地改动
假如操作失误（当然，这最好永远不要发生），可以使用如下命令替换掉本地改动：
```bash
$ git checkout -- <filename>
```
此命令会使用 ***HEAD*** 中的最新内容替换掉我们的工作目录中的文件。**已添加到暂存区的改动以及新文件都不会受到影响**。

假如想丢弃在本地的所有改动与提交，可以到服务器上获取最新的版本历史，并将本地主分支指向它：
```bash
$ git fetch origin
$ git reset --hard origin/master
```

---
# 4. 推送到远程仓库
我们了解了对本地单一仓库的操作，是时候接触远程仓库——和本地仓库相对独立的另一个仓库。先在GitHub上创建一个仓库，并且设置为本地仓库的远程仓库。

参考之前的步骤在GitHub上新建一个仓库，**仓库名要和本地仓库保持一致**，即 `git-tutorial` 。创建时不勾选 `add a README file` 选项（一旦勾选，GitHub一侧的远程仓库会自动生成 `README` 文件，**从创建之初就和本地仓库失去了整合性**，虽然也可以后期强制覆盖，但是麻烦），直接 `Create repository` 即可。

可以查看远程仓库：
```bash
# 查看远程,列出指定的每一个远程服务器的简写
git remote 
# 查看远程,列出简称和地址
git remote -v  
# 查看远程仓库详细地址
git remote show  <仓库简称>
```

## 4.1 `git remote add` 添加远程仓库
添加远程仓库的命令如下：
```bash
# 添加远程仓库
git remote add <远端名称> <仓库路径URL>
# 移除远程仓库和本地仓库的关系(只是从本地移除远程仓库的关联关系，并不会真正影响到远程仓库)
git remote rm <远端名称>
```
在GitHub上创建的仓库路径为 `https://github.com/memcpy0/git-tutorial.git` ，现在用 `git remote add` 命令将其添加为本地仓库的远程仓库：
```bash
$ git remote add origin https://github.com/memcpy0/git-tutorial.git
```
这一命令执行后，Git会自动将 `https://github.com/memcpy0/git-tutorial.git` 远程仓库的名称设置为 `origin` （标识符）。

## 4.2 `git push` 推送至远程仓库
`git push` 命令格式如下，==如果远程分支名与本地分支名相同，则可以只写本地分支==，`--set-upstream` 推送到远端的同时、并且建立和远端分支的关联关系（第一次push的时候使用）；**如果当前分支已经和远端分支关联，则可以省略分支名和远端名，即 `git push`** ；`-f` 表示强制推送、强制覆盖，一般被禁用：
```bash
# 将本地仓库分支推送至远程仓库的某个分支
git push [-f] [--set-upstream] [远端名称 [本地分支名][:远端分支名]]
```
我们的改动现在已经在本地仓库的 ***HEAD*** 中了。如果要将**当前分支下本地仓库的内容**推送到远程仓库，需要用到 `git push origin master` 命令。可以把 `master` 换成想要推送的任何分支。假定我们就是在 `master` 分支下操作：
```bash
$ git push -u origin master
Enumerating objects: 20, done.
Counting objects: 100% (20/20), done.
Writing objects: 100% (20/20), 1.81 KiB | 205.00 KiB/s, done.
Total 20 (delta 0), reused 0 (delta 0), pack-reused 0
To https://github.com/memcpy0/git-tutorial.git
 * [new branch]      master -> master
Branch 'master' set up to track remote branch 'master' from 'origin'.
```
**这样执行 `git push` 命令，当前本地仓库 `master` 分支的内容就会被推送到GitHub的远程仓库 `origin` 的 `master` 分支**。`-u` 参数可以在推送的同时，将 `origin` 仓库的 `master` 分支，设置为本地仓库当前分支的 `upstream` 上游。此后，**运行 `git pull` 从远程仓库获取内容时，本地仓库的分支就能直接从 `origin` 的 `master` 分支获取内容，省去另外添加参数的麻烦**。

在GitHub上，可以确认远程仓库 `master` 分支的内容，和本地 `master` 分支相同：
![img|500x250](https://img-blog.csdnimg.cn/20200907161442873.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
除了 `master` 分支以外，远程仓库还可以创建其他分支。如果要推送到 `master` 以外的分支，比如说，在本地仓库创建 `feature-D` 分支，并将它以同名形式 `push` 到远程仓库：
```bash
$ git checkout -b feature-D
Switched to a new branch 'feature-D'
$ git push -u origin feature-D
Total 0 (delta 0), reused 0 (delta 0), pack-reused 0
remote:
remote: Create a pull request for 'feature-D' on GitHub by visiting:
remote:      https://github.com/memcpy0/git-tutorial/pull/new/feature-D
remote:
To https://github.com/memcpy0/git-tutorial.git
 * [new branch]      feature-D -> feature-D
Branch 'feature-D' set up to track remote branch 'feature-D' from 'origin'.
```
现在，在GitHub中可以看到 `feature-D` 分支：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200907162748536.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
如果你还没有克隆现有仓库，并欲将你的仓库连接到某个远程服务器，可以使用命令添加：`git remote add origin <server>` 。如此就能将你的改动推送到所添加的服务器上去了。
## 4.3 解决远端冲突
假设A用户在本地修改代码后优先推送到远程仓库，此时B用户在本地修订代码，提交到本地仓库后，也需要推送到远程仓库，此时B用户晚于A用户，故**需要先拉取远程仓库的提交，经过合并后才能推送到远程分支**。如下图所示：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302170049004.png)

在B用户拉取代码时，如果A、B用户在同一段时间修改了同一个文件相同位置的代码，就会发生合并冲突。==远程分支也是分支，所以合并时冲突的解决方法也和解决本地分支冲突相同==。

---
# 5. 从远程仓库中获取
上面我们将GitHub上新建的仓库设置为远程仓库，并向其 `push` 了 `feature-D` 分支。现在，所有能够访问这个远程仓库的人，都可以获取 `feature-D` 分支并加以修改。

现在，我们装作又有一个开发者来共同开发目标仓库——在另一个目录下新建一个本地仓库，学习从远程仓库获取内容的操作，相关命令如下：
```bash
# 从远程仓库克隆
git clone <url>

# 从远程仓库抓取 (将仓库的更新都抓取到.git目录,不会合并到工作区,工作区不发生变化)
# 如果不指定远端名称和分支名,则抓取所有分支
git fetch <remote-name>  <分支名称>
# 手动合并,把某个版本的某个分支合并到当前工作区
git merge <remote-name>/<分支名称>

# 从远程仓库拉取 (将远端仓库的修改拉取到.git目录,合并到工作区,工作区发生变化) 
# = fetch+merge
# 如果不指定远端名称和分支名,则抓取所有分支
git pull  <remote-name>  <分支名称>
git pull  <remote-name>  <分支名称>  --allow-unrelated-histories  #  强制拉取合并
```

## 5.1 `git clone` 获取远程仓库
首先到其他目录下，将GitHub上的仓库 `clone` 到本地，**不要和之前操作的仓库处于同一目录下**。我创建了一个 `my-git` 目录，然后将远程仓库复制到这一目录下：
```bash
$ git clone https://github.com/memcpy0/git-tutorial.git my-git
Cloning into 'my-git'...
remote: Enumerating objects: 20, done.
remote: Counting objects: 100% (20/20), done.
remote: Compressing objects: 100% (10/10), done.
remote: Total 20 (delta 0), reused 20 (delta 0), pack-reused 0
Unpacking objects: 100% (20/20), 1.79 KiB | 10.00 KiB/s, done.
```
如果 `git clone` 出现了 `fatal: unable to access 'https://github.com/xxxxxx/xxxx.git/': Failed to connect to xxxxx` 这一类错误，最好的办法是**将命令行里的 `https` 改为 `git` 然后重新执行**。不过这样做，提交前需要先 `git remote remove origin` ，然后 `git remote add origin https://github.com/xxxxxx/xxxx.git` ，再进行提交。

**执行 `git clone` 命令后，我们默认处于 `master` 分支下，同时系统自动将 `origin` 设置成「被 `clone` 的远程仓库」的标识符**。即，当前本地仓库的 `master` 分支和GitHub远程仓库 `origin` 的 `master` 分支在内容上完全相同。

再**用 `git branch -a` 命令查看本地仓库和远程仓库的分支信息**，结果中显示了 `remotes/origin/feature-D` ，说明远程仓库中已经有了 `feature-D` 分支：
```bash 
$ git branch -a
* master
  remotes/origin/HEAD -> origin/master
  remotes/origin/feature-D
  remotes/origin/master
```

**用 `git checkout -b feature-D origin/feature-D` 将远程的 `feature-D` 分支获取到本地仓库**，`-b` 后面是本地仓库中**新建分支的名称**，命名为 `feature-D`（和远程仓库中的对应分支保持同名）；新建分支的名称后，是**获取来源的分支名称**，这里是名为 `origin` 的（GitHub端的远程）仓库的 `feature-D` 分支作为来源，然后在本地仓库中创建 `feature-D` 分支：
```bash
$ git checkout -b feature-D origin/feature-D
Switched to a new branch 'feature-D'
Branch 'feature-D' set up to track remote branch 'feature-D' from 'origin'.
```
然后向本地的 `feature-D` 分支提交更改。在 `README.md` 文件中添加一行文字 `- this is feature-D` ，按照之前的方式提交：
```bash
$ git commit -am "Add feature-D"
[feature-D dcb426b] Add feature-D
 1 file changed, 2 insertions(+), 1 deletion(-)
```
现在推送 `feature-D` 分支：
```bash
$ git push
Enumerating objects: 3, done.
Counting objects: 100% (3/3), done.
Writing objects: 100% (3/3), 289 bytes | 96.00 KiB/s, done.
Total 3 (delta 0), reused 0 (delta 0), pack-reused 0
To https://github.com/memcpy0/git-tutorial.git
   2fd5cb9..dcb426b  feature-D -> feature-D
```
---
## 5.2 `git pull` 获取最新的远程仓库分支
回到原来的目录下，这边的本地仓库只是创建了 `feature-D` 分支，没有在其中进行任何提交。然而远程仓库的 `feature-D` 分支已经有了我们刚刚推送的提交。

此时，可以执行 `git pull` 命令，以**在工作目录中获取 `(fetch)` 并合并 `(merge)` 远端的改动**。将本地的 `feature-D` 分支更新到最新状态。当前分支为 `feature-D` 分支：
```bash
$ git pull origin feature-D
remote: Enumerating objects: 5, done.
remote: Counting objects: 100% (5/5), done.
remote: Compressing objects: 100% (2/2), done.
remote: Total 3 (delta 1), reused 2 (delta 0), pack-reused 0
Unpacking objects: 100% (3/3), 248 bytes | 9.00 KiB/s, done.
From https://github.com/memcpy0/git-tutorial
 * branch            feature-D  -> FETCH_HEAD
   2fd5cb9..dcb426b  feature-D  -> origin/feature-D
Updating 2fd5cb9..dcb426b
Fast-forward
 README.md | 3 ++-
 1 file changed, 2 insertions(+), 1 deletion(-)
```
GitHub远程仓库中的 `feature-D` 分支是最新状态，所以本地的 `feature-D` 分支就得到了更新。以后只要像平常一样在本地进行开发，再 `push` 到远程仓库，就可以和其他开发者一起在同一个分支进行作业。当然，如果两个人同时修改了同一部分的代码，`push` 时容易发生冲突。为减少冲突的发生，建议更频繁地进行 `push, pull` 操作。

总的来说，**`git pull` 和 `git push` 两种情况下，Git都会尝试去自动合并改动**。遗憾的是并非每次都成功，可能出现冲突 `(conflicts)` ，这时候就需要修改这些文件，手动合并这些冲突 `conflicts` 。改完之后，还需要执行如下命令以将它们标记为合并成功：
```bash
$ git add <filename>
```
在合并改动之前，可以使用如下命令预览差异：
```bash
$ git diff <source_branch> <target_branch>
```

---
# 6. 标签操作
标签是静态的概念，分支是动态概念，里面的文件会不断变化。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303180051506.png)

为软件发布创建标签是推荐的做法。这个概念早已存在，在SVN中也有。相关命令如下：
```bash
# 列出所有tag
git tag
# 查看tag详细信息 
git show [tagName]
# 新建一个tag
git tag [tagName]

# 提交指定tag
$ git push [仓库简称] [tagName]
# 新建一个分支,指向某个tag
$ git checkout -b [branch] [tag]
# 删除本地tag
$ git tag -d [tag]
# 删除远程tag(注意空格)
$ git push origin :refs/tags/[tag]
```
可以执行如下命令创建一个叫做 `1.0.0` 的标签：
```bash
$ git tag 1.0.0 1b2e1d63ff
```
`1b2e1d63ff` 是你想要标记的提交记录的哈希值前 $10$ 位字符。可以使用下列命令获取哈希值：
```bash
$ git log
```
也可以使用少一点的前几位哈希值，只要它的指向具有唯一性。 

---
# 学习Git的其他资料
[Git 简明指南](https://www.runoob.com/manual/git-guide/) 助你入门Git的简明指南。
[图解 Git](http://marklodato.github.io/visual-git-guide/index-zh-cn.html) 用图示学习Git的使用。
[简单版PDF命令手册](https://www.runoob.com/manual/github-git-cheat-sheet.pdf) ：记录常用的一些命令。
[Git 完整命令手册地址](http://git-scm.com/docs) ：记录了Git的完整命令，更方便查看。
[像 git 那样思考](http://think-like-a-git.net/) Git的学习网站。
[GitHub 帮助](https://docs.github.com/en) GitHub网站的使用文档。
[tryGit](http://try.github.io/)  可在Web上一边操作一边学习Git的基本功能，不过只有英文版。随便输入一些Git命令：
<img src="https://img-blog.csdnimg.cn/20200909004042558.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center" width="75%">

<b></b>
[LearnGitBranching](http://pcottle.github.io/learnGitBranching/)  学习Git基本操作的网站，它的特点在于可以直观看到分支的树形结构，还可以切换各种语言进行学习。上面的知识，我会陆续整理到本文中。
[Pro Git](http://git-scm.com/book/zh/v1) 就职于GitHub的Scott Chacon写的，零基础学习Git的资料，有各种语言版本。


