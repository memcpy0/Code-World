## Python3中的None
搜索一下，得到以下解答：
>None是一个特殊的常量。
  None和False不同。
  None不是0。
  None不是空字符串。
  None和任何其他的数据类型比较永远返回False。
  None有自己的数据类型NoneType。
 你可以将None复制给任何变量，但是你不能创建其他NoneType对象。
 
```python
>>> None == False
False
>>> None is False
False
>>> False is None
False

>>> None == 0
False
>>> None is 0
False
>>> 0 is None
False

>>> None == ''
False
>>> None is ''
False

>>> type(None)
<class 'NoneType'>
```
可以看出None和False(或0或[ ]或' '等) 既在值上不等(==)，也不是同一个对象(is) 。

我们可以认为None是一个特殊Python对象, 在Python解释器启动时自动创建，解释器退出时销毁。在一个解释器进程中只有一个None存在，因为不可能有其他对象会使用None已占用的内存(它就是占了个坑) 。

所以只有` None is None and None == None`。

## Python3中的is和==
用指针来说： 

- is: 指向同一内存地址；
- ==: 指向的内存地址中的内容是相同的。
```
a = list() 
b = [] 
print(a == b) # True 
print(a is b) # False 
```
```
c = "abc" 
d = "abc" 
print(c == d) # True 字符串池 
print(c is d) # True 
```
此时为list()和[]相当于各自分配了一次内存，所以a is not b。

但是各自分配的内存中的内容是一样的，故而a == b。 

c和d指向同一个内存地址（字符串池、引用计数机等机制）所以 c is d， c 和 d 指向的内存中存的内容也是相同的（"abc"），所以c == d。


而None，可以认为是内存中不同于其他的一块内存空间，a b c d都有各自的指向，所以不是指向None，故而a b c d is not None，自然a b c d != None。

一图胜千言 

![](http://img.blog.csdn.net/20180414130825019?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)