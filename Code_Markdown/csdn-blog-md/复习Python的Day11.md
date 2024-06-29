# 复习Python的第十一天
[TOC]

![](http://img1.imgtn.bdimg.com/it/u=624056963,3598669208&fm=11&gp=0.jpg)

## 习题 12: 提示别人 
> 对于 raw_input而言，你还可以让它显示出一个提示，从而告诉别人应该输入什么东西。你可以在 () 之间放入一个你想要作为提示的字符。

这就是提示别人了。

如下：
```python
>>> y = raw_input("Name? ") 
```
这句话会用 “Name?” 提示用户，然后将用户输入的结果赋值给变量 y。这就是 我们提问用户并且得到答案的方式。 

因此，本节的内容是将上节的代码重写了一遍。

> 习题代码

```python
>>> # 询问年龄
>>> age = raw_input("How old are you?")
>>> # 身高
>>> height = raw_input("How tall are you?")
>>> # 体重
>>> weight = raw_input("How much do you weigh?")
>>> # 打印
>>> print "So,you're %r old,%r tall and %r heavy." % (age,height,weight)

How old are you?11
How tall are you?11
How much do you weigh?6'2"
So,you're '11' old,'11' tall and '6\'2"' heavy.
```
而后我试了一下，发现**在raw_input中可以使用格式化字符。**
```python
>>> name = "myRealization"
>>> age = raw_input("Hi, %s.Your age is\n" % name)

Hi, myRealization.Your age is
18
```
也许，是因为raw_input()先打印出提示，因此在这时和print的格式可以是一样的吧。

我觉得这样做的用法，可以先创建变量，再输出含有变量的提示，输出内容随着变量内容变化，**会方便很多**：
```python
>>> name = raw_input("Name?")  # "myRealization"
>>> age = raw_input("How old are you, %s?\n" % name)
>>> print "Hi, %s.Your age is %s" % (name, age)
```

## PYDOC初探

在命令行界面下运行你的程序，然后在命令行输入pydoc raw_input 看它说了 些什么。如果你用的是 Window，那就试一下 python -m pydoc raw_input 。

![](http://img.blog.csdn.net/20180327192630467?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 注：Windows中现在可以用`pydoc+命令名`来查看语言帮助。

然后是：
2. 输入q退出 pydoc。  
q退出`More？`的询问。
3. 上网找一下 pydoc命令是用来做什么的。
pydoc — Documentation generator and online help system   文档生成与网上帮助系统
上面的英文描述多么准确和简练。

>在学习任何一门语言的时候，我们都离不开参考资料、帮助文档，因为现在语言发展的越来越完善，引进的built-in命令越来越多，**要想把它们全部记忆是非常不可能的事情**。开发人员往往会集中在某个领域，对该领域中的语言语法细节也许擅长，但偶尔也需要使用其他领域的语法细节，这时就需要参考资料了。

>现在的搜索引擎这么发达，为什么还要使用Python自带的帮助系统呢？因为：
1）利用搜索引擎搜出的结果往往太宽泛，不聚焦，一些论坛和blog给出的结论往往会针对一些过时的版本，但又不指明，有的还有一些错误，这样就增加了解决问题的时间；
2）利用搜索引擎需要联网，自带的就非常方便，随时都陪在你的身边；
3）自带的帮助系统给出的结果可靠，准确和版本一致。基于以上的原因，我们仍然需要掌握自带帮助系统的使用方法。

关于pydoc的用法我会在探究后努力总结一下的。
4. 使用pydoc再看一下 open, file, os, 和 sys 的含义。看不懂没关系，只要通读 一下，记下你觉得有意思的点就行了。
**输入open：**
```python
Help on built-in function open in module io:

open(file, mode='r', buffering=-1, encoding=None, errors=None, newline=None, closefd=True, opener=None)
    Open file and return a stream.  Raise IOError upon failure.

    file is either a text or byte string giving the name (and the path
    if the file isn't in the current working directory) of the file to
    be opened or an integer file descriptor of the file to be
    wrapped. (If a file descriptor is given, it is closed when the
    returned I/O object is closed, unless closefd is set to False.)

    mode is an optional string that specifies the mode in which the file
    is opened. It defaults to 'r' which means open for reading in text
    mode.  Other common values are 'w' for writing (truncating the file if
    it already exists), 'x' for creating and writing to a new file, and
    'a' for appending (which on some Unix systems, means that all writes
    append to the end of the file regardless of the current seek position).
    In text mode, if encoding is not specified the encoding used is platform
    dependent: locale.getpreferredencoding(False) is called to get the
    current locale encoding. (For reading and writing raw bytes use binary
    mode and leave encoding unspecified.) The available modes are:

    ========= ===============================================================
    Character Meaning
    --------- ---------------------------------------------------------------
    'r'       open for reading (default)
    'w'       open for writing, truncating the file first
    'x'       create a new file and open it for writing
    'a'       open for writing, appending to the end of the file if it exists
-- More  --
```
**输入os：**
```
python
Help on module os:
NAME
    os - OS routines for NT or Posix depending on what system we're on.
DESCRIPTION
    This exports:
      all functions from posix or nt, e.g. unlink, stat, etc.
      os.path is either posixpath or ntpath
      os.name is either 'posix' or 'nt'
      os.curdir is a string representing the current directory (always '.')
      os.pardir is a string representing the parent directory (always '..')
      os.sep is the (or a most common) pathname separator ('/' or '\\')
      os.extsep is the extension separator (always '.')
      os.altsep is the alternate pathname separator (None or '/')
      os.pathsep is the component separator used in $PATH etc
      os.linesep is the line separator in text files ('\r' or '\n' or '\r\n')
      os.defpath is the default search path for executables
      os.devnull is the file path of the null device ('/dev/null', etc.)
Programs that import and use 'os' stand a better chance of being
    portable between different platforms.  Of course, they must then
    only use functions that are defined by all platforms (e.g., unlink
    and opendir), and leave all pathname manipulation to os.path
    (e.g., split and join).
```

很简洁明了的讲述，和直接看官方文档比起来各有千秋，但这种可能更舒服。

其它的用法日后再说吧。

总之，知道pydoc的我……

![](http://img3.imgtn.bdimg.com/it/u=4232769941,1845050132&fm=11&gp=0.jpg)