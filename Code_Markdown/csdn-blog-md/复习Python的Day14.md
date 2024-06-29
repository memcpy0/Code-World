# 复习Python的第十四天
[TOC]
## 习题 15: 读取文件
> 你已经学过了raw_input 和 argv，这些是你开始学习读取文件的必备基础。你可能需要多多实验才能明白它的工作原理，所以你要细心做练习，并且仔细检查结果。

本节目的：
> 我们要做的是把该文件用我们的脚本“打开(open)”，然后打印出来。
> 然而把文件名 ex15_sample.txt 写死(hardcode)在代码中不是一个好主意，这些信息应该 是用户输入的才对。
> 如果我们碰到其他文件要处理，写死的文件名就会给你带来麻烦了。
> 我们的解决方案是**使用argv和raw_input来从用户获取信息**，从而知道哪些文件该被处理。

对的，硬编码是个很烦的事情，尤其是想要改程序的时候。所以我们不能总是把全部的东西都写在程序里面。况且使用者也想某个程序适用性更广一点（至少在某一方面如此）。

这就有需求了，需要我们从用户手中获取信息。raw_input和argv方法就派上用处了。至于谁更好，只有天知道(笑)。

写一个`ex15_sample.txt`，这是供我们的脚本读取的文件。
```txt
This is stuff I typed into a file. 
It is really cool stuff. 
Lots and lots of fun to have in here.
```
代码文件如下：
```python
from sys import argv
# 使用argv来获取文件名
script,filename = argv 

"""
open和你自己的脚本、或者raw_input命令类似，它会接受一个 参数，并且返回一个值，可以将这个值赋予一个变量。这就是打开文件的过程。
"""
txt = open(filename) # txt获取了open返回的文件file
print "Here's your file %r:" % filename
print txt.read()  # 用句点.读取文件

print "Type the filename again:"
file_again = raw_input("> ")
txt_again = open(file_again)
print txt_again.read()
```
运行结果：
```python
(bbpy2) C:\Users\dell\Desktop\bb2>python ex15.py ex15_sample.txt
Here's your file 'ex15_sample.txt':
This is stuff I typed into a file.
It is really cool stuff.
Lots and lots of fun to have in here.

Type the filename again:
> ex15_sample.txt
This is stuff I typed into a file.
It is really cool stuff.
Lots and lots of fun to have in here.
```

## 习题：
**题1**. 我使用了“命令”这个词，不过实际上它们的名字是“函数（function） ”和“方法（method）。 上网搜索一下这两者的意义和区别。看不明白也没关系，迷失在别的程序员的知识海洋里是很正常的一件事情。
 
 - 函数：函数是封装了一些独立的功能，可以直接调用，python内置了许多函数，同时可以自建函数来使用。
   
 - 方法：方法和函数类似，同样封装了独立的功能，但是方法是需要通过对象来调用的，表示针对这个对象要做的操作，使用时采用点方法。

其实，就是说方法是类中的函数。这里还有[别人的讲解](https://blog.csdn.net/amoscn/article/details/77074403)。


**题2**.运行 pydoc file 向下滚动直到看见 read() 命令（函数/方法）。看到很多别的命令了吧，你可以找几条试试看。不需要看那些包含 __ （两个下划线）的命令， 这些只是垃圾而已。 

我在Python3的环境下运行`pydoc file`，发现似乎Python3中没有file的文档。？？？

![这里写图片描述](http://img.blog.csdn.net/20180331231413944?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 现在我知道了，Python3中将file删除了，因为file和open在功能上冗余了；
> 以前python2.x的时候还可以用file()打开文档，不过python3中已经删去了，所以没有相关文档。

然后我搜索`pydoc open`，文档如下，很清楚。
```python
C:\Users\dell>pydoc open
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
这个似乎不怎么用解释，主要是open的模式参数。'r'代表只读，open默认是'r'模式，除此之外还有写入、追加、新建并写入……自己试一下就知道了。不过也是很重要的。

我以前写了个下小说的脚本，发现次次都只下下来最后一章，很烦，修改了不少次。后来发现我SB地用了'w'，即没有该文件会新建再写入，然后有了该文件，就会先清空该文件的内容再写入……换了个'a'，就全部解决了。

只能说我基本功不牢吧。所以现在多补几本Python的书好了。

另外当我在python2虚拟环境中运行时，结果如下：

![这里写图片描述](http://img.blog.csdn.net/20180331232354213?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
什么鬼？！不过我也不想看了，虽然我学这本书的时候一直是用Python2的环境，但以后毕竟是要用Python3的……就这样吧。

## 补充知识：open函数相关
想了想还是把open的具体用法全面的总结一遍吧，免得基础不牢。

1. 作用：“花式”打开一个文件。

2. 语法：（看看Python2和3的不同还是很有趣的！） 
- **Python2**：Python2的文档字符串[]里面的参数**都是可以省略的**，如果你还是用Python2的话，像我以前就不清楚这一点，看的时候一脸懵逼。
`open(file[, mode[, buffering[, encoding[, errors[, newline[, closefd=True]]]]]])`

- **Python3**：Python3中的就指出来哪些是默认参数，哪些是一般我们不用管，用的时候用关键字参数形式传入即可。
`open(file, mode='r', buffering=-1, encoding=None, errors=None, newline=None, closefd=True, opener=None)`
> 在使用open()的只读模式时，如果文件不存在，那么将会返回FileNotFoundError。

3. 参数说明，常用的是file，mode和encoding这三个参数：

- file： 要打开的文件名，除非是在当前目录，否则需加文件路径。是唯一强制参数，调用时必须传入，其他参数可以选用。在代码中对其他参数使用了默认值。
- mode： 文件打开的模式，默认为'r'
- buffering： 设置buffer（取值为0,1,>1）
- encoding： 返回数据的编码（一般为UTF8或GBK） 
- errors： 报错级别（一般为strict，ignore） 
- newline： 用于区分换行符(只对文本模式有效，可以取的值有None,’\n’,’\r’,”,’\r\n’) 
- closefd： 传入的file参数类型（缺省为True）

4. 参数详细说明： 
- mode：文件打开的模式。有如下几种模式 
‘r’  ： 以只读模式打开（缺省模式），必须保证文件存在；
‘w’：以**只写模式**打开。若文件存在，则会自动清空文件，然后重新创建；若文件不存在，则新建文件。使用这个模式必须要保证文件所在目录存在，文件可以不存在；
‘a’ ：以**追加模式**打开。若文件存在，则会追加到文件的末尾；若文件不存在，则新建文件。
 'x' ：创建新文件，并写入内容，如果**文件已存在，将会报错**：FileExistsError
 
 - **下面四个模式要和上面的模式组合使用:**
‘b’：以二进制模式打开 
‘t’： 以文本模式打开（缺省模式） 
‘+’：以读写模式打开 
‘U’：以通用换行符模式打开
> 后来补充：
>  'U' mode is deprecated and will raise an exception in future versions of Python.  It has no effect in Python 3.  Use newline to control universal newlines mode.
>  在Python3中'U'模式已被弃用了，我们应该使用newline参数来控制通用换行符。

 - **常见的mode组合** 
        | 组合| 含义|
      |:----------------------------------|:----------------------------------------------------|
    |‘r’或’rt’| 默认模式，文本读模式 
     |‘w’或’wt’| 以文本写模式打开（打开前文件会被清空） 
    |‘rb’| 以二进制读模式打开 
    |‘ab’| 以二进制追加模式打开 
   |‘wb’|以二进制写模式打开（打开前文件会被清空） 
   |‘r+’| 以文本读写模式打开，可以写到文件任何位置；默认写的指针开始指在文件开头, 因此会覆写文件 
   |‘w+’| 以文本读写模式打开（打开前文件会被清空）
   |‘a+’| 以文本读写模式打开（写只能写在文件末尾） 
  |‘rb+’| 以二进制读写模式打开 
  |‘wb+’| 以二进制读写模式打开（打开前文件会被清空）
  |‘ab+’| 以二进制读写模式打开


- buffering：设置buffer缓存策略
		- 在二进制模式下，使用0来切换缓冲；在文本模式下，通过1表示行缓冲（固定大小的缓冲区）。
		- 在不给参数的时候，二进制文件的缓冲区大小由底层设备决定，可以通过io.DEFAULT_BUFFER_SIZE获取，通常为4096或8192字节
		- 文本文件则采用行缓冲。
 0： 代表buffer关闭（只适用于二进制模式） 
 1： 代表line buffer（只适用于文本模式）

- encoding：编码或者解码方式。默认编码方式依赖平台，如果需要特殊设置，可以参考codecs模块，获取编码列表。

- errors：报错级别，可选，并且不能用于二进制模式，指定了编码错误的处理方式，可以通过codecs.Codec获得编码错误字符串
		- strict： 字符编码出现问题时会报错 
		- ignore： 字符编码出现问题时程序会忽略而过，继续执行下面的程序

- newline：换行控制，参数有：None，'\n'，'\r'，'\r\n'。
		- 输入时，如果参数为None，那么行结束的标志可以是：'\n'，'\r'，'\r\n'任意一个，并且三个控制符都首先会被转化为：'\n'，然后才会被调用；
		- 如果参数为''，所有的通用的换行结束标志都可以用，但是行结束标识符返回调用不会被编码。
		- 输出时，如果参数为None，那么行结束的标志可以是：'\n'被转换为系统默认的分隔符；如果是''，'\n'则不会被编码。

- closefd： 
True： 传入的file参数为文件的文件名 
False： 传入的file参数只能是文件描述符 
Ps： 文件描述符，就是一个非负整数，在Unix内核的系统中，打开一个文件，便会返回一个文件描述符。

- opener：可以通过调用*opener*方式，使用自定义的开启器。底层文件描述符是通过调用*opener*或者*file*, *flags*获得的。
*opener*必须返回一个打开的文件描述。将os.open作为*opener*的结果，在功能上，类似于通过None。

> 如有问题，日后还会修改。

![](http://img.blog.csdn.net/20180404191226614?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)