# 复习Python的第十八天

## 习题19：函数和变量 

```python2
# 定义一个函数，有两个形式参数
def cheese_and_crackers(cheese_count, boxes_of_crackers): 
    print "You have %d cheeses!" % cheese_count 
    print "You have %d boxes of crackers!" % boxes_of_crackers 
    print "Man that's enough for a party!" 
    print "Get a blanket.\n" 
 
 
print "We can just give the function numbers directly:" 
cheese_and_crackers(20, 30)  # 传入两个数字
 
 
print "OR, we can use variables from our script:" 
amount_of_cheese = 10 
amount_of_crackers = 50 
# 传入两个变量 
cheese_and_crackers(amount_of_cheese, amount_of_crackers) 
 
# 传入两个表达式 
print "We can even do math inside too:" 
cheese_and_crackers(10 + 20, 5 + 6) 
 
print "And we can combine the two, variables and math:" 
cheese_and_crackers(amount_of_cheese + 100, amount_of_crackers + 1000) 
```
本节没有什么新东西，若说有的话就是向函数内部传入参数的问题。

可以说，向函数内部传入参数，**本质上是一个隐式的赋值过程**。和for语句等都是python赋值机制的隐式运作。

作者也说：
> 函数的参数和我们的生成变量时用的 = 赋值符类似。事实上，如果一个物件你可以用 = 将其命名，你通常也可以将其作为参数传递给一个函数。
 
 因此我们可以在函数里用变量名，可以在函数里做运算，甚至可以将变量和运算结合起来。