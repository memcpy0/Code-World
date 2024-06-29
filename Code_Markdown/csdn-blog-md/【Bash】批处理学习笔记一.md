@[toc]

找了几篇批处理的文章学习一下，总结一下。不过这些文章都比较古老，有的不一定能够执行，有的现在也没有多大的实际意义了，我会选择性的总结。

---
# 一、引言
批处理(BATCH)，其文件后缀 `.BAT` 取自前三个字母。它没有固定的构成格式，只需要遵守下面的规定：
- 每一行都可以看做一条命令；
- 每条命令中可以包含多条子命令；
- 从第一条命令开始执行，知道最后一条

批处理运行的平台是DOS。它使用方便、灵活、功能强大，自动化程度高。常常被运维人员使用。

给出以下脚本，将其保存为 `pingsz.bat` 并运行：
```bash
ping sz.tencent.com > a.txt
ping sz1.tencent.com >> a.txt
ping sz2.tencent.com >> a.txt
ping sz3.tencent.com >> a.txt
ping sz4.tencent.com >> a.txt
ping sz5.tencent.com >> a.txt
ping sz6.tencent.com >> a.txt
ping sz7.tencent.com >> a.txt
exit 
```
## 1. 简单脚本——找到速度最快的服务器
我用的是 `Notepad++` 编辑批处理脚本：
<img src="https://img-blog.csdnimg.cn/20200421175456332.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">
<img src="https://img-blog.csdnimg.cn/20200421175820130.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">

这是一个很简单的脚本，但是功能还是比较实用的，它可以测试腾讯服务器的耗时。

执行后会建立一个 `a.txt` 的文件，将 `ping` 得到的信息写入其中，`>` 是写入文件；`>>` 的作用和 `>` 相同，不过更准确的说是“追加”。

最后 `a.txt` 中存储的信息可以帮你判断哪个QQ服务器速度最快。

## 2. 简单脚本——自动清除系统垃圾

```bash
@echo off 
if exist c:\windows\temp. del c:\windows\temp\. 
if exist c:\windows\Tempor~1. del c:\windows\Tempor~1\. 
if exist c:\windows\History. del c:\windows\History\. 
if exist c:\windows\recent. del c:\windows\recent\.
```
上述脚本会删除系统中的垃圾文件。需要注意的是两点：
- DOS不支持长文件名，所以出现了 `Tempor~1` 这个名字；
- 可以自己改动，符合自身的要求。

## 3. 运用批处理的精髓
> 你知道运用批处理的精髓是什么吗？其实很简单：思路要灵活！... 批处理是一个人的天堂，你可以为所欲为，没有达不到的境界！
 
要注意的是，很多常见DOS命令在批处理脚本中有着广泛的应用，它们是批处理脚本的BODY部分，但批处理比DOS更灵活多样。要学好批处理，DOS一定要有扎实的基础。

---
 # 二、开始
```bash
::close echo
@echo off

::clean screen
cls

::display info
echo This programme is to make the MASM programme automate

::display info
echo Edit by CODERED

::display info
echo Mailto me: xxxxxxxxxx@qq.com

::if input without parameter goto usage
if "%1" == "" goto usage
::if parameter is "/?" goto usage
if "%1" == "/?" goto usage
::if parameter is "help" goto usage
if "%1" == "help" goto usage

::pause to see usage
pause

::assemble the .asm code
masm %1.asm

::if error pause to see error msg and edit the code
if errorlevel i pause & edit %1.asm
::else link the .obj file and execute the .ext file
link %1.obj & %1

:usage
::set usage
echoh Usage: This BAT filename [asm file name]
::display usage
echo Default BAT filename is START.BAT
```
- `::` ：注释，在脚本执行时不显示，也不起到任何作用，在批处理脚本中和 `rem` 命令等效；
- `@` ： 这个符号在批处理中很重要，它的作用是：不在执行窗口中显示这一行的命令本身。如果一行命令的行首有了这个符号，这一行的命令就不会显示出来了。 `@echo off`，就是让脚本在执行时不显示后面的 `echo off` 部分。
- `echo` ： `echo` 本身是“回声、回响”的意思，这里做“反馈、回显”之义。这是一个开关命令，有 `on` 和 `off` 两种状态；如果直接执行 `echo` 就会显示当前 `echo` 命令的状态，是 `on` 还是 `off`；
<img src="https://img-blog.csdnimg.cn/20200421193723957.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">
 (1) 执行 `echo off` 将会关闭执行窗口中的显示，**它后面执行的所有命令都不显示命令本身，只显示执行的结果**。`echo on` 则是打开回显。下面是在命令行中的执行结果：
 <img src="https://img-blog.csdnimg.cn/20200421193944931.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">
 当我执行 `echo off` 后，连 `D:\Bash_Projects>` 都不显示了，不过还是要在命令行中输入命令，所以会看到 `ping baidu.com` 。因此，**`echo off` 最好写在批处理文件中**。
 同时要知道的是，`echo off` 只是不显示它后面执行的命令本身，但是 `echo off` 这条命令会被显示出来，这个时候就需要 `@echo off` 了—— 联合起来，达到：不显示 `echo off` 命令本身，也不显示以后的各行命令本身。

	(2) `echo` 除了开关之外，还有一种用法：用它来显示信息！比如脚本的倒数第二行 `Default BAT filename is START.BAT` 将在执行窗口中显示，但是由于前面的 `@echo off` ，`echo` 命令本身不显示；
	
	(3) `echo` 命令的又一种用法——直接编辑文本文件。

- `pause` ：就是“暂停”，这是最简单的一个命令，但是很实用——它的执行效果是：**让当前进程暂停一下，并显示一行你可能很熟悉的信息“请按任意键继续...”**

......
