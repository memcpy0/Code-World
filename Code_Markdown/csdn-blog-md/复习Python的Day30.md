# 复习Python的第三十天
## 习题33：While 循环
本节介绍的是while-loop`（while循环）。`while-loop会**一直**执行它下面的代码片段，直到它对应的布尔表达式为False时才会停下来。

书中提到：
> 为了避免“死循环”，你需要遵循下面的规定： 
> 1. 尽量少用while-loop，大部分时候 for-loop 是更好的选择。 
> 2. 重复检查你的 while语句，确定你测试的布尔表达式最终会变成False 。
> 3. 如果不确定，就在 while-loop 的结尾打印出你要测试的值。看看它的变化。

```python
i = 0 
numbers = [] 
while i < 6: 
	print "At the top i is %d" % i 
	numbers.append(i)
    i = i + 1 
    print "Numbers now: ", numbers 
    print "At the bottom i is %d" % i 

print "The numbers: " 
for num in numbers: 
    print num
```
## 加分习题：
几个题就一起写了：
```python
# -*- coding : utf-8 -*-
numbers = []

def while_loop(var, begin=0, stride=1):
    while begin < var:
        print "At the top i is %d" % begin
        numbers.append(begin)
            
        begin += stride
        
        print "Numbers now:", numbers
        print "At the bottom i is %d" % begin
            
    print "The numbers:"
    for num in numbers:
        print num
        
var = int(raw_input("Please input a number:"))
# begin = int(raw_input("Please input the beginning number:"))
stride = int(raw_input("Please input the stride number:"))
while_loop(var, stride)
```

注意：raw_input返回的是字符串str，必须转换为int对象，不然会失误。即使你输入的是数字如6，Python得到的将是'6'的ASCII码值。

Python中，将ASCII字符转换为对应的ASCII码数值，如：‘a’-->65，使用ord函数，`ord('a')`。
反之，使用chr函数，将数值转换为对应的ASCII字符，`chr(65)`。