# 复习Python的第八天

## 习题 9: 打印，打印，打印

继续我的打印之旅。
>  这一节有一点新的东西。
```python2
>>> # Here's some new strange stuff, remember type it exactly. 
>>> days = "Mon Tue Wed Thu Fri Sat Sun"
>>> months = "Jan\nFeb\nMar\nApr\nMay\nJun\nJul\nAug"

>>> print "Here are the days:",days

>>> print "Here are the months:",months

>>> # 多行打印
>>> print """                   
There's something going on here.
With the three double-quotes.
We'll be able to type as much as we like.
Even 4 lines if we want,or 5,or 6.
"""
Here are the days: Mon Tue Wed Thu Fri Sat Sun
Here are the months: Jan
Feb
Mar
Apr
May
Jun
Jul
Aug
                   
There's something going on here.
With the three double-quotes.
We'll be able to type as much as we like.
Even 4 lines if we want,or 5,or 6.
```
\n是格式控制字符，相当于回车，表示到这里要换行。这类型字符会在下节总结一下。

另外，我对原代码在这句的写法"Jan\nFeb\nMar\nApr\nMay\nJun\nJul\nAug"感觉有点……怪怪的
，不推荐这种写法，至少在\n后加空格，才是好的写法。

空格可以使代码更清楚。之前作者是有空格的习惯的，像 '=' 号前后都有空格，这样**看着就很舒服**。

前后用三个引号括起来可以print多行字符串。

