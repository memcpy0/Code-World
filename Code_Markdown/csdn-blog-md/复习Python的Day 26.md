# 复习Python的第二十六天

## 习题28: 布尔表达式练习
“布尔逻辑表达式(boolean logic expression)”在程序中很常见，它们经常用于条件测试，而且用布尔表达式还能编写出一些精巧的语句。因此我们必须掌握。
```python2
>>> True and True 
True
>>>  False and True
False
>>>  1 == 1 and 2 == 1
False
>>>  "test" == "test" 
True
>>>  1 == 1 or 2 != 1 
True
>>>  True and 1 == 1
True
>>>  False and 0 != 0 
False
>>>  True or 1 == 1 
True
>>>  "test" == "testing" 
False
>>>  1 != 0 and 2 == 1 
False
>>>  "test" != "testing" 
True
>>>  "test" == 1 
False
>>>  not (True and False)
True
>>>  not (1 == 1 and 0 != 1)
False 
>>>  not (10 == 1 or 1000 == 1000) 
False
>>>  not (1 != 10 or 3 == 4) 
False
>>>  not ("testing" == "testing" and "Zed" == "Cool Guy") 
True
>>>  1 == 1 and not ("testing" == 1 or 1 == 0) 
True
>>>  "chunky" == "bacon" and not (3 == 4 or 3 == 3) 
False
>>>  3 == 3 and not ("testing" == "testing" or "Python" == "Fun" )
False
```
判断步骤：

 1. 找到**相等判断的部分** (== or !=)，将其改写为其最终值 (True 或 False)。 
 2. 找到**括号里的 and/or**，先算出它们的值。 
 3. 找到**每一个not**，算出它们反过来的值。 
 4. 找到剩下的 and/or，解出它们的值。
 5. 等你都做完后，剩下的结果应该就是 True 或者 False 了。 

这个步骤很合理，因为相不相等一眼就看得出来，而寻找括号是因为其优先级最高，not的判断也很简单。

这样清理下来，剩下的就简单了，我们也不需要从头看到尾、从左至右地扫视。那样吃力不讨好。