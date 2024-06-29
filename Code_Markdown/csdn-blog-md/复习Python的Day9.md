# 复习Python的第九天
[TOC]

## 习题10: 那是什么？ 
在习题9中我接触了一些新东西。总结如下：

**第一种打印多行字符的方法**是在月份之间用 \n (back-slash n )隔开。这两个字符的作用是在该位置上放入一个“新行(new line)”字符。 

> 使用反斜杠 \ (back-slash) 可以将难打印出来的字符放到字符串。针对不同的符号有很多这样的所谓“转义序列(escape sequences)”。

但有一个特殊的转义序列， 就是双反斜杠(double back-slash) \\ 。这两个字符组合会打印出一个反斜杠来。

另外一种重要的转义序列是用来将单引号 ' 和双引号 " 转义。方法如下所示：
```python
>>> print "I am 6'2\" tall." # 将字符串中的双引号转义
>>> print 'I am 6\'2" tall.' # 将字符串中的单引号转义
I am 6'2" tall.
I am 6'2" tall.
```
这里是第二种方法。

**第二种方法是使用“三引号(triple-quotes)”**，你可以在一组三引号之 间放入任意多行的文字。用法如下：
```python
>>> tabby_cat = "\tI'm tabbed in."
>>> persian_cat = "I'm split\non a line."
>>> backslash_cat = "I'm \\ a\\ cat."

# \t意为打印一个制表符
>>> fat_cat = """
I'll do a list:
\t* Cat food            
\t* Fishies
\t* Catnip\n\t* Grass 
"""

>>> print tabby_cat
>>> print persian_cat
>>> print backslash_cat
>>> print fat_cat

I'm tabbed in.
I'm split
on a line.
I'm \ a\ cat.

I'll do a list:
	* Cat food
	* Fishies
	* Catnip
	* Grass
```

## 加分习题  : 转义字符

上网搜索一下还有哪些可用的转义字符。

### 总结如下  : 字符串转义序列
|   转义字符     |          	  描述
|:------------------|:-----------------------------
| (在行尾时)\	    |              续行符
|  双斜杠 \ \	         |              反斜杠符号
| \'	         |              单引号
| \"	         |              双引号
| \a	         |              响铃
| \b	         |           退格(Backspace)
| \e	         |               转义
| \000	         |             空
| \n	         |               换行
| \v	         |             纵向制表符
| \t	         |             横向制表符
| \r	         |                 回车
| \f	         |                换页
| \oyy	         |   八进制数yy代表的字符，例如：\o12代表换行
| \xyy	         |   十进制数yy代表的字符，例如：\x0a代表换行
| \other	      |          其它的字符以普通格式输出


-
> 用法实例：
```
>>> print 'I hope\a to eat an apple.'
>>> print 'I hope\b to eat an apple.'
>>> print 'I hope\n\t to eat an apple.'
>>> print 'I hope\n\\ to eat an apple.\n'
>>> st = 'I hope\'today\" to eat an apple.'
>>> print "%r\n%s" % (st,st)

I hope to eat an apple.
I hop to eat an apple.
I hope
	 to eat an apple.
I hope
\ to eat an apple.

'I hope\'today" to eat an apple.'
I hope'today" to eat an apple.
```
\a基本看不到影响，\b是退格，即删除前一格的内容，\n不必多说，\\\是打印一个\出来。

而\'和\"就有点怪，不过我想明白了，只有在单引号引起的字符串中使用单引号才需转义(**需要不让Python认错前后字符串的边界，从而把字符串弄错，就要告诉 python ，字符串里边的单引号不是真正的单引号**)，而在其中使用双引号是不用转义的，所以这里用%r打印st字符串，\"只打印出了"。

同样，在双引号中使用双引号也须转义，在双引号中使用单引号则不需。

![](http://img2.imgtn.bdimg.com/it/u=280209502,1999290580&fm=27&gp=0.jpg)