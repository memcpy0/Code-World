# 复习Python的第十六天
[TOC]

![](http://img3.imgtn.bdimg.com/it/u=4279947575,3011415842&fm=27&gp=0.jpg)

## 习题 17: 更多文件操作（复制文件到另一个文件） 
下边的习题是将原先存在的文本文件内容复制到一个新的文本文件中。

```python2
from sys import argv 
from os.path import exists  
'''
import exists引入os模块中的exists命令，作用是将文件名字符串作为参数，如果文件存在，返回True，否则返回False;也可以判断路径是否存在
'''
script, from_file, to_file = argv 
 
print "Copying from %s to %s" % (from_file, to_file) 
 
# we could do these two on one line too, how? 
# 以只读模式打开文件，从而可以使用read()命令
input = open(from_file) 
#使用read函数读取文件内容，并将文件内容赋值给indata
indata = input.read() 

# 打印文本字节数 
print "The input file is %d bytes long" % len(indata) 

# 输出的文件是否存在 
print "Does the output file exist? %r" % exists(to_file) 
print "Ready, hit RETURN to continue, CTRL-C to abort." 
raw_input() 
 
output = open(to_file, 'w') 
output.write(indata) 
print "Alright, all done." 
 
output.close() 
input.close() 
```
 
 
## 加分习题： 
1 这个脚本实在是有点烦人。没必要在拷贝之前问一遍把，没必要在屏幕上输出那么多东西。试着删掉脚本的一些功能，让它使用起来更加友好。 
```python2
# 友好版
from sys import argv
script, from_file, to_file = argv 

input = open(from_file) 
indata = input.read()
output = open(to_file, 'w') 
output.write(indata) 

output.close() 
input.close() 
```
```python2
# 简写版
from sys import argv
script, from_file, to_file = argv
output = open(to_file, 'w')
output.write(open(from_file).read())
```
一行我做不到，除非使用Windows下的copy语法。

2 我使用了一个叫cat的东西，这个古老的命令的用处是将两个文件“连接 (con*cat*enate)”到一起，不过实际上它最大的用途是打印文件内容到屏幕上。你可以通过 man cat 命令了解到更多信息。使用 Windows的同学，你们可以给自己找一个 cat 的替代品。关于man的东西 就别想太多了，Windows下没这个命令。

> cat是一个很常用的命令。通常查看文本内容的时候都会使用cat，它能够展示文本信息的同时还仅仅是对文件的读取操作，并不会改变文件的内容。
> 鉴于有时候有些情况下有些公司禁止员工自由使用开发工具并且只能够使用Windows，所以Windows下还是有替代者的，即为type。

在cmd命令行中，type的使用帮助： `help type`，它的作用是显示文本文件的内容。具体参数如下：

`TYPE[drive:][path]filename`

![这里写图片描述](http://img.blog.csdn.net/20180401103559736?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可以说是说明十分简洁，使用也十分简单。使用示范如下：

![](http://img.blog.csdn.net/20180401103729644?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因为是在当前工作目录下，所以不加路径，否则要加文件路径。

显示我们知道用的是Type，而复制的话是copy。可能暂且不会，多用用就可以了。
```python
C:\Users\dell\Desktop>help copy
将一份或多份文件复制到另一个位置。

COPY [/D] [/V] [/N] [/Y | /-Y] [/Z] [/L] [/A | /B ] source [/A | /B]
     [+ source [/A | /B] [+ ...]] [destination [/A | /B]]

  source       指定要复制的文件。
  /A           表示一个 ASCII 文本文件。
  /B           表示一个二进位文件。
  /D           允许解密要创建的目标文件
  destination  为新文件指定目录和/或文件名。
  /V           验证新文件写入是否正确。
  /N           复制带有非 8dot3 名称的文件时，
               尽可能使用短文件名。
  /Y           不使用确认是否要覆盖现有目标文件
               的提示。
  /-Y          使用确认是否要覆盖现有目标文件
               的提示。
  /Z           用可重新启动模式复制已联网的文件。
/L           如果源是符号链接，请将链接复制
               到目标而不是源链接指向的实际文件。

命令行开关 /Y 可以在 COPYCMD 环境变量中预先设定。这可能会被命令行上的 /-Y 替代。除非 COPY
命令是在一个批处理脚本中执行的，默认值应为在覆盖时进行提示。

要附加文件，请为目标指定一个文件，为源指定数个文件(用通配符或 file1+file2+file3 格式)。
```

关于Windows下的shell语法，以后还可以了解一下。

## 本节补充：OS.path模块
查看关于os.path的文档：`pydoc os.path`
```python2
exists(path)
        Test whether a path exists.  Returns False for broken symbolic links
```
相似的有：
```python2
lexists(path)
        Test whether a path exists.  Returns True for broken symbolic links
```
[这个链接](https://blog.csdn.net/u013232456/article/details/78738862)讲的很仔细，自己看一下吧。

![](http://img0.imgtn.bdimg.com/it/u=783754584,2833957392&fm=27&gp=0.jpg)