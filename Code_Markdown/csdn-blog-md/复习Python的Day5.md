# 复习Python的第五天

[TOC]



## 习题6：字符串(string)和文本 

第6课讲字符串和文本。
>字符串通常是用来展示给别人或是从程序中导出一段字符。
>Python是通过使用双引号或单引号识别字符串。
>字符串可以包含格式化字符%s，用法之前已经练习过：将格式化的变量放到字符串中，其后紧跟百分号%，再写变量名即可。
>多变量要使用括号，变量用逗号隔开。
>学会使用字符串和格式化字符可以节约时间。
>当然，也要学会简写，使用简写的变量名。要使用“驼峰式写法”。

给出的代码如下：
```python2
x = "There are %d types of people." % 10
binary = "binary"
do_not= "don't"
y = "Those who know %s and those who %s." % (binary, do_not)

print x
print y

print "I said: %r." % x
print "I also said: '%s'." %y
hilarious = False
joke_evaluation = "Isn't that joke so funny?! %r"

print joke_evaluation % hilarious

w = "This is the left side of..."
e = "a string with a right side."

print w + e
```

非常好理解，这里不再一一赘述其原理，只说需要注意的地方：

可以使用语句里面插入%s或者%r 来输出变量的值，该值将输出到插入的地方。

>C语言使用&来指明变量，而Python使用%，如果有多个变量，可以使用括号括住，逗号加空格隔开。

不使用双引号，也不用在语句中插入%s或者%r，直接print加变量名就可以输出，也可以直接使用%加变量名的方式输出。例子print joke_evaluation % hilarious

**%r 和%s格式化字符区别**
%r	优先用repr()函数进行字符串转换（Python2.0新增）
%s	优先用str()函数进行字符串转换

**str()和repr()的区别：**

 - str() 是该对象产生的一个字符串，多用于显示方便，不保证该字符串可以通过eval() 之类的动态解析函数重新生成原始对象。
 - repr() 是该对象产生的一个代表字符，保证可以用这个代表字符重新生成原对象。


### 格式化操作符（%）

"%"是Python风格的字符串格式化操作符，非常类似C语言里的printf()函数的字符串格式化（C语言中也是使用%）。

下面整理了一下Python中字符串格式化符合：

|格式化符号     |       说明                         |    
|:-----------------|:-----------------------------------------------------|
|%c           |     转换成字符（ASCII 码值，或者长度为一的字符串）(  %c requires int or char)|
|%d / %i       |      转成有符号十进制数                 | 
|%e / %E       |     转成科学计数法（e / E控制输出e / E）       |
|%f / %F      |      转成浮点数（小数部分自然截断）          |
|%g / %G      |      %e和%f / %E和%F 的简写（(根据值的大小采用%e或%f/%E  或%F）|
|%n         |      存储输出字符的数量放进参数列表的下一个变量中  |    
|%o         |       转成无符号八进制数               |
|%p         |       指针(用十六进制打印值的内存地址)     |
|%r         |       优先用repr()函数进行字符串转换       |
|%s         |       优先用str()函数进行字符串转换      |
|%u         |       转成无符号十进制数              |
|%x / %X     |       转成无符号十六进制数（x / X 代表转换后的十六进制字符的大小写）  |
|%%       |        输出% （格式化字符串里面包括百分号，那么必须使用%%）|

这里列出的格式化符合都比较简单，唯一想要强调一下的就是"%s"和"%r"的差别。

### 格式化操作符辅助符
** 通过"%"可以进行字符串格式化，但是"%"经常会结合下面的辅助符一起使用。**

|辅助符号      |       说明                 |
|:---------------|:----------------------------------------------
|星号         |    定义宽度或者小数点精度        |
|-         |     用做左对齐                | 
|+        |    在正数前面显示加号(+)             |
|0       |    显示的数字前面填充"0"而不是默认的空格 |
|(var)    |  映射变量（通常用来处理字段类型的参数）    |
|#          |  在八进制数前面显示零(0)，在十六进制前面显示"0x"或者"0X"（取决于用的是"x"还是"X"） |
|m.n     |    m 是显示的最小总宽度，n 是小数点后的位数（如果可用的话）     |
