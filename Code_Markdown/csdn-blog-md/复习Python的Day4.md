# 复习Python的Day4
[TOC]

![image](http://img1.imgtn.bdimg.com/it/u=788905235,4280771310&fm=27&gp=0.jpg)

## 习题 5: 更多的变量和打印 

每一次使用" "把一些文本引用起来，你就建立了一个字符串，的的确确，字符串是非常好用的东西。

> 字符串是程序将信息展示给人的方式。你可以打印它们，可以将它们写入文件，还可以将它们发送给网站服务器，很多事情都是通过字符串交流实现的。 

Python中使用' '或" "来表示字符串，字符串可以包含格式化字符%s，用法已经练习过：将格式化的变量放到字符串中，其后紧跟百分号%，再写变量名即可。**多变量要使用括号**，**变量用逗号隔开**。学会使用字符串和格式化字符可以节约时间。
 
**本节练习重点:**
**学会如何创建包含变量内容的字符串。**

>使用专门的格式和语法把变量的内容放到字符串里，相当于来告诉 python:“嘿，这是一个格式化字符串，把这些变量放到那几个位置。” 

### 习题代码
```python
my_name = 'Zed A. Shaw'
my_age = 35
my_height = 74
my_weight = 180
my_eyes = 'blue'
my_teeth = 'White'
my_hair = 'Brown'

print "Let's talk about %s." % my_name
print "He's %d inches tall." % my_height
print "He's %d pounds heavy." % my_weight
print "Actually that's not too heavy."
print "He's got %s eyes and %s hair." % (my_eyes, my_hair)
print "His teeth are usually %s depending on the coffee." % my_teeth
print "If I add %d, %d, and %d I get %d." % (my_age, my_height, my_weight, my_age + my_height + my_weight)
```

### 加分习题 

3. 在网上搜索所有的 Python 格式化字符。（已做）
4. 试着使用变量将英寸和磅转换成厘米和千克。不要直接键入答案。使用 Python的计算功能来完成。 
```python2
# transform.py
my_height_ft = 74 
my_height_cm = 74 * 2.540005
my_weight_lb = 180
my_weight_kg = 180 * 0.454

print "I'm %d cm tall." % my_height_cm
print "I'm %d kg heavy." % my_weight_kg
```
***
## 格式化操作符（%）

"%"是Python风格的字符串格式化操作符，非常类似C语言里的printf()函数的字符串格式化（C语言中也是使用%）。

下面整理了一下Python中字符串格式化符合：
> 这里我的整理是按照常用程度来的，由高到低。

|格式化符号     |       说明                         |    
|:-----------------|:-----------------------------------------------------|
|%s         |       优先用str()函数进行字符串转换      |
|%d / %i       |      转成有符号十进制数                 | 
|%e / %E       |     转成科学计数法（e / E控制输出e / E）       |
|%f / %F      |      转成浮点数（小数部分自然截断）          |
|%g / %G      |      %e和%f / %E和%F 的简写（(根据值的大小采用%e或%f/%E  或%F）|
|%%       |        输出% （格式化字符串里面包括百分号，那么必须使用%%）|
|%r         |       优先用repr()函数进行字符串转换       |
|%c           |     转换成字符（ASCII 码值，或者长度为一的字符串）(  %c requires int or char)|
|%n         |      存储输出字符的数量放进参数列表的下一个变量中  |    
|%o         |       转成无符号八进制数               |
|%p         |       指针(用十六进制打印值的内存地址)     |
|%u         |       转成无符号十进制数              |
|%x / %X     |       转成无符号十六进制数（x / X 代表转换后的十六进制字符的大小写）  |

  ······

这里列出的格式化字符都比较简单。
**唯一想要强调一下**的就是"%s"和"%r"的差别。

接下来，我用一些实例来测试了一下这些格式化字符。
```python2
# 试着使用更多的格式化字符
# char1 = ''  
# %c不接受标点符号
>>> char2 = 66
>>> char3 = 'a'
>>> print "ascll码表66和'a'代表%c和%c" %(char2,char3)
ascll码表66和'a'代表B和a
```
```python2
>>> num1 = 0xEF3
>>> num2 = 0xAB03
>>> print "转换成无符号十进制分别为：%u和%u" %(num1,num2)
>>> print "转换为有符号十进制分别为：%d和%d" %(num1,num2)
>>> print "转换为有符号十进制分别为：%i和%i" %(num1,num2)
转换成十进制分别为：3827和43779
转换为十进制分别为：3827和43779
转换为十进制分别为：3827和43779
```
```python2
>>> num3 = 1200000
>>> print "转换为科学计数法为：%e" % (num3)
>>> print "转换为科学计数法为：%E" % (num3)
>>> print "转换为科学计数法为：%g和%G" %(num3,num3)
转换为科学计数法为：1.200000e+06
转换为科学计数法为：1.200000E+06
转换为科学计数法为：1.2e+06和1.2E+06

>>> num4 = 1.
>>> num5 = 1
>>> print num4,num5
>>> print "以浮点数输出：%f和%F" %(num4,num4)
>>> print "以浮点数输出：%f和%F" %(num5,num5)
>>> num6 = 1.6666666
>>> print "以浮点数输出为：%f和%F" % (num6,num6) # 自然截断为六位数
1.0 1
以浮点数输出：1.000000和1.000000
以浮点数输出：1.000000和1.000000
以浮点数输出为：1.666667和1.666667
```
```python2
# "%s"和"%r"的差别
>>> string = 'Hello\tworld!\n'
>>> print "%s" % string
>>> print string
>>> print "%r"% string
>>> print repr(string)
>>> print eval(repr(string))
Hello	world!

Hello	world!

'Hello\tworld!\n'
'Hello\tworld!\n'
Hello	world!
```
这里的差异是str()和repr()两个**内建函数之间的差异**：

 - str()得到的字符串是面向用户的，具有较好的可读性。
 - repr()得到的字符串是面向机器的。
 - 通常(不是所有)repr()得到的效果是：obj == eval(repr(obj))

## 格式化操作符辅助符
**通过"%"可以进行字符串格式化，但是"%"经常会结合下面的辅助符一起使用。**

|辅助符号      |       说明                 |
|:---------------|:----------------------------------------------
|星号         |    定义宽度或者小数点精度        |
|-         |     用做左对齐                | 
|+        |    在正数前面显示加号(+)             |
|0       |    显示的数字前面填充"0"而不是默认的空格 |
|(var)    |  映射变量（通常用来处理字段类型的参数）    |
|#          |  在八进制数前面显示零(0)，在十六进制前面显示"0x"或者"0X"（取决于用的是"x"还是"X"） |
|m.n     |    m 是显示的最小总宽度，n 是小数点后的位数（如果可用的话）     |


![image](https://img-blog.csdnimg.cn/2022010701174215982.jpeg)