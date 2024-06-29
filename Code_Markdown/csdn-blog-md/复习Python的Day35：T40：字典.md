# 复习Python的第三十五天
## 习题40:  字典, 可爱的字典
本节我们学习的另一个数据容器是字典(dictionary)，其类型是“dict”，但在其他语言中叫“hash”。

列表可以让你通过数字索引找到其中的元素，与之相比，字典能让你通过任意东西找到元素，它可以将一个物件和另外一个东西关联，不管它们的类型是什么。

所以，**字典的本质是映射，表现出来就是key-value键值对**，其中，键必须是不可变对象，也就是字符串、元祖、数字等，不能是列表、字典等可变对象。另外，键值对的排列是无序的。
```python
>>> stuff = {'name': 'Zhang', 'age': 19, 'height': 6*10}
>>> print stuff['name']  # 用字符串来从字典中获取stuff 
Zhang
>>> print stuff['age']
19
>>> print stuff['height']
60
>>> stuff['city'] = "San Francisco"  # 通过赋值在原地向字典中添加项
>>> print stuff['city']
San Francisco

>>> stuff[1] = "Wow"  # 通过赋值在原地向字典中添加项；模拟列表
>>> stuff[2] = "Neato"
>>> print stuff[1]  # 用数字来从字典中获取stuff 
Wow
>>> print stuff[2]
Neato
>>> print stuff
{'city': 'San Francisco', 2: 'Neato', 1: 'Wow','name': 'Zhang', 'age': 19, 'height': 6*10}

>>> del stuff['city']  # 删除对应项
>>> del stuff[1]
>>> del stuff[2]
>>> stuff
{'name': 'Zhang', 'age': 19, 'height': 6*10}
```
> 本节的代码很有意思，值得学习。
```Python
cities = {'CA':'San Francisco','MI':'Detroit','FL':'Jacksonville'}

cities['NY'] = 'New York'
cities['OR'] = 'Portland'

def find_city(themap,state):
    if state in themap:  # 可以根据条件不同，选择不同的返回值
        return themap[state]
    else:
        return 'Not found.'
    
# ok pay attention!
cities['_find'] = find_city

while True:
    print "State?(ENTER to quit)",
    state = raw_input(">")
    # 没有输入就为''，为空，退出循环
    if not state:break  
        
    # this line is the most important ever! study!
    city_found = cities['_find'](cities,state)
    print city_found
```
注意的地方有几个，其中之一就是用themap而不是 map。这是因为 Python 已经有一个函数称作map 了，如果我们用map做变量名，就会挤占命名空间，覆盖原来的map函数。

另外，在函数中，我们可以使用if-else语句，根据不同的条件返回不同的结果。虽然强调函数的单一出口，但是有时是做不到的。如果没有返回值的函数，默认返回None。

最重要的方面之一，是我们可以将字典的键映射到函数上面。`cities['_find'] = find_city`，实际上，**当函数定义时，就已经将函数对象赋值给了函数头部的函数变量名，函数变量名通过对象引用调用函数主体**。

函数变量名可以赋给其他变量，就如上面的例子，也可以说是将函数变量名赋给了cities['_find']，或者如：`cityfind = find_city`，这时该变量名和原来的函数变量名共享对函数对象的引用，可以通过`cities['_find'](cities,state)`或者`cityfind(cities,state)`或者`find_city(cities,state)`调用函数。

当然，还可以直接将字典的键映射到函数的调用上面，如`cities['_find'] = find_city()`，不过这种适用范围不一定多宽。

推及列表，也是可以的，我们可以创建一个函数变量名或函数调用的列表，通过偏移值索引进行函数对象引用和函数调用。

而上面的这个程序将cities字典变为了一个**集合数据和搜索功能**的工具，且支持持续循环，因为`while True`。

只有当输入为空时才会退出。这也很好地符合了之前说明的使用while循环的原则。

---

## Python3中的dictionary操作文档
```python
>>> dir(dict)
['__class__', '__contains__', '__delattr__', '__delitem__', '__dir__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__getitem__', '__gt__', '__hash__', '__init__', '__init_subclass__', '__iter__', '__le__', '__len__', '__lt__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__setitem__', '__sizeof__', '__str__', '__subclasshook__', 

'clear', 'copy', 'fromkeys', 'get', 'items', 'keys', 'pop', 'popitem', 'setdefault', 'update', 'values']
```
dir()函数输出所有的变量名，包括函数变量、值变量和方法名。因此比用pydoc查看的变量名多几个，不过也没多少区别。事实上，用pydoc看有哪些变量和方法更舒服。

而且用print(dict.__doc__)和使用pydoc看文档效果也不一样。所以pydoc真是我们的福音啊。

贴出常用函数的文档：
```python
 |  clear(...)
 |      D.clear() -> None.  Remove all items from D.
 |
 |  copy(...)
 |      D.copy() -> a shallow copy of D
 |
 |  fromkeys(iterable, value=None, /) from builtins.type
 |      Returns a new dict with keys from iterable and values      equal to value.
 |
 |  get(...)
 |      D.get(k[,d]) -> D[k] if k in D, else d.  d defaults to None.
 |
 |  items(...)
 |      D.items() -> a set-like object providing a view on D's items
 |
 |  keys(...)
 |      D.keys() -> a set-like object providing a view on D's keys
 |
 |  pop(...)
 |      D.pop(k[,d]) -> v, remove specified key and return the corresponding value.
 |      If key is not found, d is returned if given, otherwise KeyError is raised
 |
 |  popitem(...)
 |      D.popitem() -> (k, v), remove and return some (key, value) pair as a
 |      2-tuple; but raise KeyError if D is empty.
 |
 |  setdefault(...)
 |      D.setdefault(k[,d]) -> D.get(k,d), also set D[k]=d if k not in D
 |
 |  update(...)
 |      D.update([E, ]**F) -> None.  Update D from dict/iterable E and F.
 |      If E is present and has a .keys() method, then does:  for k in E: D[k] = E[k]
 |      If E is present and lacks a .keys() method, then does:  for k, v in E: D[k] = v
 |      In either case, this is followed by: for k in F:  D[k] = F[k]
 |
 |  values(...)
 |      D.values() -> an object providing a view on D's values
```
其中的items()、keys()、values()常用于字典的迭代操作。