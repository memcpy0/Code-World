打算把不同网站上面的题目分开整理，免得麻烦。Code War上面我还是刷了一堆6级及以下的题目的，不过价值不大，这种不太能够训练实际解决问题的能力，所以我已经很久没上过了，有时间了可能会重新上去刷题吧，到时候再更新这篇文章。对于刷题类的文章，我都准备长期更新。
## 180720 Simple Pig Latin
- **Exercise** 
**Move the first letter of each word to the end of it, then add "ay" to the end of the word. Leave punctuation marks untouched.**
- **Examples**
> pig_it('Pig latin is cool')   # igPay atinlay siay oolcay
   pig_it('Hello world !')       # elloHay orldWay !
- **Source of the exercise**
From [Wikipidia](https://en.wikipedia.org/wiki/Pig_Latin)
> Pig Latin is a **language game or argot** in which words in English are **altered**, usually by adding a fabricated suffix or by moving the onset or initial consonant or consonant cluster of a word to the end of the word and adding a vocalic syllable to create such a suffix. 
- **My own thoughts**
1. 以空格为识别符分割字符串text为单词列表；
2. 遍历列表，对在`string.punctuation`中的标点符号和不在其中的英语单词分别处理。
3. 重新拼接，组成并返回新字符串。
```python
>>> string.punctuation
'!"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~'
```
#### My own codes(Python)
```python
def pig_it(text):
    import string
    text_list = text.split()
    space = []
    for latin in text_list:
        if latin not in string.punctuation:
            space.append(latin[1:]+latin[0]+"ay")
        else:
            space.append(latin)
    return " ".join(space)
```
#### THE BEST CODES
```python
def pig_it(text):
    lst = text.split()
    return ' '.join(word[1:] + word[:1] + 'ay'\
    if word.isalpha() else word for word in lst)
```
用了**生成器表达式**和**字符串的相关方法**。

生成器表达式中的条件语句使用方法拓展了我的表达方式，原本一直写成以下形式：
```python
[res for x in s if ……
     for y in n if ……]
```
现在使用`res1 if condition else res2`强化了条件判断的能力。

 `string.isalpha()`方法: 检测字符串是否只由字母组成。原本我是判断是否是标点符号的，现在则可以判断字符串是否整个都是由字母组成的。
1. **语法**：str.isalpha()  
```python
 S.isalpha() -> bool
   Return True if all characters in S are alphabetic
   and there is at least one character in S, False otherwise.
   如果字符串至少有一个字符
   并且所有字符都是字母则返回True,
   否则返回False
```
2. **实例:**
```python
>>> "123".isalpha()
False
>>> "abc".isalpha()
True
>>> "abc!".isalpha()
False
```
3. 类似方法
```python
string.isalnum()   至少有一个字符并且所有字符都是字母或数字
string.isdecimal() 只包含十进制数字
string.isdigit()   只包含数字
string.isnumeric() 只包含数字字符
string.isspace()   只包含空格
string.istitle()   仅包含以大写字母开头、后面都是小写字母的单词。
string.islower()   包含至少一个区分大小写的字符，并且所有这些(区分大小写的)字符都是小写
string.isupper()   至少一个区分大小写的字符，并且所有这些(区分大小写的)字符都是大写
```

---
## 180801 Diophantine Equation
- **Exercise**
**In mathematics, a Diophantine equation is a polynomial equation, usually with two or more unknowns, such that only the integer solutions are sought or studied.**
**In this kata we want to find all integers x, y (x >= 0, y >= 0) solutions of a diophantine equation of the form: x2 - 4 * y2 = n(where the unknowns are x and y, and n is a given positive number) in decreasing order of the positive xi.**
**If there is no solution return [] or "[]" or "". **
- **Examples**
> solEquaStr(90005) --> "[[45003, 22501], [9003, 4499], [981, 467], [309, 37]]"
> solEquaStr(90002) --> "[ ]"
- **Source of the exercise**
> [丢番图方程](https://baike.baidu.com/item/%E4%B8%A2%E7%95%AA%E5%9B%BE%E6%96%B9%E7%A8%8B/5466939?fromtitle=Diophantine%20Equation&fromid=11235831&fr=aladdin)（Diophantine Equation）
> 有一个或者几个变量的整系数方程，它们的求解仅仅在整数范围内进行(with two or more unknowns, such that only the integer solutions are sought or studied)。最后这个限制使得丢番图方程求解与实数范围方程求解有根本的不同。丢番图方程又名不定方程、整系数多项式方程，是变量仅容许是整数的多项式等式。
![这里写图片描述](https://img-blog.csdn.net/20180801175738974?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Also can see [Wikipidia](https://en.wikipedia.org/wiki/Diophantine_equation).
- **My own thoughts**
#### My own codes
这题我没有写出来，最佳代码如下。
#### THE BEST CODES
**Hint**
      x2 - 4 * y2 = (x - 2*y) * (x + 2*y)

```python
from math import sqrt
def sol_equa(n):
    res = []
    for i in range(1, int(sqrt(n)) + 1):
        if n % i == 0:
            j = n // i
            if (i + j) % 2 == 0 and (j - i) % 4 == 0:
                x = (i + j) // 2
                y = (j - i) // 4
                res.append([x, y])        
    return res
```

