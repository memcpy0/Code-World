@[toc]

---
# 1. Chocolatey是什么？
一言以概之，Chocolaty就是一款专为Windows系统开发的、基于NuGet的包管理器工具，是Windows下的npm（Node.js）、brew（MacOS）、yum或apt-get（Linux），是一个集中打包和分发软件的包管理工具，使用它可以快速下载安装、管理、卸载应用程序和工具。 

# 2. 为什么要使用Chocolatey？
如果你更习惯Unix/Linux环境，怀念在*nux中安装程序的快捷和高效，而不是：
- 去Python官网下载CPython；
- 去Ruby官网下载Ruby；
- 去Chrome官网下载Chrome；
- ……

又或者你想要管理一些小众、轻量的开源软件，Chocolatey就很适合你。目前它有8533个社区维护软件包：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718170132660.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

Chocolatey更大的优势在于，某些软件安装网站经常修改原来的安装包，夹带自己的广告，搞全家桶流氓安装，比如某华军软件园。但是Chocolatey完全使用官网链接下载，还会在下载完成后检查安装包是否和官网一致，这样一来，我们就可以享受到最新、纯净、官网版本的应用程序。

# 3. 如何下载Chocolatey？
Chocolatey是用来从命令行安装软件的，总不能让我们再下个安装包来安装Chocolatey吧？我们可以看一下它的[安装说明](https://chocolatey.org/install)：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718170723153.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

要求我们有Windows 7+、PowerShell v2+、.NET Framework 4+。这些条件应该不会有人不满足吧？然后在管理员身份下运行PowerShell，`Get-ExecutionPolicy` 查看执行策略，`Set-ExecutionPolicy AllSigned` 修改执行策略——或者直接在PowerShell中运行下面的代码：
```shell
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
```
没有报错就可以使用Chocolatey了，通过 `choco` 或 `choco -?` 查看帮助。 PowerShell中，本人的执行过程如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718173334199.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
# 4. 如何使用Chocolatey？
Chocolatey的命令很简单，以 `choco` 开头，告诉系统使用 `Chocolatey` ，然后用 `install` 表明要安装程序，最后跟上需要安装的程序名称。如果添加 `--yes` ，就意味着对Chocolatey安装过程的认可；如果不添加这个参数，Chocolatey就会在安装的每一个步骤前，停下来询问是否继续安装。另外，Chocolatey还可以一键安装多个软件，只需要在 `choco install` 后面多写几个软件名即可。使用Chocolatey卸载软件也非常简单，将 `install` 换成 `uninstall` 即可，比如 `choco uninstall` 。

下面还列出了Chocolatey命令集合，还可以在命令行中使用 `choco command -help` 进一步查看命令帮助，用 `choco list --local-only` 查看本地已经安装的软件，用 `choco upgrade all` 一键更新所有已安装的软件：
```handlebars
Commands

 * list - lists remote or local packages
 * find - searches remote or local packages (alias for search)
 * search - searches remote or local packages (alias for list)
 * info - retrieves package information. Shorthand for choco search pkgname --exact --verbose
 * install - installs packages from various sources
 * pin - suppress upgrades for a package
 * outdated - retrieves packages that are outdated. Similar to upgrade all --noop
 * upgrade - upgrades packages from various sources
 * uninstall - uninstalls a package
 * pack - packages up a nuspec to a compiled nupkg
 * push - pushes a compiled nupkg
 * new - generates files necessary for a chocolatey package from a template
 * sources - view and configure default sources (alias for source)
 * source - view and configure default sources
 * config - Retrieve and configure config file settings
 * feature - view and configure choco features
 * features - view and configure choco features (alias for feature)
 * setapikey - retrieves, saves or deletes an apikey for a particular source (alias for apikey)
 * apikey - retrieves, saves or deletes an apikey for a particular source
 * unpackself - have chocolatey set itself up
 * version - [DEPRECATED] will be removed in v1 - use `choco outdated` or `cup <pkg|all> -whatif` instead
 * update - [DEPRECATED] RESERVED for future use (you are looking for upgrade, these are not the droids you are looking for) 
```

此外，软件名称和软件包名称可能不一样，推荐去[官方软件列表](https://community.chocolatey.org/packages)搜到名字再进行安装。如果想试试Chocolatey的图形界面，可以安装 `choco install chocolateygui` 。

---
# 5. 示例一：使用Chocolatey下载Ditto（剪贴板增强工具神器）
关于Ditto的使用，可以看这篇文章，介绍得很完整：[Ditto - 开源免费的 Windows 剪贴板增强工具神器 (方便复制粘贴多条历史记录)](https://www.iplaysoft.com/ditto.html)。在VS Code的Git Bash终端/命令提示符/Windows PowerShell中运行 `choco install ditto` ，结果均发现报错——**对路径“C:\ProgramData\chocolatey\\.chocolatey”的访问被拒绝**。Chocolatey的提示如下，要我们使用 `elevated shell` 即以管理员身份运行，或者指定其他有权限写入的文件夹：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718180411912.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

为了方便，我选择了在管理员身份下的命令提示符中运行：
```bash
C:\WINDOWS\system32>choco install ditto
Chocolatey v0.10.15
Installing the following packages:
ditto
By installing you accept licenses for the packages.

ditto v3.23.124.0 [Approved]
ditto package files install completed. Performing other installation steps.
The package ditto wants to run 'chocolateyInstall.ps1'.
Note: If you don't run this script, the installation will fail.
Note: To confirm automatically next time, use '-y' or consider:
choco feature enable -n allowGlobalConfirmation
Do you want to run the script?([Y]es/[A]ll - yes to all/[N]o/[P]rint): Y

Installing 64-bit ditto...
ditto has been installed.
  ditto can be automatically uninstalled.
Environment Vars (like PATH) have changed. Close/reopen your shell to
 see the changes (or in powershell/cmd.exe just type `refreshenv`).
 The install of ditto was successful.
  Software installed to 'C:\Program Files\Ditto\'

Chocolatey installed 1/1 packages.
 See the log for details (C:\ProgramData\chocolatey\logs\chocolatey.log).
```
现在可以使用Ditto了，按下**Ctrl+`**，查看多个复制记录（随手复制的）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718182248952.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

 
# 6. 示例二：使用Chocolatey下载Screen To Gif（录制Gif动画的工具）
有了ScreenToGif，就可以很轻松的展示某些操作，比如安装软件、介绍功能等等。我们可以在[“Chocolatey市场”](https://community.chocolatey.org/packages)找到这个软件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718182754190.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
然后运行 `choco install screentogif` 即可完成安装。下面简单录制一个Gif：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718183423480.gif)

 
# 7. 示例三：使用Chocolatey下载FastStone Capture（滚动截屏）
在Chocolatey市场搜索到这个软件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/78f3c8ee2cb647feb3096a0dcf5c5c9c.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

然后在管理员模式下运行命令 `choco install fscapture` 。注意，安装的FastStone Capture是30天试用的，要长期使用需要掏钱买。嘛，总之，现在可以用 `Ctrl+Alt+PrtScn` 滚动截屏了。

 
# 8. 示例四：使用Chocolatey下载Treesize Free （整理文件）
要查找和整理大文件，可以安装Treesize Free，这个软件用树或者色块大小显示文件夹在硬盘中的占用。在Chocolatey市场搜索到这个软件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/86d9dee0b3024c6aa8f05e7b2632a2f1.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
然后用 `choco install treesizefree` 进行安装，使用这个软件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/c451d78a30524ed38bdd7eaa6ef07100.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)



---
# 9. 其他好用软件推荐

文件搜索和定位有：`choco install listary` ，Listary可以像Spotlight一样可以找文件、开程序、自定义快捷操作。 

压缩文件管理上： `choco install 7zip` 安装7Zip； ` choco install peazip` 安装PeaZip，图形界面更好看。

文件清理软件：`choco install ccleaner` 安装CCleaner。

软件删除器：`choco install adwcleaner` 安装AdwCleaner，用于删除流氓软件。

FTP软件：`choco install filezilla` ， 还可以 `choco install filezilla.server` 安装它的Server，用移动设备播放电脑上的电影。 

视频播放软件：`choco install mpv` 安装mpv。
 
视频剪辑软件：`choco install shotcut` 安装Shotcut，用于简单的视频剪辑。
 
翻译软件：`choco install qtranslate` 安装QTranslate，轻量翻译软件，遇上生词可以翻译。
