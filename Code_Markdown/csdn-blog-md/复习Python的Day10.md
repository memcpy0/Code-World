# 复习Python的第十天
[TOC]
## 习题11:  提问

![](http://img2.imgtn.bdimg.com/it/u=1821684036,2885722734&fm=27&gp=0.jpg)

现在要做的是把数据读到我的程序里边去。即学习读入用户的输入。

> 一般软件做的事情主要就是下面几条： 
1. 接受人的输入。（Input）
2. 改变输入。 
3. 打印出改变了的输入。 （Print）

这是作者说的，也是程序设计思维模式，IPO方法。

> 习题代码
```python
>>> print "How old are you?",
>>> age = raw_input()
>>> print "How tall are you?",
>>> height = raw_input()
>>> print "How much do you weigh?",
>>> weight = raw_input()
>>> print "So,you're %r old,%r tall and %r heavy." %(age,height,weight)

How old are you?11
How tall are you?11
How much do you weigh?11
So,you're '11' old,'11' tall and '11' heavy.
``` 
> 注意到在每行print 后面加了个逗号(comma) ", "了吧？这样的话print就不会输出新行符而结束这一行跑到下一行去了。

## 加分习题11
####1. Python2的raw_input 实现的功能

**raw_input的功能是方便的从控制台读入数据。**

>input与raw_input都是Python的内建函数，实现与用户的交互，但是功能不同。
    
#### 2. raw_input 与 input的区别 
>这两个函数都可以读取用户的输入，不同的是input（）函数要求用户输入有效的表达式，而raw_input()函数将用户输入的任意类型数据都转换为一个字符串。
   
#### 3. raw_input 与input的联系
>查看Built-in-functions可得知，input是由raw_input实现的。即调用raw_input（）后再调用eval（）函数。

#### 4. 从 raw_input() 到 input()

**Python 2.X**
   
>在2.x版本中，raw_input()会从标准输入(sys.stdin)读取输入值并返回一个字符串，且尾部换行符从末尾移除。
input()不同，需要输入有效的表达式，如3+8，'abc'。

**Python 3**
>在Python 3中，将raw_input()重命名为input()，这样一来，无需导入也可从标准输入获得数据。

我的一点尝试，部分例子如下：
```python
# 1、输入字符串
>>> raw1 = raw_input()
>>> raw1
abc
Out[1]:str

# 2、输入整数
>>> raw2 = raw_input()
>>> type(raw2)
123
Out[3]:str

# 3、输入浮点数
>>> raw3 = raw_input()
>>> type(raw3)
1.2
Out[4]:str

# 4、输入16进制数
>>> raw4 = raw_input()
>>> type(raw4)
0x20
Out[5]:str

# 5、输入八进制数
>>> raw5 = raw_input()
>>> type(raw5)
122
Out[6]:str
```
其他的特性似乎不太重要，最重要的是知道它是返回字符串的就好了。


![](http://img1.imgtn.bdimg.com/it/u=3080445328,2538262045&fm=27&gp=0.jpg)