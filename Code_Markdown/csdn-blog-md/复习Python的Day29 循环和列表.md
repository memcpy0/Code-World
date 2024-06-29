# 复习Python的第二十九天

## 习题32: 循环和列表
将“if 语句”和“布尔表达式”结合起来可以让程序作出一些智能化的事情。然而，我们的程序还需要能很快地完成重复的事情。这节习题中我们将使用 **for-loop（for 循环）**来创建和打印出各种各样的**列表**。

- for-loop：循环结构
- list(列表)：按顺序存储数据的容器，即为序列，列表既支持python中一般的序列方法，也有自己独特的方法。

创建列表要做的是以 [（左括号）开头“打开”列表，然后写下要放入列表的东西，用逗号隔开，就跟函数的参数一样，最后需要用]（右括号）结束右方括号的定义。

然后将该列表赋值给一个变量，列表才算被创建。
> 代码部分
```python2
# 创建了三个列表
the_count = [1, 2, 3, 4, 5] 
fruits = ['apples', 'oranges', 'pears', 'apricots'] 
change = [1, 'pennies', 2, 'dimes', 3, 'quarters']  # 列表可以由不同类型混合

# 以下两个循环遍历列表
# this first kind of for-loop goes through a list 
for number in the_count: 
	print "This is count %d" % number 

# same as above 
for fruit in fruits: 
	print "A fruit of type: %s" % fruit 

# also we can go through mixed lists too 
# notice we have to use %r since we don't know what's in it 
# 用%r作为格式化字符
for i in change: 
	print "I got %r" % i 

# we can also build lists, first start with an empty one 
elements = [] 
# then use the range function to do 0 to 5 counts 
for i in range(0, 6): 
	print "Adding %d to the list." % i 
    # append is a function that lists understand 
	elements.append(i)

# now we can print them out too 
for i in elements: 
	print "Element was: %d" % i
```
查了以下Python3中range()的用法：
```python3
Help on class range in module builtins:

class range(object)
 |  range(stop) -> range object
 |  range(start, stop[, step]) -> range object
 |
 |  Return an object that produces a sequence of integers from start (inclusive)
 |  to stop (exclusive) by step.  range(i, j) produces i, i+1, i+2, ..., j-1.
 |  start defaults to 0, and stop is omitted!  range(4) produces 0, 1, 2, 3.
 |  These are exactly the valid indices for a list of 4 elements.
 |  When step is given, it specifies the increment (or decrement).
```
它会返回一个range对象，是一个可迭代对象，但不是迭代器。当我们用for遍历它的时候，其实是隐形地将其转化为迭代器，再使用迭代协议将其输出。
```python3
>>> range(5)
range(0, 5)
>>> next(range(5))
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: 'range' object is not an iterator

>>> iter(range(5))
<range_iterator object at 0x000001983872C910>
>>> next(iter(range(5)))
0
```
可以直接将 elements 赋值为 range(0,6)，而无需使用 for 循环。
```python
>>> elements = list(range(5)) # 使用list转换
>>> elements
[0, 1, 2, 3, 4]

>>> elements = [i for i in range(5)]  # 使用列表解析
>>> elements
[0, 1, 2, 3, 4]
```

查询python3的文档中list的用法和支持的操作：
```python
>>> dir([])
['__add__', '__class__', '__contains__', '__delattr__', '__delitem__', '__dir__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__getitem__', '__gt__', '__hash__', '__iadd__', '__imul__', '__init__', '__init_subclass__', '__iter__', '__le__', '__len__', '__lt__', '__mul__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__reversed__', '__rmul__', '__setattr__', '__setitem__', '__sizeof__', '__str__', '__subclasshook__', 

'append', 'clear', 'copy', 'count', 'extend', 'index', 'insert', 'pop', 'remove', 'reverse', 'sort']
```
一般而言，第二段的方法是很常用的，从append到sort。
