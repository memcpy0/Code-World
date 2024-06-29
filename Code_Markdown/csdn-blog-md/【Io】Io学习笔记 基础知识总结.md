@[toc]


---
# 第三章 Io
## 3.1 Io简介
Io由Steve Dekorte在2002年编写解释器练手项目时发明，并且发展到了今天，依然小巧玲珑，**一刻钟学会语法、半小时学会基本原理都不是梦**。
![在这里插入图片描述](https://img-blog.csdnimg.cn/15cccdf260a74a7da6c06d0e8c256933.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
和Lua、JavaScript一样，Io是一种**原型语言**，也是一种带有**微型虚拟机**和**丰富并发特性**的可嵌入语言。核心优势是拥有大量可定制语法和函数、强力的并发模型，更要重点关注**简单语法**和**原型编程模型**。还要认真学习Io库，因为**这门语言的复杂性和丰富性都来自于库的设计**。

语言作者本人认为，Io充分简化的语义使其**更具灵活性**，即使核心的Io元素也不带有最简单的语法糖，让学习者能够**快速读懂和使用这些语法**，还可以**创作**一些实现Io语言时尚未定义的**全新基本元素**。但是Io的灵活性也让它应对许多常见用途时**速度较慢**，较少的语法也**让快速检查代码变得更加困难**。不过，Io在某些方面也**有很大的速度优势**（协程 `coroutine` 、异步套接字、SIMD支持等），比采用传统线程写的程序快很多。
> SIMD `Single Instruction Multiple Data` 单指令多数据流：指的是在同一时间对多个数据进行同一指令操作，以此实现并行化的技术。

## 3.2 第一天：逃学吧，轻松一下
Io的语法简单直接。**在Io中万事万物皆消息**，每条消息会返回另一个接收消息的对象，每条消息也都带有置于括号内的可选参数，Io语法只是把消息全部串联起来。

**Io中没有关键字**，只有一些行为上接近关键字的字符。

**Io中不必既操心类又操心对象，它不区分类和对象，或者说只有对象、只用操作对象，必要时复制一下对象**。被复制的对象就是**原型**。在Ruby中，可以通过定义类来创建一个新的对象种类，对某个类用 `new` 创建一个新对象。而在Io中，每个对象不是类的复制品，是实实在在的对象。此外，Io还能带我们无限接近面向对象的Lisp。


### 1. 开场白（语言基础入门）
打开解释器，输入经典程序，发送 `print` 消息给字符串对象，**接收者在左、消息在右**。没有任何语法糖，只是把消息发送给对象：
```haskell
Io 20110905
Io> "Hello, Io" print
Hello, Io==> Hello, Io
```
Io中没有类和对象之分，**通过复制现有对象（即原型）来创建新对象**，从而完成了面向对象语言中由类完成的工作。`Object` 是**根对象**，**发送 `clone` 消息过去会返回一个新对象**，然后把新对象赋值给 `Vehicle` 。`Vehicle` 不是用来创建对象的类或模板，它只是一个（基于 `Object` 原型的）对象。
```haskell
Io> Vehicle := Object clone
==>  Vehicle_0x2c761a8:
  type             = "Vehicle"
```
**每个对象都有一组槽** `slot` ，可以把这组槽认为是散列表，通过发送槽名消息（类似散列表的键）引用任何一个槽。我们可以用 `:=` 给槽赋值，不存在该槽时Io会创建一个槽（类似于C/C++中的赋值初始化）；或者用 `=` 给槽赋值，不存在槽时Io会抛出异常（类似于C/C++中的普通赋值，变量必须已经定义）。下面创建一个槽 `description` ，然后向 `Vehicle` 对象发送槽名获取槽中的值：
```haskell
Io> Vehicle description := "Something to take you far away"
==> Something to take you far away
Io> Vehicle nonexistingSlot = "This won't work."

  Exception: Slot nonexistingSlot not found. Must define slot using := operator before updating.
  ---------
  message 'updateSlot' in 'Command Line' on line 1

Io> Vehicle description
==> Something to take you far away
```
对象不只是一组槽，通过向 `Vehicle` 对象**发送 `slotNames` 消息查看对象上所有槽的名字**，返回的是一个槽名列表，其中有两个槽 `description, type` ：
```haskell
Io> Vehicle slotNames
==> list(description, type)
Io> Vehicle type
==> Vehicle
```
**Io中几乎所有对象都有 `type` 这个槽**，它在某种程度上代表你当前处理对象的种类，不过**该类型仍然是对象而不是类**。



---
### 2. 对象、原型和继承（类型模型）
一辆小汽车 `car` 自然也是一种交通工具 `vehicle` ，一个法拉利 `ferrari` 对象则是小汽车的实例。在面向对象语言中，我们用类、继承和对象完成这一点：
![在这里插入图片描述](https://img-blog.csdnimg.cn/13a6e8a5b5014686bead5d1b1e2dee27.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
在原型语言中，我们除了 `Vehicle` 对象外，还需要另外一些对象。先向 `Vehicle` 对象原型发送 `clone` 消息，创建一个新对象 `Car` ，它只有 `type` 槽没有 `description` 槽。接着把 `description` 消息发送给 `Car` ，**由于在 `Car` 中找不到该槽，Io会把 `description` 槽名消息转发给 `Car` 的原型 `Vehicle` ，并在 `Vehicle` 中找到这个槽**，这种消息转发机制简单却十分强大：
```haskell
Io> Car := Vehicle clone
==>  Car_0x2c72038:
  type             = "Car"

Io> Car slotNames
==> list(type)
Io> Car type
==> Car
Io> Car description
==> Something to take you far away
```
现在来一个 `ferrari` 对象，复制 `Car` 原型。发现它没有 `type` 槽，原因在于**Io中类型应以大写字母开头，不是类型的对象没有 `type` 槽，对它调用 `type` 槽会得到它原型的类型**：
```haskell
Io> ferrari := Car clone
==>  Car_0x2b9f1f0:

Io> ferrari slotNames
==> list()
Io> ferrari type
==> Car
```
如果让法拉利也变成类型，要用大写字母开头：
```haskell
Io> Ferrari := Car clone
==>  Ferrari_0x2bdd718:
  type             = "Ferrari"

Io> Ferrari slotNames
==> list(type)
Io> Ferrari type
==> Ferrari
```
这就是Io**对象模型**的工作方式。对象是槽的容器；发送槽名给对象可以获得该槽；如果该槽不存在，则调用父对象（对象原型）的槽。**Io中没有类、父类、元类，不会有接口或模块**，有的只有对象：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2aa32d147cf54072b9e74b30bf3fb423.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
对象还遵循一个简明的编程惯例（不是完整的语言特性）以区分"类型"和"实例"——以大写开头的对象是类型，Io对它设置 `type` 槽；**对象**（类型或者实例）的复制品如仍以大写字母开头就仍是类型，如以小写字母开头则没有 `type` 槽，对它调用 `type` 会转而调用它父对象的 `type` 槽（一般如此，在单例模式中会看到特例）。**其他方面，类型和实例的行为完全相同**。

在C++/Python/Ruby/Java中，类是用来创建对象的模板，`memcpy0 = Person.new` 从 `Person` 类创建一个新的 `Person` 对象 `memcpy0` ，`memcpy0` 和 `Person` 两个实体完全不同，一个是对象一个是类。而Io中 `memcpy0 := Person clone` 从 `Person` 原型对象创建一个复制品对象 `memcpy0` ，两者都是对象，只是 `Person` 还是类型还有 `type` 槽，其他方面两个实体完全相同。

---
### 3. 方法（原型编程范型基本原则总结）
**Io中方法也是对象**，我们用 `method()` 简单创建它、用 `type` 获取它的类型、把它赋值给一个槽。**如果某个槽是方法，调用这个槽会调用该方法**。对于实例调用该方法时，若不存在该槽，就会转发调用它父对象的方法：
```haskell
Io> method("So, you've come for an argument." println)
==> method(
    "So, you've come for an argument." println
)
Io> method() slotNames
==> list()
Io> method() type
==> Block
Io> Car drive := method("Vroom" println)
==> method(
    "Vroom" println
)
Io> Car drive
Vroom
==> Vroom
Io> ferrari slotNames
==> list()
Io> ferrari drive
Vroom
==> Vroom
```
我们不仅可以获取对象的槽名列表、对象的类型，还可以用 `getSlot` **获取槽中的内容**，对变量会获取变量值，对方法会获取方法定义：
```haskell
Io> ferrari getSlot("description")
==> Something to take you far away
Io> ferrari getSlot("drive")
==> method(
    "Vroom" println
)
```
用 `getSlot("type")` 也能获取对象的类型，如果该槽不存在，`getSlot` 会提供父对象的槽内容：

```haskell
Io> Car type
==> Car
Io> Car getSlot("type")
==> Car 
Io> ferrari type
==> Car
Io> ferrari getSlot("type")
==> Car
```
可以用 `proto` **获取对象的原型和自定义槽**，如下面用来复制得到 `ferrari, Car, Vehicle` 的原型分别是 `Car, Vehicle, Object` ：
```haskell
Io> ferrari proto
==>  Car_0x2c72038:
  drive            = method(...)
  type             = "Car"

Io> Car proto
==>  Vehicle_0x2c761a8:
  description      = "Something to take you far away"
  type             = "Vehicle"

Io> Vehicle proto
==>  Object_0x10324b8:
                   = Object_()
  !=               = Object_!=()
  -                = Object_-()
  ..               = method(arg, ...)
  <                = Object_<()
  <=               = Object_<=()
  ==               = Object_==()
  >                = Object_>()
  >=               = Object_>=()
  ?                = method(...)
  @                = method(...)
  @@               = method(...)
  actorProcessQueue = method(...)
  ...
  (有点长,自己运行自己看)
```
还有**主命名空间** `Lobby` ，包含所有的已命名对象，以及 `exit` 的实现、`forward` 、`Protos` 等等：
```haskell
Io> Lobby
==>  Object_0x29a0558:
  Car              = Car_0x2c72038
  Ferrari          = Ferrari_0x2bdd718
  Lobby            = Object_0x29a0558
  Protos           = Object_0x29a09d8
  Vehicle          = Vehicle_0x2c761a8
  _                = Object_0x29a0558
  exit             = method(...)
  ferrari          = Car_0x2b9f1f0
  forward          = method(...)
  set_             = method(...)
```

至此为止，Io的**核心组织原则**已经介绍完毕，包括：
- Io的基本语法，基础的消息转发机制；
- Io的类型模型，定义和使用类型对象和实例对象，尤其是继承自 `Object` 对象的 `getSlot, proto, slotNames, type` 等；
- 通过把内容赋值给对象的槽，可以在对象上添加数据和行为；
- 我们可以看见、改变任何槽或对象，进行一些复杂的元编程

现在可以总结**原型编程范型**的八条基本原则了：
- 所有**事物**皆是对象；
- 我们要做的不是实例化类，而是**复制那些原型对象**；
- 对象会记住它的原型；
- 对象有一组**槽**，是槽的容器；
- 槽中包含对象，包括一般的值和方法对象；
- 所有和对象的交互都是**消息**；
- 消息（即和对象的交互）要么返回槽中的值，要么调用槽中的方法；
- 如果对象无法响应某消息，它就会把该消息转发给自己的原型。


---
### 4. 列表 `List` 和映射 `Map`（核心数据结构）
有了前面的知识，使用列表和映射就很简单了。Io中有列表 `list` 可以存储任意类型的对象，**所有列表的原型都是 `List` 对象**，类似于Python的列表。Io还有映射 `map` ，**所有键值对映射的原型都是 `Map` 对象**，像Ruby的散列表、Python的字典。下面可以查看它们的槽名列表：
```haskell
Io> Map type
==> Map
Io> Map slotNames sort
==> list(addKeysAndValues, asJson, asList, asObject, at, atIfAbsentPut, atPut, detect,
empty, foreach, hasKey, hasValue, isEmpty, isNotEmpty, justSerialized, keys, map, merge, 
mergeInPlace, removeAt, reverseMap, select, size, values, with)

Io> List type
==> List
Io> List slotNames sort
==> list(ListCursor, append, appendIfAbsent, appendSeq, asEncodedList, asJson, asMap,
asMessage, asSimpleString, asString, at, atInsert, atPut, average, capacity, contains,
containsAll, containsAny, containsIdenticalTo, copy, cursor, detect, difference, 
empty, exSlice, first, flatten, foreach, fromEncodedList, groupBy, indexOf, 
insertAfter, insertAt, insertBefore, intersect, isEmpty, isNotEmpty, itemCopy, join, 
justSerialized, last, map, mapFromKey, mapInPlace, max, min, pop, preallocateToSize, 
prepend, push, reduce, remove, removeAll, removeAt, removeFirst, removeLast, 
removeSeq, rest, reverse, reverseForeach, reverseInPlace, reverseReduce, second, 
select, selectInPlace, setSize, size, slice, sliceInPlace, sort, sortBy, sortByKey, 
sortInPlace, sortInPlaceBy, sortKey, sum, swapIndices, third, union, unique, 
uniqueCount, with)
```
可以向 `List` 发送 `clone` 消息创建一个新列表，不过Io中创建列表有一个**快捷方式**：由 `Object` 对象提供的 `list` 方法，将把传入该方法的参数包装起来形成列表。列表中提供了多种方法，可以进行数学运算，或把列表用作其他数据类型如栈来使用：

```haskell
Io> toDos := list("find my car", "find Continuum Transfunctioner")
==> list(find my car, find Continuum Transfunctioner)
Io> toDos size
==> 2
Io> toDos max
==> find my car  
Io> toDos append("Find a present")
==> list(find my car, find Continuum Transfunctioner, Find a present)

Io> list(1, 2, 3, 4)
==> list(1, 2, 3, 4)
Io> list(1, 2, 3, 4) average
==> 2.5
Io> list(1, 2, 3, 4) sum
==> 10
Io> list(1, 2, 3) at(0)
==> 1
Io> list(1, 2, 3) at(3)
==> nil
Io> list(1, 2, 3) append(4)
==> list(1, 2, 3, 4)
Io> list(1, 2, 3) prepend(4)
==> list(4, 1, 2, 3)
Io> list(1, 2, 3) pop
==> 3
Io> list() isEmpty
==> true
```
Io中的 `Map` 没有语法糖，必须用API操作它：

```haskell
Io> elvis := Map clone
==>  Map_0x2b8fc08:

Io> elvis atPut("home", "Hunan Province")
==>  Map_0x2b8fc08:

Io> elvis at("home")
==> Hunan Province
Io> elvis atPut("style", "pop music")
==>  Map_0x2b8fc08: 

Io> elvis asList
==> list(list(home, Hunan Province), list(style, pop music))
Io> elvis keys
==> list(home, style)
Io> elvis values
==> list(Hunan Province, pop music)
Io> elvis size
==> 2
Io> elvis asJson
==> {"home":"Hunan Province","style":"pop music"}
```
Io对象的结构和散列表很相似，一个个绑定了值的槽不就是散列表的键吗？反过来说，散列表的键对应槽、值对应槽中的值，映射是键值对的组合对应对象是槽的容器。**岂能如此相似（笑）！** `Map` 对象甚至为此提供了将一个映射（键值对的组合）方便快捷转换为对象（槽的组合）的方法 `asObject` ：

```haskell
Io> elvis asObject
==>  Object_0x2b51e38:
  home             = "Hunan Province"
  style            = "pop music"

Io> sd := elvis asObject
==>  Object_0x3129fe8:
  home             = "Hunan Province"
  style            = "pop music"

Io> sd slotNames
==> list(home, style)
Io> sd type
==> Object
```
---
### 5. true、false、nil以及单例
在学习Io的控制结构之前，要先了解布尔值。和Ruby一样，Io中 `0` 是 `true`（C/C++中是 `false`）： 
```haskell
Io> true slotNames sort
==> list(asSimpleString, asString, clone, else, elseif, ifFalse, ifTrue,
justSerialized, not, or, then, type)
Io> false slotNames sort
==> list(and, asSimpleString, asString, clone, else, elseif, ifFalse, 
ifTrue, isTrue, justSerialized, not, or, then, type)
Io> nil slotNames sort
==> list(and, asSimpleString, asString, catch, clone, else, elseif, 
ifNil, ifNilEval, ifNonNil, ifNonNilEval, isNil, isTrue, justSerialized, 
not, or, pass, then, type)

Io> 4 < 5
==> true
Io> 4 <= 3
==> false
Io> true and false
==> false
Io> true and true
==> true
Io> true or true
==> true
Io> true or false
==> true
Io> 4 < 5 and 6 > 7
==> false
Io> true and 6
==> true
Io> true and 0
==> true
```
`true, false, nil` 都是对象，原型都是 `Object` ，而且这三者都是**单例** `singleton` ——**对它们进行复制，返回的都是单例对象自身的值**：

```haskell
Io> true proto
==>  Object_0x9827a8:
...
Io> false proto
==>  Object_0x9827a8:
...
Io> nil proto
==>  Object_0x9827a8:
...
Io> true clone
==> true
Io> false clone
==> false
Io> nil clone
==> nil
Io> T := true clone
==> true 
Io> T == true
==> true
Io> f := false clone
==> false
Io> f == false
==> true
```
**单例模式**是一种相当常见的软件设计模式，在面向对象语言中它保证**某个类只会有一个对象实例存在**，目的在于，**很多时候系统只需要一个全局对象、而不是每次都返回一个新的对象实例**。在Io这种原型语言中，单例模式保证**某个（类型或实例）对象始终只有自身存在，对其进行（类型或实例）复制返回的仍是该对象本身**，只是 `type` 槽的存在与否有些特殊情况。

在Io中实现单例模式是很简单的，对于 `Highlander` 这个类型对象，我们重定义它的 `clone` 方法，让它返回 `Highlander` 对象自身，然后分别复制出一个实例对象 `fred` 和一个类型对象 `Fred` ，发现**实例对象 `fred` 竟然有 `type` 槽，类型对象 `Fred` 调用 `type` 返回的不是 `Fred` 而是 `Highlander`** ，因为它们都是 `Highlander` 这个类型对象自身，只是名字各不相同：

```haskell
Io> Highlander := Object clone
==>  Highlander_0x312b818:
  type             = "Highlander"

Io> Highlander slotNames
==> list(type)
Io> Highlander type
==> Highlander
Io> Highlander clone := Highlander
==>  Highlander_0x312b818:
  clone            = Highlander_0x312b818
  type             = "Highlander"

Io> fred := Highlander clone
==>  Highlander_0x312b818:
  clone            = Highlander_0x312b818
  type             = "Highlander"
Io> fred slotNames
==> list(type, clone)
Io> fred type
==> Highlander

Io> Fred := Highlander clone
==>  Highlander_0x312b818:
  clone            = Highlander_0x312b818
  type             = "Highlander" 
Io> Fred slotNames
==> list(type, clone)
Io> Fred type
==> Highlander

Io> fred == Fred
==> true 
Io> fred == Highlander and Highlander == Fred
==> true
```

对 `lowlander` 这个实例对象，我们也一样让它返回 `lowlander` 对象自身，只是这回的结果有点诡异！我们分别复制出一个实例对象 `fred` 和一个类型对象 `Fred` ，发现**实例对象 `fred` 开始没有 `type` 槽，调用 `type` 返回的类型是 `Object`**（从 `fred -> lowlander -> Object` 一路转发）。但是**创建类型对象 `Fred` 后，`fred, lowlander` 有了 `type` 槽，对 `lowlander, fred, Fred` 分别调用 `type` 返回的不是 `Object` 而是 `Fred`** 。

原因是什么呢？我想，它们三个都是 `lowlander` 这个实例对象自身，只是名字各不相同。在创建类型对象 `Fred` 时实际上得到的是 `lowlander` 这个实例对象，由于Io语言机制要求对首字母大写的类型对象添加 `type` 槽，而这三者实为一体，于是 `lowlander, fred` “也”有了 `type` 槽，槽内容正好是 `Fred` ：

```haskell
Io> lowlander := Object clone
==>  Object_0x2c46418:

Io> lowlander slotNames
==> list()
Io> lowlander type
==> Object  
Io> lowlander clone := lowlander
==>  Object_0x2c46418:
  clone            = Object_0x2c46418

Io> fred := lowlander clone
==>  Object_0x2c46418:
  clone            = Object_0x2c46418
Io> fred slotNames
==> list(clone)
Io> fred type
==> Object 

Io> Fred := lowlander clone
==>  Fred_0x2c46418:
  clone            = Fred_0x2c46418
  type             = "Fred"
  
Io> Fred slotNames
==> list(type, clone)
Io> Fred type
==> Fred
Io> lowlander slotNames
==> list(type, clone)
Io> lowlander type
==> Fred
Io> fred slotNames
==> list(type, clone)
Io> fred type
==> Fred

Io> fred == Fred and Fred == lowlander
==> true

==>  Fred_0x2c46418:
  clone            = Fred_0x2c46418
  type             = "Fred"

Io> fred
==>  Fred_0x2c46418:
  clone            = Fred_0x2c46418
  type             = "Fred"

Io> Fred
==>  Fred_0x2c46418:
  clone            = Fred_0x2c46418
  type             = "Fred"
```
无论怎样，Io机制保证了仅有一个 `Highlander` 对象和 `lowlander` 对象存在。这种改变对象的 `clone` 方法以实现单例模式的做法，虽然有些古怪，但是相当简明优雅、令人神驰目眩。
> 尽管如此，你仍要多加小心。不管对Io是爱是恨，你都无法否认，**它是一门有趣的语言**。和Ruby一样，Io也能让人爱恨交加。你几乎可以改变任意对象上的任意一个槽，甚至对那些定义这门语言的对象也概莫能外。

比如说，如果我们一不小心覆盖了 `Object` 上的 `clone` 方法，那就无法再创建对象了，而且**无法修复、只有终止进程**。

---
### 6. 第一天学到了什么、第一天自习
第一天学到了Io的核心组织原则（基本语法、消息转发机制、类型模型等）、核心数据结构以及原型编程范型思想、布尔值、单例模式的实现。下面完成自习任务。

#### 找：Io示例问题、Io社区、Io指南
① Io的示例问题：
② 一个可解答问题的Io社区：

③ 带有Io惯用法的风格指南：
- 这里我给出的是 **`io guide`**  [https://iolanguage.org/guide/guide.html](https://iolanguage.org/guide/guide.html)。
- 它的 `tutorial` [https://iolanguage.org/tutorial.html](https://iolanguage.org/tutorial.html) 乱七八糟的，不用多看。
- 还有 **`reference`** [https://iolanguage.org/reference/index.html](https://iolanguage.org/reference/index.html) ，好像是Io中各种对象、数据结构的参数说明、用法示例。
- 然后是 **`package`** [https://iolanguage.org/packages.html](https://iolanguage.org/packages.html)，Io的库说明、支持框架。
- Io源代码仓库 [https://github.com/IoLanguage/io](https://github.com/IoLanguage/io)。

#### 答：四道题目
① 对 `1+1` 求值，然后对 `1 + "one"` 求值。Io是强类型还是弱类型？用代码证实你的答案。
**个人回答**：Io是弱类型的动态语言。
```haskell
Io> 1 + 1
==> 2
Io> 1 + "one"

  Exception: argument 0 to method '+' must be a Number, not a 'Sequence'
  ---------
  message '+' in 'Command Line' on line 1
```
② 0是true还是false？空字符串是true还是false？nil是true还是false？用代码证实你的答案。
**个人回答**：`0` 和空字符串都是 `true` ，`nil` 是 `false` ，它们都不等于 `true` 也不等于 `false` ：
```haskell
Io> true and 0
==> true
Io> false or 0
==> true
Io> 0 or 0
==> true

Io> true and ""
==> true
Io> false or ""
==> true
Io> "" or ""
==> true

Io> true and nil
==> false
Io> false or nil
==> false
Io> nil or nil
==> false
Io> nil or 0
==> true
Io> nil or ""
==> true

Io> 0 != true and 0 != false
==> true
Io> "" != true and "" != false
==> true
Io> nil != true and nil != false
==> true
```
③ 如何知道某个原型都支持哪些槽？
**个人回答**：对这个原型对象，发送 `slotNames` 消息即可。
 
④ `=`（等号）、`:=`（冒号等号）、`::=`（冒号冒号等号）之间有何区别？你会在什么情况下使用它们？


#### 做：从文件运行Io程序、执行槽中代码
1.从文件中运行Io程序。 
2.给定槽的名称，执行该槽中的代码。

---
## 3.3 第二天：香肠大王
在第二天中，我们将要了解到如何用Io的**槽**和**消息**构造核心行为。
### 1. 条件和循环（控制结构）
Io的所有条件语句也没有什么语法糖，
### 2. 运算符
### 3. 消息
### 4. 反射
### 5. 第二天我们学到了什么、第二天自习
> **如果你跟得上进度，第二天该算是突飞猛进的一天**。你对Io的了解应达到在**文档**的些许帮助下能完成基本任务的水平……**一定要把Io彻底弄明白**。否则我们后面进一步讨论Io，包括元编程和并发空间等问题时，你绝对会后悔当初怎么没完全掌握这些基本知识。

---
## 3.4 第三天：花车游行和各种奇妙经历
### 1. 领域特定语言
### 2. Io的 `method_missing`
### 3. 并发
### 4. 第三天学到了什么
### 5. 第三天自习

---
## 3.5 趁热打铁
### 1. 核心优势
### 2.  不足之处
### 3. 最后思考

