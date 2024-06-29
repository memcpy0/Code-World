@[toc]

---
# 2. 类型系统和函数
本节中简要了解Haskell中的**类型系统**和**函数**。

本来计算机中是没有类型这回事的，用户操作的变量在计算机眼中都是些二进制数，但在编程语言中需要将这些二进制数加以分类、便于处理，这就是类型 `type` 。

一种类型可以有1个值、多个值，甚至没有值（为空），可以是有限的、无限的，比如整数类型有无限个值，而布尔类型只有 `True` 和 `False` 两个值。

Haskell不仅对**二进制数**进行分类形成了类型，还对不同的**类型**进行了整理和分类，将有着共同属性的类型归为一种特定的**类型类** `typeclass` ，即类型的分类。整数类型和布尔类型都可以比较相等，比如 $1 \ne 2$、$\mathtt{True} == \mathtt{True}$ ，因此这两个类型被归于**相等类型类** `Eq` ，**用于判定一个类型的两个值是否相等**。整数和布尔类型还可以被归为**可显示类** `Show` ，因为它们都可以打印在命令行中（`Prelude` 预加载库中定义了**可显示类** `Show`）。

初步了解了Haskell的**类型系统**（类型和类型类）后，学习函数、$\lambda$ 表达式，然后是如何使用Haskell中的各种**表达式**来**定义函数**。在过程中，了解**表达式和函数的关系**、**函数和类型系统的关系**。

---
## 2.1 Haskell的数据和类型
Haskell是一种强类型编程语言，**每个数据都有严格精确的类型**。函数和数据没有本质区别，**函数也是数据的一种**，因此**函数也有类型**。
### 2.1.1 Haskell常用数据类型
这小节介绍常用的数据类型。注意，**数据类型的首字母都是大写的**，这是因为Haskell中规定**类型的名字和该类型的数据（类型构造器）的首字母要大写**，数的类型除外。

① 布尔类型 `Bool` ，**只有 `True, False` 两个值**。它的运算符号有逻辑与 `&&` 、逻辑或 `||` 、逻辑非 `not` 。查看 `True` 的类型，GHCi输出 `True :: Bool` ，意为 `True` 有着 `Bool` 类型：
```haskell
Prelude> True && False            
False
Prelude> True || False            
True
Prelude> not True                 
False
Prelude> :t True
True :: Bool
Prelude> :t False
False :: Bool
```

② 字符类型 `Char` ，**由单引号包裹的单个字符都是 `Char` 类型**，和C/C++一致。
```haskell
Prelude> 'c'
'c'
Prelude> :t 'a'
'a' :: Char
```
使用反斜杠 `\` 与ASCII码值的组合，也可以表示一个字符，比如 `97 ~ 122 : a ~ z`、`65 ~ 90 : A ~ Z` ：
```haskell
Prelude> '\100'
'd'      
Prelude> '\97'
'a'   
```
还有一些ASCII码字符用于控制输入和中断等，需要用反斜杠来转义，如退格键 `\b` 、换行符 `\n` 、制表符 `\t` 、空字符 `\&` 、反斜杠 `\\` 、双引号 `\"` 、单引号 `\'` 、`\ESC` 等等。如果是汉字或其他语言的字符，GHCi会返回对应的Unicode码点 `Unicode codepoint` ：
```haskell
Prelude> putStrLn "abc\&def\tghi\njk\blnm\"opq\'rst\\uvw\ESCxyz"
abcdef  ghi       
jlnm"opq'rst\uvwyz
Prelude> '我'
'\25105'
Prelude> '你'
'\20320'
Prelude> 'あ'
'\12354'
```

③ 有符号整数类型 `Int` ，**范围与操作系统和GHC位数有关**。32位GHC中有符号整数的范围是 $[-2^{31}, 2^{31} - 1]$ ，64位GHC中有符号整数的范围是 $[-2^{63}, 2^{63} - 1]$ 。在GHCi中输入以下代码，发现我用的是64位的GHC。`:: Int` 要求编译器把数作为**有符号整数**类型处理，不声明为 `Int` 时**Haskell会默认整数为任意精度整数类型**。$2^{63}$ 相当于在 $2^{63} - 1$ 加 $1$ 后转回到 $-2^{63}$ ，$2^{64}$ 相当于 $2^{63}$ 再加上 $2^{63}$ ，即 $-2^{63}$ 加上 $2^{63}$ ，结果是 $0$ 。
```haskell
Prelude> 2^32 :: Int
4294967296
Prelude> 2^63 :: Int
-9223372036854775808
Prelude> -2^63 :: Int
-9223372036854775808
Prelude> 2^64 :: Int
0
```
④ 无符号整数类型 `Word` ，**范围也是系统相关的**，类似于C语言中的 `unsigned int` 类型。32位GHC中无符号整数的范围是 $[0, 2^{32}  - 1]$ ，64位GHC中有符号整数的范围是 $[0, 2^{64} - 1]$ 。使用这一类型需要导入 `Data.Word` 库：
```haskell
Prelude> :m + Data.Word
Prelude Data.Word> -1 :: Word
18446744073709551615
```
虽然输入了 $-1$ ，但是因为值的类型是 `Word` ，所以下溢得到 $2^{64} - 1$ 。

⑤ 任意精度整数 `Integer` ，**只要计算机内存足够就可以表示任意大小的整数**，在编写RSA加密、大数运算等程序时十分必要，只是其性能无法比拟原生整数，**性能十分敏感时慎用**。可以尝试输入以下代码，计算 `1 ~ 10000` 所有整数的乘积：
```haskell
Prelude> product [1..10000]
Prelude> 2^32 :: Integer
4294967296
```
除了 `Int, Word, Integer` 外，Haskell还提供了 `Int8, Int16, Int32, Int64, Word8, Word16, Word32, Word64` ，即8位、16位、32位、64位有/无符号整数，其中无符号整数类型均定义在 `Data.Word` 中。这些类型只在一些特殊情况才会用到。

**Haskell中还可以在数值前加上 `0b, 0o, 0x` 前缀，使用二进制、八进制和十六进制来表示这些整数**，其中二进制表示需要GHC 7.10版本以上的 `BinaryLiterals` 语言扩展，因为Haskell 2010标准中没有规定二进制（在代码文件中使用二进制数值，需要在文件首部加上 `{-# LANGUAGE BinaryLiterals #-}`）：
```haskell
Prelude> :set -XBinaryLiterals --必须使用语言扩展
Prelude> 0b11111111
255
Prelude> 0o12
10
Prelude> 0x3a
58
```
⑥ 小数类型 `Float, Double` ，即单/双精度浮点数类型，和其他语言没有太大区别：
```haskell
Prelude> pi :: Float
3.1415927
Prelude> pi :: Double
3.141592653589793
```

⑦ 有理数类型 `Rational` ，**用两个任意精度整数作为分子和分母，来表示一个任意精度有理数**，在高精运算时很有用。下面代码中的 `%` 相当于分数线，左为分子，右为分母：
```haskell
Prelude> 4.1332 :: Rational
10333 % 2500
```

⑧ 字符串类型 `String` ，**在Haskell中字符串类型只是其他类型的别名**，它的类型定义为 `[Char]` ，即 `Char` 列表。输入一个字符列表，得到一个字符串（毕竟使用双引号更方便）：
```haskell
Prelude> ['h', 'e', 'l', 'l', 'o']
"hello"
Prelude> :t "Hello"
"Hello" :: [Char]
Prelude> :t ['h', 'e', 'l', 'l', 'o']
['h', 'e', 'l', 'l', 'o'] :: [Char]
```

⑨ 元组类型 `Tuple` ，在Haskell中**非常实用**，是非常重要的数据载体。可以从**数量**和**类型**两个维度归类元组。

二元元组用于需要一个**数对**来表示数据时，如整数坐标的类型可以写成 `(Int, Int)` ，就是一个元组 `Tuple` ，元组中的两个整数是该元组的**元件**  `Component` 。对于二元元组来说，有两个重要的函数 `fst, snd` ，分别返回元组里的第一、二个元件：
```haskell
Prelude> fst (5, True)
5
Prelude> snd (5, True)
True
Prelude> :t (5, True)
(5, True) :: Num t => (t, Bool)
```

不仅只有**二元元组**，还有**多元元组**，多元元组可以有很多个元件，**Haskell中最多支持62个元素的元组**。元组中的元件类型也可以互不不同，如 `(5, True, "Hello")` 。只不过，**元组中元件的个数一旦确定就不可伸缩，每个元件的类型也不能改变**。
```haskell
Prelude> ("memcpy0", "male", 22, "123456")
("memcpy0","male",22,"123456")
```
上述大的类型（布尔、字符、整数、浮点数、有理数、字符串、元组）是可以互相组合使用的，还可以声明复杂类型为一个新的类型，组合出更复杂的类型。


---
### 2.1.2 函数类型
#### (1) 函数类型的定义
**函数和普通数据没有本质区别，只是一种特殊的数据**，普通数据有类型，函数也有类型——由于函数是**从参数到结果的一个映射**，函数类型可以理解为**从一种（参数的）类型到另一种（结果的）类型的映射**，即 `T1 -> T2` （分清楚函数类型和返回值类型），**函数本身只是函数类型的值**。 

**所有函数都符合上述定义**，只是 `T1, T2` 可能更加复杂，甚至可能是函数类型——**`T1` 或 `T2` 若为函数，那么 `T1 -> T2` 函数就是高阶函数**  `higher order function` 。**但函数无论是一个怎样的类型，都符合函数类型的定义**。
 
比如定义一个整数加法函数，将两个参数以**二元元组**的形式作为函数输入，将一个**整数**结果作为函数输出。代码中的第一行 `(Int, Int) -> Int` 就是这个函数的**类型**，表示给定一个二元整数元组返回一个整数；第二行是这个函数的**定义**，给出一个二元元组 `(x, y)` ，返回 `x, y` 的和：
```haskell
-- 在文件中定义然后导入
add :: (Int, Int) -> Int
add (x, y) = x + y

-- 或者在命令行中定义
Prelude> let add (x, y) = (x + y) :: Int
Prelude> add (1, 2)
3
Prelude> :t add
add :: (Int, Int) -> Int
```
这就是Haskell中的函数类型和函数定义……如果到此为止就没什么意思，和C/C++差不多嘛，无非是把函数声明/定义拆开来，和使用类模板 `function` 相差无几：
```cpp
int add(int x, int y) {
	return x + y;
}
//写成:
std::function<int(int, int)> 
add = [](int x, int y) { return x + y; }
```
下文将从不同的特点介绍Haskell中的函数。
#### (2) 非柯里化函数和柯里化函数
非柯里化函数 `uncurried function` 是一种**函数类型**：
> **定义2.1.1** 当函数有多个参数时，必须通过元组一次性传入，然后返回结果。这样的函数就是非柯里化函数。

在定义非柯里化函数时，函数要多少参数我们就给多少元的元组。比如四个整数相加的函数类型：
```haskell
add4 :: (Int, Int, Int, Int) -> Int
```
柯里化函数 `curried funtion` 也是一种**函数类型**：
> **定义2.1.2** 当函数有多个参数时，参数可以一个一个依次输入，如果参数不足，将返回一个函数作为结果。这样的函数就是柯里化函数。

Haskell中的函数是一等公民，能够作为值返回。比如两个整数加法可以写成：
```haskell
-- 在文件中定义然后导入
add' :: Int -> Int -> Int
add' x y = x + y

-- 或者在命令行中定义
Prelude> let add' x y = x + y :: Int
Prelude> :t add'
add' :: Int -> Int -> Int
```
使用这样的函数，最大的好处是**符合数学习惯**，数学中经常使用这种函数。之前的 `add` 函数定义是 $f(x, y) = x + y$ ，如果给定第一个参数 $x = 3$ ，**希望这个函数变成一个一元函数** $f(3, y) = 3 + y$ 或者干脆写成 $f(y) = 3 + y$ 。使用元组很难做到，但是 `add'` 就可以解决：
```haskell
Prelude> :t add' 3 
add' 3 :: Int -> Int
```
`add' 3` 的类型意为，给 `add'` 传入一个 $3$ 会得到一个函数，这个函数还需要一个整数，我们再给一个整数就会返回最终的结果。即，**调用柯里化函数时如果参数不足，会得到一个还需要参数的函数**。当给定第一个参数到一个 $n$ 元函数，返回的结果会是一个 $n - 1$ 元函数。没有给出所有参数的函数应用称为函数的**不完全应用** `partial application` 或**偏函数调用**。 

不过，**将参数依次传入柯里化函数** `add'` 和**通过元组一次性将参数传入非柯里化函数** `add` ，二者实质上是等价的、能相互转换的。从非柯里化函数得到柯里化函数的过程，就是**柯里化** `curry` ，即 `add (x, y)` 到 `add' x y` ；从柯里化函数到非柯里化函数的过程，叫做**非柯里化** `uncurry` ，即 `add' x y` 到 `add (x, y)` 。`Prelude` 已经定义了这两个函数：
```haskell
Prelude> :t curry
curry :: ((a, b) -> c) -> a -> b -> c 
Prelude> :t (curry add) 3
(curry add) 3 :: Int -> Int  
Prelude> curry add 3 5
8

Prelude> :t uncurry
uncurry :: (a -> b -> c) -> (a, b) -> c    
Prelude> (uncurry add') (3, 5)
8
```

#### (3) 多态类型和多态函数
很多函数的参数不一定要求**具体类型** `concrete type`  ，比如元组类型的 `fst, snd` 函数，能返回任意二元元组中的第一个元件、第二个元件，元件的类型是无穷无尽的，如 `fst` 的类型是 `(a, b) -> a` ，即输入一个由任意 `a` 类型和任意 `b` 类型组成的元组、返回元组中的第一个元件作为结果：
```haskell
Prelude> fst (1, True)
1
Prelude> fst ([1, 2, 3], False)
[1,2,3]
Prelude> snd ("Hello Haskell", (0.75 :: Rational, 1.2))
(3 % 4,1.2)
Prelude> :t fst
fst :: (a, b) -> a
Prelude> :t snd
snd :: (a, b) -> b
```
这样可以应用在多种类型上的函数称为**多态函数** `polymorphic function` ，多态函数的定义如下，和C++的模板有点类似：
> **定义2.1.3** 一个函数的某个参数可以是任何类型的值，这个函数就是多态函数。

在Haskell中，我们用小写字母开头的单词表示这个任意的类型，如 `a, b, key` 等，它是类型中的**类型变量** `type variable` ，**在应用函数时可以接受并替换为任意的其他类型**（具体类型或多态类型）。总之，多态函数依赖于多态的类型变量（多态类型）。

Haskell中有很多多态函数。如 `length` 函数用于求出列表长度，忽略列表元素的具体类型，它在GHC 7.8前的类型是 `length :: [a] -> Int` ，类型变量 `a` 会在调用时由编译器根据**实际参数的类型**推导得出。

现在的类型则是 `length :: Foldable t  => t a -> Int` ，`Foldable` 是个类型类，以后再说， 可以直接理解 `Foldable t => t a` 为 `[a]` 。还有很多函数基于 `Foldable` 类型类，不基于它的函数被放在了 `GHC.OldList` 模块中。
```haskell
Prelude> :t length
length :: Foldable t => t a -> Int
Prelude> length [1, 2, 3, 4]
4
Prelude> length ['a', 'b', 'c']
3
```
通过隐藏新版本、新类型的函数，载入 `GHC.OldList` 模块可以使用GHC 7.8以前的函数：
```haskell
Prelude> import Prelude hiding (null, length, any, all) -- 还有其他函数如concat
Prelude> import GHC.OldList
Prelude GHC.OldList> :t null
null :: [a] -> Bool
Prelude GHC.OldList> :t length
length :: [a] -> Int
```
还有 `head` 函数，取列表的首元素，如列表为空则报错，类型是 `head :: [a] -> a` ：
```haskell
Prelude> :t head
head :: [a] -> a 
Prelude> head [1, 2, 3]
1
Prelude> head "Hello"
'H'
```
`zip` 函数可以将两个列表的元素一一对应合在一起、返回一个二元元组的列表，它的类型是 `zip :: [a] -> [b] -> [(a, b)]` 。当一个列表的元素更多时，只会以短列表的长度为准进行折叠：
```haskell
Prelude> :t zip
zip :: [a] -> [b] -> [(a, b)]
Prelude> zip [1, 2, 3] [4, 5, 6]
[(1,4),(2,5),(3,6)] 
Prelude> zip "abc" [1, 2, 3]
[('a',1),('b',2),('c',3)]
Prelude> zip "hello" [1, 2, 3]
[('h',1),('e',2),('l',3)]
```
#### (4) 重载类型函数
到这里为止，一切都很美好，我们有了数据、有了数据类型、也有了函数类型，还了解了多态函数、（非）柯里化函数等等。但是一个数 `5` 虽然被当成是整数 `Int` ，它还可以是一个任意精度整数 `Integer` ，是一个浮点数 `Float, Double` 、有理数，这样就有点不协调，我们对 `5` 查看类型时，它到底该是哪种类型呢？好像都不合适，因为 `5` 是一个有很多类型的值。

> **定义2.1.4** 重载的字面值 `overloaded literals` ：可以有多种类型的字面值，比如 `5, 7, 5.0, 7.0` 等等。`True, 'a'` 这些有着明确类型的值，不是重载的字面值。
> 
Haskell中用**类型类** `typeclass` 来对这些类型进行了细致的分类。`=>` 是类型类的限定符号，即 `5` 有任意一个 `t` 类型（整数、小数或其他类型），但这个类型必须限定在名为 `Num` 的类型类中，即**类型 `t` 必须是 `Num` 类型类的一个类型实例**，只有 `a` 符合这样的条件才可以使用相应数学函数，比如绝对值函数 `abs` 就需要一个实现了 `Num` 类型类的类型类实例 `a` ：
```haskell
Prelude> :t 5
5 :: Num t => t
Prelude> :t abs
abs :: Num a => a -> a
```
引入了类型类的概念，我们可以**限定参数的类型**，这样小数、整数等数类型都能计算绝对值，而布尔值、字符串不可以。像 `abs` 这样的函数就是**重载函数**，参数 `a` 受到 `Num` 类型类的限定（在声明时把类型类限定写到类型签名中 `=>` 的左侧），`a` 也可以叫做**受到类型类限定的（多态）类型**，或者**特定的多态**  `ad-hoc polymorphism` ——它们只能变成某些特定类型。
> **定义2.1.5** 重载函数：同一个名字的函数，可以根据类型或者参数数量的不同，有不同的实现。比如 `abs` 函数对 `Double` 和 `Int` 的实现就不同。

不过Haskell的重载函数，主要针对不同的参数类型进行重载，没有根据参数的数量进行重载。不像C++/Java，它们可以自由根据参数类型和数量进行重载，因为在调用函数时它们必须给出全部的参数，即非柯里化函数：
```cpp
int func(int para1, bool para2) {}
int func(int para1, char para2) {}
```
而Haskell中存在柯里化函数，给出部分参数后会返回函数作为结果，像调用 `func 5` 时，其类型就有两种结果 `Bool -> Int, Char -> Int` ，函数类型就不确定了！这违反了Haskell的规则——**函数和运算符的定义是唯一的，每个函数的类型（签名）有且只有一种**。因此，Haskell也不能像C++/Java一样定义多个不同参数类型和数量的同名函数来直接进行重载。需要重载时要用到类型类，**类型类是Haskell进行函数重载的重要手段**。
```haskell
func :: Int -> Bool -> Int
func para1 para2 = ...

func :: Int -> Char -> Int
func para1 para2 = ...
```
像Haskell中的 `print` 函数，就是让不同的类型实现 `Show` 类型类来完成这种重载功能。所有实现了 `Show` 类型类实例的类型，都可以用 `print` 函数输出在命令行上：
```haskell
Prelude> :t print
print :: Show a => a -> IO ()
```

---
### 2.1.3 类型的别名
为了便于使用，可以用 `type` 关键字统一将某个复杂类型命名为其他简单的名字，即**类型的别名** `type synonym` ，注意**类型名要以大写字母开头**。

```haskell
Prelude> type RGB = (Int, Int, Int)
Prelude> type Picture = [[RGB]]
```
在用来替换类型时，`type` 的功能类似于C/C++的 `define` （实际上 `type` 远远比不了 `define`），更准确地说是类似于 `typedef` ，**只能赋予现有类型以一个新的数据类型名，不能构造出新的数据类型**。

---
### 2.1.4 类型的重要性
强制用户使用类型与类型系统、不支持自由转型的目的和作用，无非是像C/C++/Java一样有如下的好处。现在可不能用Python作为借口了，不然 `Python Annotation` 是用来干什么的？
- **类型系统便于错误检查**。通过类型检查不能说明函数完全没有问题，但是类型错误就一定有问题。Haskell有强大的类型推断系统，可以自动推断一个函数的类型。但是提前在定义时给出函数或值的类型，类型推断系统就能在编译前做编译检查，一定程度保证定义的正确性，让用户少犯错误，犯错后更快找到错误的位置。
- **类型系统作为文档补充**。作为补充文档以增加程序的可读性，团队开发时用作程序员讨论的媒介，后期便于软件维护。
- **类型系统提供程序的抽象**。Haskell的抽象能力，相较C++更甚一筹，这离不开强大的类型系统，学习Monod时会领悟到这一点。

---
## 2.2 Haskell中的类型类
归为一个类型类的多个类型，有着共同的属性——所谓属性，指的是**该类型可以实现特定的函数**（像Java中的 `Interface` ）。
### 2.2.1 相等类型类 `Eq`


---
### 2.2.2 有序类型类 `Ord`

---
### 2.2.3 枚举类型类 `Enum`

---
### 2.2.4 有界类型类 `Bounded`

---
### 2.2.5 数类型类 `Num`


---
## 2.3 Haskell中的函数
### 2.3.1 Haskell中的值
Haskell中**所有变量的值在绑定后不会改变也无法改变**，基本可以认为是定值，**用户只能引用值而不能修改值**。随便创建一个 `.hs` 文件：
```haskell
-- a和b都是全局常量, 计算过程中a,b的值不会改变
a :: Int
a = 5
b :: Bool
b = True
```
然后导入到GHCi，无论如何应用，`a, b` 的值都是不变的：
```haskell
Prelude> 2 * a
10
Prelude> 1 + a
6
```
Haskell中值和函数是统一的，**函数的行为在运行过程中也不会发生改变，对某个特定的输入返回的总是确定的结果**。这就是纯函数。

### 2.3.2 函数思想入门
这节介绍定义这些纯函数的思想。首先，**函数是从参数到结果的一个映射**，将一个类型 `T1` 的值映射到另一个类型 `T2` 的值，值既指普通数据值，也包括函数。

**Haskell中的所有运算符也可以看作是函数**。比如加号 `+` 是一个需要两个参数的函数。将中缀运算符提前，像函数一样使用加号。`(+)` 的类型是 `Num a => a -> a -> a` ，说明这是一个柯里化函数，需要两个参数，这两个参数必须实现 `Num` 类型类，最后会返回一个数：
```haskell
Prelude> (+) 5 7
12
Prelude> :t (+)
(+) :: Num a => a -> a -> a
```
注意，`->` 是向右结合的符号，`(Num a) => a -> a -> a` 等同于 `(Num a) => a -> (a -> a)` ，第3个 `a` 先与最后一个 `a` 结合。给定了第一个参数后，将类型中第一个 `a` 消去，就得到另一个函数 `(5 +)` ，它的类型是 `(Num a) => (a -> a)` ，需要一个数作为参数来返回一个数字：
```haskell
Prelude> :t (+) 5
(+) 5 :: Num a => a -> a
Prelude> :t (+ 5)
(+ 5) :: Num a => a -> a
Prelude> :t (5 +)
(5 +) :: Num a => a -> a
```
我们再举几个例子来说明：
- 数学上：
	- 二元直角坐标系上的坐标表示为 `(x, y)` ，这一元组的类型是 `Num a => (a, a)` ；
	- 平面上的直线方程 `y = kx + b` ，`k, b` 为常量、`x` 为自变量，对于给定的 `x` 值总得到一个 `y` 值，类型是 `Num a => a -> a` ；
	- 求导数（微分）的过程，就是给定一个函数作为参数输入、然后返回一个函数作为结果的函数，即高阶函数，类型是 `derive :: Num a => (a -> a) -> (a -> a)` 。
	- 不定积分会返回一个曲线族，即返回一个函数和一个常数组成的元组，写成 `(Num a) => (a -> a) -> ((a -> a), C)` ，`C` 类型表示任意常数。不使用任意常数，可以理解积分会返回一个（无限长的）函数列表，类型是 `(Num a) => (a -> a) -> [(a -> a)]` 。
- 图像处理，对一个名为 `Picture` 的图片类型：
	- 对图片的水平翻转，实际是以一张图片为参数、处理后得到一张新图为结果的函数，类型是 `Picture -> Picture` ；
	- 对图片的锐化、模糊，需要一个修饰的程度值，再给定一张图片，返回处理后的图片，类型应该是 `Double -> Picture -> Picture` ；
	- 对图片的选择，给定一个旋转的角度和一张图片，返回旋转后的图片，类型是 `Int -> Picture -> Picture` 。
- 文件操作，比如文件类型的转换、文件的摘要、压缩、加密，都可以当作函数来处理。
- 编译器，可以理解为一个以程序代码字符串 `String` 为输入、得到可执行的机器语言作为输出的函数，由很多复杂函数复合而成，类型可以认为是 `String -> [Code]` ，`Code` 表示二进制指令。

这里演示一下微分、积分的示例，由于使用的是Stack，我们通过 `stack install` 分别下载 `numbers` 库实验求导、`integration` 库实验积分：
![](https://img-blog.csdnimg.cn/27ec28ce48b54c539329e37f2a696ce8.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
$f(x) = x^2$ 的导函数是 $f'(x) = 2x$ ，在 $x = 4$ 处的导数是 $f'(4) = 2 \times 4 = 8$ 。然后 `absolute` 函数用绝对误差来求积分，要求结果误差小于 $10^{-6}$ ，然后用并行梯形法 `parTap (parallel trapezoid)` 求 $\int^{\pi}_{\frac {\pi}{2}} sin (x) dx$ 的结果：
```haskell
Prelude> :m + Data.Number.Dif
Prelude Data.Number.Dif> let f x = x ^ 2
Prelude Data.Number.Dif> :t f
f :: Num a => a -> a
Prelude Data.Number.Dif> :t deriv f
deriv f :: (Eq b, Num b) => b -> b
Prelude Data.Number.Dif> deriv f 4
8

Prelude Data.Number.Dif> :m + Numeric.Integration.TanhSinh
Prelude Data.Number.Dif Numeric.Integration.TanhSinh> absolute 1e-6 (parTrap sin (pi / 2) pi)
Result {result = 0.9999999999999312, errorEstimate = 2.721789573237518e-10, evaluations = 25}
```

---
### 2.3.3 函数的基本定义格式
在Haskell中定义函数的方法：
- 第一行定义**函数名**和**函数类型**，称为**类型签名** `type signature` ；
- 第二行写**函数名**、**参数**、`=` ；
- 最后定义**函数体**

大致格式如下：
```haskell
函数名 :: 参数1的类型 -> 参数2的类型 -> ... -> 结果类型
函数名 参数1 参数2 ... = 函数体
```
`::` 用于指定函数（和值）的类型。如果简单理解类型为集合，可以认为 `::` 是 $\in$ ，`::` 后面就是函数（和值）的类型。如将 `Int -> Int` 作为函数的集合，那么阶乘、平方等函数都属于该集合。相同类型的函数可以定义在一起，函数名用 `,` 分隔：
```haskell
add, sub :: Int -> Int -> Int
add a b = a + b
sub a b = a - b
```
如果有多个类型类限定在一个类型上时，要么写在括号内、用 `,` 分隔，要么像柯里化一样使用 `=>` 连接：
```haskell
func :: (Show a, Ord a) => a -> a -> a
func2 :: Show a => Ord a => a -> a -> a
```
**Haskell中格式十分重要**：
- `.hs` 文件中，全局函数的名字要从每行顶端开始写；
- 函数名不能以大写的英文字母和数字开头，类型（类型构造器/类型数据）开头才需要用大写的字母；
- 写Haskell代码，最好设置 `Tab` 键自动换成空格，避免GHCi无法正确解析代码。

创建 `function.hs` ，写一些初等数学函数，如 $f(x) = 4x + 1$ 、求圆面积的函数 $area(r) = \pi r^2$ 、多元函数 $f(x, y) = 4x + 5y + 1$ 的非柯里化版本和柯里化版本：
```haskell
f :: Num a => a -> a
f x = 4 * x + 1

area :: Double -> Double
area r = pi * r ^ 2

f2 :: Num a => (a, a) -> a
f2 (x, y) = 4 * x + 5 * y + 1

f2' :: Num a => a -> a -> a
f2' x y = 4 * x + 5 * y + 1
```
在GHCi中调用这些函数：
```haskell
*Main> f 5
21
*Main> area 3
28.274333882308138 
*Main> f2 (2, 3)
24
*Main> f2' 2 3
24
```

---
## 2.4 $\lambda$ 表达式
### 2.4.1 $\lambda$ 表达式写法、$\alpha$ 替换、$\beta$ 化简、$\eta$ 化简
---
### 2.4.2 $\lambda$ 表达式的应用

---
### 2.4.3 参数的绑定
数学中求三角形面积常用海伦公式 `Heron's formula` ：
$$S = \sqrt {p(p-a)(p-b)(p-c)} \quad (p = \frac {a+b+c}{2})$$
由于 $p$ 的值只用定义一次，可以用 `let .. in ..` 在函数定义中做替换，应用函数时会自动替换 `p` 为 `(a + b + c) / 2` ：
```haskell
s :: Double -> Double -> Double -> Double
s a b c = let p = (a + b + c) / 2
		  in sqrt (p * (p - a) * (p - b) * (p - c))
```
除了绑定表达式到参数还可以绑定函数，多个绑定需要用分号隔开：
```haskell
Prelude> let f x = x + 1 in f 5
6
Prelude> let x = 2; y = 2 in x + y
4
```
`let .. in ..` 可以有多级，只是需要注意**命名捕获** `name capture` 的问题，下面的表达式中虽然是三个 `x` 相乘，但 `x` 的值是不等的：
```haskell
Prelude> let x = 6 in x * let x = 2 in x * x
24
```
除了 `let .. in ..` 之外，还有 `where` ，先定义好函数，然后在 `where` 关键字后定义参数 `p` 作为函数定义的**补充说明**（类似于上面的数学公式后的括号，对公式作补充）：
```haskell
s' :: Double -> Double -> Double -> Double
s' a b c = sqrt (p * (p - a) * (p - b) * (p - c))
		    where
		    	p = (a + b + c) / 2
```
`where` 可以像 `let .. in ..` 一样有多级，即 `where` 内定义的函数内又有 `where` 。要注意的是，**定义了一个函数并用来 `where` 绑定后，它在函数的全部范围内都是有效的**，而且不可能像 `let .. in ..` 一样用同一个变量的名字。

---
## 2.5 Haskell中的表达式
## 2.6 在GHCi中定义函数
在GHCi 8.0以前定义函数，要以 `let` 开头，这一章中我们用过好几次了：
```haskell
Prelude> let add (x, y) = (x + y) :: Int
Prelude> let add' x y = x + y :: Int
Prelude> let f x = x ^ 2
Prelude> let f x y = 4 * x + y ^ 2
```
GHCi 8.0以后，不需要用 `let` 开头定义函数：
```haskell
Prelude> multiply x y = x * y
Prelude> :t multiply
multiply :: Num a => a -> a -> a
Prelude> :{
Prelude| multiply :: Num a => a -> a -> a
Prelude| multiply x y = x * y
Prelude| :}
Prelude> multiply 3 5
15
```
模式匹配表达式一定要写为多选的形式，要在GHCi中用 `:{` 开头、`:}` 结尾（Haskell中各种麻烦的格式真多-_-||）：
```haskell
Prelude> :{
Prelude| let { foo :: Int -> Int
Prelude| ; foo 1 = 2
Prelude| ; foo 2 = 1
Prelude| ; foo n = 0 }
Prelude| :}
Prelude> foo 3
0
```
