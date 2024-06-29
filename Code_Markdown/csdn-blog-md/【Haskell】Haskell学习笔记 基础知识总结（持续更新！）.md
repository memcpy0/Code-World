@[toc]


---
# 0. 简介和准备
Haskell是一门声明式、函数式的编程语言，相比起命令式编程，它的代码相当简洁、表达力强。一个示例是Haskell写的八皇后：
```haskell
safe :: Int -> [Int] -> Int -> Bool
safe _ [] _ = True
safe x (x1:xs) n = x /= x1 && x /= x1 + n && x /= x1 - n && safe x xs (n+1)

queens :: Int -> [[Int]]
queens 0 = [[]]
queens n = [ x:y | y <- queens (n-1), x <- [1..8], safe x y 1]
```
要做的准备是，按照[Windows10下VS Code配置Haskell语言环境](https://memcpy0.blog.csdn.net/article/details/118878150)这篇文章，在Windows下搭建一个VS Code+Stack+GHC的语言学习环境。

然后在VS Code中新建一个 `test.hs` ，并在其所处文件夹打开终端，输入 `stack exec ghci` 或者 `stack exec -- ghci` 或者 `stack --compiler ghc-8.0.2 exec ghci` 或者 `stack --compiler ghc-8.0.2 exec -- ghci` ，进入Haskell解释器环境。

---
# 1. 基础语法
## 函数
在 `test.hs` 中输入：
```haskell
doublex x = x + x
```
然后在REPL中导入这个Haskell文件，在终端使用这个 `doublex` 函数：
```haskell
Prelude> :l test
[1 of 1] Compiling Main             ( test.hs, interpreted )
Ok, modules loaded: Main.
*Main> doublex 5
10
```
类似的，`duoblex x` 可以看作是**函数名**和**传入的参数**， `=` 后面则是**函数返回值**。相当于：
```py
def doublex(x):
	return x + x
```
更进一步的，我们在函数 `doublexy` 中调用我们写的函数 `doublex` ：
```haskell
doublex x = x + x
doublexy x y = doublex(x) + doublex(y)
```
导入终端后，调用 `doublexy` 函数，传入 `5 4` ，相当于 `5 * 2 + 4 * 2 = 18` ：
```haskell
*Main> :l test
[1 of 1] Compiling Main             ( test.hs, interpreted )
Ok, modules loaded: Main.
*Main> doublexy 5 4
18
```
## 列表
列表的定义和Python差不多：
```haskell
numbers = [1, 3, 4, 5]
```
导入后输出这个列表：
```haskell
*Main> numbers
[1,3,4,5]
```
### 列表拼接 `++` 
在终端输入以下代码，发现输出了 `[1, 3, 4, 5, 7, 10]` ：

```haskell
*Main> [1, 3, 4, 5] ++ [7, 10]
[1,3,4,5,7,10]
```
Haskell中用 `"abcde"` 表示一个字符串，不过**字符串其实是列表的语法糖**，`"abcde"` 其实是 `['a', 'b', 'c', 'd', 'e']` 。更进一步，**列表本身也是一个语法糖**，`[1, 3, 4, 5]`  其实是 `1 : 3 : 4 : 5 : []` 的语法糖，`[]` 表示一个空列表。不信可以在终端输入代码：
```haskell
*Main> ['a', 'b', 'c']
"abc"
*Main> 1:3:4:5:[]
[1,3,4,5]
```
另外，`2 : [3, 4, 5]` 是正确的，而 `[2, 3, 4] : 5` 是错误的。
```haskell
*Main> 2 : [3, 4, 5]
[2,3,4,5]
*Main> [2, 3, 4] : 5 

<interactive>:16:1: error:
    ? Non type-variable argument in the constraint: Num [[t]]
      (Use FlexibleContexts to permit this)
    ? When checking the inferred type
        it :: forall t. (Num [[t]], Num t) => [[t]]
```
**在列表的头部插入元素，比在尾部插入元素快很多**。

### 列表取值 `!!`
`[...] !! t` 表示从列表中取出第 `t` 个元素，下标从零开始：
```haskell
*Main> [23, 7, 52, -1] !! 3
-1     
*Main> [23, 7, 52, -1] !! 0
23     
*Main> [23, 7, 52, -1] !! 9
*** Exception: Prelude.!!: index too large
```
### 列表比较
列表之间可以通过 `>, <, >=, <=` 来比较列表大小，标准是字典序。还可以通过 `==, !=` 来判断列表是否相等。

### 列表常用函数
有如下常用函数：
`head` 用于返回列表头部元素，`tail` 为列表去掉 `head` 后的部分。
`init` 返回列表去掉 `last` 后的部分，`last` 返回列表尾部元素。
`length` 返回列表长度，`null` 检查列表是否为空，
`reverse` 反转列表，`take` 用一个整数做参数，返回一个列表的前几个元素，`drop` 用一个整数做参数，返回删除前几个元素之后的列表，`cycle` 用一个列表做参数，返回又给由其无限循环组成的列表，`repeat` 用一个整数作为参数，返回由这个数无限循环组成的列表。
```haskell
*Main> numbers
[1,3,4,5]
*Main> head numbers
1
*Main> tail numbers
[3,4,5]
*Main> init numbers
[1,3,4]
*Main> last numbers
5
*Main> length numbers
4
*Main> null numbers
False
*Main> reverse numbers
[5,4,3,1]
*Main> numbers
[1,3,4,5]
*Main> take 2  numbers 
[1,3]
*Main> drop 2  numbers 
[4,5]
```

### 区间表示
`[1..20]` 会返回1~20的列表，`[2,4..30]` 会返回2~30中所有偶数的列表，`[1,3..]` 返回所有奇数——不过Haskell是**惰性求值**，所以不会真正生成无限多的元素，而是当你需要时求值，比如 `take 10 [1,3..]` 返回这个无限列表中的前十个元素。

```haskell
*Main> [1..20]
[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20]
*Main> [2, 4..30]
[2,4,6,8,10,12,14,16,18,20,22,24,26,28,30]
*Main> [1,3..11]
[1,3,5,7,9,11]
*Main> take 10 [1,3..]
[1,3,5,7,9,11,13,15,17,19]
```
## 列表推导
有点类似与Python的列表推导式，在列表中用一个 `|` 分隔开，前面写参数的表达式，后面用属于符号 `<-` 写参数的取值范围，Haskell会自动返回取值范围内、参数通过表达式计算出的值组成的列表。

比如 `[x * 2 | x <- [1..10]]` 返回一个2~20的偶数组成的列表；还可以写多个取值范围、逻辑表达式、if表达式：
```haskell
*Main> [x * 2 | x <- [1..10]]
[2,4,6,8,10,12,14,16,18,20]
*Main> [x | x <- [50..100], x `mod` 7 == 3] 
[52,59,66,73,80,87,94]
```
还可以写一个函数 `boomborg` ，传入一个列表参数 `ls` ，当参数 `x` 属于这个列表 `ls` 并且 `x` 是奇数时，使用参数的表达式——如果 `x < 10` 就生成 `boom` 、否则生成 `borg` ：

```haskell
boomborg ls = [if x < 10 then "boom" else "borg" | x <- ls, odd x]
```
导入后调用该函数，用列表做参数：
```haskell
*Main> :l test
[1 of 1] Compiling Main             ( test.hs, interpreted )
Ok, modules loaded: Main. 
*Main> boomborg [1..20]  
["boom","boom","boom","boom","boom","borg","borg","borg","borg","borg"]
```
