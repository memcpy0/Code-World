`TT` 是一款古老的运行在DOSBox下的打字软件，大约在1995年以前流行。我就经常用它来打字。

不过今天打开的时候，发现它说，“找不到DOSBox的位置”之类的。emmmm，想了一下才想起，我把CMD打开的默认路径改了，应该是这个原因。

我不得已打开了 `放大版TT.BAT`，发现内容很简单，虽然我不太了解 **Bash** ：

```bash
@ECHO OFF
cd DOSBOX
start dosbox -CONF dosbox3.conf -NOCONSOLE
```
查了一下：
- 其中，`@ECHO OFF` 是禁止回显的意思；
- `cd` 都知道；
- `start` 启动指定程序，后面的几个选项不太看得懂；

我把路径改了一下，就能够运行了。

```bash
@ECHO OFF
cd D:\Program Files (x86)\TT\DOSBOX
start dosbox -CONF dosbox3.conf -NOCONSOLE
```
虽然TT已经很古老了，但是用来训练打字效果还是很不错的。

突然有点想学一下 `bash` 了，以后玩DOXBox游戏说不定能够用到呢？

<img src="https://img-blog.csdnimg.cn/20200421173106142.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">
