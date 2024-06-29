# 复习Python第二十天
[TOC]
## 习题 21: 函数可以返回东西 

```python2
# 定义加法函数
def add(a,b):
    print "ADDING %d + %d" %(a,b)
    return a+b
# 定义减法函数
def subtract(a,b):
    print "SUBTRACTING %d - %d" %(a,b)
    return a - b
# 定义乘法函数
def multiply(a,b):
    print "MULTPLYING %d * %d" %(a,b)
    return a * b
# 定义除法函数
def divide(a,b):
    print "DIVIDING %d / %d" %(a,b)
    return a/b

print "Let's do some math with just functions!"
# 年龄，调用函数add，打印一条信息，同时返回a+b的值
age = add(30,5)
# 同上
height = subtract(78,4)
# 同上
weight = multiply(90,2)
# 同上
iq = divide(100,2)

print "Age:%d, Height:%d ,weight:%d,IQ:%d" % (age,height,weight,iq)

# A puzzle for the extra credit,type it in anyway.
print "Here is a puzzle."
# 函数嵌套调用
what = add(age,subtract(height,multiply(weight,divide(iq,2))))

print "That becomes:" , what ,"Can you do it by hand?"
```
函数调用嵌套图:
![](http://img.blog.csdn.net/20180404193153797?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
运行结果：
```python
Let's do some math with just functions!
ADDING 30 + 5
SUBTRACTING 78 - 4
MULTPLYING 90 * 2
DIVIDING 100 / 2
Age:35, Height:74 ,weight:180,IQ:50
Here is a puzzle.
DIVIDING 50 / 2
MULTPLYING 180 * 25
SUBTRACTING 74 - 4500
ADDING 35 + -4426
That becomes: -4391 Can you do it by hand?
```

> 现在我们创建了我们自己的加减乘除数学函数： add, subtract, multiply, 以 及 divide。重要的是函数的最后一行，例如 add 的最后一行是 return a+b。意指返回a+b的值，并将赋值给一个变量。

> 注：任何可以放在 = 右边的东西都可以作为一个函数的返回值。

在这一节中我觉得最重要的是，我们在函数调用的时候，既能执行函数功能，又能同时返回(return)值创建变量。

## 加分习题
用正常的方法实现和这个表达式一样的功能。
```python2
a = divide(iq, 2)
b = multiply(weight, a)
c = subtract(height, b)
d = add(age, c)
```
就把它拆解就可以了。
其他习题没什么好做的。

![](http://img.blog.csdn.net/20180404194311874?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)