# 复习Python的第三十四天
## 习题39：列表的操作
本节的一个重要观念是关于Python中的函数调用过程。
以`mystuff.append('hello')`为例，它表示在原地向列表结尾附加新的项'hello'。
但真正发生的事情是`append(mystuff, 'hello')`，因此函数实际上（比我们看起来的只有一个）接受两个参数，额外的参数就是mystuff本身。

这是一个列表的操作复习课。

```python2
ten_things = "Apples Oranges Crows Telephone Light Sugar"
print "Wait there's not 10 things in that list, let's fix that."

stuff = ten_things.split(' ')  # split(ten_things, ' ')
more_stuff = ["Day", "Night", "Song", "Frisbee", "Corn",
"Banana", "Girl", "Boy"]

while len(stuff) != 10:
	next_one = more_stuff.pop()  
	print "Adding: ", next_one 
	stuff.append(next_one)  # append(stuff, next_one)
	print "There's %d items now." % len(stuff)

print "There we go: ", stuff
print "Let's do some things with stuff."

print stuff[1]  
print stuff[-1] # whoa! fancy
print stuff.pop() 
print ' '.join(stuff) # what? cool!  # join(' ', stuff)
print '#'.join(stuff[3:5]) # super stellar!  # join('#', stuff[3:5])
```
Object Oriented Programming，简称为OOP，面向对象编程；函数式编程(functional programming)。

1. stuff.split(’ ‘)，以空格为标志分割字符串，**默认全部分割**，可以在括号里”后面指定参数以使解释器按规定次数分割。 
2. stuff.append(next_one)，向列表stuff中增添元素next_one。
3. print '#'.join(stuff[3:5]) ，用#将stuff中的第3个元素和第4个元素连接起来，[3:5]这个参数给出了要连接的元素在列表中的位置，**注意不包括第5个元素**

### 关于列表操作的文档
```python
 |  append(...)  # 返回None，是因为在原地修改列表，而非创建新列表，因此以L=L.append()就会失去该列表和修改
 |      L.append(object) -> None -- append object to end 
 |
 |  clear(...)   # 清除所有项，返回None，这和其它删除命令不同
 |      L.clear() -> None -- remove all items from L
 |
 |  copy(...)    # 创建列表的浅拷贝
 |      L.copy() -> list -- a shallow copy of L
 |
 |  count(...)   # 计数，返回整数值，即为该值出现的次数
 |      L.count(value) -> integer -- return number of occurrences of value
 |
 |  extend(...)  # 取出可迭代对象中的元素并附加到列表中，返回None
 |      L.extend(iterable) -> None -- extend list by appending elements from the iterable
 |
 |  index(...)   # 返回value第一次出现位置的索引值，value不存在的话会抛出ValueError
 |      L.index(value, [start, [stop]]) -> integer -- return first index of value.
 |      Raises ValueError if the value is not present.
 |
 |  insert(...)  # 向index指定的位置之前插入该项
 |      L.insert(index, object) -- insert object before index
 |
 |  pop(...)    # 删除列表指定位置的元素，指定位置可选，默认从最后开始删除，会返回删除的元素
 |      L.pop([index]) -> item -- remove and return item at index (default last).
 |      Raises IndexError if list is empty or index is out of range.
 |
 |  remove(...) # 移除第一次出现的该value值
 |      L.remove(value) -> None -- remove first occurrence of value.
 |      Raises ValueError if the value is not present.
 |
 |  reverse(...)  # 翻转该列表
 |      L.reverse() -- reverse *IN PLACE*
 |
 |  sort(...)  # 为列表排序
 |      L.sort(key=None, reverse=False) -> None -- stable sort *IN PLACE*
```
**多查多看，日常多用**，列表的上述操作很简单的。