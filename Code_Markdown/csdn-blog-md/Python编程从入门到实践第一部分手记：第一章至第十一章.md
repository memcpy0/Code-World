# Python编程从入门到实践

![](https://img-blog.csdnimg.cn/img_convert/056b6dacf9469173bed0627f0cc2cbfa.png)

@[toc]
## 第一章 起步
没什么好说的。

我以后主要会写动手试一试这部分的内容。

而作者在`1-1 python.org`中建议我们浏览[Python主页](http://python.org/)，寻找我们感兴趣的内容和主题，作者还说，“你对Python越熟悉，这个网站对你来说就越有用。”

对我来说，最有用的就是[PyPi](https://pypi.org/search/?q=&o=)(因为文档可以在线下从IDLE中查看)，包托管网站，可以从Python主页进入，里面有很多有趣的包，能够学一学。不过，我自己现在也还不太熟悉。

1-3 无穷的技艺：描述三个想开发的程序：

- 图像型PDF和PDF的通用识别；
- 批量下载网络小说、图片和音频的程序；
- 做模拟命令行命令形式的对文件操作的程序；
- 做一些数据分析

>如果心中有目标，就能立即将新学到的技能付诸应用；现在正是草拟目标的大好时机。

>将心中的想法记录下来是个不错的习惯，这样每当需要开始新项目的时候，都可以参考它们。

## 第二章 变量和简单数据类型
### 动手试一试
#### 2-1 简单消息
```python
>>> message = {'email':123456,
               'address':"WhuUniversity",
               'email':"2183927003@qq.com"}
>>> print(message)
{'email': '2183927003@qq.com', 'address': 'WhuUniversity'}
```
这里我故意写了两个同样的键(key) "email"，发现打印出来以后，后面的"email"覆盖了前面的键。所以，字典的键具有唯一性。

#### 2-2 	多条简单消息
```python
In [12]: quote = """
             Life is short, I use python
             """
In [13]: print(quote)
        Life is short, I use python
In [14]: quote = "Give you a stick, and you will steal a bag."

In [15]: print(quote)
Give you a stick, and you will steal a bag.
```
这里在三引号字符串中，我们的格式被保存了下来，所以它的缩进就是这样了。当然，我们也可以用换行符和制表符等转义字符来进行格式排列。

#### 2-3 个性化消息
```python
In [16]: person_name = "Zed Zet"

In [17]: print("Hello %s,would you like to learn some Python today?" %person_name)
Hello Zed Zet,would you like to learn some Python today?
```

#### 2-4 调整名字的大小写
```python
In [18]: person_name = 'zed Zet'
In [22]: print("The Upper Case:%s;\nThe Lower Case:%s;\nThe Capital Case:%s" % (person_name.upper(),person_name.lower(),person_name.title()))
The Upper Case:ZED ZET;
The Lower Case:zed zet;
The Capital Case:Zed Zet
```
`lower()、upper()、title()`这几个简单的大小写处理方法是很有用的。

>我们就不必依靠用户提供正确的大小写了，只需全部转换到小写，以后需要显示的时候在以合适的大小写方式转换并显示出来。

我们自己能够模拟吗？当然可以！**随便写写，上代码**！
```python
In [23]: def low_write(string):
	         """
             return the lower case of the string
             """
	         string = list(string)
	         new = []
	         for i in string:
	           if 65 <= ord(i) <= 90:
	             i = chr(ord(i)+32)
	             new.append(i)
	           else:
	             new.append(i)
	         stri = "".join(new)
	         return stri
```
在交互式下写这个，而且复制过来好麻烦。不过比起用CMD命令行，我还是选择IPython！如果要写这么长的也可以用Jupyter Notebook。

我这里是用了一个列表，肯定还有更好的方法。**自己模拟一下Python的内置函数和方法是一个很好的学习途径**，它让我们对Python可能的运作机制会有更深的体会。这是《Python学习手册第四版》教我的！

同理，字符串全变成大写也一样。自己试一下吧！
> ASCII码使用指定的7位或8位二进制数组合来表示128 或256 种可能的字符。标准ASCII码也叫基础ASCII码，使用7位二进制数（剩下的1位二进制为0）来表示所有的大写和小写字母，数字0到9、标点符号， 以及在美式英语中使用的特殊控制字符。

> 其中：0～31及127(共33个)是控制字符或通信专用字符（其余为可显示字符），如控制符：LF（换行）、CR（回车）、FF（换页）、DEL（删除）、BS（退格)、BEL（响铃）等；通信专用字符：SOH（文头）、EOT（文尾）、ACK（确认）等；ASCII值为8、9、10和13分别转换为退格、制表、换行和回车字符。它们并没有特定的图形显示，但会依不同的应用程序，而对文本显示有不同的影响。

> 32～126(共95个)是字符(32是空格），其中48～57为0到9十个阿拉伯数字。

> **65～90为26个大写英文字母，97～122号为26个小写英文字母**，其余为一些标点符号、运算符号等。

#### 2-5 名言
```python
In [27]: nobility = "Albert Einstein"

In [29]: print("{0} once said, \"A\nperson who never made a mistake\nnever tried anything new.".format(nobility))
# 输出
Albert Einstein once said, "A
person who never made a mistake
never tried anything new.
```
这里用了.format方法，其字符串的大括号中可以使用索引，也可以使用**关键字参数**，下面就是例子之一。

#### 2-6 名言2
```python
In [34]: message = "{nobility} once said, \"I am become Death, the destroyer of worlds.\"".format(nobility = "J.Robert Oppenheimer")

In [35]: print(message)
J.Robert Oppenheimer once said, "I am become Death, the destroyer of worlds."
```

#### 2-7 剔除人名中的空白
三个字符串方法`lstrip()、rstrip()、strip()`，分别用于删除字符串左端、右端和两端的空白字符，包括制表符、空白符、换行符等，且**不管空白字符数有多少**。

> 这些剥除函数常用于在存储用户输入前对其进行清理。
```python
In [47]: f = '\tJ.Robert Oppenheimer\t\n\t '
In [48]: f.lstrip()
Out[48]: 'J.Robert Oppenheimer\t\n\t '

In [49]: f.rstrip()
Out[49]: '\tJ.Robert Oppenheimer'

In [50]: f.strip()
Out[50]: 'J.Robert Oppenheimer'
```

```python
# 文档
lstrip(...)
    S.lstrip([chars]) -> str

    Return a copy of the string S with leading whitespace removed.
    If chars is given and not None, remove characters in chars instead.
    # 仔细理解文档，当chars被给出的时候，将移除字符串首的属于chars中的字符，没有顺序
    
rstrip(...)
    S.rstrip([chars]) -> str

    Return a copy of the string S with trailing whitespace removed.
    If chars is given and not None, remove characters in chars instead.
    # 仔细理解文档，当chars被给出的时候，将移除字符串尾的属于chars中的字符，没有顺序
    
strip(...)
    S.strip([chars]) -> str

    Return a copy of the string S with leading and trailing
    whitespace removed.
    If chars is given and not None, remove characters in chars instead.
    # 仔细理解文档，当chars被给出的时候，将移除字符串首尾的属于chars中的字符，没有顺序
```

#### 2-8 数字8
```python
In [61]: print(4 + 4)
8

In [62]: print(2 * 4)
8

In [63]: print(16 / 2)
8.0

In [64]: print(10 - 2)
8
```
#### 2-9 最喜欢的数字
```python
In [65]: unfortunate_number = 13

In [66]: message = "I love the number %d very much" % unfortunate_number

In [67]: print(message)
I love the number 13 very much
```

#### 2-10 
> 作为新手，最值得养成的习惯之一就是，在代码中编写清晰、简洁的注释。

```python
# myRealization 18/04/28
# Do nothing
```

#### 2-11 Python之禅
```python
In [68]: import this
The Zen of Python, by Tim Peters

Beautiful is better than ugly.    
Explicit is better than implicit. 
Simple is better than complex.    
Complex is better than complicated.
Flat is better than nested.
Sparse is better than dense.
Readability counts.
Special cases aren't special enough to break the rules.
Although practicality beats purity.
Errors should never pass silently.
Unless explicitly silenced.
In the face of ambiguity, refuse the temptation to guess.
There should be one-- and preferably only one --obvious way to do it.
Although that way may not be obvious at first unless you're Dutch.
Now is better than never.
Although never is often better than *right* now.
If the implementation is hard to explain, it's a bad idea.
If the implementation is easy to explain, it may be a good idea.
Namespaces are one honking great idea -- let's do more of those!
```
```python
Python之禅 by Tim Peters
 
优美胜于丑陋（Python 以编写优美的代码为目标）
明了胜于晦涩（优美的代码应当是明了的，命名规范，风格相似）
简洁胜于复杂（优美的代码应当是简洁的，不要有复杂的内部实现）
复杂胜于凌乱（如果复杂不可避免，那代码间也不能有难懂的关系，要保持接口简洁）
扁平胜于嵌套（优美的代码应当是扁平的，不能有太多的嵌套）
间隔胜于紧凑（优美的代码有适当的间隔，不要奢望一行代码解决问题）
可读性很重要（优美的代码是可读的，即使是复杂的代码，也要编写有益的注释，让它易于理解）
即便假借特例的实用性之名，也不可违背这些规则（这些规则至高无上）
 
不要包容所有错误，除非你确定需要这样做（精准地捕获异常，不写 except:pass 风格的代码）
 
当存在多种可能，不要尝试去猜测
而是尽量找一种，最好是唯一一种明显的解决方案（如果不确定，就用穷举法）
虽然这并不容易，因为你不是 Python 之父（这里的 Dutch 是指 Guido ）
 
做也许好过不做，但不假思索就动手还不如不做（动手之前要细思量）
 
如果你无法向人描述你的方案，那肯定不是一个好方案；反之亦然（方案测评标准）
 
命名空间是一种绝妙的理念，我们应当多加利用（倡导与号召）
```
然而，this模块的代码完全违背了这些原则，把它贴出来：
```python
s = """Gur Mra bs Clguba, ol Gvz Crgref
 
Ornhgvshy vf orggre guna htyl.
Rkcyvpvg vf orggre guna vzcyvpvg.
Fvzcyr vf orggre guna pbzcyrk.
Pbzcyrk vf orggre guna pbzcyvpngrq.
Syng vf orggre guna arfgrq.
Fcnefr vf orggre guna qrafr.
Ernqnovyvgl pbhagf.
Fcrpvny pnfrf nera'g fcrpvny rabhtu gb oernx gur ehyrf.
Nygubhtu cenpgvpnyvgl orngf chevgl.
Reebef fubhyq arire cnff fvyragyl.
Hayrff rkcyvpvgyl fvyraprq.
Va gur snpr bs nzovthvgl, ershfr gur grzcgngvba gb thrff.
Gurer fubhyq or bar-- naq cersrenoyl bayl bar --boivbhf jnl gb qb vg.
Nygubhtu gung jnl znl abg or boivbhf ng svefg hayrff lbh'er Qhgpu.
Abj vf orggre guna arire.
Nygubhtu arire vf bsgra orggre guna *evtug* abj.
Vs gur vzcyrzragngvba vf uneq gb rkcynva, vg'f n onq vqrn.
Vs gur vzcyrzragngvba vf rnfl gb rkcynva, vg znl or n tbbq vqrn.
Anzrfcnprf ner bar ubaxvat terng vqrn -- yrg'f qb zber bs gubfr!"""
 
d = {}
for c in (65, 97):
    for i in range(26):
        d[chr(i+c)] = chr((i+13) % 26 + c)
 
print "".join([d.get(c, c) for c in s])
```
这段晦涩、复杂、凌乱的代码……emmmmm……肯定是反例吧！

***
## 第三章 列表简介
### 动手试一试
#### 3-1 姓名
```python
In [1]: numbers = ['Xu Mian', 'Zhou Zukang','Xu Yangzheng']
In [3]: for name in numbers:
	        print(name)

Xu Mian
Zhou Zukang
Xu Yangzheng
```
#### 3-2 问候语
```python
In [8]: for name in numbers:
	        message = 'Good Morning!'
	        print(name + ' ' + message)
   
Xu Mian Good Morning!
Zhou Zukang Good Morning!
Xu Yangzheng Good Morning!
```
#### 3-3 自己的列表
```python
In [9]: commute = ['bicycle', 'subway', 'bus', 'train']

In [10]: print('I hope to own a' + ' ' + commute[0])
I hope to own a bicycle

In [11]: print('I haven\'t take the %s to commute.' % commute[1])
I haven't take the subway to commute.

In [12]: print('I always take the school %s to commute.' % commute[2])
I always take the school bus to commute.

In [13]: print('I can\'t think we need to have a class with %s one day.' % commute[3])
I can't think we need to have a class with train one day.
```
#### 3-4 嘉宾名单
```python
In [17]: friends =  ['Xu Mian', 'Zhou Zukang','Xu Yangzheng']

In [18]: print('\n'.join(list(('I hope to share a supper with'+' '+friends[i] for i in range(len(friends))))))
I hope to share a supper with Xu Mian
I hope to share a supper with Zhou Zukang
I hope to share a supper with Xu Yangzheng
```
这里用了`join()`方法和`生成器表达式`，尽管不是太难懂，但是明显不太符合Python之禅的要求。可以改为等效的for循环，或直接一句一句的打印好了。

当然，原书这里还没有讲到循环，作者的意思估计是让我们一句一句的打印一系列的消息。

#### 3-5 修改嘉宾名单
```python
In [19]: print('Unfortunately. %s couldn\'t come tomorrow.' % friends[2])
Unfortunately. Xu Yangzheng couldn't come tomorrow.

In [20]: friends.remove('Xu Yangzheng')

In [21]: print(friends)
['Xu Mian', 'Zhou Zukang']

In [22]: friends.append('Cheng Jinjin')

In [23]: print(friends)
['Xu Mian', 'Zhou Zukang', 'Cheng Jinjin']

In [45]: for name in friends:
	         print('I hope to share a supper with ' + name +'.')
I hope to share a supper with Xu Mian.
I hope to share a supper with Zhou Zukang.
I hope to share a supper with Cheng Jinjin.	         	  
```
删除可以使用`remove()`、`pop()`和`del`，三者均对Python列表进行动态操作，修改该列表，其中remove需要知道要删除的值，pop默认抽出最后一个，也可以用索引。
```python
remove(...)
     L.remove(value) -> None -- remove first occurrence of value.
     Raises ValueError if the value is not present.

pop(...)
     L.pop([index]) -> item -- remove and return item at index (default last).
     Raises IndexError if list is empty or index is out of range.

__delitem__(self, key, /)
     Delete self[key].
```
这里的添加也可以使用`friends = friends + [name]`的形式，必须赋值，不然列表friends还是原来那样，或者`insert()`。
```python
insert(...)
      L.insert(index, object) -- insert object before index
```
#### 3-6 添加名单
```python
In [46]: friends.insert(0, 'Zhu Yuhuan')

In [47]: friends
Out[47]: ['Zhu Yuhuan', 'Xu Mian', 'Zhou Zukang', 'Cheng Jinjin']

In [48]: friends.insert(2, 'Xu Jing')

In [49]: friends
Out[49]: ['Zhu Yuhuan', 'Xu Mian', 'Xu Jing', 'Zhou Zukang', 'Cheng Jinjin']

In [50]: friends.append('Qiong Mei')

In [51]: friends
Out[51]:
['Zhu Yuhuan',
 'Xu Mian',
 'Xu Jing',
 'Zhou Zukang',
 'Cheng Jinjin',
 'Qiong Mei']

In [52]: for name in friends:
	         message = 'I hope to share dinner with %s' % name
	         print(message)
    
I hope to share dinner with Zhu Yuhuan
I hope to share dinner with Xu Mian
I hope to share dinner with Xu Jing
I hope to share dinner with Zhou Zukang
I hope to share dinner with Cheng Jinjin
I hope to share dinner with Qiong Mei

In [53]: print('Because I have found a large table!')
Because I have found a large table!
```
```python
# 文档
append(...)
      L.append(object) -> None -- append object to end
```
#### 3-7 缩减名单
```python
In [54]: print("Oh NO! Because the new table hasn't come, I have to share dinner with two friends!")
Oh NO! Because the new table hasn't come, I have to share dinner with two friends!

In [55]: print("I am sorry! I can't have dinner with you, %s." % friends.pop())
I am sorry! I can't have dinner with you, Qiong Mei.

In [56]: print("I am sorry! I can't have dinner with you, %s." % friends.pop())
I am sorry! I can't have dinner with you, Cheng Jinjin.

In [57]: print("I am sorry! I can't have dinner with you, %s." % friends.pop())
I am sorry! I can't have dinner with you, Zhou Zukang.

In [58]: print("I am sorry! I can't have dinner with you, %s." % friends.pop())
I am sorry! I can't have dinner with you, Xu Jing.

In [59]: for name in friends:
	         print("You still need to come! " + name)
    
You still need to come! Zhu Yuhuan
You still need to come! Xu Mian

In [60]: del friends[0:2]

In [61]: friends
Out[61]: []
```
```python
# 还可以使用clear()
clear(...)
     L.clear() -> None -- remove all items from L
```
#### 3-8 放眼世界
```python
In [66]: scenic_spot = ['Great Wall', 'Palace Museum', 'Beihai Park', 'Three Gorges', 'Crescent Spring']

In [67]: print(scenic_spot)
['Great Wall', 'Palace Museum', 'Beihai Park', 'Three Gorges', 'Crescent Spring']

In [68]: print(sorted(scenic_spot))
['Beihai Park', 'Crescent Spring', 'Great Wall', 'Palace Museum', 'Three Gorges']

In [69]: print(scenic_spot)
['Great Wall', 'Palace Museum', 'Beihai Park', 'Three Gorges', 'Crescent Spring']

In [70]: print(sorted(scenic_spot, reverse=True))  # 
['Three Gorges', 'Palace Museum', 'Great Wall', 'Crescent Spring', 'Beihai Park']

In [71]: print(scenic_spot)
['Great Wall', 'Palace Museum', 'Beihai Park', 'Three Gorges', 'Crescent Spring']

In [72]: scenic_spot.reverse()

In [73]: print(scenic_spot)
['Crescent Spring', 'Three Gorges', 'Beihai Park', 'Palace Museum', 'Great Wall']

In [74]: scenic_spot.reverse()

In [75]: print(scenic_spot)
['Great Wall', 'Palace Museum', 'Beihai Park', 'Three Gorges', 'Crescent Spring']

In [76]: scenic_spot.sort()

In [78]: print(scenic_spot)
['Beihai Park', 'Crescent Spring', 'Great Wall', 'Palace Museum', 'Three Gorges']

In [79]: scenic_spot.sort(reverse=True)  #

In [80]: print(scenic_spot)
['Three Gorges', 'Palace Museum', 'Great Wall', 'Crescent Spring', 'Beihai Park']
```
```python
# 文档
sort(...)
      L.sort(key=None, reverse=False) -> None -- stable sort *IN PLACE*

Help on built-in function sorted in module builtins:

reverse(...)
      L.reverse() -- reverse *IN PLACE*

sorted(iterable, /, *, key=None, reverse=False)
    Return a new list containing all items from the iterable in ascending order.

    A custom key function can be supplied to customize the sort order, and the
    reverse flag can be set to request the result in descending order.
```
关于`sorted()`和`sort()`的使用，还有一些地方书中没有提到，初级编程的时候也不一定要使用的技巧。


#### 3-9 晚餐嘉宾
```python
In [81]: friends = ['Zhu Yuhuan', 'Xu Mian', 'Xu Jing', 'Zhou Zukang', 'Cheng Jinjin']

In [82]: print("I have invited %d friends to share a supper with me." % len(friends))
I have invited 5 friends to share a supper with me.
```
#### 3-10 尝试使用各个函数
```python
# 略过
```

#### 3-11 有意引发一个错误
```python
In [93]: scenic_spot[5]
---------------------------------------------------------------------------
IndexError       Traceback (most recent call last)
<ipython-input-93-01622c5acc63> in <module>()
----> 1 scenic_spot[5]

IndexError: list index out of range
```
作者的提示很有道理，这里贴出来：
> 列表可能与你以为的截然不同，在程序对其进行了动态处理时尤其如此，通过查看列表或其包含的元素数，可帮助你找出这种逻辑错误。

***
## 第四章 操作列表
### 动手试一试
作者在4.1.1中的一个提示：
> **使用单数和复数式名称**，可帮助你判断代码段处理的是单个列表元素还是整个列表。

延伸一下，我们对类、模块使用名词，对方法和函数使用动词。

#### 4-1 比萨
```python
In [94]: pizzas = ['seafood pizza', 'sausage pizza', 'beef pizza', 'corn pizza']
In [95]: for pizza in pizzas:
             print(pizza)
seafood pizza
sausage pizza
beef pizza
corn pizza

In [97]: for pizza in pizzas:
	         print('I like %s.' % pizza)

I like seafood pizza.
I like sausage pizza.
I like beef pizza.
I like corn pizza.

In [100]: for pizza in pizzas:
	          print('I like %s.' % pizza)
	      print('I really love pizza! But I have never eaten one of them!')
     
I like seafood pizza.
I like sausage pizza.
I like beef pizza.
I like corn pizza.
I really love pizza! But I have never eaten one of them!			
```

#### 4-2 动物
```python
In [102]: for animal in animals:
	          print("A %s would not make a great pet." % animal)

A Lion would not make a great pet.
A Leopard would not make a great pet.
A Bear would not make a great pet.

In [103]: for animal in animals:
	          print("A %s would not make a great pet." % animal)
	      print("Any of thses animals can kill many of us.")

A Lion would not make a great pet.
A Leopard would not make a great pet.
A Bear would not make a great pet.
Any of thses animals can kill many of us.
```
之后学的是创建数字列表：

- 使用range()并创建数字列表；
- 对数字列表执行简单的**统计计算**；
- 列表解析；

#### 4-3 数到20
```python
In [2]: for i in range(1,21):
	         print(i , end = ' ')

1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
```
#### 4-4 一百万
```python
In [3]: for i in range(0,1000000):
	        print(i , end = ' ')
```
使用CTRL + C退出，会抛出`KeyboardInterrupt`异常，这也是少数不属于`Exception`的子类的异常。

#### 4-5 计算1-1000000的总和：
```python
In [9]: lst = list(range(1,1000001))

In [10]: min(lst)
Out[10]: 1

In [11]: max(lst)
Out[11]: 1000000

In [12]: sum(lst)
Out[12]: 500000500000

In [13]: sum(i for i in range(1,1000001))
Out[13]: 500000500000
```
这里只是一次性的使用列表，却要一次就生成所有的列表元素，很浪费内存，因此我们可以使用生成器表达式。
```python
# 文档
min(...)
    min(iterable, *[, default=obj, key=func]) -> value
    min(arg1, arg2, *args, *[, key=func]) -> value

    With a single iterable argument, return its smallest item. 
    The default keyword-only argument specifies an object 
    to return if the provided iterable is empty.
    With two or more arguments, return the smallest argument.

max(...)
    max(iterable, *[, default=obj, key=func]) -> value
    max(arg1, arg2, *args, *[, key=func]) -> value

    With a single iterable argument, return its biggest item. 
    The default keyword-only argument specifies an object 
    to return if the provided iterable is empty.
    With two or more arguments, return the largest argument.

sum(iterable, start=0, /)
    Return the sum of a 'start' value (default: 0) plus 
    an iterable of numbers

    When the iterable is empty, return the start value.
    This function is intended specifically for use with 
    numeric values and may reject non-numeric types.
```
对于这几个函数还有些不太明白的地方，果然，不写博客就不会知道自己的不足之处啊！！！
#### 4-6 奇数
```python
In [17]: odd_num = []

In [18]: for i in range(1,21,2):
	         odd_num.append(i)
    

In [19]: print(odd_num)
[1, 3, 5, 7, 9, 11, 13, 15, 17, 19]

In [20]: for i in odd_num:
	         print(i)
    
1
3
5
7
9
11
13
15
17
19
# 可以用列表解析
In [21]: odd_lst  = [i for i in range(1,21,2)]
In [23]: print(odd_lst)
[1, 3, 5, 7, 9, 11, 13, 15, 17, 19]
```
#### 4-7 3的倍数
```python
In [26]: thr = [i for i in range(3,31,3)]

In [27]: for i in thr:
	         print(i, end=' ')
3 6 9 12 15 18 21 24 27 30
```
#### 4-8 立方
```python
In [29]: for i in cube:
	         print(i, end=' ')
1 8 27 64 125 216 343 512 729 1000
```
#### 4-9 立方解析
```python
# 同上
```

接下来学的是使用列表的一部分：

- 切片；
- 遍历切片；
- 切片可以用来**复制列表**。

> 在很多情况下，切片都很有用；例如，写游戏时，可以在玩家退出游戏时将其最终得分加入到一个列表中，为获得该玩家的三个最高得分，可以降序排列，在创建一个只包含前三个得分的切片……编写web程序时，可以**使用切片来分页显示，并在每页显示数量适合的信息**。

#### 4-10 切片
```python
In [3]: my_foods = ['pizza', 'falafel', 'carrot cake', 'ice-cream']

In [4]: print("The first three items in the list are:",my_foods[:3])
The first three items in the list are: ['pizza', 'falafel', 'carrot cake']

In [5]: print("The first three items in the list are:",my_foods[-3:])
The first three items in the list are: ['falafel', 'carrot cake', 'ice-cream']
```
#### 4-11 你的披萨和我的披萨
```python
In [6]: pizzas = ['seafood pizza', 'sausage pizza', 'beef pizza', 'corn pizza']

In [7]: friend_pizzas = pizzas[:]

In [8]: pizzas.append("carrot pizza")

In [9]: friend_pizzas.append("falafel pizza")

In [10]: for pizza in pizzas:
	         print("My favorite pizzas are:", pizza)

My favorite pizzas are: seafood pizza
My favorite pizzas are: sausage pizza
My favorite pizzas are: beef pizza
My favorite pizzas are: corn pizza
My favorite pizzas are: carrot pizza

In [11]: for pizza in friend_pizzas:
	         print("My friend's favorite pizzas are:", pizza)

My friend's favorite pizzas are: seafood pizza
My friend's favorite pizzas are: sausage pizza
My friend's favorite pizzas are: beef pizza
My friend's favorite pizzas are: corn pizza
My friend's favorite pizzas are: falafel pizza
```
这里和题目要求的不太一样，不过也差不多。

>复制列表的一种极好的方法是**创建一个包含整个列表的切片**，方法是同时省略起始索引和终止索引([:])。这让Python创建一个始于第一个元素终于最后一个元素的切片，即复制整个列表。

#### 4-12 使用多个循环
```python
In [12]: my_foods = ['pizza', 'falafel', 'carrot cake']

In [13]: friend_foods = my_foods[:]

In [14]: print("My favorite food are:")
My favorite food are:

In [15]: for food in my_foods:
	         print(food, end=' ')
pizza falafel carrot cake

In [16]: print("My friend's favorite food are:")
My friend's favorite food are:

In [17]: for food in friend_foods:
	         print(food, end=' ')
pizza falafel carrot cake
```
---
接下来学的有：
> 列表非常**适合用于存储在程序运行期间可能变化的数据集**，列表是可以修改的，这对处理网站的用户列表或游戏中的角色列表至关重要。
 
 
- 定义元组；
- 遍历元组中的所有值；
- 修改元组变量（即给储存元组的变量重新赋值，这是合法的）；
> 相比于列表，元组是更简单的数据结构。**如果需要存储的一组值在程序的整个生命周期内都不变，可以使用元组**。

#### 4-13 自助餐
```python
In [19]: foods = ('carrot', 'cake', 'beef', 'milk', 'salad')

In [20]: foods[2] = 'CocaCola'
---------------------------------------------------------------------------
TypeError          Traceback (most recent call last)
<ipython-input-20-de82c7707f0a> in <module>()
----> 1 food[2] = 'CocaCola'

TypeError: 'tuple' object does not support item assignment

In [21]: foods = ('carrot', 'cake', 'soup', 'ham', 'salad')

In [22]: for food in foods:
	         print(food, end=' ')
carrot cake soup ham salad
```
接下来学的是设置代码格式：
> 随着你的程序越来越长，有必要了解一些代码格式设置约定。
> 请**花时间让你的代码尽可能易于阅读**；让代码易于阅读有助于你掌握程序是做什么的，也可以帮助他人理解你写的代码。

- PEP（Python Enhancement Proposal，Python改进提案）；

> **代码被阅读的次数比编写的次数多**……如果一定要在让代码易于编写和易于阅读将做出选择，Python程序员几乎总是会选择后者。

- 缩进；
- 行长；
- 空行；
- 其他格式设置指南

#### 4-14 PEP 8
访问[PEP  8](https://python.org/dev/peps/pep-0008/)，大致阅读一下PEP 8格式设置指南。

#### 4-15 代码审核
略。
> 每级缩进都使用四个空格，对你使用的文本编辑器进行设置，**使其在你按Tab键时都插入四个空格**。
> 每行都不要超过80字符，对你使用的文本编辑器进行设置，**使其在第80个字符处显示一条垂直参考线**。
> 不要过多的使用空行。

***
## 第五章 if语句

为了检查感兴趣的任何条件，我们要分解条件测试可以做到的各种情况，并且分别训练。本书这一节写得还不错。

这里学过的内容有

- 检查是否相等；
- 检查是否相等时不考虑大小写；
>网站**使用类似的方式来让用户输入的数据符合特定的格式**，如使用类似的测试来确保用户名是独一无二的，而非只是与另一个用户名的大小写不同。用户提交新的用户名时，转换成小写并与所有的既有用户名的小写版本比较。

- 检查是否不相等；
> 我们编写的大多数程序是检查相等的，但**有时检查不等效率更高**。

- 比较数字；
- 检查各种条件；
  - 使用and检查多个条件；
  - 使用or检查多个条件；

 > 为改善可读性，可以将每个测试都分别放在一对括号内；实在过于复杂时，可以**将该条件测试赋值给一个变量，注意变量命名**。

- 检查特定值是否包含在列表中；
> 有时，执行操作前必须检查列表来看是否包含特定的值，如结束用户注册过程时前，需要检查他提供的用户名是否已经包含在用户名列表中。
> 在地图程序中，可能要检查用户提交的位置是否包含在已知的位置列表中。
> 检查文本中有没有敏感词也是其用途之一。

- 检查特定值是否不包含在列表中；
> 有时确定特定的值未包含在列表中很重要；如有一个列表，包含被禁止在论坛上发表评论的用户，就可在允许用户提交评论前检查他**是否被禁言**。
- 布尔表达式。

### 动手试一试
#### 5-1 条件测试
略。
#### 5-2 更多的条件测试
略。

接下来学到的一些：

- 简单if语句；
- if-else语句；
- if-elif-else语句（依次检查每个条件测试，直到遇到通过了的条件测试……执行后跳过剩下的测试）；

#### 5-3 外星人颜色#1
```python
>>> alien_color = 'green'
>>> if alien_color == 'green':
	    print("You player got 5 points.")
You player got 5 points.

>>> alien_color = 'yellow'
>>> if alien_color == 'green':
	    pass

```
#### 5-4 外星人颜色#2
```python
>>> alien_color = 'green'
>>> if alien_color == 'green':
	    print("You player got 5 points.")
	else:
	    print("You player got 10 points.")
You player got 5 points.

>>> alien_color = 'red'
>>> if alien_color == 'green':
	    print("You player got 5 points.")
	else:
	    print("You player got 10 points.")
You player got 10 points.
```
#### 5-5 外星人颜色#3
```python
>>> alien_color = 'red'
>>> if alien_color == 'green':
	    print("You player got 5 points.")
    elif alien_color == 'yellow':
	    print("You player got 10 points.")
	else:
	    print("You player got 15 points.")
You player got 15 points.
```
#### 5-6 人生的不同阶段
```python
>>> age = 1
>>> if age < 2:
	    print("You are a baby.")
	elif age < 4:
	    print("You are making a toddler.")
	elif age < 13:
	    print("You are a child.")
	elif age < 20:
	    print("You are a teen.")
	elif age < 65:
	    print("You are an adult.")
	else:
	    print("You are an old man.")
You are a baby.
```
#### 5-7 喜欢的水果
```python
>>> favorite_fruits = ['apples', 'pears', 'watermelons']
>>> if 'apples' in favorite_fruits:
	    print("You really like apples!")
	if 'pears' in favorite_fruits:
	    print("You really like pears!")
	if 'peaches' in favorite_fruits:
	    print("You really like peaches!")
	if 'bananas' in favorite_fruits:
	    print("You really like bananas!")
	if 'grapes' in favorite_fruits:
	    print("You really like grapes!")
You really like apples!
You really like pears!
```
#### 5-8 以特殊的方式跟管理员打招呼
```python
>>> lst = ['admin', 'Eric', 'Tony', 'Bob', 'Tiger']
>>> for l in lst:
	    if l == 'admin':
	        print("Hello admin, would you like to see a status report?")
	    else:
	        print("Hello "+l+", thank you for logging in again!")
Hello admin, would you like to see a status report?
Hello Eric, thank you for logging in again!
Hello Tony, thank you for logging in again!
Hello Bob, thank you for logging in again!
Hello Tiger, thank you for logging in again!
```
#### 5-9 处理没有用户的情况
```python
>>> lst = []
>>> if not lst:
	    print("We need to find some users!")
	else:
	    for l in lst:
	        if l == 'admin':
	            print("Hello admin, would you like to see a stutas report?")
	        else:
	            print("Hello "+l+", thank you for logging in again!")
We need to find some users!
```
#### 5-10 检查用户名
```python
>>> current_users = ['John', 'Eric', 'Tony', 'Bob', 'Tiger']
>>> new_users = ['Titan', 'Frack', 'eric', 'Rider', 'tony']
>>> for new_user in [new_user.lower() for new_user in new_users]:
        if new_user in [current_user.lower() for current_user in current_users]:
            print("You need to use another user name!")
        else:
            print("This user name is not used!")
This user name is not used!
This user name is not used!
You need to use another user name!
This user name is not used!
You need to use another user name!
```
#### 5-11 序数
```python
>>> for i in list(range(1, 10)):
	    if i == 1:
	        print('1st')
	    elif i == 2:
	        print('2nd')
	    elif i == 3:
	        print('3rd')
	    else:
	        print(str(i)+'th')
1st
2nd
3rd
4th
5th
6th
7th
8th
9th
```

#### 5-12 设置if语句的格式
略。
#### 5-13 自己的想法
- 抓取网页制作成PDF……

## 第六章 字典
### 动手试一试
#### 6-1 人
```python
>>> familiar = {
    'first_name':'Zhtttty',
    'last_name':'zhaake',
    'age':12,
    'city':'Chongqin',
	}
>>> for key, value in familiar.items():
	    print(key.ljust(15), '=>', value)
	    
first_name      => Zhtttty
last_name       => zhaake
age             => 12
city            => Chongqin
```
#### 6-2 喜欢的数字
```python
>>> num = {
	    'zhangqisi':9,
	    'xuanjiexi':10,
	    'wangwuxia':13,
	    'qiandachu':100,
	}
>>> for key, value in num.items():
	    print(key, value)
zhangqisi 9
xuanjiexi 10
wangwuxia 13
qiandachu 100
```
#### 6-3 词汇表和OrderedDict
略。

这里还提到过按顺序遍历字典，也就是采用`for key in sorted(dict.keys())`的形式而已。

其实还可以采用`collections`中的`OrderedDict`:
```python
>>> from collections import OrderedDict
>>> cities = OrderedDict()
>>> cities[1] = 'ok'
>>> cities[2] = 'jk'
>>> cities[3] = 'fk'

>>> from pprint import pprint
>>> pprint(cities)
OrderedDict([(1, 'ok'), (2, 'jk'), (3, 'fk')])
>>> pprint(cities)
OrderedDict([(1, 'ok'), (2, 'jk'), (3, 'fk')])
```
两次打印结果的顺序都一样。

#### 6-4 词汇表2
```python
>>> program_words = {
	    'print':'print something to the standard console',
	    'input':'make people to input something',
	    'csv':'a format to store data with comma-seperate-file',
	    'dict':"python's data structure which can store key-value data",
	}
>>> for key, value in program_words.items():
	    print(key.ljust(6), '=>', value)
print  => print something to the standard console
input  => make people to input something
csv    => a format to store data with comma-seperate-file
dict   => python's data structure which can store key-value data
```
#### 6-5 河流
```python
>>> rivers = {
	    'nile':'egypt',
	    'yangtse':'china',
	    'yellow river':'china',
	}
>>> for river, country in rivers.items():
	    print('The '+river.title()+' runs through '+country+'.')
The Nile runs through egypt.
The Yangtse runs through china.
The Yellow River runs through china.
```
#### 6-6 调查
略。
#### 6-7 人
```python
>>> person1 = {
	    'first_name':'Zheyin',
	    'last_name':'zhake',
	    'age':12,
	    'city':'Chongqin',
	    }
>>> person2 = {
	    'first_name':'Xiny',
	    'last_name':'aiyuan',
	    'age':22,
	    'city':'Beijing',
	    }
>>> person3 = {
	    'first_name':'Ailian',
	    'last_name':'ruangui',
	    'age':32,
	    'city':'Zejiang',
	    }
>>> people = [person1, person2, person3]
>>> for person in people:
	    print(person['first_name'], person['last_name'], person['age'], person['city'])
Zheyin zhake 12 Chongqin
Xiny aiyuan 22 Beijing
Ailian ruangui 32 Zejiang
```
#### 6-8 宠物
同上一题，使用字典的列表，这里略。
#### 6-9 喜欢的地方
```python
>>> favorite_places = {
	    'Tianxi' : ['Yangtse  River'],
	    'Wanglei': ['The great wall', 'Tian Jing', 'The wall street'],
	    'Xumian' : ['Taohuayuan', 'Biquge']
	}
>>> for name, places in favorite_places.items():
	    print(name+"'s favorite places are")
	    for place in places:
	        print('\t'+place)
Tianxi's favorite places are
	Yangtse  River
Wanglei's favorite places are
	The great wall
	Tian Jing
	The wall street
Xumian's favorite places are
	Taohuayuan
	Biquge
```
#### 6-10 喜欢的数字
同上一题，使用列表的字典，这里省略。
#### 6-11 城市
```python
>>> cities = {
	    'choang':{
	        'country':'china',
	        'population':6661111,
	        'fact':'very hot'
	    },
	    'huawie':{
	        'country':'american',
	        'population':11133345,
	        'fact':'messy'
	    },
	    'xingwan':{
	        'country':'europe',
	        'population':47111,
	        'fact':'white men'
	    }
	}
>>> for cityname, city_info in cities.items():
	    print("Cityname: " + cityname)
	    print("\tCountry: " + city_info['country'])
	    print("\tPopulation: " + str(city_info['population']))
	    print("\tFact: " + city_info['fact'])

Cityname: choang
	Country: china
	Population: 6661111
	Fact: very hot
Cityname: huawie
	Country: american
	Population: 11133345
	Fact: messy
Cityname: xingwan
	Country: europe
	Population: 47111
	Fact: white men
```
#### 6-12 扩展：
略。

***
## 第七章 用户输入与while循环
#### 7-1 汽车租赁
```python
>>> car = input("Which kind of car do you want to rent?")
Which kind of car do you want to rent?Subaru

>>> print("Let me see if I can find you a Subaru")
Let me see if I can find you a Subaru
```
#### 7-2 餐馆订位
```python
>>> eating_people = int(input("用餐人数？"))
    if eating_people > 8:
	    print("没有空桌了！")
	else:
	    print("有空桌")
用餐人数？9
没有空桌了！
```
#### 7-3 10的整数倍
```python
>>> num = input("Please input an interger:")
	if int(num) % 10 == 0:
	    print("This number is integer multiple of ten.")

Please input an interger:20
This number is integer multiple of ten.
```

这里提到了while标志的事，在Pygame游戏编程里面，**标志**是经常被使用的一种技巧。
> 导致程序结束的事件有很多时，如果在一条while语句中检查所有这些条件，将既复杂又困难。
> 在要求很多条件都满足才继续运行的程序中，可定义一个变量，用于判断整个程序是否处于活动状态。这个变量被称为标志，充当了程序的交通信号灯。你可让程序在标志为`True`时继续运行，并在任何事件导致标志的值为`False`时让程序停止运行。
> 这样，在while语句中就**只需检查一个条件**——标志的当前值是否为`True` ，并将所有测试（是否发生了应将标志设置为`False`的事件）都放在其他地方，从而让程序变得更为整洁。

> 在复杂的程序中，如很多事件都会导致程序停止运行的游戏中，标志很有用：在其中的任何一个事件导致活动标志变成`False`时，主游戏循环将退出，此时可显示一条游戏结束消息，并让用户**选择是否要重新玩**。

#### 7-4  比萨配料
略。
#### 7-5 电影票
```python
while True:
    age = int(input("Please tell me your age:"))
    if age < 3:
        price = 0
    elif age <= 12:
        price = 10
    else:
        price = 15
    print("Your ticket price is %d dollars." % price)
    if input("?") == "quit":
        break
```
#### 7-6 三个出口
略。
#### 7-7 无限循环
```python
while True:
    print(1)
```

#### 7-8
```python
>>> sandwich_orders = ['三文鱼三明治', '鸡肉三明治','牛肉三明治']
>>> finished_sandwiches = []
>>> while sandwich_orders:
	    sandwich = sandwich_orders.pop()
	    print("我们做好了你的" + sandwich)
	    finished_sandwiches.append(sandwich)
	print(finished_sandwiches)
我们做好了你的牛肉三明治
我们做好了你的鸡肉三明治
我们做好了你的三文鱼三明治
['牛肉三明治', '鸡肉三明治', '三文鱼三明治']
```
#### 7-9 五香烟熏牛肉卖完了
```python
>>> sandwich_orders = ['三文鱼', '五香烟熏牛肉', '鸡肉', '五香烟熏牛肉', '牛肉', '五香烟熏牛肉']
	print("熟食店的五香烟熏牛肉卖完了!")
	while '五香烟熏牛肉' in sandwich_orders:
	    sandwich_orders.remove('五香烟熏牛肉')
	print(sandwich_orders)

熟食店的五香烟熏牛肉卖完了!
['三文鱼', '鸡肉', '牛肉']
```
这个练习就是反复用`dict.remove()`删除某个元素。
#### 7-10 梦想的度假圣地
```python
>>> survey = {}
>>> active = True
>>> while active:
	    name = input("What's your name?")
	    response = input("If you could visit one place in the world, where would you go?")
	    survey[name] = response

	    repeat = input("Would you like to let another person respond? (yes/ no) ")
	    if repeat == 'no':
	        active = False
	print("\n--- Survey Result---")
	for name, response in survey.items():
	    print(name + " would like to visit " + response + ".")

What's your name?ZZ
If you could visit one place in the world, where would you go?The great wall
Would you like to let another person respond? (yes/ no) no

--- Survey Result---
ZZ would like to visit The great wall.
```

***
## 第八章 函数
### 动手试一试
#### 8-1 消息
```python
def display_message(message):
    print("I learned %s in this chapter!" % message)

display_message('function')
```
#### 8-2 喜欢的图书
```python
def favorite_book(title):
    print("One of my favorite books is %s!" % title)

favorite_book('Alicc in Wonderland')
```
#### 8-3 T恤
```python
def make_shirt(size, word_style):
    print("This T-shirt is %s and has a word style of '%s'." % (size, word_style))

make_shirt('small', 'Alicc in Wonderland')
make_shirt(word_style='GayHub', size='large')
```
#### 8-4 大号T恤
```python
def make_shirt2(size='large', word_style="I love Python"):
    print("This T-shirt is %s and has a word style of '%s'." % (size, word_style))

make_shirt2()
make_shirt2(size='medium')
make_shirt2(size="small", word_style="PHP is the best programming language!")

```
#### 8-5 城市
```python
def describe_city(cityname, country="Iceland"):
    print("%s is in %s." % (cityname, country))

describe_city('Reyjavik')
describe_city('Beijing', 'China')
describe_city('New York', country='USA')
```
#### 8-6 城市名
略。
#### 8-7 专辑
略。无非是在函数中提供一个可选形参`song_num=None`即可。
#### 8-8 用户的专辑
略。即在每次用户输入时检查是否为`q`，是则退出while循环。
#### 8-9
```python
def show_magicians(magicians):
	pass
```
#### 8-10
```python
def make_great(magicians):
	pass
show_magicians(magicians)
make_great(magicians)
show_magicians(magicians)
```
#### 8-11
```python
def make_great(magicians):
	pass
show_magicians(magicians)
magicians_ver2 = make_great(magicians[:]) # 传入副本
show_magicians(magicians_ver2)
```
#### 8-12 三明治
```python
def describe_sandwich(*toppings):
	fo topping in toppings:
		pass

```
#### 8-13 用户简介
```python
def build_profile(first, last, **user_info):
"""创建一个字典，其中包含我们知道的有关用户的一切"""
	profile = {}
	profile['first_name'] = first
	profile['last_name'] = last
	for key, value in user_info.items():
		profile[key] = value
	return profile
user_profile = build_profile('albert', 'einstein', location='princeton', field='physics')
print(user_profile)
```
#### 8-14 汽车
略。同上一题基本一样。
#### 8-15 打印模型
略。
#### 8-16 导入
- `import  module_name`
- `from  module_name import  function_name`
- `from  module_name import  function_name as fn`
- `import  module_name as mn`
- `from  module_name import *`

五种导入语句，可以任选，不过最后一种少用为好。

***
## 第九章 类
### 动手试一试
#### 9-1 餐馆
```python
class Restaurant:
    def __init__(self, restaurant_name, cuisine_type):
        self.restaurant_name = restaurant_name
        self.cuisine_type = cuisine_type
    def describe_restaurant(self):
        print("This restaurant is %s and has cuisine type: %s" % (self.restaurant_name, self.cuisine_type))
    def open_restaurant(self):
        print("This restaurant %s is opening!" % self.restaurant_name)

restaurant = Restaurant("LinDa's Home", 'fish')
print(restaurant.restaurant_name, restaurant.cuisine_type)
restaurant.describe_restaurant()
restaurant.open_restaurant()
```
#### 9-2 三家餐馆
```python
class Restaurant:
    def __init__(self, restaurant_name, cuisine_type):
        self.restaurant_name = restaurant_name
        self.cuisine_type = cuisine_type
    def describe_restaurant(self):
        print("This restaurant is %s and has cuisine type: %s" % (self.restaurant_name, self.cuisine_type))
    def open_restaurant(self):
        print("This restaurant %s is opening!" % self.restaurant_name)

restaurant1 = Restaurant("LinDa's Home", 'fish')
restaurant1.describe_restaurant()
restaurant1.open_restaurant()

restaurant2 = …… # 分别调用它的两个方法
restaurant3 = ……
```
#### 9-3 用户
基本同上面的餐馆类一样，不过我寻思可以使用任意数量的关键字参数。
```python
class User:
    def __init__(self, first_name, last_name, **kwargs):
        self.first_name = first_name
        self.last_name = last_name
        if kwargs:
            self.args = kwargs
    
    def describe_user(self):
        print("This user is "+(self.last_name+' '+self.first_name).title()+'.')
        for k, v in self.args:
            print(k, '=>', v)
        
    def greet_user(self):
	    name = self.last_name+' '+self.first_name
        print('Hello,', name.title())
```
#### 9-4 就餐人数
```python
class Restaurant:
    def __init__(self, restaurant_name, cuisine_type, number_served):
        self.restaurant_name = restaurant_name
        self.cuisine_type = cuisine_type
        self.number_served = number_served
    def describe_restaurant(self):
        print("This restaurant is %s and has cuisine type: %s" % (self.restaurant_name, self.cuisine_type))
    def open_restaurant(self):
        print("This restaurant %s is opening!" % self.restaurant_name)
    def set_number_served(self, value):
        self.number_served = value
    def increment_number_served(self, value):
        if value > self.number_served:
            self.number_served = value
        else:
            print("The number of people who eat at this restaurant can't be rolled back")

restaurant = Restaurant("LinDa's Home", 'fish', 100)
print(restaurant.number_served)
restaurant.number_served = 1000
print(restaurant.number_served)

restaurant.set_number_served(4444)
print(restaurant.number_served)

restaurant.increment_number_served(555)
```
#### 9-5 尝试登录次数
```python
class User:
    def __init__(self, first_name, last_name, login_attempts, **kwargs):
        self.first_name = first_name
        self.login_attempts = login_attempts
        self.last_name = last_name
        if kwargs:
            self.args = kwargs

    def describe_user(self):
        name = self.last_name+' '+self.first_name
        print("This user is "+name.title()+'.')
        for k, v in self.args.items():
            print(k.ljust(8), '=>', v)

    def greet_user(self):
        name = self.last_name + ' ' + self.first_name
        print('Hello,', name.title())

    def increment_login_attempts(self):
        self.login_attempts = self.login_attempts + 1

    def reset_login_attempts(self):
        self.login_attempts = 0

us = User('qi', 'zhang', 3, age=22, height=177, weight=122)
us.greet_user()
us.describe_user()
us.increment_login_attempts()
print(us.login_attempts)
us.reset_login_attempts()
print(us.login_attempts)
```
#### 9-6 冰淇淋小店
```python
class IceCreamStand(Restaurant):
    def __init__(self, restaurant_name, cuisine_type, number_served, flavors):
        super().__init__(restaurant_name, cuisine_type, number_served)
        self.flavors = flavors

    def show_favors(self):
        for flavor in self.flavors:
            print(flavor+' ice-cream')
            
flavors = ['apple', 'peach', 'pear']
ice_cream_stand = IceCreamStand('Alice"s', 'Ice-Cream', 200, flavors)
ice_cream_stand.show_favors()
```
#### 9-7 管理员
```python
class Admin(User):
    def __init__(self, first_name, last_name, login_attempts, privileges, **kwargs):
        super().__init__(first_name, last_name, login_attempts, **kwargs)
        self.privileges = privileges

    def show_privileges(self):
        print("The Admin can do:")
        for privilege in self.privileges:
            print("=> ", privilege)

privileges = [ "can add post", "can delete post", "can ban user"]
admin = Admin('qi', 'zhang', 3, privileges)
admin.show_privileges()
```
#### 9-8 权限
```python
class Privileges:
    def __init__(self, privileges):
        self.privileges = privileges

    def show_privileges(self):
        print("The Admin can do:")
        for privilege in self.privileges:
            print("=> ", privilege)

class Admin(User):
    def __init__(self, first_name, last_name, login_attempts, privileges, **kwargs):
        super().__init__(first_name, last_name, login_attempts, **kwargs)
        self.privileges = Privileges(privileges)

privileges = [ "can add post", "can delete post", "can ban user"]
admin = Admin('qi', 'zhang', 3, privileges)
admin.privileges.show_privileges()
```
这里，**将一个类的实例组合到另一个类的属性中**，是个很好的技巧。
#### 9-9 电瓶升级
略。
####  9-10 导入Restaurant类
略。
#### 9-11 导入Adm类
略。
#### 9-12 多个模块
略。
#### 9-13 使用OrderedDict
已经用过，故省略。
#### 9-14 骰子
```python
from random import randint

class Die:
    def __init__(self, sides=6):
        self.sides = sides

    def roll_die(self):
        roll_num = randint(1, self.sides)
        print("您扔出了%d点的骰子。" % roll_num)

die = Die()
for i in range(10):
    die.roll_die()
print('\n')

die10 = Die(10)  # 10面的骰子
for i in range(10):
    die10.roll_die()
print('\n')

die20 = Die(20)  # 20面的骰子
for i in range(10):
    die20.roll_die()
```
#### 9-15 Python Module of the Week
有兴趣可以一观。

这里还有一点关于类编码风格的：
> **类名应采用驼峰命名法**，即将类名中的每个单词的首字母都大写，而不使用下划线。
> **实例名和模块名(还有函数名和变量名)都采用小写格式**，并在单词之间加上下划线。

***

## 第十章 文件和异常
### 本章内容
> 学会处理文件，让程序能够快速处理大量的数据；
> 学习错误处理，避免程序在面对意外情形时崩溃；
> 学习异常，异常是Python创建的特殊对象，用于管理程序运行时出现的错误；
> **学习模块json**，它让你保存用户数据，以免在程序停止运行后丢失。

这些技能可以**提高程序的适用性、可用性和稳定性**。

### 动手试一试
#### 10-1 Python学习笔记
略。
#### 10-2 C语言学习笔记
略。
#### 10-3 访客
```python
username = input("Please input your name: ")
if username:
	with open("guest.txt", 'w') as f:
		f.write(username)
```
#### 10-4 访客名单
```python
while True:
	username = input("Please input your name: ")
	if username:
		print("Hello, " + username)
		with open("guest.txt", 'a') as f:
			f.write(username + '\n')
	else:
		break
```
#### 10-5 关于编程的调查
略。和上一题雷同。

#### 10-6 加法运算
见下一题。

#### 10-7 加法计算器
```python
print("We can do addition with two numbers.")
while True:
	try:
		a = int(input("Please input the first number:"))
		b = int(input("The second number:"))
	except TypeError as t:
		print("Maybe you input the text rather than number.")
		continue
	else:
		print('%d+%d=%d' % (a, b, sum(a+b)))
		if input("Continue?") == 'q':
			break
```
#### 10-8 猫和狗
```python
filename = input("Please input the filename that you want to open(cats.txt or dogs.txt):")
try:
	with open(filename, 'r') as f:
		for line in f:
			print(line.rstrip())
except FileNotFoundError:
	msg = "Sorry, the file "+filename+" doesn't exist!"
	print(msg)
```

#### 10-9 沉默的猫和狗
```python
filename = input("Please input the filename that you want to open(cats.txt or dogs.txt):")
try:
	with open(filename, 'r') as f:
		for line in f:
			print(line.rstrip())
except FileNotFoundError:
	pass
```
#### 10-10 常见单词
略。不过[古腾堡](http://gutenberg.org/)的确有一些资源，以后可以去看看。

#### 10-11 喜欢的数字
```python
import json
num = input("Please input your favorite number:")
with open("favorite_num.txt", 'w') as f:
	json.dump(num, f)
#-----------------------------------------------------------
with open("favorite_num.txt", 'r') as f:
	your_num = json.load(f)
	print("I know your favorite number! It's " + your_num)
```
#### 10-12 记住喜欢的数字
```python
import json
# 如果以前存储了喜欢的数字，就加载它
# 否则，提示用户输入喜欢的数字
filename = 'favorite_num.txt'
try:
	with open(filename) as f:
		your_num = json.load(f)
except FileNotFoundError:
	favorite_num = input("Your favorite number? ")
	with open(filename, 'w') as f:
		json.dump(favorite_num, f)
		print("We remember your favorite number!")
else:
	print("Your favorite number is " + your_num)
```
#### 10-13 验证用户
```python
# 修改greet_user()函数即可
def greet_user():
	"""问候用户，并指出其名字"""
	username = get_stored_username()
	if username:
		msg = "Is this your name? "+username+" (y or n)"
	    if input(msg) == 'y': 
			print("Welcome back, " + username + "!")
		else:
			username = get_new_username()
			print("We'll remember you when you come back, " + username + "!")
	else:
		username = get_new_username()
		print("We'll remember you when you come back, " + username + "!")
```

## 第十一章 
> 程序员都会犯错，因此每个程序员**都必须经常测试其代码**，在用户发现问题前找出它们。

这一章我们将学到测试的内容，也是我所欠缺的：
- 使用Python模块 unittest 中的工具
- 学习**编写测试用例**，核实一系列输入都将得到预期的输出
- 学习**测试函数和类**，并将知道该为项目编写多少个测试

> 单元测试用于核实**函数的某个方面**没有问题。
> 测试用例是一组单元测试，这些单元测试一起核实函数在**各种情形**下的行为都符合要求。
> 全覆盖式测试用例包含一整套单元测试，涵盖了各种可能的函数使用方式。

### 动手试一试
#### 11-1 城市和国家
```python
# city_functions.py
def get_formatted_city_country(city, country, population):
    description = city + ', ' + country 
    return description.title() 

# test_cities.py
import unittest
from city_functions import get_formatted_city_country

class CityTestCase(unittest.TestCase):

    def test_city_country(self):
        city_country = get_formatted_city_country('santiago', 'chile')
        self.assertEqual(city_country, 'Santiago, Chile')

unittest.main()
```
一开始我是在Pycharm里面直接运行的，结果报错，而且还看不懂：
```python
======================================================================
ERROR: test_cities (unittest.loader._FailedTest)
----------------------------------------------------------------------
AttributeError: module '__main__' has no attribute 'test_cities'

----------------------------------------------------------------------
Ran 1 test in 0.001s

FAILED (errors=1)

以下省略……
```

后来我在Pycharm的终端控制台使用`python test_cities.py CityTestCase`运行(即运行`test_cities.py`脚本的`CityTestCase`类中的测试用例)，才成功：
```
.
----------------------------------------------------------------------
Ran 1 test in 0.001s

OK
```
查找了一下，有人说：
> 出现这个问题是因为打开的是文件夹，Pycharm作为一个project来打开的，这时候执行单元测试的脚本，就会出问题。
> 如果是作为文件打开，就是不打开文件夹，这样进行运行是没有任何问题的。
> 
> 但是为什么会出现这个问题，还没有发现更深的原因，猜测是和Pycharm的Project的内部处理有一定关系的。
> 所以解决方法有几个：
> 1. pycharm中单个打开文件，不要打开文件夹；
> 2. 换一个IDE，比如Notepad++等等


#### 11-2 人口数量
```python
# city_functions.py 增加population参数
def get_formatted_city_country(city, country, population):
    description = city + ', ' + country
    return description.title() + ' - population ' + str(population)
    
# 第一次测试
E
======================================================================
ERROR: test_city_country (__main__.CityTestCase)
----------------------------------------------------------------------
Traceback (most recent call last):
  File "test_cities.py", line 13, in test_city_country
    city_country = get_formatted_city_country('santiago', 'chile')
TypeError: get_formatted_city_country() missing 1 required positional argument: 'pop
ulation'

----------------------------------------------------------------------
Ran 1 test in 0.002s

FAILED (errors=1)

# city_functions.py 将形参population修改为可选 
def get_formatted_city_country(city, country, population=None):
    description = city + ', ' + country
    description = description.title()
    if population:
        description = description + ' - population ' + str(population)
    return description

# 第二次测试
.
----------------------------------------------------------------------
Ran 1 test in 0.000s

OK

# 再编写一个test_city_country_population()的测试
import unittest
from city_functions import get_formatted_city_country

class CityTestCase(unittest.TestCase):

    def test_city_country(self):
        city_country = get_formatted_city_country('santiago', 'chile')
        self.assertEqual(city_country, 'Santiago, Chile')

    def test_city_country_population(self):
        city_country_population = get_formatted_city_country('santiago', 'chile', population=5000000)
        self.assertEqual(city_country_population, 'Santiago, Chile - population 5000000')

unittest.main()

# 第三次运行测试
..
----------------------------------------------------------------------- 
Ran 2 tests in 0.001s

OK
```
#### 11-3 雇员
```python
# employee_class 待测试的类
class Employee:
    def __init__(self, last_name, first_name, annual_salary):
        self.last_name = last_name
        self.first_name = first_name
        self.annual_salary = annual_salary

    def give_raise(self, got_raise=5000):
        self.annual_salary += got_raise

# test_employee.py 测试用例
import unittest
from employee_class import Employee

class EmployeeTestCase(unittest.TestCase):
    def setUp(self):
        first_name = 'Jahn'
        last_name = 'Warm'
        self.employee = Employee(last_name, first_name, 5000)

    def test_give_default_raise(self):
        self.employee.give_raise()
        self.assertEqual(self.employee.annual_salary, 10000)

    def test_give_custom_raise(self):
        self.employee.give_raise(4444)
        self.assertEqual(self.employee.annual_salary, 9444)

unittest.main()

# 测试
..
----------------------------------------------------------------------
Ran 2 tests in 0.000s

OK
```

接下来还有几句提示：
> 作为初学者，并非必须为你尝试的所有项目编写测试；
> 但参与工作量较大的项目时，你应对自己编写的函数和类的重要行为进行测试。
> 请通过多开展测试来熟悉代码测试过程。


## 第一部分结束
好了，《Python从入门到实践》的第一部分我就完成了，其中大多数都是极其简单的问题，有些我都不想做了，所以一些题会有省略。

无论如何，接下来就开始第二部分的项目吧！
