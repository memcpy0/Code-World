# 复习Python第36天
## 习题 41:  来自 Percal 25  号行星的哥顿人(Gothons)
首先作者以如何阅读下面的代码为例，简介了一些关于阅读难懂代码的方法和经验。
```python2
cities['_find'] = find_city
city_found = cities['_find'](cities, state)
```
我们看书：
> 在磨练的过程中，我学会了下面三种读代码的方法，它们是用户几乎所有的编程语言：
1. 从前向后。
2. 从后向前。
3. 逆时针方向。
下次碰到难懂的语句时，你可以试试这三种方法。

确实是很好的经验。
> 本节代码很长，有168行，但其中大部分是print的游戏说明语句，我们主要就是关注其中使用的不懂的方法和函数，然后是整个程序的中转结构，特别是如何从一个房间到另一个房间的方法设计。
```python
from sys import exit
from random import randint

def death():
    quips = ["Your died. You kinda suck at this.",
            "Nice job, you died ...jackass.",
            "Such a luser.",
            "I have a small puppy that's better at this"]
    print quips[randint(0, len(quips)-1)]
    exit(1)

def central_corridor():
    print "The Gothnons of Planet Percal #25 have invaded yourship and destroyed"
    print "Your entire crew. You are the last surviving menber and your last"
    print "mission is to get the neutron destruct bomb from the Weapons Armory,"
    print "Put it in the bridge, and blow the ship uo after getting into an "
    print "ecape pod."
    print "\n"
    print "Your are runing down the central corridor to the Weapons Armory when"
    print "a Gothon jumps out, red scaly skin, dark grimy teeth, and evil clown costume"
    print "flowing around his hate filled body. He's blocking the door to the"
    print "Armory and about to pull a weapon to blast you."

    action = raw_input("> ")

    if action == "Shoot!":

        print "Quick on the draw you yank out your blaster and fire it at the Gothon."
        print "His clown costume is flowing and moving around his body, which throws"
        print "off your aim. Your laser hits his costume but missed him entirely. This"
        print "completely ruins his brand new costume his mother bought him, which"
        print "makes him fly into an insane rage and blast you repeatedly in the face until"
        print "you are dead. Then he eats you."
        return 'death'

    elif action == "dodge!":
        print "Like a world class boxer you dodge, weave, slip and slide right"
        print "as the Gothon's blaster cranks a laser past your head."
        print "In the middle of your artful dodge your foot slips and you"
        print "bang your head on the metal wall and pass out."
        print "You wake up shortly after only to die as the Gothon stomps on"
        print "your head and eats you."
        return 'death'

    elif action == "tell ajoke":
        print "Lucky for you they made you learn Gothon insults in the academy."
        print "You tell the one Gothon joke you know:"
        print "Lbhe zbgure vf fb sng, jura fur fvgf nebhaq gurubhfr, fur fvgf nebhaq gur ubhfr."
        print "The Gothon stops, tries not to laugh, then busts out laughing and can't move."
        print "While he's laughing you run up and shoot him square in the head"
        print "putting him down, then jump through the Weapon Armory door."
        return 'laser_weapon_armory'

    else:
        print "DOES NOT COMPUTE!"
        return 'central_corridor'

def laser_weapon_armory():
    print "You do a dive roll into the Weapon Armory, crouch and scan the room"
    print "for more Gothon that might be hiding. It's dead quiet, too quiet."
    print "You stand up and run the far side of the room and find the"
    print "neutron bomb in its container. There's a keypad lock on the box"
    print "and you need the code to get the bomb out. If you get the code"
    print "wrong 10 times then the lock closes forever and you can't"
    print "get the bomb. The code is 3 digits."

    code = "%d%d%d" %(randint(1,9), randint(1,9),randint(1,9))

    guess = raw_input("[keypad]> ")
    guesses = 0

    while guess !=code and guesses < 10:
        print "BZZZZEDDD!"
        guesses += 1
        guess = raw_input("[keypad]> ")

    if guess == code:
        print "The container clicks open and the seal breaks, letting gas out."
        print "You grab the neutron bomb and run as fast as you can to the"
        print "bridge where you must place it in the right spot."
        return 'the_bridge'

    else:
        print "The lock buzzes one last time and then you hear a sickening"
        print "melting sound as the mechanism is fused together."
        print "You decide to sit there, and finally the Gothons blow up the"
        print "ship from their ship and you die."
        return 'death'

def the_bridge():
    print "You burst onto the Bridge with the neutron destruct bomb"
    print "under your arm and surprise 5 Gothons who are trying to"
    print "take control of the ship.  Each of them has an even uglier"
    print "clown costume than the last.  They haven't pulled their"
    print "weapons out yet, as they see the active bomb under your"
    print "arm and don't want to set it off."

    action = raw_input("> ")

    if action == "throw the bomb":
        print"In a panic you throw the bomb at the group of Gothons"
        print "and make a leap for the door.  Right as you drop it a"
        print "Gothon shoots you right in the back killing you."
        print "As you die you see another Gothon frantically try to disarm"
        print "the bomb. You die knowing they will probably blow up when"
        print "it goes off."
        return 'death'

    elif action == "slowly place the bomb":
        print "You point your blaster at the bomb under your arm"
        print "and the Gothons put their hands up and start to sweat."
        print "You inch backward to the door, open it, and then carefully"
        print "place the bomb on the floor, pointing your blaster at it."
        print "You then jump back through the door, punch the close button"
        print "and blast the lock so the Gothons can't get out."
        print "Now that the bomb is placed you run to the escape pod to"
        print "get off this tin can."
        return 'escape_pod'

    else:
        print "DOES NOT COMPUTE!"
        return "the_bridge"

def escape_pod():
    print "You rush through the ship desperately trying to make it to"
    print "the escape pod before the whole ship explodes.  It seems like"
    print "hardly any Gothons are on the ship, so your run is clear of"
    print "interference.  You get to the chamber with the escape pods, and"
    print "now need to pick one to take.  Some of them could be damaged"
    print "but you don't have time to look.  There's 5 pods, which one"
    print "do you take?"

    good_pod = randint(1,5)
    guess = raw_input("[pod #]> ")

    if int(guess) != good_pod:
        print "You jump into pod %s and hit the eject button." % guess
        print "The pod escapes out into the void of space, then"
        print "implodes as the hull ruptures, crushing your body"
        print "into jam jelly."
        return 'death'

    else:
        print "You jump into pod %s and hit the eject button." % guess
        print "The pod easily slides out into space heading to"
        print "the planet below.  As it flies to the planet, you look"
        print "back and see your ship implode then explode like a"
        print "bright star, taking out the Gothon ship at the same"
        print "time.  You won!"
        eixt(0)

ROOMS = {
    'death': death,
    'central_corridor' : central_corridor,
    'laser_weapon_armory' : laser_weapon_armory,
    'the_bridge' : the_bridge,
    'escape_pod' : escape_pod
}

def runner(map, start):
    next = start

    while True:
        room = map[next]
        print "\n--------"
        next = room()

runner(ROOMS, 'central_corridor')
```
### 本节课代码知识
① `from sys import exit` ：从sys库中引入exit退出功能。

-  pydoc文档描述：
```python3
Help on built-in function exit in sys:

sys.exit = exit(...)
    exit([status])  # 状态码可选

    Exit the interpreter by raising SystemExit(status). 
    # 引发一个 SystemExit异常，若没有捕获这个异常，Python解释器会直接退出

	If the status is omitted or None, it defaults to zero (i.e., success).  
    # 默认为零，0为正常退出，其他数值（1-127）为不正常，可抛异常事件供捕获
    
    If the status is an integer, it will be used as the system exit status. 
    If it is another kind of object, it will be printed and the system exit status will be one (i.e., failure).
```
补充的一点是Python退出程序的第二种方式：os._exit()
```python3
Help on built-in function _exit in os:

os._exit = _exit(status)
    Exit to the system with specified status, without normal exit processing.
```
os._exit() 直接退出 Python程序，其后的代码也不会继续执行。

一般来说os._exit() 用于在线程中退出，sys.exit() 用于在主线程中退出。
 
② `from random import randint`：从random模组中引入randint即产生随机整型数。

- 关于random的常用方法文档：
```python3
 'betavariate', 'choice', 'choices', 'expovariate', 
 'gammavariate', 'gauss', 'getrandbits', 'getstate', 
 'lognormvariate', 'normalvariate', 'paretovariate', 'randint', 
 'random', 'randrange', 'sample', 'seed', 'setstate', 'shuffle', 
 'triangular', 'uniform', 'vonmisesvariate', 'weibullvariate']
```
- 其中randint的文档：
```python3
randint(a, b) method of Random instance  # 返回[a,b]范围内的随机整数
        Return random integer in range [a, b], including both end  points.
```

另外几个常用函数：

- random.choice()：从序列中获取一个随机元素，参数sequence表示一个有序类型。这里要说明的是：**sequence在python不是一种特定的类型，而泛指一系列的类型。**列表，元组，字符串都属于sequence。有关sequence可以查看python手册数据模型这一章。
```python
choice(seq) method of Random instance
        Choose a random element from a non-empty sequence.
```
-  random.random()：用于生成一个0到1（不包括1）的随机浮点数。
```python3
Help on built-in function random: 
random(...) method of random.Random instance 
    random() -> x in the interval [0, 1). 
```
- random.randrange()：从指定范围内按指定基数递增的元素集合中，获取一个随机数。
```python
randrange(start, stop=None, step=1, _int=<class 'int'>) 
method of Random instance
        Choose a random item from range(start, stop[, step]).

        This fixes the problem with randint() which includes the
        endpoint; in Python this is usually not what you want.
```
- random.sample()：从指定序列中随机获取指定长度的片断，sample函数不会修改原有序列。
```python3
sample(population, k) method of Random instance
        Chooses k unique random elements from a population sequence or set.

        The resulting list is in selection order so that all sub-slices will also be valid random samples.  
        
        This allows raffle winners (the sample) to be partitioned into grand prize and second place winners (the subslices).
		Members of the population need not be hashable or unique.  If the population contains repeats, then each occurrence is a possible selection in the sample.
        
        To choose a sample in a range of integers, use range as an argument.
        This is especially fast and space efficient for sampling from a large population: sample(range(10000000), 60)
```
- random.shuffle()：用于将一个列表中的元素打乱。
```python
shuffle(x, random=None) method of Random instance
        Shuffle list x in place, and return None. # 在原地打乱，返会None

        Optional argument random is a 0-argument function returning a random float in [0.0, 1.0); if it is the default None, the standard random.random will be used.
```
- random.uniform()：用于生成一个指定范围内的随机浮点数。
```python
uniform(a, b) method of Random instance
        Get a random number in the range [a, b) or [a, b] depending on rounding.
```
③本段代码从定义ROOMS开始：ROOMS是一个字典，包含了所有的房间（room），也就是游戏的地图，不是为了遍历而是为了来回跳转。

然后定义了一个runner函数，while True在这种时候就发挥作用了。

最后调用`runner(ROOMS, 'central_corridor')`，将ROOMS传给map，将`'central_corridor'`传给start，又将其传给next。

然后无限循环。先取出字典map(或ROOMS)中next指向的函数变量名对象，赋给room，之后用room调用函数，并将返回的字符串类型的结果赋给next。

room可以整齐地调用所有的房间函数，可以根据结果反复跳转，这不是遍历能做到的事。

**我个人觉得有必要记住这段代码。不过我对这种游戏没有太多兴趣。**

之后是作者提到的除了让每个函数打印自己外，学一下文档字符串式的注释，将房间描述写成文档之后串，然后打印出文档注释。

我只改了一小部分，很简单以其中一个函数为例：
```python2
def central_corridor():
    
   """
    The Gothons of Planet Percal #25 have invaded your ship and destroyed your entire crew.
    
    You are the last surviving member and your last mission is to get the neutron destruct bomb from the Weapons Armory,
    
    put it in the bridge, and blow the ship up after getting into an escape pod.
    
    You're running down the central corridor to the Weapons Armory when a Gothon jumps out,red scaly skin, dark grimy teeth, and evil clown costume flowing around his hate filled body. 
    
    He's blocking the door to the Armory and about to pull a weapon to blast you. 
   """
print central_corridor.__doc__
```
结果是：

![](http://img.blog.csdn.net/20180415100059739?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**这样就不用打印用户提示了**，而我们的程序函数就是一些if语句组合，而少了很多的print语句堆积。


### 有限状态机
> 有限状态机（Finite State Machine或者Finite State Automata)是软件领域中一种重要的工具，很多东西的模型实际上就是有限状态机。

[别人提到的](https://blog.csdn.net/thisinnocence/article/details/41171991)，可以去看看。