@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「玩转Git/GitHub」系列文章之一，将随着时间不断修改完善，目的在于为读者提供有关Git/GitHub使用的大部分参考信息。由于内容随时可能发生更新变动，欢迎关注和收藏[玩转Git/GitHub系列文章目录](https://memcpy0.blog.csdn.net/article/details/119684105)以作备忘。

---
# 1. 什么是GitHub
GitHub是**为开发者提供Git仓库的托管服务**、**让开发者与他人共享代码**的完美场所。此外，它还为开发者或团队提供了一系列功能，帮助其高效率、高品质地进行代码编写。

## (1) Git仓库
一般情况下，我们可以免费建立任意个GitHub提供的公开仓库。而且微软收购GitHub后，Github用户也可以**无限免费使用私有仓库**。

## (2) Organization
个人使用只需要个人账户就够了。如果是公司或者交流群、IT小团体进行软件开发，建议使用Organization账户。它的优点在于：能够统一管理账户和权限，统一支付一些费用。

## (3) Issue
Issue功能，是将一个任务或问题分配给一个Issue、进行追踪和管理。在GitHub上，每当我们进行Pull Request的时候，都会创建一个Issue。

每个功能的更改或者修正，都对应一个Issue，讨论都以这个Issue为中心进行。只要查看Issue，就能知道和这个更改相关的一切信息，并对此进行管理。

## (4) Wiki
通过Wiki功能，任何人都能随时对一篇文章进行更改并保存，因此可以多人共同完成一篇文章。该功能常用在**开发文档和使用手册的编写中**。

Wiki页也是作为Git仓库进行管理的，改版的历史记录会被切实保存下来，使用者可以放心改写。

## (5) Pull Request
开发者向GitHub的仓库推送更改或者功能添加后，可以通过Pull Request功能向别人的仓库提出申请，请求对方合并。Pull Request发出后，目标仓库的管理者等人能够查看Pull Request的内容及其中包含的代码更改。

同时，开发者们还可以在Pull Request上进行评论交流，以行为单位对源代码添加评论。通过这个功能，开发者可以轻松更改源代码，并公开更改的细节。

---
# 2. Git的导入和初始配置
Git是由Linus开发的一款功能强大、性能卓越的分散型版本管理系统。**Git的仓库管理功能是GitHub的核心。** 因此，使用GitHub之前需要掌握Git的相关知识，在本地安装Git的环境。

> 版本管理：管理更新的历史记录，例如记录一款软件添加或更改源代码的过程，回滚到特定阶段，恢复误删除的文件等。

## (1) 安装Git
下面在本地环境中实际安装Git，进行各种设置。在Windows环境中，最简单的方法是使用 [`msysGit`](https://gitforwindows.org/) ，按照 `Downloads` 的向导下载安装包。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200906152951618.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
安装包下载完毕后，双击运行，按照向导一步步进行安装即可。下面对安装的设定进行讲解。
- 组件的选择：由于所有必要组件都已经默认勾选，可以直接进入下一步；
- 设置环境变量：设置调用Git的环境变量，由于这里只用到 `msysGit` 中附属的Git Bash，因此选择最上面的 `Use Git Bash Only` ，然后进入下一步；
- 换行符的处理：由于GitHub中公开的代码大部分都以Mac或者Linux中的 `LF(Line Feed)` 换行，而Windows中是以 `CRLF(Carriage Return + Line Feed)` 换行的，因此在非对应的编辑器中将无法正常显示。Git可以通过设置自动转换这些换行符，即使用Windows的用户需要选择推荐的 `Checkout Windows-style, commit Unix-style line endings"` ——换行符在签出时自动转换为 `CRLF` ，在提交时自动转换为 `LF` 。
- `Git Bash` ：顺利安装好 `msysGit` 后，`Git Bash` 作为一个应用程序添加进系统，双击启动，会弹出一个名为 `Git Bash` 的命令提示符。如果按照上述流程，`git` 命令就只能够在 `Git Bash` 中使用，而不能在Windows的命令行中使用。

下载Git后，附带的还有一些工具，比如GnuPG，有时间会介绍一下它。

## (2) 初始设置
下面对本机中安装的Git进行设置。首先设置使用Git时的**姓名和邮箱地址**，用英文输入。由于在GitHub上公开仓库时，这里的姓名和邮箱地址也会随着**Git的提交日志**一起被公开，所以需要使用便于公开的信息：
```bash
$ git config --global user.name "your name"
$ git config --global user.email "your email"
```
如果将 `color.ui` 设置为 `auto` ，就可以让命令的输出拥有更高的可读性：
```bash
$ git config --global color.ui auto
```
注意，上述 `git config` 命令会在 `~/.gitconfig` 这一文件中以如下的形式输出设置文件：
```cpp
[user]
  name = your name
  email = your_email@example.com
[color]
  ui = auto 
```
想要更改这些信息时，可以用命令进行设置，或者直接编辑这个设置文件。另外，如果想要查看全局变量有哪些，可以输入：
```bash
$ git config --global -l
filter.lfs.clean=git-lfs clean -- %f
filter.lfs.smudge=git-lfs smudge -- %f
filter.lfs.process=git-lfs filter-process
filter.lfs.required=true
user.name=memcpy0
user.email=xxxxxxxx
color.ui=auto
pack.windowmemory=100m
pack.sizelimit=100m
pack.threads=1
pack.window=0
```

---
# 3. 使用GitHub的准备工作
下面将讲述使用GitHub前的一些准备工作。
## (1) 创建账户
打开[创建账户的页面](https://github.com/join)，会看到如下的画面：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200906160505171.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
在 `username` 中用英文和数字输入ID，需要注意的是，个人公开页面的URL `(https://github.com/xxx)` 会使用到这个ID。其他项目按照要求输入即可。

填写完这些项目后，点击 `Create account` ，就完成了账户的创建。之后直接进入登录状态，用户可以立即开始使用GitHub，设置头像和具体信息。

## (2) 设置SSH Key
GitHub上连接已有仓库时，需要通过**使用了SSH的公共密钥**进行认证。现在来创建公共密钥认证所需的SSH Key，并将其添加至GitHub。如果已经创建过，就使用现有的密钥进行设置。

运行下面的命令创建SSH Key：
```bash
$ ssh-keygen -t rsa -C "创建GitHub账户时用的邮箱地址"
Enter file in which to save the key (/c/Users/dell/.ssh/id_rsa): (按回车键)
Enter passphrase (empty for no passphrase): (输入密码)
Enter same passphrase again: (输入密码)
```
输入密码后出现如下的结果：
```sql
Your identification has been saved in /c/Users/your_user_directory/.ssh/id_rsa.
Your public key has been saved in /c/Users/your_user_directory/.ssh/id_rsa.pub.
The key fingerprint is:
fingerprint值 your_email@example.com
The key's randomart image is:
略
```
密钥都被保存在文件中：`id_rsa` 是私有密钥，`id_rsa.pub` 是公开密钥。注意，要牢记SSH Key的密码，如果忘记了可是无法找回的，只能够再次运行上述命令，覆盖之前生成的SSH密钥。

## (3) 添加公开密钥
在GitHub中添加公开密钥，之后就可以用私有密钥进行认证。我们点击右上角的 `Account Settings` ，点击 `new SSH Key` 。在 `Title` 中输入适当的密钥名称，`Key` 部分复制粘贴 `id_rsa.pub` 文件里的内容——用如下方法查看 `id_rsa.pub` 的内容：
```bash
$ cat ~/.ssh/id_rsa.pub
ssh-rsa 公开密钥的内容 your_email@example.com
```
添加成功后，创建账户所用到的邮箱会收到一封提示“公共密钥添加完成”的邮件。此时，我们可以用手中的私人密钥与GitHub进行认证和通信：
```bash
$ ssh -T git@github.com
The authenticity of host 'github.com (192.30.255.113)' can't be established.
RSA key fingerprint is fingerprint值.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added 'github.com,192.30.255.113' (RSA) to the list of known hosts.
Enter passphrase for key '/your_user_directory/.ssh/id_rsa':
Hi memcpy0! You've successfully authenticated, but GitHub does not provide shell access.
```
出现上述的结果，即为成功。

## (4) 使用社区功能
我们可以试试 `Follow` 别人，在用户信息页面的右上角点击 `Follow` 按钮即可。此后 `Follow` 的用户的活动就会显示在个人的控制面板页面中，我们可以通过这个方式来了解他人在GitHub上的活动。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200906165257240.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)


如果要获得仓库的最新开发信息，可以使用 `Watch` 功能。

---
# 4. 实际动手使用Git与GitHub
## (1) 创建仓库

我们动手创建一个公开的仓库。点击工具栏中的 `New repository` 图标，创建新的仓库，弹出如下界面：

![在这里插入图片描述](https://img-blog.csdnimg.cn/202009061658091.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
我们进行输入：
- 在 `Repository name` 中输入仓库的名称。这里输入 `Hello-World` ；
- 在 `Description` 中设置仓库的说明，可以留空；
- 在 `Public、Private` 这一栏中，选择公开还是私有。如果创建公开仓库，仓库的所有内容都会被公开；如果是私有仓库，用户可以设置访问权限；
- `Initialize this repository with` ：
   - 如果在 `Add a README file` 上打钩，GitHub会自动初始化仓库并设置 `README` 文件，**让用户可以立刻 `clone` 这个仓库**；**想要向GitHub添加手中已有的Git仓库，就不用勾选，直接手动 `push` 即可**；
   - 如果在 `Add .gitignore` 上打钩，就会帮我们把不需要在Git仓库中进行版本管理的文件和目录记录在 `.gitignore` 文件中，省去每次根据框架进行设置的麻烦。 
   - 如果在 `Choose a license` 上打钩，就要根据仓库代码确定的许可协议，选择要添加的许可协议文件。随后将自动生成包含许可协议内容的 `LICENSE` 文件，它将被用来表明该仓库内容的许可协议。

最后，点击 `Create repository` 按钮，完成仓库的创建。

## (2) 连接仓库
我们刚刚创建了一个 `Hello-World` 仓库，这个URL是它的页面：`https://github.com/用户名/helloworld` 。其中，（如果勾选则）`README.md` 在初始化时已经生成，**它的内容会自动显示在仓库的首页当中**。因此，开发者一般在这个文件中，使用Markdown语法描述本仓库所包含的软件的概要、使用流程、许可协议等信息。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200906173104345.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

要注意的是：GitHub上进行交流时用到的Issue、评论、Wiki，都可以用 `(GitHub Flavored) Markdown` 语法进行表述。虽然GFM在Markdown上面做了扩充，但是一般来说按照原本的Markdown语法进行书写即可。


## (3) 公开代码
### ① clone已有仓库
我们将在已有仓库中添加代码并加以公开。首先将已有的仓库 `clone` 到开发环境中：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200906172916766.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

```bash
$ git clone https://github.com/memcpy0/helloworld.git
Cloning into 'helloworld'...
remote: Enumerating objects: 8, done.
remote: Counting objects: 100% (8/8), done.
remote: Compressing objects: 100% (7/7), done.
remote: Total 8 (delta 1), reused 3 (delta 0), pack-reused 0
Unpacking objects: 100% (8/8), done.
```
如果是第一次 `clone` ，会要求输入GitHub上设置的公开密钥的密码。认证成功后，仓库就会被 `clone` 到仓库名后的目录中。**将要公开的代码提交到这个仓库，再 `push` 到 `GitHub` 的仓库中，代码就会被公开**。

我们编写一个简单的 `hello_github.cpp` 文件，用来输出 `"Hello GitHub"` ：
```cpp
#include <iostream>
using namespace std;

int main() {
	cout << "Hello GitHub" << endl;
	return 0;
}
```
进入到仓库的目录下，由于 `hello_github.cpp` 没有添加到Git仓库，所以 `git status` 会显示它为 `Untracked files` ：
```bash
$ git status
On branch master
Your branch is up to date with 'origin/master'.

Untracked files:
  (use "git add <file>..." to include in what will be committed)

        hello_github.cpp

nothing added to commit but untracked files present (use "git add" to track)
```
### ② 加入暂存区和提交文件
将 `hello_github.cpp` 提交到Git仓库中。此后，这个文件将进入版本管理系统的管理之下，之后的更改管理都由Git进行：
```bash
$ git add hello_github.cpp
$ git commit -m "Add hello github script by cpp"
[master 29cfdfe] Add hello github script by cpp
 1 file changed, 7 insertions(+)
 create mode 100644 hello_github.cpp
```
上面的 `git add` 命令将文件加入暂存区（它记录文件提交之前的状态），再通过 `git commit` 命令提交文件，这里 `-m, --message <message>` 会提交对应的描述信息。

添加、提交之后，可以通过 `git log` 命令查看提交日志：
```bash
$ git log
commit 29cfdfef01b9c255cdee257dccc5da501f16b01c (HEAD -> master)
Author: memcpy0 <2183927003@qq.com>
Date:   Sun Sep 6 17:46:30 2020 +0800

    Add hello github script by cpp
```
### ③ push更新仓库
之后只要执行 `push`，GitHub上的对应仓库就会被更新：
```bash
$ git push
Username for 'https://github.com': memcpy0
Counting objects: 3, done.
Writing objects: 100% (3/3), 414 bytes | 103.00 KiB/s, done.
Total 3 (delta 0), reused 0 (delta 0)
To https://github.com/memcpy0/helloworld.git
   f7c5dd1..29cfdfe  master -> master
```
此时，代码就在GitHub上公开了。到这里为止，我们才算正式踏入GitHub的世界。

---

# 5. 示例：创建和修改图片仓库
本节示范创建一个空仓库后，**在本地创建一个新仓库**，在本地添加修改然后提交到GitHub。以及删除本地仓库后，对于GitHub上已有的这个非空仓库， `git clone` 下来，在本地添加修改然后提交到GitHub。

第一步是动手创建一个公开的仓库。点击工具栏中的 `New repository` 图标，创建新的空仓库，进入如下界面：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021070200283595.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
第二步，由于GitHub上是新建的空仓库，打开Git Bash，运行如下命令，在本地个人代码库 `C:/CodeWorld/Other_Projects` 中，建立一个 `ImagesReposiroty` 文件夹（库）：
```bash
$ cd C:/CodeWorld/Other_Projects
$ mkdir ImagesRepository
$ cd Images* 
$ echo "# ImagesRepository" >> README.md 
$ echo "**This is my repository of images.**" >> README.md 
$ cat README.md
# ImagesRepository
**This is my repository of images.** 
```
然后是初始化仓库、提交文件、推送仓库等等：
```bash 
.../ImagesRepository (master) 
$ git init
Initialized empty Git repository in C:/CodeWorld/Other_Projects/ImagesRepository/.git/ 
 
.../ImagesRepository (master) 
$ git status
On branch master

No commits yet

Untracked files:
  (use "git add <file>..." to include in what will be committed)
        README.md

nothing added to commit but untracked files present (use "git add" to track)
 
.../ImagesRepository (master)  
$ git add README.md
warning: LF will be replaced by CRLF in README.md.
The file will have its original line endings in your working directory
  
.../ImagesRepository (master) 
$ git status
On branch master

No commits yet

Changes to be committed:
  (use "git rm --cached <file>..." to unstage)
        new file:   README.md

.../ImagesRepository (master) 
$ git status # 复制几个图片文件到这个文件夹
On branch master

No commits yet

Changes to be committed:
  (use "git rm --cached <file>..." to unstage)
        new file:   README.md

Untracked files:
  (use "git add <file>..." to include in what will be committed)
        1.jpg
        2.jpg
        3.jpg
 
.../ImagesRepository (master)  
$ git add 1.jpg 2.jpg 3.jpg 

.../ImagesRepository (master) 
$ git status
On branch master

No commits yet

Changes to be committed:
  (use "git rm --cached <file>..." to unstage)
        new file:   1.jpg
        new file:   2.jpg
        new file:   3.jpg
        new file:   README.md
 
.../ImagesRepository (master) 
$ git commit -m "first commit with 3 miku images" # 提交暂存区的文件
[master (root-commit) 9797ada] first commit with 3 miku images
 4 files changed, 2 insertions(+)
 create mode 100644 1.jpg
 create mode 100644 2.jpg
 create mode 100644 3.jpg
 create mode 100644 README.md
 
.../ImagesRepository (master) 
$ git status
On branch master
nothing to commit, working tree clean

.../ImagesRepository (master) 
$ git branch
* master
 
.../ImagesRepository (master) 
$ git branch -l
* master

.../ImagesRepository (master) 
$ git branch -M main # 修改当前分支的名字
 
.../ImagesRepository (main)
$ git remote add origin https://github.com/memcpy0/ImagesRepository.git
  
.../ImagesRepository (main)
$ git push -u origin main # 连接报错
fatal: unable to access 'https://github.com/memcpy0/ImagesRepository.git/': Failed to connect to github.com port 443: Timed out
 
.../ImagesRepository (main) 
$ git config --global --unset https.proxy # 修改HTTPS代理设置
  
.../ImagesRepository (main) 
$ git push -u origin main
fatal: helper error (-1): ����һ����������
Enumerating objects: 6, done.
Counting objects: 100% (6/6), done.
Delta compression using up to 4 threads
Compressing objects: 100% (6/6), done.
Writing objects: 100% (6/6), 1.52 MiB | 1.39 MiB/s, done.
Total 6 (delta 0), reused 0 (delta 0), pack-reused 0
To https://github.com/memcpy0/ImagesRepository.git
 * [new branch]      main -> main
Branch 'main' set up to track remote branch 'main' from 'origin'. 
```
如果想修改这个仓库，而原先的本地仓库已经被删除，可以 `git clone` 下来，进行修改然后推送：
```bash
... /c/CodeWorld/Other_Projects
$ git clone git://github.com/memcpy0/ImagesRepository.git
Cloning into 'ImagesRepository'...
remote: Enumerating objects: 6, done.
remote: Counting objects: 100% (6/6), done.
remote: Compressing objects: 100% (6/6), done.
remote: Total 6 (delta 0), reused 6 (delta 0), pack-reused 0
Receiving objects: 100% (6/6), 1.52 MiB | 273.00 KiB/s, done.

... /c/CodeWorld/Other_Projects
$ cd Images*

... /c/CodeWorld/Other_Projects/ImagesRepository (main)
$ git status
On branch main
Your branch is up to date with 'origin/main'.

nothing to commit, working tree clean

... /c/CodeWorld/Other_Projects/ImagesRepository (main) 
$ git status # 加入了一个图片文件,然后查看状态
On branch main
Your branch is up to date with 'origin/main'.

Untracked files:
  (use "git add <file>..." to include in what will be committed)
        4.jpg

nothing added to commit but untracked files present (use "git add" to track)

... /c/CodeWorld/Other_Projects/ImagesRepository (main)
$ git add 4.jpg

... /c/CodeWorld/Other_Projects/ImagesRepository (main)
$ git commit -m "commit another image of Itsuka Shiori" # 提交了一张五河士织的图片
[main d90a768] commit another image of Itsuka Shiori
 1 file changed, 0 insertions(+), 0 deletions(-)
 create mode 100644 4.jpg

... /c/CodeWorld/Other_Projects/ImagesRepository (main)
$ git log # 查看仓库日志
commit d90a76844440033d33319c93f3f3d45cce219271 (HEAD -> main)
Author: memcpy0 <2183927003@qq.com>
Date:   Fri Jul 2 12:16:27 2021 +0800

    commit another image of Itsuka Shiori

commit 9797ada9623a3d29327d1447f4f1a43a4a344b7e (origin/main, origin/HEAD)
Author: memcpy0 <2183927003@qq.com>
Date:   Fri Jul 2 10:11:24 2021 +0800

    first commit with 3 miku images

... /c/CodeWorld/Other_Projects/ImagesRepository (main)
$ git push -u origin main # 注意使用git://...下载后,提交时会出现的错误
fatal: remote error:
  You cant push to git://github.com/memcpy0/ImagesRepository.git
  Use https://github.com/memcpy0/ImagesRepository.git

... /c/CodeWorld/Other_Projects/ImagesRepository (main)
$ git remote remove origin 

... /c/CodeWorld/Other_Projects/ImagesRepository (main)
$ git remote add origin https://github.com/memcpy0 /ImagesRepository.git # 先移除然后重新添加远端仓库的URL

... /c/CodeWorld/Other_Projects/ImagesRepository (main)
$ git push -u origin main
Enumerating objects: 4, done.
Counting objects: 100% (4/4), done.
Delta compression using up to 4 threads
Compressing objects: 100% (3/3), done.
Writing objects: 100% (3/3), 884.23 KiB | 8.42 MiB/s, done.
Total 3 (delta 1), reused 0 (delta 0), pack-reused 0
remote: Resolving deltas: 100% (1/1), completed with 1 local object.
To https://github.com/memcpy0/ImagesRepository.git
   9797ada..d90a768  main -> main
Branch 'main' set up to track remote branch 'main' from 'origin'.
```
