# 复习Python的Day7
## 习题8：打印，打印 
**练习的目的是巩固我学到的东西。**
```python2
>>> formatter = "%r %r %r %r"
>>> print formatter %(1,2,3,4)
>>> print formatter %("one","two","three","four")
>>> print formatter %(True,False,False,True)
>>> print formatter %(formatter,formatter,formatter,formatter)
>>> print formatter %(
    "I had this thing.",
    "That you could type up right.",
    "But it didn't sing.",
    "So I said goodnight."

1 2 3 4
'one' 'two' 'three' 'four'
True False False True
'%r %r %r %r' '%r %r %r %r' '%r %r %r %r' '%r %r %r %r'
'I had this thing.' 'That you could type up right.' "But it didn't sing." 'So I said goodnight.'
)
```
从中可以看到括号可以分行写，但要保证括号是成对出现的；括号里面的内容也可以分行写，只需加上逗号，解释器就能够连在一起输出。就和前文一样。

## 加分习题：
**注意最后一行程序中既有单引号又有双引号，你觉得它是如何工作的？**

关于这个我还真没想到，"%r"应该是不管什么都输出才对（优先用repr()函数进行字符串转换），但这里的"%r %r %r %r"输出为单引号，而只有"But it didn't sing."输出为双引号。

Why？I can't understand!


后来试了一下：
```python2
>>> formatter = "%r"
>>> print formatter % "I don't understand."
>>> print formatter % "Oh I know."
"I don't understand."
'Oh I know.'
```
我猜，在输入代码的时候字符串是被双引号括起来的，但在"But it didn't sing."字符串中包含了单引号，所以在输出的时候显示为双引号。字符串中包含有单引号的用双引号括起来用来区分；字符串中没有单引号在输出时显示单引号。
***
顺便推荐一本学Python的好书：《Python学习手册》。

![](http://img.blog.csdn.net/20180406002001901?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)