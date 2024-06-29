# 复习Python的第六天

## 习题7：更多打印
目的是为了熟练打字吧。
> 代码部分
```python2
# 纯打印
print "Mary had a little lamb."
# 打印格式化字符串
print "Its fleece was white as %s." % 'snow'
# 纯打印
print "And everywhere that Mary went." # 打漏了这一行
# 复制十倍
print "." * 10 # what'd that do?

# 赋值
end1 = "c"
end2 = "h"
end3 = "e"
end4 = "s"
end5 = "s"
end6 = "e"
end7 = "B"
end8 = "u"
end9 = "r"
end10 = "g"
end11 = "e"
end12 = "r"

# watch that comma at the end .try removing it to see what happens
# 拼接和打印
print end1 + end2 + end3 + end4 + end5 + end6 , # 和下文结合
print end7 + end8 + end9 + end10 + end11 + end12
```

关于输出字符串的格式，逗号的问题：
```python2
>>> print 'Hello'
>>> print 'World'
Hello
World
>>> print 'Hello',
>>> print 'World'
Hello World
```
它使得上一次输出的结果和下次输出的结果处于同一行。这里说明的是，逗号表示该输出内容没有结束，即使到了输出另一个语句，也会随着前一行后面输出；而去掉逗号，就是再回车到下一行，分行输出。

Python3没有这种结果。

还有一点，print “.” * 10 # what’d that do? 
这里使用计算的方式给出了打印10个*号的方法，**这个方法蛮不错的，记住它能简化代码**。

![image](https://timgsa.baidu.com/timg?image&quality=80&size=b9999_10000&sec=1521555983367&di=0348b6172e97d444b8fae7966275e339&imgtype=0&src=http%3A%2F%2Fstatic.mukewang.com%2Fimg%2F58257a3b000164cd06000338.jpg)
