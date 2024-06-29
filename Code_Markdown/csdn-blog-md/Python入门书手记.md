##《编程小白的第一本Python入门书》
Python3的入门书，蛮简单的，**决定了，这几天看一下这本吧**。

![](//img-blog.csdn.net/20180429001526422?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

[TOC]

### 第二章 现在就开始
#### 使用IDE工具
在安装Python的IDLE环境后，还需配置一个程序员专属工具，作者推荐的Python IDE叫[PyCharm](https://www.jetbrains.com/PyCharm/)。

还有一些容易上手的pycharm视频:
1. 快速上手的[中文视频](http://v.youku.com/v_show/id_XODMyMzM1NzQ4.html)；
2. 官方快速[上手视频](https://www.jetbrains.com/pycharm/documentation/)
3. 系列[文档](https://pedrokroger.net/getting-started-pycharm-python-ide/)：可以在上手之后持续学习。

### 第三章 变量和字符串
- 开始学习编程
- 变量
- print()
> 作者的提醒：
> 
> 要保持良好的命名习惯**应该尽量使用英文命名**，学编程的同时还能同时背单词，岂不一举两得？**过一阵子你就会发现英文教程也会阅读得很顺畅！**

- 字符串
有这么一个场景：很多时候，我们使用手机号注册账号信息的时候，只显示后面四位，其余用*代替，我们也可以用字符串的方法完成这一个功能：
```python
In [11]: phone_number = input("Please input your 11-digits telephone number=>")
Please input your 11-digits telephone number=>……

In [12]: hiding_number = phone_number.replace(phone_number[:-4], '*' * 7)

In [13]: print(hiding_number)
*******7213
```
```python
replace(...)
   S.replace(old, new[, count]) -> str

   Return a copy of S with all occurrences of substring
   old replaced by new.  If the optional argument count is
   given, only the first count occurrences are replaced.
```
- 字符串格式符
```python
"""
最常见的三种使用方法，当字符串中有多个这样的空需要填写的时候，
我们用.format()方法进行批处理
"""
In [18]: print('{} a word she can get what she {} for.'.format('with', 'came'))
with a word she can get what she came for.

In [20]: print('{preposition} a word she can get what she {verb} for.'.format(preposition = 'with', verb = 'came'))
with a word she can get what she came for.

In [22]: print('{0} a word she can get what she {1} for.'.format('with', 'came'))
with a word she can get what she came for.
```
***
### 第四章 最基本的魔法函数
![](//img-blog.csdn.net/20180429144847346?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

- 开始创建函数
- 练习题
```python
# coding : utf-8
def weight_converter(g):
	"""
	input a number in g and return this number in kg.
	"""
	kilogram = g / 1000
	return kilogram

In [24]: weight_converter(1000000)
Out[24]: 1000.0
```
```python
def hypotenuse(right_angle_side1, right_angle_side2):
	"""
	input the length of two right angle side and give the length of the hypotenuse in the string. 
	But the result is an integer.
	"""
	from math import sqrt
	hypotenuse = sqrt(right_angle_side1 ** 2 + right_angle_side2 ** 2)
	print("The right triangle third side's length is %d." % hypotenuse)
```
- 传递参数和参数类型

#### 敏感词过滤 - 设计自己的函数
这是一个小型的文字过滤器，从中，我得到了一些启发：
> 函数名称和变量名可以写的很清楚简明；
> 函数最好小型化、独立化；
> 使用函数的目的不仅在于重用代码，更在于对问题的思考和拆解；
> 将函数**组合**起来完成整体的功能，某种意义上来说和类的组合模式很像，都从小到大形成了对一个问题的完整解决方案。
```python
def text_create(name, msg):
    desktop_path = 'C://Users/dell/Desktop/'
    full_path = desktop_path + name + '.txt'
    file = open(full_path, 'w')
    file.write(msg)
    file.close()
    print("Done!")

In [45]: text_create('hello', 'hello world')
Done!

def text_filter(word, censored_word = 'lame', changed_word = 'Awesome'):
    return word.replace(censored_word, changed_word)

In [47]: text_filter("Python is lame!")
Out[47]: 'Python is Awesome!'

def censored_text_create(name, msg):
	clean_msg = text_filter(msg)
	text_create(name, clean_msg)

In [49]: censored_text_create('Try', 'lame!lame!lame!')
Done!
```
***
### 第五章 循环与判断

- 逻辑判断
- 比较运算
- 成员运算和身份运算
  - `is` 和`is not`
  - `in` 和`not in`
#### 密码重置
- 条件控制：
```python
In [1]: password_list = ['*#*#', '12345']
# 可重置密码
In [2]: def account_login():
	        password = input('Password:')
	        password_correct = password == password_list[-1]
	        password_reset = password == password_list[0]
	        if password_correct:
	            print("Login success!")
	        elif password_reset:
	            new_password = input('Enter a new password:')
	            password_list.append(new_password)
	            print('Your password has changed successfully!')
	            account_login()
	        else:
	            print('Wrong password or invalid input!')
	            account_login()

In [3]: account_login()
Password:12345
Login success!

In [4]: account_login()
Password:3341
Wrong password or invalid input!
Password:*#*#
Enter a new password:123
Your password has changed successfully!
Password:1234
Wrong password or invalid input!
Password:123
Login success!
```
> 值得一提的是，如果if后面的布尔表达式过长或难于理解，可以采取给变量赋值的办法来存储布尔表达式返回的布尔值True和False。

**这段代码写得很优美，不是吗？**

- 循环(loop)
```python
In [7]: for i in range(1,10):
	        for j in range(1,10):
	           print("{0} * {1} = {2}".format(i, j, i * j),end = '|')
  
1 * 1 = 1|1 * 2 = 2|1 * 3 = 3|1 * 4 = 4|1 * 5 = 5|1 * 6 = 6|1 * 7 = 7|1 * 8 = 8|1 * 9 = 9|2 * 1 = 2|2 * 2 = 4|2 * 3 = 6|2 * 4 = 8|2 * 5 = 10|2 * 6 = 12|2 * 7 = 14|2 * 8 = 16|2 * 9 = 18|3 * 1 = 3|3 * 2 = 6|3 * 3 = 9|3 * 4 = 12|3 * 5 = 15|3 * 6 = 18|3 * 7 = 21|3 * 8 = 24|3 * 9 = 27|4 * 1 = 4|……|9 * 9 = 81|
```
```python
# account_login()
"""
append a new function to this account_login() func so that we can't input passwords more than three times.
"""

password = ['*#*#', '12345']
def account_login():
	tries = 3
	while tries >= 0:
		password = input('Password=>') 
		password_correct = password == password_list[-1]
		password_reset = password == password_list[0]
		
		if password:
			print('Login success!')
		elif password_reset:
			new_password = input('Enter a new password:')
			password_list.append(new_password)
			print('Password has changed successfully!')
			account_login()
		else:
			print('Wrong password or invaild input!')
			tries = tries - 1
			print(tries, 'times left!')
	else:
		print('Your account has been suspended') # 通知用户账户被暂停

account_login()
```
**练习题：**

- 题一：
```python
In [12]: def txt_created(path='C:\\Users\\dell\\Desktop\\'):
	         """
	         The func can be used to create new txt-file with digit-name
	         """
	         for name in range(1,11):
	             filename = path + str(name) + '.txt'
	             with open(filename, 'w') as f:
	                 f.close()

In [13]: txt_created()
```
- 题二
```python
In [20]: def invest(amount, rate, time):
	         """
	         This funct used to compute the total amount of the money every year.
	         """
	         print("Principal amount:", amount)
	         for year in range(time):
	             new_amount = amount * (1 + rate) ** (year + 1)
	             res = '${}'.format(new_amount)
	             print('year', year+1, ':', res)
    
In [21]: invest(100, 0.05, 5)
Principal amount: 100
year 1 : $105.0
year 2 : $110.25
year 3 : $115.76250000000002
year 4 : $121.55062500000003
year 5 : $127.62815625000003
```
- 题三
```python
In [22]: for even in range(2,101,2):
	         print(even,end = '\t')
    
2       4       6       8       10      12      14      16      18      20      22      24      26      28      30      32      34      36      38      40      42      44      46      48      50      52      54      56      58      60      62      64      66      68      70      72      74      76      78      80      82      84      86      88      90      92      94      96      98      100
```
> **如果你决心掌握编程这种魔法，实际上最需要的是，发展出设计和拆解事物的思路。所谓逻辑关系就是不同事物之间的关联性，它们以何种事物连接、作用，又在什么边界条件下实现转换或互斥。与其说编程有趣，倒不如说是引发的这种思考给开发者带来了乐趣。**

***
### 第六章 数据结构
 
- 列表(list)：可变的序列类型；可以容纳Python中的任意对象
```python
all_list = [
	1,
	1.0,
	'a word',
	True,
	[1,2],
	(1,2),
	{1:'the','key':'value'}
	print(1),
	input
	]
```
- 列表的增删改查

- 字典(Dictionary)：字典的键是唯一且不可变的，值是可变的且可以重复的，且值可以是任意对象。

- 字典的增删改查
`dict.update()`：添加多个元素的做法。

- 元组(Tuple)
- 集合(Set)：无序、不重复；不可切片和索引，可以添加和删除；可以说各有一部分列表和字典的特性。

- 数据结构的一些技巧
   - 列表推导式(list comprehension)：
```python
# time的使用
import time 

a = []
t0 = time.clock()  # 
for i in range(1,20000): 
	a.append(i)
print(time.clock() - t0 , "seconds process time") 

t0 = time.clock()  #
b = [i for i in range(1,20000)]
print(time.clock() - t0, "seconds process time")	
```
 - 循环列表时获取元素的索引：使用`enumerate()`
 
#### 综合项目-词频统计
```python
In [39]: import string
	     path = "C:\\Users\\dell\\Desktop\\Walden.txt"
	     with open(path, 'r') as text:
	         words =[raw_word.strip(string.punctuation).lower() for raw_word in text.read(). split()] # 
	         words_index = set(words) 
	         counts_dict = {index:words.count(index) for index in words_index}  #
	         for word in sorted(counts_dict, key=lambda x: counts_dict[x], reverse=True):  # 
	             print('{}-{} times'.format(word, counts_dict[word]))
```
这段代码写得有水平。

***
### 第七章 类与可口可乐

-  定义一个类

> 但凡是可以称之为一类的物体，他们都有相似的特征和行为方式，即类就是一些有着共同特征和行为事物的抽象概念的总和。

```python
In [38]: class CocaCola:
			formula = ['caffeine', 'sugar', 'water', 'soda']
```
> 在类里赋值的变量就是类的变量，而类的变量有一个专门的术语，`Class Attribute`类属性。

- 类的实例化
  - 类属性引用（Attribute References）: **类的属性会被所有类的实例共享。**
```python
In [39]: coke_for_me = CocaCola()
		 coke_for_you = CocaCola()

		 print(CocaCola.formula)
		 print(coke_for_me.formula)
		 print(coke_for_you.formula)

['caffeine', 'sugar', 'water', 'soda']
['caffeine', 'sugar', 'water', 'soda']
['caffeine', 'sugar', 'water', 'soda']
```
按配方将可乐生产出来的过程就是“实例化”。

![这里写图片描述](//img-blog.csdn.net/20180429204039704?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```python
class CocaCola:
	formula = ['caffeine', 'sugar', 'water', 'soda']

coke_for_China = CocaCola()
coke_for_China.local_logo = '可口可乐' # create an instance attribute

print(coke_for_China.local_logo)  # 打印实例属性引用结果
```
如下图：

![这里写图片描述](//img-blog.csdn.net/20180429204932856?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

- 实例方法
- 更多参数
```python
class CocaCola:
		formula = ['caffeine', 'sugar', 'water', 'soda']
	
	def drink(self, how_much):
		if how_much == 'a sip':
			print('Cool~' * 4)
		elif how_much == 'whole bottle':
			print('Headache!')

ice_coke = CocaCola()
ice_coke.drink('a sip')
```

- 魔术方法

> Python中的类存在一些方法，被称为“魔术方法”，`__init__()`就是其中之一。
>
> `__init__()`的神奇之处在于，如果你在类里定义了它，在创建实例的时候它能帮你自动处理很多事情，比如新增加的实例属性。
>
> 其实`__init__()`是initialize初始化的缩写，这也就意味着即使我们在创建实例的时候不去引用该方法，**其中的命令也会先自动的执行**。

```python
class CocaCola():
	formula = ['caffeine', 'sugar', 'water', 'soda']
	def __init__(self):
		self.local_logo = '可口可乐'
	def drink(self):
		print('Energy!') 

coke = CocaCola()
print(coke.local_logo)
```
改变一下：
```python
class CocaCola():
	formula = ['caffeine', 'sugar', 'water', 'soda']
	def __init__(self):
		for element in self.formula:
			print("Coke has {}!".format(element))
	
	def drink(self):
		print('Energy!')

In [2]: coke = CocaCola()
Coke has caffeine!
Coke has sugar!
Coke has water!
Coke has soda!
```

当然，`__init__`有自己的参数，不需要`obj.__init__()`这样调用（因为是自动执行），而是在实例化的时候在类的后面括号放进参数，相应的所有参数都会传入这个特殊的方法中，和函数的参数用法完全一致。

- 类的继承
```python
# 使用可口可乐网站上的配方重新定义这个类
class CocaCola():
	calories = 140
	sodium = 45
	total_carb = 39
	caffeine = 34
	ingredients = [
		'High Fructose from Syrup',
		'Carbonated Water',
		'Phosphoric Acid',
		'Natural Flavors',
		'Caramel Color',
		'Caffeine'
		]
	def __init__(self, logo_name):
		self.local_logo = logo_name
	
	def drink(self):
		print('You got {} cal energy!'.format(self.calories))
```
```python
class CaffeineFree(CocaCola):
	caffeine = 0
	ingredients = [
		'High Fructose from Syrup',
		'Carbonated Water',
		'Phosphoric Acid',
		'Natural Flavors',
		'Caramel Color'
		]
coke_a = CaffeineFree('Cocacola-FREE')
		
```

***
### 第八章 开始使用第三方库
#### 寻找第三方库
我们要学会如何根据需求找库。

**首先**是在[awesome-python.com](https://awesome-python.com/#http)上面找第三方库。

![这里写图片描述](//img-blog.csdn.net/20180430002014212?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**另外**，还可以通过搜索引擎来查找，甚至通过英文查找，如`python web crawling library`，来发现优质的资源。

#### 安装第三方库
##### 简单方法：在PyCharm中安装
在PyCharm中可以快捷的安装和卸载库。

##### 最直接方式：在终端安装

- pip
- conda

##### 最原始方式：手动

- pypi.org，搜索并下载
  - exe
  - .whl（**自动安装依赖的包**）
  - zip \ tar.zip \ tar.bz2

#### 安装自己的库
```python
import sys
print(sys.path[3]) ＃库的安装路径所在
```
将自己的库复制过去，而文件名将作为你引用的路径。