# 复习Python的第十五天

[TOC]


![](http://img0.imgtn.bdimg.com/it/u=1554546114,101168088&fm=27&gp=0.jpg)

## 习题16：读写文件

这一节的要记住的命令如下：

- `close`关闭文件
- `read`读取文件的内容，返回一个文件对象
- `truncate`清空文件
- `write(stuff)`写入文件

> 代码部分：
```python
from sys import argv
script, filename = argv

# 打印提示
print "We're going to erase %r." % filename 
print "If you don't want that,hit CTRL-C (`C)." 
print "If you do want that,hit RETURN." 

# 打开相应文件，清空其内容
raw_input("?")
print "Opening the file……" 
target = open(filename,'w')
print "Truncating the file.Goodbye!" 
target.truncate()

# 提示输入三行字
print "Now I'm going to ask you for three lines." 
line1 = raw_input("line 1:")
line2 = raw_input("line 2:")
line3 = raw_input("line 3:")
print "I'm going to write these to file." 

# 写入三行字符串
target.write(line1)
target.write('\n')
target.write(line2)
target.write('\n')
target.write(line3)
target.write('\n')

# 提示关闭文件
print "And finally,we close it." 
target.close()
```
这是作者的一个小提示，蛮有用的，感觉和刚刚看的单元测试有点关联，说起来这方面的知识我也不太懂……
> 慢慢来，仔细检查，让它能运行起来。有一个小技巧就是你可以让你的脚本一部分一部分地运行起来。 先写1-8 行，让它运行起来，再多运行 5 行，再接着多运行几行，以此类推， 直到整个脚本运行起来为止。

## 加分习题：
**1. 文件中重复的地方太多了。试着用一个 target.write() 将 line1, line2, line3写进来，你可以使用字符串、 格式化字符、以及转义字符。**这个题还有点意思：
```python2
……# 以上略
# 写入三行字符串
formatter = "%s \n %s \n %s \n"
target.write(formatter % (line1, line2, line3)
…… # 以下略
```
结果成功。
```python2
……# 以上略
# 写入三行字符串
for line in (line1,line2,line3):
    target.write(line+'\n')
…… # 以下略    
```
也一样。

**2. 找出为什么我们需要给open多赋予一个 'w' 参数。提示: open 对于文件的写入操作态度是安全第一，所以你只有特别指定以后，它才会进行写入操作。**

就如提示所言，python中对文件操作是很谨慎的，只有改变了默认参数后才能进行写入。

## 补充知识：truncate函数、tell函数及读取函数
来自Python2的文档。
```python2
truncate(...)
      truncate([size]) -> None.  Truncate the file to at most size bytes.
	  Size defaults to the current file position, as returned by tell().
```
如果指定了可选参数 size，则表示截断文件为 size 个字符。 如果没有指定size，则从当前位置起截断；截断之后 size 后面的所有字符被删除。如果是刚打开文件就截断，`at most size bytes`意味着清空文件内容。

该函数无返回值。

而tel()函数，内容如下：
```python2
tell(...)
     tell() -> current file position, an integer (may be a long integer)
```

tell() 方法返回文件的当前位置，即**文件指针当前位置**的字节数。现在看不懂也不用在意。

关于读取函数的文档：
```python2
read(...)
       read([size]) -> read at most size bytes, returned as a string.
 
       If the size argument is negative or omitted, read until EOF is reached.
       Notice that when in non-blocking mode, less data than what was requested
       may be returned, even if no size parameter was given.
 
readinto(...)
       readinto() -> Undocumented.  Don't use this; it may go away.
 
readline(...)
       readline([size]) -> next line from the file, as a string.
 
       Retain newline.  A non-negative size argument limits the maximum
       number of bytes to return (an incomplete line may be returned then).
       Return an empty string at EOF.
 
readlines(...)
       readlines([size]) -> list of strings, each a line from the file.
 
       Call readline() repeatedly and return a list of the lines so read.
       The optional size argument, if given, is an approximate bound on the
       total number of bytes in the lines returned.
```
看看就懂了。

 
