# 复习Python的第三十七天 

## 习题 42:  物以类聚
`将函数放到字典里是很有趣的一件事情`，这也是我们上节课所做的东西，我们可以通过键取出相应的房间函数：
```python
ROOMS = {
'death': death,
'central_corridor': central_corridor,
'laser_weapon_armory': laser_weapon_armory,
'the_bridge': the_bridge,
'escape_pod': escape_pod
}
```
但其实不必这样做的，或者说，我们完全可以让Python自动创建更棒的“函数字典”，这就是类。

本书无法让我们深入类，作者只建议我们`学会把它们当作高级的“函数字典”使用就可以`。而使用到类的语言被称作OOP（面向对象编程）。

**Python中类无处不在。**像`stuff` 这个变量其实是一个`list class` （列
表类）。而`' '.join(stuff)` 里调用函数`join`的字符串`' '`（就是一个空格）也是一个class —— 它是一个`string class`(字符串类)
> 本节代码1
```python2
class TheThing(object):  # 创建TheThing类，object不能省；
                         # 另外类名各单词首字母均大写
	def __init__(self):  # 初始化类的属性，给self设置了一个number属性
		self.number = 0  # 使用点记法为类属性赋值
	
	def some_function(self):  # 定义类中的方法
		print "I got called." # 方法(method)就是类中的函数，这就是为什么作者将类称作“函数字典”
	
	def add_me_up(self, more):
		self.number += more   
		return self.number

# two different things
# 通过将TheThing()赋给a和b创建了两个实例(instance)
a = TheThing()    
b = TheThing()

a.some_function()  # 对a调用TheThing类里的some_function函数，下同
b.some_function()

print a.add_me_up(20)  # 对a调用TheThing类里的add_me_up函数，下同
print a.add_me_up(20)
print b.add_me_up(30)
print b.add_me_up(30)
 
print a.number    # 打印当前a实例的number属性值，下同
print b.number
```
以下是运行结果：
```python
I got called.
I got called.
20
40 
30
60
40  # 类自动记录类属性的变动值
60
```
本书后面的解答很重要，像self和__init__的知识是必须要明白的。self承担着类中信息传递的重任，通过它，我们也才能实现` a.some_function()`，因为它把前者翻译成`some_function(a)`并执行。

就像前文提到的`list.append(item)`实际上是`append(list, item)`一样。

当我们通过赋值将类赋予给变量时，一个新的实例就被创建了，实例a具有TheThing类的一切行为和属性。它在类中的指代就是self。现在每当我们在类中通过self传递信息和充当函数参数时，都可以看做对a的操作。因为现在我们使用类中的函数时，都是将a作为隐性参数传入的。也就是说：
```python
def some_function(self): 
		print "I got called."

# 等同于
def some_function(a):
		print "I got called."
		
def add_me_up(self, more):  
		self.number += more   
		return self.number

# 此时等同于
def add_me_up(a, more):
        a.number += more
        return a.number
```
所以我们用`a.add_me_up(20)`时实际上是这样用的`add_me_up(a,20)`。

而list无疑也是类，当我们通过赋值创建一个列表lst的时候，也就创建了一个list的实例，在类中，lst将被self代表，所以我们的`lst.append(item)`在类中可能是定义为`def append(self, item)`这样的。

同样，当我们创建b实例的时候，我们又可以将self作为b的指代来在类中操作。
> 类是创建实例的工厂。

这句话说得真的很对。就像社会是创造我们的工厂一样，我们被作为人类这个类创造出来，因此我们都具有社会化的共通的self属性值（社会我），但我们经过不同的锤炼，就有了个人化的self属性值和行为（个体我）。类也是一样。

然后，关于__init__：
> 看到  `__init__`函数了吗？这就是你为 Python class 设置内部变量的方式。你可以使用  . 将它们设置到  self 上面。另外看到我们使用了`add_me_up()`为你创建的`self.number`加值。后面你可以看到我们怎样可以使用这种方法为数字加值，然后打印出来。

-
>以下是关于本节的习题代码：
>我只会贴上有限的几段，因为其他的都是同样的。
```python
from sys import exit
from random import randint

class Game(object):
	def __init__(self, start): # 初始化类的属性
		self.quips = [
					"You died. You kinda suck at this.",
					"Your mom would be proud. If she were smarter.",
					"Such a luser.",
					"I have a small puppy that's better at this."
					]
		self.start = start
	
	def play(self):
		next = self.start 
		
		while True:
			print "\n--------"
			room = getattr(self, next) # built—in function
			next = room()

	def death(self): # 通过self通信
		print self.quips[randint(0, len(self.quips)-1)]
		exit(1)

………………
# 创建了一个实例，实例的类中self代表a_game;
# 传入"…"被赋值给start，并通过self.start=start成为了self(即a_game)的特殊属性(游戏开局)
a_game = Game("central_corridor") 
a_game.play()
```

关键的地方有以下几点：
> 1. 怎样创建一个class Game(object) 并且放函数到里边去。
> 2. `__init__`是一个特殊的初始方法，可以预设重要的变量在里边。
> 3. 为class添加函数的方法是将函数在class下再缩进一阶，class的架构就是通过缩进实现的，这点很重要。
> 4. 你在函数里的内容又缩进了一阶。
> 5. 注意冒号的用法。
> 6. 理解self的概念，以及它在`__init__`、  play 、death里是怎样使用的。
> 7. 研究play里的getattr的功能，这样你就能明白play所做的事情。其实你可以手动在Python命令行实验一下，从而弄懂它。
> 8. 最后我们怎样创建了一个Game ，然后通过play() 让所有的东西运行起来。

#### 关于`getattr()`和`__dict__`
```python
Help on built-in function getattr in module builtins:
# 内建函数
getattr(...)
    getattr(object, name[, default]) -> value
    Get a named attribute from an object; 
    getattr(x, 'y') is equivalent to x.y  
    # 从对象中得到一个被命名的属性(包括类中的方法名、定义的变量名和self的属性)
    When a default argument is given, it is returned when the attribute doesn't exist; 
    without it, an exception is raised in that case.
```
英文很简单，不需要翻译吧。
然后是`__dict__`，它是类的属性，通过类的属性可以实现很多操作。
这里有详细的[介绍](https://blog.csdn.net/business122/article/details/7568446)，大家点过去就可以了。