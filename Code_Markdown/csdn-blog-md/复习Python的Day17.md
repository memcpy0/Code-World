# 复习Python的第十七天
[TOC]
## 习题 18: 命名、变量、代码、函数 
本节要学会的知识：`def`：新建函数，就是英语的“定义”define。

函数的作用：

- 给代码片段命名，就像给脚本、字符串和数字命名
- 接受参数，像我们的脚本接受argv一样
- 创建“微型脚本”或者“小命令”
- 工作起来和脚本一样

> 代码
```python
# this one is like your scripts with argv 
# 以列表形式接受任意长度的参数，和argv相似
def print_two(*args):  
    arg1, arg2 = args 
    print "arg1: %r, arg2: %r" % (arg1, arg2) 
 
# ok, that *args is actually pointless, we can just do this 
# 接受两个参数
def print_two_again(arg1, arg2): 
    print "arg1: %r, arg2: %r" % (arg1, arg2) 
 
# this just takes one argument 
# 接受一个参数
def print_one(arg1): 
    print "arg1: %r" % arg1 
 
# this one takes no arguments 
# 无参数
def print_none(): 
    print "I got nothin'." 
 
print_two("Zed","Shaw") 
print_two_again("Zed","Shaw") 
print_one("First!") 
print_none() 
```
函数的话题说来话长，我将分为几个方面结合上述代码说一说。

## 1、函数类型
**函数是一组语句的集合，用以实现某一特定的功能。**

在python中的函数分为内置函数和自定义函数。当自定义函数很多时，为了方便管理，就将函数分类保存到不同的模块中，因此，模块可以看做一系列函数的集合。很多函数库在python中都是以模块的形式提供的。

## 2、自定义函数

编写脚本的工作中有很多重复的工作，当我们完成了可以实现相应功能的代码后，就会将能够优化的重复工作的代码语句提取出来，编写为函数。


### 2-1 函数声明
在Python中，使用`def`可以声明一个函数。完整的函数如同上面的示例代码一样，由函数名、参数、函数实现语句和返回值组成，以缩进表示函数体范围，其中参数和返回值不是必须的。

如有返回值，将在函数中使用`return`语句返回结果；有时，也不需要返回值，如上面的示例，只执行了打印的功能。

一般形式如下：
```python
def 函数名(参数列表):
    函数语句
    return 返回值
```
完整函数示例：
```python2
# 求列表中所有数之和，也可以求元组中的数字和
def listSum(L):
   res = 0
   for i in L:
      res += i
      return res
```
Python中的函数在调用过程中十分灵活，不需要为不同类型的参数声明多个函数，或在处理不同类型数据的时候调用多个函数。大部分情况下都可以只用同一个函数调用不同的数据类型。这就是Python很重要的 一个特性，**多态**。
```python2
>>> def printAll(g):
	  for i in g:
		print i
>>> def printAll(g):
      for i in g:
        print i

>>> L = [1,2,'string']
>>> printAll(L)
1
2
string	
>>> t = 'Ok,I\'m fine'	
>>> printAll(t)
O
k
,
I
'
m

f
i
n
e
```
我们分别传入了一个列表和字符串，都成功调用了该函数，说明了python中函数的灵活性之强。

### 2-2 函数调用
很简单，以函数名调用函数即可，然后在括号中传入参数，**多个参数以逗号隔开**。

> 注意的是，函数调用必须在函数声明之后；
> 即使函数不需要参数，也要在参数名后面使用一对空的圆括号。

