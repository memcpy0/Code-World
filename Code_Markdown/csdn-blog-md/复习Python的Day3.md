# 复习Python的Day3

[TOC]

## 习题4：变量(variable)和命名 

在编程中，**变量是用来指代某个东西的名字**。

但有时候起一个好名字(既简洁又达意)是件很困难的事情。

左边是变量名，用“=”给变量赋值。

### 习题代码
```python 
cars = 100 
space_in_a_car = 4.0 
drivers = 30 
passengers = 90 
cars_not_driven = cars - drivers 
cars_driven = drivers 
carpool_capacity = cars_driven * space_in_a_car 
average_passengers_per_car = passengers / cars_driven 
 
print "There are", cars, "cars available." 
print "There are only", drivers, "drivers available." 
print "There will be", cars_not_driven, "empty cars today." 
print "We can transport", carpool_capacity, "people today." 
print "We have", passengers, "to carpool today." 
print "We need to put about", average_passengers_per_car, "in each car." 
```

作者说：
>space_in_a_car 中的 _ 是 下划线(underscore) 字符。你要自己学会怎样打出这 个字符来。这个符号在变量里通常被用作假想的空格，用来隔开单词。 

个人感觉Python的命名规则和C很像，但是Python中使用变量无须像C一样进行类型声明，Python会自动赋予变量不同的类型。

这里我们也可以看出，Python中的print与C中的printf也有一些差异，Python里在什么位置输出变量对应的值，就在该位置使用两个逗号将变量名插进去，两个逗号之前和之后的文字描述都使用” “括起来。

当然，Python2的print输出方式不止这一种。**总结语法**如下：

```
print "String" , Variable ,"String"

print "String %s"  % Variable
```
其中%s 是格式化字符输出的符号之一，表示将后面的变量值作为字符串输出到“%s”的位置。

格式化字符日后会详细总结。

而作者遇到的错误`NameError: name 'car_pool_capacity' is not defined `，是因为作者打错了变量名，使得Python无法输(print)出相应变量值。

### 加分习题

1.在程序中用4.0作为 space_in_a_car的值，没有必要，因为这里不需要浮点数，改为4在该程序中没有问题。

3.**给每一个变量加注释**
```python
#汽车的数量是100
cars = 100
#汽车空间等于4
space_in_a_car = 4
#司机的数量等于30
drivers = 30
#乘客的数量等于90
passengers = 90
#汽车和司机数量之差
cars_not_driven = cars - drivers
#司机的数量
cars_driven = drivers
#可以合伙使用汽车的数量
carpool_capacity = cars_driven * space_in_a_car
#平均每辆车的乘客的数量
average_passengers_per_car = passengers / cars_driven
```
### 本节知识引申: Python 里面的浮点数

从作者的例子中，可以看出Python里面给变量赋值时用的是小数，该变量会自动被标识为浮点数。

Python有两个主要的数值类型：int（整数）和float（浮点数）。

浮点数的范围很大，但是，它们在表示有理数的时候不是非常精确。

Python还为科学计算提供了一个内置的复数类型。

Python的数据类型：

- int 带符号整数    举例： -1，0，0xE8C6，0377，42

- float 浮点数        举例： 1.25，4.3e+2，-5，-9.3e，0.375

- complex 复数    举例：2+2j，.3-j，-10.3e+5-60j

![image](http://img.blog.csdn.net/20180314190251475?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)