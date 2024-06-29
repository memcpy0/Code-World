@[toc]

本文是中国大学慕课《Haskell函数程序设计基础》这门课程在PTA上的习题解答：
![在这里插入图片描述](https://img-blog.csdnimg.cn/88a57b3e62504484b9f33c77f203d177.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 0-1 乘3加1函数 (10 分) 
欢迎！请熟悉一下提交程序任务。多数题目只要求提交给定函数的定义，包括函数的类型。 在这里要求实现一个乘3加1函数：$f\ x=3x+1$。

**函数接口定义：**

```haskell
f :: Integer -> Integer
-- 注意，上面的函数名f及其类型通常是给定的，不可更改。
f x = 3*x + 1
```

在提交函数定义前，先在本地检查自己定义的函数是否给出正确结果。 例如，在解释器中计算： 
```haskell
Main>f 3
```

试着将以上定义提交给系统。

下面说明系统如何测试你的程序。你可以暂时不理会。

**裁判测试程序样例：**

```haskell
convert :: Read a => String -> [a]
convert = map read . words
main = do
    line <- getLine
    let [n] = convert line :: [Integer] in print (f n)
}
```

注意，以下是测试程序使用的输入和输出，可以忽略。

**输入样例：**
例如：
```haskell
3
```

**输出样例：**
例如：
```haskell
10
```

题解如下：
```haskell
f :: Integer -> Integer
f x = 3 * x + 1
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/6142faf7a0eb4d72a0229074b592e3f4.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 2-1 计算身体质量指数ＢＭＩ (10 分)

计算身高体重指数（BMI），其公式为 $w / h ^ 2$ 。其中 $w$ 表示体重（kg），$h$ 表示身高（m）。 实现一个名字叫bmi的函数：

**函数接口定义：**

```haskell
bmi :: Float -> Float -> Float
```

请在本地运行成功，答案正确后再提交。只需提交类型定义和函数定义。

下面给出一组输入和相应的输出。

**输入样例：**

例如：

```haskell
1.7
60
```

**输出样例：**

例如：

```haskell
20.761246
```
题解如下：

```haskell
bmi :: Float -> Float -> Float
bmi h w = w / h ^ 2
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/135b584111974cbd8d35137cc1359469.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)


# 2-2 函数定义：判断奇偶性 (10 分)

编写函数，判断一个整数是否偶数。如果是偶数，则返回True，否则，返回False

**函数接口定义：** 
```haskell
isEven :: Integer -> Bool   -- 拷贝这一行
-- 填入下列函数定义等号右边的结果
isEven n = ...
```
类型Bool有两个值True和False。本题要求在定义等号右边用一个表达式表示：如果n是偶数，则结果是True，否则结果是False。 例如，在解释器下计算：

```haskell
Main > isEven 12
True
Main > isEven 13
False
```

请在本题运行成功后再提交。下面的说明系统如何测试你的程序，可以暂时不理会。

**裁判测试程序样例：**
```haskell
module Main where

convert :: Read a => String -> [a]
convert = map read . words
main :: IO ()
main = do
    line <- getLine
    let [a] = convert line :: [Integer] in putStrLn(show(isEven a))
```

**输入样例：**

测试程序读取一个整数n，如：

```haskell
12
```

**输出样例：**

测试程序打印isEven n的值，如

```haskell
True
```
题解如下，注释掉的是其他解法：
```haskell
-- isEven :: Integer -> Bool
-- isEven n = if n `mod` 2 == 0 then True else False

-- isEven :: Integer -> Bool
-- isEven n
--     | (n `mod` 2 == 0) = True
--     | otherwise = False

isEven :: Integer -> Bool
isEven x = mod x 2 == 0
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/d0000ab426384054a34baa47125c3574.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 2-3 判断整除 (10 分)

判断一个非零整数是否整除另一个整数。

**函数接口定义：**

```haskell
factor :: Int -> Int -> Bool
factor m n = ...
```

如果m整除n（或者m是n的因子），则结果为True，否则结果为Flase。 请在本地运行成功答案正确后再提交。只需提交类型定义和函数定义。

**输入样例：**

```haskell
 6 18
```

**输出样例：**

```haskell
True
```
题解如下：
```haskell
-- factor :: Int -> Int -> Bool
-- factor m n = n `mod` m == 0

-- factor :: Int -> Int -> Bool
-- factor m n = if n `mod` m == 0 then True else False

factor :: Int -> Int -> Bool
factor m n 
    | (n `mod` m == 0) = True
    | otherwise = False
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/4abed272351d48bda86e2a023aaf3fec.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 2-4 求最大值 (10 分)

请编写一个名为 `max3` 的函数，计算三个实数（Float）中最大者。

**函数接口定义：**

```haskell
max3 :: Float -> Float -> Float -> Float
```

请在本地运行成功答案正确后再提交。只需提交类型定义和函数定义。下面是系统测试程序，可以不理会。

**测试程序**

```haskell
convert :: Read a => String -> [a]
convert = map read . words

main = do
    line <- getLine
    let [a, b,c] = convert line :: [Float] in print (max3 a b c)
```

**输入样例：**

```haskell
5.0  6.1 7.2
```

**输出样例：**

```haskell
7.2
```
题解如下：
```haskell
max2 :: Float -> Float -> Float
max2 a b = if a > b then a else b
```
# 2-5 乘3加1函数 (10 分)

欢迎！请熟悉一下提交程序任务。多数题目只要求提交给定函数的定义，包括函数的类型。 在这里要求实现一个乘3加1函数：$f\ x=3x+1$ 。

**函数接口定义：**

```haskell
f :: Integer -> Integer
-- 注意，上面的函数名f及其类型通常是给定的，不可更改。
f x = 3*x + 1
```

在提交函数定义前，先在本地检查自己定义的函数是否给出正确结果。 例如，在解释器中计算：

```haskell
Main>f 3
```

试着将以上定义提交给系统。

下面说明系统如何测试你的程序。你可以暂时不理会。

**裁判测试程序样例：**

```haskell
convert :: Read a => String -> [a]
convert = map read . words
main = do
    line <- getLine
    let [n] = convert line :: [Integer] in print (f n)
}
```

注意，以下是测试程序使用的输入和输出，可以忽略。

**输入样例：**
```haskell
3
```

**输出样例：**
```haskell
10
```
题解如下：

```haskell
f :: Integer -> Integer
f x = 3 * x + 1
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/510684beebf84b4f9d3a056e9678f7c6.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
# 2-6 定义整数到自然数的一一对应 (10 分)

定义一个整数到自然数的函数，它将非负整数n对应到2n，将负整数n对应到−2n−1。

**函数接口定义：**

```haskell
g :: Integer -> Integer
g n = ...
```

请在本地运行成功再提交。例如，加载你的脚本后计算将g应用于某个整数：

```haskell
Main> g 2
4
Main > g (-3)
5
```

注意-3周围要使用括号。下面的裁判程序和输入输出样例可以忽略。

**裁判测试程序样例：**
```haskell
convert :: Read a => String -> [a]
convert = map read . words
main = do
    line <- getLine
    let [n] = convert line :: [Integer] in print (g n)
```

**输入样例：**
例如：
```haskell
2
```

**输出样例：**
在这里给出相应的输出g。例如, g 2：
```haskell
4
```
题解如下：
```haskell
-- g :: Integer -> Integer
-- g n = if n >= 0 then 2 * n else -2 * n - 1

g :: Integer -> Integer
g n 
    | (n >= 0) = 2 * n
    | otherwise = -2 * n - 1
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/508053ec6279465c9475d605d829cb4b.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 2-7 温度转换 (10 分)

输入一个华氏温度，要求输出摄氏温度。 转换公式为  $\mathtt{^\circ C = \frac {5} {9} (^\circ F - 32)}$

**函数接口定义：**

```haskell
fToC  :: Float -> Float
fToC f  = ...
```

**输入样例：** 

```haskell
49
```

**输出样例：**
例如(保留了两位小数)：
```haskell
9.44
```

**提示**
只需提交函数的定义，无需考虑小数点问题。

题解如下：
```haskell
fToC  :: Float -> Float
fToC f = 5 * (f - 32) / 9
```
---
# 3-1 定义异或 (10 分)

Bool类型上的表达式 `True || True` 计算结果为 `True` 。定义一个“异或”函数，只有两个输入恰好有一个为 `True` 时，结果才为 `True` 。

**函数接口定义：**
```haskell
exor :: Bool -> Bool -> Bool
```

例如，在解释器下计算：

```haskell
Prelude>exor True True
False
Prelude>exor True False
True
Prelude>exor False False
False
Prelude>exor False True
True
```

只需提交函数exor的类型说明和定义。

**提示：**
可以用多个等式列出定义，如：

```haskell
exor False True = True
exor True False = True 
...
```

这种定义就是模式匹配。也可以用一个等式描述结果：
```haskell
exor x y = ...
```

可以忽略下面测试程序读入和输入和输出。

**输入样例：** 
```haskell
True True
```

**输出样例** 
```haskell
False
```
题解如下：

```haskell
-- exor :: Bool -> Bool -> Bool
-- exor x y = if ((x && y) || (not x && not y)) then False else True

-- exor :: Bool -> Bool -> Bool
-- exor x y 
--     | (x && y) = False
--     | (not x && not y) = False
--     | otherwise = True

exor :: Bool -> Bool -> Bool
exor True True = False
exor True False = True
exor False False = False
exor False True = True
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/b27ab6ce77a44e9d82a3067a8e3dfc8b.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 3-2 判断一个一元二次方程是否存在实根 (10 分)

定义函数，判断给定的一个一元二次方程 $ax^2+bx+c=0$ 是否存在实根。可以将该函数的输入理解为方程的三个系数，结果是Bool型值。如果存在实根，则结果为True，否则结果为False。

**函数接口定义：**

```haskell
hasRoot :: Float -> Float -> Float -> Bool
hasRoot a b c = ...
```

**输入样例：**
例如 $x^2+2x+1=0$ 对应的系数：
```haskell
1 2 1
```
**输出样例：** 
```haskell
True
```

题解如下：
```haskell
hasRoot :: Float -> Float -> Float -> Bool
hasRoot a b c = if (b ^ 2 >= 4 * a * c) then True else False
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/284373ccdb8149598dd3d39aea4d7853.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 3-3 分数标准化 (10 分)

一个分数a/b可以表示为(a, b)，其中b>0, 而且a和b没有大于1的公因子。例如，1/2和－2/3可分别表示为(1,2)和(-2,3)。注意，标准化后负号总是在第一个分量上。 请编写一个名为 `normalize` 的函数，将一个二元组表示的分数（Fraction）最简化。

**函数接口定义：**
```haskell
type Fraction = (Integer, Integer)
normalize:: Fraction -> Fraction
```

请在本地运行成功答案正确后再提交。只需提交类型定义和函数定义。例如，在解释器中计算：

```haskell
Main>normalize (2,4) 
```

注意，以下是测试程序使用的输入和输出，可以不理会。

**输入样例：**
例如(2,4)：
```haskell
2 4
```

**输出样例：**
例如, normalize (2,4)：
```haskell
(1,2)
```

**提示：**
-  定义中需要求最大公因子，可以使用预定义函数gcd。当然，也可以自定义一个mygcd。试试看。
 - 整数除法用 `div` ，而不是浮点数除法 `/` 。
-  测试程序提供了Fraction的定义，你只需提交nomalize的类型声明和定义。

题解如下：
```haskell
mygcd :: Integer -> Integer -> Integer
mygcd a b = if b == 0 then a else (mygcd b (a `mod` b))

normalize :: Fraction -> Fraction
normalize (x, y) = (div x (mygcd x y), div y (mygcd x y))
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/9666fa9daeba498b812647cd44489e2c.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
# 3-4 分数加法 (10 分)
一个分数a/b可以表示为(a, b)，其中b>0, 而且a和b没有大于1的公因子。例如，1/2和－2/3可分别表示为(1,2)和(-2,3)。 请编写一个名为 `ratplus` 的函数，计算两个分数（Fraction）之和。

**函数接口定义：**

```haskell
type Fraction = (Integer, Integer)
ratplus :: Fraction -> Fraction -> Fraction
```
 
请在本地测试，例如，在解释器中计算：

```haskell
Main>ratplus (1,2) (1,2)
```

注意，以下是测试程序使用的输入和输出，可以忽略。

**输入样例：** 
在这里给出一组输入。例如 `(1,2),(1,2)` ：

```haskell
1 2 1 2
```

**输出样例：** 
在这里给出相应的输出 `ratplus (1,2) (1,2)` 。例如：

```haskell
(1,1)
```

**提示：**
-  分数相加的结果需要化简。
-  可以使用另一个问题定义的化简函数normalize。这种将问题分解或者分步的方法也是程序设计常用的方法。
-  如果使用normalize，需要将该函数的定义同时提交。
- 程序程序已经提供Fraction定义，请勿重复定义。

题解如下：
```haskell
mygcd :: Integer -> Integer -> Integer
mygcd a b = if b == 0 then a else (mygcd b (a `mod` b))

normalize :: Fraction -> Fraction
normalize (x, y) = (div x (mygcd x y), div y (mygcd x y))

ratplus :: Fraction -> Fraction -> Fraction
ratplus (a, b) (c, d) = normalize(a * d + b * c, b * d)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/476c9dd504c34eec952c6614edccd9ae.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 3-5 分数减法 (10 分)

一个分数a/b可以表示为(a, b)，其中b>0, 而且a和b没有大于1的公因子。例如，1/2和－2/3可分别表示为(1,2)和(-2,3)。 请编写一个名为 `ratminus` 的函数，计算两个分数（Fraction）之差。

**函数接口定义：**

```haskell
type Fraction = (Integer, Integer)
ratminus :: Fraction -> Fraction -> Fraction
```

请在本地测试，例如，在解释器中计算：

```haskell
Main>ratminus (1,2) (1,2)
```

注意，以下是测试程序使用的输入和输出，可以忽略。

**输入样例：**
例如(1,2) (1,2)：
```haskell
1 2 1 2
```
**输出样例：**
例如， ratplus (1,2) (1,2)：
```haskell
(0,1)
```

**提示：**
- 分数相减的结果需要化简。
- 可以使用另一个问题定义的化简函数normalize。这种将问题分解或者分步的方法也是程序设计常用的方法。
- 如果使用normalize，需要提交该函数的定义。
-  程序程序已经提供Fraction定义，请勿重复定义。

题解如下：
```haskell
mygcd :: Integer -> Integer -> Integer
mygcd a b = if b == 0 then a else (mygcd b (a `mod` b))

normalize :: Fraction -> Fraction
normalize (x, y) = (div x (mygcd x y), div y (mygcd x y))

ratplus :: Fraction -> Fraction -> Fraction
ratplus (a, b) (c, d) = normalize(a * d + b * c, b * d)

ratminus :: Fraction -> Fraction -> Fraction
ratminus (a, b) (c, d) = ratplus (a, b) (-c, d)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/e906755d37594d21bedecad294954995.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 3-6 判断分数相等 (10 分)

一个分数a/b可以表示为(a, b)，其中b>0, 而且a和b没有大于1的公因子。例如，1/2和－2/3可分别表示为(1,2)和(-2,3)。 请编写一个名为 `rateq` 的函数，判断两个分数（Fraction）是否相等。

**函数接口定义：**
```haskell
type Fraction = (Integer, Integer)
rateq :: Fraction -> Fraction -> Bool
```
**提示**
测试程序已经提供Fraction定义，请勿重复定义。

**输入样例：**
在这里给出一组输入。例如：
```haskell
1 2 2 4
```

**输出样例：**
在这里给出相应的输出。例如： 
```haskell
True
```
题解如下：
```haskell
mygcd :: Integer -> Integer -> Integer
mygcd a b = if b == 0 then a else (mygcd b (a `mod` b))

normalize :: Fraction -> Fraction
normalize (x, y) = (div x (mygcd x y), div y (mygcd x y))

rateq :: Fraction -> Fraction -> Bool
rateq (a, b) (c, d) = normalize (a, b) == normalize (c, d)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/f67079679f2648feb2096d90ba011064.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
# 3-7 将二元组表示的分数转换为浮点数 (10 分)

一个分数a/b可以表示为(a, b)，其中b>0, 而且a和b没有大于1的公因子。例如，1/2和－2/3可分别表示为(1,2)和(-2,3)。注意，标准化后负号总是在第一个分量上。 请编写一个名为 `ratfloat` 的函数，将一个二元组表示的分数（Fraction）转换为浮点数。

**函数接口定义：**
```haskell
type Fraction = (Integer, Integer)
ratfloat :: Fraction -> Float
```
例如：
```haskell
Main> ratfloat (15,8)
1.875
```
**输入样例：** 
```haskell
15 8
```
**输出样例：** 
```haskell
1.875
```

提示：
- 需要使用浮点数除法 `/` 。
 -  整数做浮点数除法需要先做类型转换，比如 `fromInteger` 将 `Integer` 类型转换为 `Float` 类型。
 -  测试程序提供了Fraction定义，请勿重复定义。
 -  无需考虑四舍五入问题。


题解如下：

```haskell
ratfloat :: Fraction -> Float
ratfloat (a, b) = fromInteger a / fromInteger b 
```

---
# 4-1 计算阶加 (10 分)
类似于阶乘，可以定义“阶加”(termial function): $n?=1+2+...+n=∑_{k=1}^n​k$。试用递归定义该函数。

**函数接口定义：**
```haskell
termial :: Integer -> Integer
```

**输入样例：**
```haskell
3
```

**输出样例：**
```haskell
6
```
题解如下：
```haskell
-- termial  :: Integer -> Integer
-- termial  n = if (n <= 1) then n else n + termial (n - 1)

-- termial  :: Integer -> Integer
-- termial  n
--     | (n <= 1) = n
--     | otherwise = n + termial (n - 1)

termial  :: Integer -> Integer
termial 0 = 0
termial 1 = 1
termial n = n + termial (n - 1)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/3b92b40090f2459aab97559201da194e.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 4-2 求递推关系定义的数列 (10 分)
假定一个数列定义为 $a_0​=1,a_1​=2,a_n​=2a_{n−1}​+3a_{n−2}$​，试用递归计算 $a_n$ ​。

**函数接口定义：**
```haskell
series :: Integer -> Integer
```

这里 `series n` 表示 $a_n​(n≥0)$ 。

**输入样例：** 
```haskell
2
```
**输出样例：**
```haskell
7
```
题解如下：
```haskell
-- series :: Integer -> Integer
-- series n = if n == 0 then 1 else
--             if n == 1 then 2 else
--              2 * series (n - 1) + 3 * series (n - 2)
             
             
-- series :: Integer -> Integer
-- series n 
--     | (n == 0) = 1
--     | (n == 1) = 2
--     | otherwise = 2 * series (n - 1) + 3 * series (n - 2)

series :: Integer -> Integer
series 0 = 1
series 1 = 2
series n = 2 * series (n - 1) + 3 * series (n - 2)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/cb3796917615437c9cf90123e410e862.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 4-3 求级数和 (10 分)
试利用递归求调和级数之和 $1+1/2+⋯+1/n$ 。

**函数接口定义：**
```haskell
hseries :: Integer -> Float
```
注意，在使用 `1 / n` 时，因为 `n` 是整数，要参与浮点数的除法运算 `/` ，需要使用类型转换运算：`1 / (fromInteger n)` 。

**输入样例：**  
```haskell
3
```
**输出样例：**
例如（保留了2位小数，但是，你无需考虑小数点问题）：
```haskell
1.83
```
题解如下：

```haskell
hseries :: Integer -> Float
hseries n = if (n == 1) then 1 else 1 / (fromInteger n) + hseries (n - 1)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/ba6c96d48fce4990b55599988921fdd9.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 4-4 实现牛顿-拉弗森迭代公式 (10 分)

求r平方根的牛顿-拉森迭代法：$x_0​=1,x_n​=(x_{n−1​}+r/x_{n−1}​)/2$

定义函数，用牛顿-拉森迭代公式求任意正实数 $r$ 的第 $n$ 次迭代结果 $x_n$ ​。

**函数接口定义：**
```haskell
nl :: Float -> Int -> Float
```

这里用 `nl r n` 表示 $x_n$ ​。

提交前先在本地运行，如：

```haskell
Main> nl 2.1 3
```

**输入样例：** 
```haskell
2.1
3
```

**输出样例：** 
```haskell
1.45
```

提示：不需要考虑小数点问题。

题解如下：
```haskell
-- nl :: Float -> Int -> Float
-- nl r n = if (n == 0) then 1 else (nl r (n - 1) + r / (nl r (n - 1))) / 2 

-- nl :: Float -> Int -> Float
-- nl r n
--     | (n == 0) = 1
--     | otherwise = (nl r (n - 1) + r / (nl r (n - 1))) / 2 

nl :: Float -> Int -> Float
nl r 0 = 1
nl r n = (nl r (n - 1) + r / (nl r (n - 1))) / 2
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/78aa8b09c02441f09fc655e103871a8c.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 4-5 计算平方和 (10 分)

给定正整数 $n$ ，计算和 $1+2^2+3^2+⋯+(n−1)^2+n^2$ 。

**函数接口定义：**
```haskell
sumSq :: Integer -> Integer
```

**输入样例：** 
```haskell
1
```

**输出样例：** 
```haskell
1
```
题解如下：
```haskell
-- sumSq :: Integer -> Integer
-- sumSq n = if (n == 1) then 1 else n ^ 2 + sumSq (n - 1)

-- sumSq :: Integer -> Integer
-- sumSq n
--     | (n == 1) = 1
--     | otherwise = n ^ 2 + sumSq (n - 1)

sumSq :: Integer -> Integer
sumSq 1 = 1
sumSq n = n ^ 2 + sumSq (n - 1)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/a6c94d4eb925428d9000c9c73e0ec222.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 4-6 最大公因子 (10 分)

根据欧几里得算法求两个非负整数m,n的最大公因子：
- 任意非负整数 $n$ 和 $0$ 的最大公因子为 $n$ 。
-  如果 $m,n$ 均不为 $0$ ，则 $m$ 和 $n$ 的最大公因子等于 $n$ 和 $m\% n$ 的最大公因子。

**函数接口定义：** 
```haskell
mygcd :: Integer -> Integer -> Integer
```

**输入样例：**
```haskell
6
15
```

**输出样例：** 
```haskell
3
```

题解如下：
```haskell
-- mygcd :: Integer -> Integer -> Integer
-- mygcd a b = if b == 0 then a else mygcd b (a `mod` b)

-- mygcd :: Integer -> Integer -> Integer
-- mygcd a b
--     | (b == 0) = a
--     | otherwise = mygcd b (a `mod` b)
    
mygcd :: Integer -> Integer -> Integer
mygcd a 0 = a
mygcd a b = mygcd b (a `mod` b)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/9e68f8c7d230496db25dc34b39613fc6.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 4-7 泰勒级数 (10 分)

函数 $sin(x)$ 的泰勒级数为 $sin(x)=∑_{n=0}^∞​(−1)^n \frac {x^{2n+1}}{(2n+1)!}$ ​，因此可以用 $sin(x)≈∑_{n=0}^m​(−1)^n \frac{x^{2n+1}}{(2n+1)!}$ ​计算 $sin(x)$ 的近似值。

**函数接口定义：**

```haskell
sseries :: Float -> Int -> Float
```

这里 `sseries x m` 表示 $∑_{n=0}^m​(−1)^n \frac{x^{2n+1}}{(2n+1)!}$ 。

**提示：**
- 对第二个输入参数m递归。
- 可能需要类型转换函数 `fromIntegral` ，将 `Int` 类型的阶乘转换为 `Float` ，以便做浮点数除法。

**输入样例：** 
```haskell
1.67
3
```

**输出样例：** 
```haskell
0.99
```

提示：无需考虑小数点问题。

题解如下：
```haskell
factorial :: Int -> Int
factorial n = if (n <= 1) then n else n * factorial (n - 1)

-- sseries :: Float -> Int -> Float
-- sseries x m = if (m == 0) then x else 
--     ((-1) ^ m) * ((x ^ (2 * m + 1)) / (fromIntegral (factorial (2 * m + 1)))) + sseries x (m - 1)

-- sseries :: Float -> Int -> Float
-- sseries x m
--     | (m == 0) = x 
--     | otherwise = ((-1) ^ m) * ((x ^ (2 * m + 1)) / (fromIntegral (factorial (2 * m + 1)))) 
--     + sseries x (m - 1)

sseries :: Float -> Int -> Float
sseries x 0 = x
sseries x m = ((-1) ^ m) * ((x ^ (2 * m + 1)) / (fromIntegral (factorial (2 * m + 1)))) + sseries x (m - 1)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/6ebdd2446f224fdea7343c1883ade3b9.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 4-8 二项式系数 (10 分)

利用杨辉-帕斯卡等式 $\binom{n}{k}=\binom{n-1}{k}+\binom{n-1}{k-1}$ 计算二项式系数 $\binom{n}{k}$ 或 $C^k_n,\ C(n,k)$。

**函数接口定义：**
```haskell
binom :: Integer -> Integer -> Integer
```
其中 `binom n k` 表示 $\binom{n}{k}$ 。

**提示：**
- 要求使用递归。
-   杨辉-帕斯卡等式说明递归步，可见需要对两个输入参数递归。
-  关键在于给出递归基。

**输入样例：**
```haskell
5 2
```

**输出样例：**  
```haskell
10
```
题解如下：
```haskell
-- binom :: Integer -> Integer -> Integer
-- binom n k = if (k == 0 || k == n) then 1
--             else binom (n - 1) k + binom (n - 1) (k - 1) 
            
-- binom :: Integer -> Integer -> Integer
-- binom n k
--     | (k == 0 || k == n) = 1
--     | otherwise = binom (n - 1) k + binom (n - 1) (k - 1)

binom :: Integer -> Integer -> Integer 
binom n 0 = 1
binom n k
    | (k == n) = 1
    | otherwise = binom (n - 1) k + binom (n - 1) (k - 1)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/13a2d8767161470989455c193403ecd1.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 4-9 汉诺塔 (10 分)
定义解决汉诺塔问题的函数。

**函数接口定义：**
```haskell
hanoi :: Int -> String -> String -> String ->[(String, String)]
```
这里假定 `hanoi n start to from` 表示将 `start` 上的 `n` 个盘子移到目标柱子 `to` 上。假定用二元组 `(f, t)` 表示将f柱子顶部的盘子移到t柱子上，那么解决汉诺塔问题是这种二元组的列表。例如，解决2个盘子的汉诺塔问题：

```haskell
*Main> hanoi 2 "start" "to" "mid"
[("start","mid"),("start","to"),("mid","to")]
```

**提示**
-  函数hanoi有4个形式参数，注意各个位置参数的含义，搞清楚在哪个参数上递归。
- 定义函数时，各个参数使用变量名，例如，
	```haskell
	hanoi n start to from = ...
	```
    **而不是**：
	```haskell
	hanoi n "start" "to" "from"
	```
    只有在将函数 `hanoi` 应用于实际参数时，才提供具体的整数或者字符串命名各个柱子。

**输入样例：**
```haskell
2
```

**输出样例：** 
```haskell
[("start","mid"),("start","to"),("mid","to")]
```

题解如下：
```haskell
-- hanoi :: Int -> String -> String -> String -> [(String, String)]
-- hanoi n start to mid = 
--     if (n < 1) then []
--     else if (n == 1) then [(start, to)]
--     else (hanoi (n - 1) start mid to) ++ (hanoi 1 start to mid) ++ (hanoi (n - 1) mid to start)

-- hanoi :: Int -> String -> String -> String -> [(String, String)]
-- hanoi n start to mid
--     | (n < 1) = []
--     | (n == 1) = [(start, to)]
--     | otherwise = (hanoi (n - 1) start mid to) ++ (hanoi 1 start to mid) ++ (hanoi (n - 1) mid to start)

hanoi :: Int -> String -> String -> String -> [(String, String)]
hanoi 0 start to mid = []
hanoi 1 start to mid = [(start, to)]
hanoi n start to mid = (hanoi (n - 1) start mid to) ++ (hanoi 1 start to mid) ++ (hanoi (n - 1) mid to start)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/2c1fdf5d7d1544afb420ea8dfc1069cb.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 4-10 利用递推关系计数 (10 分)
用 $a_n$ ​表示长度为 $n$ 并且含有子串 $00$ 的二进制串的个数。 例如，$a_1​=0,a_2​=1,a_3​=3(000,001,100)$ 。 请设计计算 $a_n$ ​的函数。

**函数接口定义：**
```haskell
bseries :: Integer -> Integer
```
**输入样例：** 
```haskell
3
```
**输出样例：** 
```haskell
3
```
**提示：**
先分析长度 $n$ 与长度 $n−1$ 以及 $n−2$ 并包含 $00$ 的二进制串之间的关系，求出 $a_n$ ​与 $a_{n−1}​,a_{n−2}$ ​之间的递推关系，然后使用递归定义函数。


这道题倒是要讲一下，对于长度为 $n$ 的二进制串：
- 第 $n$ 位是 $1$ 时，其含有 $00$ 的二进制串个数要加上 $a_{n-1}$ ，递归求解 $a_{n-1}$ ；
- 第 $n$ 位是 $0$ 时，其含有 $00$ 的二进制串个数取决于第 $n - 1$ 位：
	- 如果第 $n - 1$ 位是 $1$ ，其含有 $00$ 的二进制串个数要加上 $a_{n-2}$ ，递归求解 $a_{n-2}$ ；
	- 如果第 $n-1$ 位是 $0$ ，即为递归基准条件，其含有 $00$ 的二进制串个数要加上 $2^{n-2}$ ，因为无论前 $n-2$ 位如何取值，形成的长度为 $n$ 的二进制串都包含 $00$ 。

题解如下：
```haskell
bseries :: Integer -> Integer
bseries n =  
    if (n == 1) then 0
    else if (n == 2) then 1
    else if (n == 3) then 3
    else bseries (n - 1) + bseries (n - 2) + 2 ^ (n - 2)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/8a3132c82484477fbf92b58129089854.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 4-11 整数拆分为各位数字列表 (10 分)

实现将一个正整数拆分成各位数字列表（逆序）的函数，如，将12030拆分成[0, 3, 0, 2, 1]。

**函数接口定义：**
```haskell
splitInt :: Int -> [Int]
```

**输入样例：**
```haskell
1801
```

**输出样例：**

```haskell
[1,0,8,1]
```
**提示**
- 如何得到个位数，如何得到十位以上的数，例如，输入是1801，你或许希望得到180和1。
 - 需要搞清楚递归基是什么情况，递归步是什么情况， 如何表达递归步的结果。
-  函数的结果是列表。构造整数列表的方法：将构造函数(:)应用一个整数 x 和一个整数列表 xs, 得到 x:xs，或者将两个整数列表xs和ys用(++)串接：xs ++ ys。

题解如下：
```haskell
splitInt :: Int -> [Int]
splitInt n =
    if (n < 10) then [n]
    else (mod n 10) : splitInt(div n 10)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/868115fca78541afa227021b8ed553d2.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

---
# 6-1 分数化简的性质 (10 分)

编写表达分数化简结果的性质：
-  化简前后结果相等
- 化简后分子分母没有大于1的公因子

**函数接口定义：**

```haskell
prop_normalize :: Fraction -> Bool
prop_normalize (a, b) = ... 
```

**提示：**
 - 可以假定测试程序提供了函数 `normalize` 的实现，提交时只需提交 `prop_normalize` 的定义。
 - 测试程序使用的 `normalize` 可能包含语义错误。

**输入样例：** 
```haskell
2 4
```

**输出样例：** 
```haskell
True
```
题解如下：
```haskell
mygcd :: Integer -> Integer -> Integer
mygcd a b = if b == 0 then a else mygcd b (a `mod` b)

rateq :: Fraction -> Fraction -> Bool
rateq (a, b) (c, d) = (a * d) == (b * c)

prop_normalize :: Fraction -> Bool
prop_normalize (a, b) = rateq q (a, b) && gcd (fst q) (snd q) == 1 where q = normalize (a, b)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/c9bdee7d8f794d53943c9adeaabe540d.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 6-2 测试解一元二次方程根函数的性质 (10 分)

给定一个解一元二次方程的函数：
```haskell
solve :: Float -> Float -> Float -> (Float, Float)
```
当一元二次方程 $ax^2+bx+c=0$ 有实根时，`solve a b c` 返回两个实根。

试编写一个性质，以便测试函数 `solve` 返回的根是否正确。 一种方法是将两个根带入原方程，判断它们是否为解。为了解决浮点数运算的误差问题，只需要判断将根 $r$ 代入方程后，$∣ar^2+br+c∣<0.001$即可。试实现这个性质的定义。

**函数定义接口** 
```haskell
prop_solve :: Float -> Float -> Float -> Bool
prop_solve a b c = ...
```

**提示**
- 假定测试程序提供了函数 `solve` 的定义，但是该函数定义不一定正确。
-  假定测试的一元二次方程存在实根。
-  只需提交性质的定义。
-  只需要用 `prop_solve a b c` 表述 `solve` 返回的是一元二次方程 $ax^2+bx+c=0$ 的两个实根即可。（或许你会说，这个性质不足以说明 `solve` 给出了正确的解。）

**输入样例：**
 
```haskell
1 -2 1 
```

**输出样例：**
 
```haskell
True
```
题解如下：
```haskell
prop_solve :: Float -> Float -> Float -> Bool
prop_solve a b c = abs (a * x * x + b * x + c) < 0.001 
                && abs (a * y * y + b * y + c) < 0.001
                where x = fst (solve a b c)
                      y = snd (solve a b c)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/0136d678c3924b568fd86ff08c0a3d64.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
# 6-3 测试二项式系数的递归定义 (11 分)

假定给出二项式系数 $\binom {n}{k}$ 的递归定义：
```haskell
binom :: Integer -> Integer -> Integer
```

现在请你用公式 $\binom {n}{k} = \frac{n(n-1)\dots(n-k+1)}{k!}$​验证递归实现是否正确。

**函数接口定义：**
```haskell
prop_binom :: Integer -> Integer ->Bool
prop_binom n k = ...
```
等式右边表示递归实现结果与公式计算结果相等。

**提示** 
假定测试程序提供 `binom` 的实现，因此只需提交函数 `prop_binom` 的定义。

**输入样例：**
在这里给出一组输入。例如： 
```haskell
6 2
```

**输出样例：**
在这里给出相应的输出。例如： 
```haskell
True
```
题解如下：
```haskell
nfact :: Integer -> Integer -> Integer
nfact n end
    | (n == end) = end
    | otherwise = n * (nfact (n - 1) end) 

prop_binom :: Integer -> Integer ->Bool
prop_binom n k = (binom n k) == div (nfact n (n - k + 1)) (nfact k 1)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/d2246766625b4e68b2db40c7bb858056.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

---
# 6-1 毕达哥拉斯三元组 (10 分)
一个整数三元组 $(x,y,z)$ 如果满足 $x^2+y^2=z^2$ ，则称 $(x,y,z)$ 为毕达哥拉斯三元组。试定义一个函数：
```cpp
triads :: Int -> [(Int, Int, Int)]
```

对于任意正整数n，`triads n` 给出所有分量介于1和n之间的毕达哥拉斯三元组 $(x,y,z) (x<y)$ ，按照第一个分量从小到大顺序列出，而且不包括重复的三元组。例如，(3,4,5)和(4,3,5)属于重复的三元组。 因此， triads 10 返回列表[(3,4,5),(6,8,10)]。

**函数接口定义：**
```cpp
triads :: Int -> [(Int, Int, Int)]
```
**输入样例：**
在这里给出一组输入。例如：
```cpp
5
```
**输出样例：**
在这里给出相应的输出。例如：
```cpp
[(3,4,5)]
```
**提示**
- 考虑使用列表概括。
 -  三元组 $(x,y,z)$ 的每个分量都是 $[1..n]$ 的元素。实际上，第二个分量 $y$ 是 $[x..n]$ 的元素。

题解如下：

```haskell
triads :: Int -> [(Int, Int, Int)]
triads n = [ (x, y, z) | x <- [1..n], y <- [x..n], z <- [y..n], x * x + y * y == z * z]
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/2b00e372fcd14ec39e84a46adb153523.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
# 6-2 牛顿-拉森迭代序列 (1) (10 分)
用牛顿-拉弗森(Newton-Raphson)公式求平方根是一种收敛很快的迭代方法。求r的平方根的牛顿-拉弗森迭代公式为：$x_{n+1}​=(x_n​+r/x_n​)/2$ ，其中 $x_0$ ​是一个初始值。

试定义计算迭代序列的函数，对于给定的 $r,x_0$ ​和 $n$ ，该函数返回迭代序列 $[x_n​,x_{n−1}​,⋯,x_1​,x_0​]$ 。

**函数接口定义：**
```haskell
nls1 :: Float -> Float -> Int -> [Float]
nls1 r x0 0 = ...
nls1 r x0 n = ...
```
**提示**
- 考虑如何用递归和列表的构造方法构造迭代序列。
-  无需考虑小数点后保留多少位问题。

**输入样例：**
例如： 
```haskell
2.0 1.0 
3
```
**输出样例：** 
例如，`nls1 2.0 1.0 3` 给出：

```haskell
[1.4142157,1.4166667,1.5,1.0]
```

题解代码：

```haskell
nls1 :: Float -> Float -> Int -> [Float]
nls1 r x0 0 = [x0]
nls1 r x0 n = xi : lst
    where lst = (nls1 r x0 (n - 1))
          xj = head lst
          xi = (xj + r / xj) / 2 
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/e48f2788e0c247218bce7924daa77781.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 6-3 牛顿-拉森迭代序列 (2) (10 分)

用牛顿-拉弗森(Newton-Raphson)公式求平方根是一种收敛很快的迭代方法。求r的平方根的牛顿-拉弗森迭代公式为：$x_{n+1}​=(x_n​+r/x_n​)/2$ ，其中 $x_0$ ​是一个初始值。

试定义计算迭代序列的函数，对于给定的 $r,x_0$ ​和 $n$ ，该函数返回迭代序列 $[x_0​,x_1​,⋯,x_n​]$ 。

**函数接口定义：**
```haskell
nls :: Float -> Float -> Int -> [Float]
nls r x0 0 = ...
nls r x0 n = ...
```
**提示**
- 考虑如何用递归和列表的构造方法构造迭代序列。
 -  无需考虑小数点后保留多少位数。

**输入样例：**
例如：
```haskell
2.0 1.0 
3
```

**输出样例：** 
例如，`nls 2.0 1.0 3` 给出：

```haskell
[1.0,1.5,1.4166667,1.4142157]
```
题解代码：

```haskell
nls :: Float -> Float -> Int -> [Float]
nls r x0 0 = [x0]
nls r x0 n = lst ++ xi
    where lst = (nls r x0 (n - 1))
          xj = last lst
          xi = [(xj + r / xj) / 2] 
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/cdde1522ee8c428d9304bf12b8967796.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
# 6-4 字符串放大 (1) (10 分)
实现习题3.9的字符放大程序。任务的关键是实现将一个字符串映射到对应放大图形表示的函数，比如命名为 `say` ，其类型为
```haskell
say :: String -> Picture
```
然后用本章图形例子的 `printPicture` 函数即可实现最后要求的函数 `'sayit'` :
```haskell
sayit :: String -> IO ()
sayit somestr = printPicutre (say somestr)
```
在习题中，我们将该任务分解成几个更简单的步骤。本周PTA作业将任务分解成三个步骤。

为简单起见，假定字符 `'A'` 的图形用列表`["AAA","AAA","AAA"]` 表示， 因此，下列表达式给出所有字母的图形表示：
```haskell
pics = [replicate 3 (replicate 3 (chr c)) | c <- [65..90]]
```
请你定义字符到其图形表示的函数：

**函数接口定义：**

```haskell
letter2pic :: Char -> Picture
```
其中 

```haskell
type Picture = [String]
```
例如，`letter2pic 'h'` 或者 `letter2pic 'H'` 将从 `pics` 中取得 `'H'` 的图形表示 `["HHH", "HHH","HHH"]` 。

**提示**
- 使用chr函数需要 `import Data.Char` 。
-  假定测试程序已经 `import Data.Char` ，并提供了 `Picture` 的类型定义以及 `pics` ，因此你只需要提交 `letter2pic` 的实现。
- 题目要求利用 `pics` 来定义 `letter2pic` ，但不可依赖于 `pics` 的具体表示，也就是说，只能假定 `pics` 是由26个字母的图形构成的列表。

**输入样例：**
例如：
```haskell
'c'
```

**输出样例：**
例如：
 
```haskell
["CCC","CCC","CCC"]
```
题解如下：
```haskell
letter2pic :: Char -> Picture
letter2pic c = pics !! ordc
    where ordc = if c >= 'a' 
                 then (ord c - ord 'a')
                 else (ord c - ord 'A')
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/93d61a34234840819031d42e768c3ba7.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
# 6-5 字符串放大（2） (10 分)

本题是习题3.9字符串放大程序的第二个任务。 构造字符串到其中字符图形列表的函数。

**函数接口定义：**
```haskell
string2pics :: String -> [Picture]
```
其中 
```haskell
type Picture = [String]
```

**提示**
-  测试程序已经导入 `Data.Char` ，可以直接使用该模块提供的函数。
-  测试程序已经给出 `Picture` 的定义，因此只需提交函数 `string2pics` 的定义。
-  可以使用先前定义的函数 `letter2pic` ，测试程序提供了该函数的定义（不要提交函数 `letter2pic` 的定义)。
-  测试程序将使用先前定义的 `pics` 测试。

**输入样例：** 
在这里给出一组输入。例如： 
```haskell
"hi"
```
**输出样例：** 
在这里给出相应的输出。例如： 
```haskell
[["HHH","HHH","HHH"],["III","III","III"]]
```
题解代码：

```haskell
string2pics :: String -> [Picture]
string2pics s = [letter2pic c | c <- s]
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/d0be64e814bd4abbbad2b84014317ed9.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)


# 6-6 字符串放大（3） (10 分)

本题是习题3.9字符串放大程序的第三个任务。 实现将一系列图形从左到右拼接的函数。

**函数接口定义：**
```haskell
concatPics :: [Picture] -> Picture
```
**提示**
-  测试程序已经导入 `Data.Char` ，可以直接使用该模块提供的函数。
- 测试程序已经给出 `Picture` 的定义，因此只需提交函数 `concatPics` 的定义。
-  测试程序将使用先前定义的 `pics` 测试。

**输入样例：** 
例如： 
```haskell
"hi"
```

**输出样例：** 
例如，`concatPics (string2pics "hi")`

```haskell
["HHHIII","HHHIII","HHHIII"]
```
题解如下：
```haskell
sideBySide :: Picture -> Picture -> Picture
sideBySide p q = [line1 ++ line2 | (line1, line2) <- zip p q]

concatPics :: [Picture] -> Picture
concatPics [] = []
concatPics [x] = x
concatPics (x : xs) = sideBySide x (concatPics xs)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/73058077182f44ea9e1a2bf4207881df.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
# 6-7 求单词出现次数 (10 分)
给定一个单词列表 `words` ，一个单词 `w` ，求 `w` 在 `ws` 中出现的次数。

**函数接口定义：**

```haskell
count :: [String] -> String -> Int
```
**例如**
```haskell
ws = ["haskell", "language", "languages", "Haskell", "fun","haskell"]
w = "haskell"
```
`count ws w` 返回 `2` 。

**提示**
- 统计区别大小写和单复数。例如，`"langugage"` 与 `"languages"` 不同，`"haskell"` 与 `"Haskell"` 不同。
-  一种方法是利用列表概括列出第一个输入形参中所有与第二个输入形参相等的字符串构成列表。例如，对于上例，先计算出 `["haskell", "haskell"]` 。

**输入样例：**
在这里给出一组输入。
```haskell
0
```
**输出样例：** 
在这里给出相应的输出。例如： 
```haskell
2
```
题解如下：
```haskell
count :: [String] -> String -> Int
count ws w = length [x | x <- ws, x == w]
```
# 6-8 求素数列表 (10 分)
给定一个自然数 $n$ ，求小于等于 $n$ 的所有素数按升序排列组成的列表。

**函数接口定义：** 

```haskell
primeBelow :: Int -> [Int]
```
**输入样例：** 
例如：
```haskell
20
```
**输出样例：**
例如： 

```haskell
[2,3,5,7,11,13,17,19]
```
**提示**
- 根据 `[2..n]` 生成素数构成的列表，考虑使用列表概括。
- 不妨借助判定素数的函数。

题解如下：
```haskell
isPrime :: Int -> Bool
isPrime n
    | (n <= 2) = (n == 2)
    | otherwise = odd n && go 3
    where
        go factor
            | (factor * factor > n) = True
            | otherwise = n `rem` factor /= 0 && go (factor + 2)

primeBelow :: Int -> [Int]
primeBelow n = [x | x <- [2..n], (isPrime x == True)]
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/75a1da0df7bc4ba483f072fb5d457d9e.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 6-9 奖牌查询 (10 分)

给定各国奖牌列表，查询某个国家的金牌数目。

**函数接口定义：** 
```haskell
type Gold = Int  --金牌数
type Silver = Int -- 银牌数
type Bronze = Int  -- 铜牌数
type Country = String  -- 国家名
goldMedal :: [(Country, Gold, Silver, Bronze)] -> Country -> Gold
```

**提示**
-  测试程序提供了如上各个类型别名的定义，你只需提交 `goldMedal` 的类型说明和定义。
 - 如果列表中不存在查询的国家，则返回0。
 - 四元组列表类型 `(Country, Gold, Silver, Bronze)` 的元素具有 `(a,b,c,d)` 的形式。理解图书管理例子有帮助。

**输入样例：** 
例如：
```haskell
>goldMedal [("China", 34, 24,16), ("United States", 29,35,27),("Japan", 22,10,14)]  "China"
34
```
忽略下面的样例。
```haskell
China
```
**输出样例：** 
例如： 
```haskell
34
```

题解如下：
```hs
goldMedal :: [(Country, Gold, Silver, Bronze)] -> Country -> Gold
goldMedal xs target = if (null ans) then 0 else head ans
    where ans = [ gold | (country, gold, silver, bronze) <- xs, country == target]
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/d2e79c6abae9422980dd97b312b5c102.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 6-10 统计奖牌总数 (10 分)

给定各国金银铜牌列表，统计各个国家的奖牌总数。

**函数接口定义：**
```haskell
type Gold = Int  --金牌数
type Silver = Int -- 银牌数
type Bronze = Int  -- 铜牌数
type Country = String  -- 国家名
type Total = Int -- 奖牌总数

medalList :: [(Country, Gold, Silver, Bronze)] -> [(Country, Gold, Silver, Bronze, Total)] 
```

**提示**
- 测试程序提供了如上各个类型别名的定义，你只需提交 `medalList` 的类型说明和定义。
- 如果列表中不存在查询的国家，则返回0。
- 统计结果列表中各个国家次序同输入列表中各个国家的次序。

**输入样例：**
请忽略样例。
```haskell
1
```

**输出样例：**
在这里给出相应的输出。例如：
```haskell
[("China",34,24,16,74)]
```
题解如下：
```hs
medalList :: [(Country, Gold, Silver, Bronze)] -> [(Country, Gold, Silver, Bronze, Total)] 
medalList xs = [(c, g, s, b, g + s + b) | (c, g, s, b) <- xs]
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/171e6052583f404db53e185cde3230d4.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

---
# 6-1 列表拆分 (10 分)
实现下列将非空列表划分为前半部分、中间元素和后半部分的函数：

**函数接口定义：**
  
```haskell
mySplit :: [a] -> ([a], a, [a])
```
 
其中前后两部分具有相同个数的元素，或者前半部分比后半部分多一个元素。例如 `mySplit [1,2,3]` 的结果是 `([1], 2, [3])` ，`mySplit [1,2]` 的结果是 `([1], 2, [])` ，`mySplit [1]` 的结果是 `([], 1, [])` 。


注意，这个函数不需要在列表元素上做任何运算，因此是一个多态函数。

**输入样例：** 
例如： 
```haskell
1
[1,2,3]
```

**输出样例：** 
例如：
```haskell
([1],2,[3])
```
题解如下：
```haskell
mySplit :: [a] -> ([a], a, [a])
mySplit xs = (lhalf, mid, rhalf)
        where 
            halfLen = div (length xs) 2
            lhalf = take halfLen xs
            mid = xs !! halfLen
            rhalf = drop (halfLen + 1) xs
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/e03701870ac64039831e9b7663ccc85d.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 6-2 有序列表插入 (10 分)

给定从小到大有序列表，将另一个给定元素插入有序列表适当位置，使得结果是有序列表。

**函数接口定义：**

```haskell
insert ::  Ord a => a -> [a] -> [a]
```
例如，`insert 3 [1,1,2,4]` 的结果是 `[1,1,2,3,4]` 。

**提示**
-  因为插入函数定义只需要比较运行，因此具有如上给定类型。
- 可以考虑使用递归。

**输入样例：** 
例如（第一行输入忽略）： 
```haskell
1
3
[1,1,2,4]
```

**输出样例：** 
在这里给出相应的输出。例如： 
```haskell
[1,1,2,3,4]
```
题解如下：
```hs
insert :: Ord a => a -> [a] -> [a]
insert a [] = [a]
insert a (x : xs)
    | (a < x) = (a : (x : xs))
    | otherwise = x : (insert a xs)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/3ccca503a28f492eaf9e487d860c555d.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 6-3 插入排序 (10 分)
**插入排序**
-  如果输入是空列表，则输出也是空列表；
-  如果输入不空，比如 `x : xs` ，那么先把 `xs` 排序为 `xs'` ，然将 `x` 插入有序列表 `xs'` 的适当位置，使得插入结果仍有是有序列表，由此得到 `x : xs` 的排序结果。

实现插入排序函数

**函数接口定义：** 
```haskell
insertionSort ::  Ord a => [a] -> [a]
```
提示：在插入排序函数insert基础上实现插入排序。

**输入样例：**
例如： 
```haskell
1
[2,1,2,3]
```

**输出样例：** 
在这里给出相应的输出。例如： 
```haskell
[1,2,2,3]
```
题解如下：

```haskell
insert :: Ord a => a -> [a] -> [a]
insert a [] = [a]
insert a (x : xs)
    | (a < x) = (a : (x : xs))
    | otherwise = x : (insert a xs)

insertionSort :: Ord a => [a] -> [a]
insertionSort [] = []
insertionSort (x : xs) = insert x (insertionSort xs)
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/8ac57aeab11b456796a689955cbd4e27.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 6-4 二分查找 (10 分)

实现二分查找函数。

**函数接口定义：** 
```haskell
binarySearch :: Ord a  => a -> [a] -> Bool
```
如果第一个参数在第二个列表参数中出现，则返回 `True` ，否则返回 `False` 。假定第二个列表是从小到大有序的。

**输入样例：** 
例如： 
```haskell
1
2
[1,2,2,3,4]
```
**输出样例：** 
例如，`binarySearch 2 [1,2,2,3,4` 返回 
```haskell
True 
```

**要求** 
需要使用递归实现二分查找，不应该使用顺序查找方法。

题解如下：
```hs
binarySearch :: Ord a  => a -> [a] -> Bool
binarySearch a xs
    | (null xs) = False
    | otherwise = 
        (binarySearch a (take halfLen xs)) || 
        (a == xs !! halfLen) ||
        (binarySearch a (drop (halfLen + 1) xs))
    where halfLen = div (length xs) 2
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/290c3a9fd15443f0b57eba8ce18e3f97.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
# 6-5 有序性 (10 分)

实现一个函数，检查给定的列表是否按照从小到大有序。 例如，`[1,1,2,3]` 、`[1.2, 2.3, 2, 3]` 和 `["hi", "there"]` 是从小到大有序的，但 `[1,2,1,3]` 、`[2.3,1.2,2.3]` 和 `["there","hi"]` 则不是从小到大有序的。

**函数接口定义：**
```haskell
ordered ::  Ord a => [a] -> Bool
```

**输入样例：**
例如：
```haskell
1
[1,1,2,3]
```

**输出样例：**
例如，`ordered [1,1,2,3]` 返回 
```haskell
True
```
题解如下：
```hs
ordered ::  Ord a => [a] -> Bool
ordered [] = True  -- 空列表
ordered [x] = True -- 一个元素的列表
ordered (x : xs)   -- 两个元素及更大的列表
    | (x > head xs) = False
    | otherwise = (ordered xs)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/0000a3e146d54372bcebc80fe8122e27.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

---
# 6-1 lambda表达式 (10 分)

对于下列函数
```haskell
f :: Int -> Int -> Float
f x y = fromIntegral x / (fromIntegral y)
```

试给出等价的 $\lambda$ 表达式。

**函数接口定义：** 
```haskell
f :: Int -> Int -> Float
f  = ... -- 右边填写lambda表达式
```

**输入样例：** 
例如： 
```haskell
2 3
```
**输出样例：**
例如，`f 2 3` 的结果： 
```haskell
0.6666667
```
题解如下：
```hs
f :: Int -> Int -> Float
f = \x y -> (fromIntegral x) / (fromIntegral y) -- 右边填写lambda表达式
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/7a26092b18ca4f7fb470b0b501291531.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 6-2 Ordering (10 分) 
类型 `Ordering` 就像 `Bool` 一样，它有三个构造函数（或者三个值）：`LT, EQ, GT` 。试定义一个函数 `comp` ，比较两个整数的大小，并返回类型 `Ordering` 的值，以表达两个比较的三种结果。

**函数接口定义：**
```haskell
comp :: Int -> Int -> Ordering
```
`comp x y` 将根据 `x<y` ，`x==y` ，或者 `x > y` 是否为真，分别返回 `LT, EQ, GT` 。

**输入样例：** 
例如： 

```haskell
2 3
```
**输出样例：** 
例如，`comp 2 3` 的值为：
```haskell
LT
```
题解如下：
```hs
comp :: Int -> Int -> Ordering
comp = \x y -> if (x < y) then LT
               else if (x == y) then EQ else GT
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/c9e9bbef75004204a93cb45fa9389d41.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 6-3 判断列表的有序性 (10 分)

判断一个列表是否按照给定的比较函数有序。

**函数接口定义：**
```haskell
ordered :: (a -> a -> Ordering) -> [a] -> Bool
```
例如 `ordered (\x y -> if x>=y then LT else GT) [1,2,3]` 返回 `False` ，`ordered (\x y -> if x>=y then LT else GT) [3,2,1]` 返回 `True` ，因为比较函数 `(\x y -> if x>=y then LT else GT)` 表达了逆序。

**提示**
所谓列表 `xs` 按照给定比较函数 `comp :: a -> a -> Ordering` “从小到大有序”，是指对于 `xs` 中任意两个相邻元素 `x, y`（`x` 在 `y` 之前），`comp x y` 的值为 `LT`（`x` "小于" `y` ）或者 `EQ`（`x` "等于" `y` ）。

**输入样例：**
例如：
```haskell
1
[1,2,3]
```

**输出样例：** 
例如，如果比较函数表达了从小到大： 
```haskell
True
```
题解如下：
```hs
ordered :: (a -> a -> Ordering) -> [a] -> Bool
ordered comp [] = True
ordered comp [x] = True
ordered comp (x : xs) = (comp x (head xs) == LT || comp x (head xs) == EQ) && (ordered comp xs)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/668c5cd8da0447cdb540f439b22efb6e.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 6-4 带参数的有序列表插入 (10 分)
熟悉库函数 `sortBy` (需要 `import Data.List` )，然后实现带比较参数的有序列表插入。参见习题5.8。

**函数接口定义：** 
 
```haskell
insert :: (a -> a -> Ordering) -> a -> [a] -> [a]
```
注意，第三个输入参数是按照第一个参数给定的比较有序的，插入结果仍然按照第一个参数有序。

**输入样例：** 
例如： 

```haskell
1
2
[1,2,3]
```
**输出样例：** 
例如，将 `2` 插入 `[1,2,3]` 的结果（从小到大）：
```haskell
[1,2,2,3]
```
题解如下：

```haskell
insert :: (a -> a -> Ordering) -> a -> [a] -> [a]
insert cmp num [] = [num]
insert cmp num (x : xs)
    | (cmp num x == LT || cmp num x == EQ) = num : (x : xs)
    | otherwise = x : (insert cmp num xs)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/4a385f36f90e4c8eba97c478a3513f68.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

# 6-5 带参数的插入排序 (10 分)
现在可以实现带参数的插入排序了！

**函数接口定义：**  
```haskell
insertionSort :: (a -> a -> Ordering) -> [a] -> [a]
```
**输入样例：** 
例如： 
```haskell
1
[2,1,2,3]
```

**输出样例：** 
例如，将 `[2,1,2,3]` 从小到大排序结果：
```haskell
[1,2,2,3]
```
题解如下：
```hs
insert :: (a -> a -> Ordering) -> a -> [a] -> [a]
insert cmp num [] = [num]
insert cmp num (x : xs)
    | (cmp num x == LT || cmp num x == EQ) = num : (x : xs)
    | otherwise = x : (insert cmp num xs)
    
insertionSort :: (a -> a -> Ordering) -> [a] -> [a]
insertionSort cmp [] = []
insertionSort cmp (x : xs) = insert cmp x (insertionSort cmp xs)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/4b50e9cb93f54c99bfc53f54334213e1.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

