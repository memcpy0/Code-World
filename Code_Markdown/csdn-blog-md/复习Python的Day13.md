#复习Python的第十三天
[TOC]

![](http://img3.imgtn.bdimg.com/it/u=65226684,2288901633&fm=27&gp=0.jpg)

## 习题14：提示和传递
这一节承接上节加分习题，将raw_input和argv一起使用，让我们的脚本从用户手中得到更多输入。作者在之前说，这个组合是个**蛮顺手**的用法。

同时，作者说：
> 下一节习题你会学习如何读写文件，这节练习是下节的**基础**。
> 在这道习题里我们将用略微不同的方法使用raw_input，让它打出一个简单的>作为提示符。

```python
from sys import argv
# 解包，将从命令行中得到的参数分别赋予前面的变量
script, user_name = argv
# 提示符
prompt = '>'
# 打印提示：Hi，XXX，我是XX脚本
print "Hi %s , I'm the %s script." % (user_name,script)
print "I'd like to ask you a few questions."

# 询问喜欢
print "Do you like me %s?" % user_name
likes = raw_input(prompt)

# 询问住址
print "Where do you live %s?" % user_name
lives = raw_input(prompt)

# 询问所用计算机种类
print "What kind of computer do you have?"
computer = raw_input(prompt)

# 输出
print """
Alright , so you said %r about liking me.
You live in %r. Not sure where that is.
And you have a %r computer. Nice.
""" % (likes,lives,computer)
```
> 当你运行这个脚本时，记住你需要把你的名字赋给这个脚本，让 argv参数接收到你的名称。 

argv接受的参数是命令行中输入的python之后的用空格分割的内容，因此第一个输入的变量**即所运行的脚本的名字**“ex14.py”被赋给script这个变量。

当然，仅凭本节和上一节的内容，无法判定argv是否像raw_input那样只能处理字符串。

我在命令行中运行，结果如下：
```python2
(bbpy2) C:\Users\dell\Desktop\bb2>python ex14.py zhang_ping
Hi zhang_ping , I'm the ex14.py script.
I'd like to ask you a few questions.
Do you like me zhang_ping?
>Yes
Where do you live zhang_ping?
>WHU
What kind of computer do you have?
>Dell

Alright , so you said 'Yes' about liking me.

You live in 'WHU'. Not sure where that is.

And you have a 'Dell' computer. Nice.
```
将**用户提示符设置为变量prompt**是个很好的做法，这样我们就不需要在每次用到raw_input 时重复输入提示用户的字符了。

而且如果我们要将提示符修改成别的字符串，只要改一个位置就可以了。这不就是变量设置的本意吗？

## 加分习题：

> Zork I是电子游戏历史上最早的一款文字冒险游戏，是Colossal Cave Adventure的一个早期后继。
> Zork的首个版本由Tim Anderson、Marc Blank、Bruce Daniels和Dave Lebling于1977至79年间在DEC PDP-10电脑上、以MDL程式语编写。他们四人全是麻省理工动力模型组的成员。在1980年时由Infocom公司发行的。
> 它有很多平台的版本，也有许多忠实的玩家群，甚至到了公司被Activision收购之后，仍然推出许多款续作，由文字界面进步到图形界面。
> 在使命召唤7：黑色行动（Call of Duty：Black Ops）中，在刑讯室从椅子上挣脱之后，能进入CIA的终端，在终端中输入zork就能进入这个经典的游戏。
> 很遗憾，我没有找到能在win10 64位版上运行的版本。
> 这个游戏类似于TBBT里面Sheldo玩的《龙与地下城》。

1. 将 prompt 变量改成完全不同的内容再运行一遍。
2. 给你的脚本再添加一个参数，让你的程序用到这个参数。 

至于上面两道题，可做可不做，我现在先不写。

![](http://img2.imgtn.bdimg.com/it/u=280209502,1999290580&fm=27&gp=0.jpg)