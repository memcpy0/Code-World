# 复习Python的Day2

> 自我反省一下，说了要一天写一篇的文章的，结果又摸了几天鱼。虽然是因为搞微信公众号去了……

![教材](//img-blog.csdn.net/2018031319305278?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
 
 [TOC]
***

##  习题 2: 注释和井号 

> 小知识：井号有很多的英文名字，例如：’octothorpe(八角帽)’，’pound(英镑符)’, ‘hash(电话 的#键)’, ‘mesh(网)’ 等。

任何一门程序语言，最重要的元素之一必然是注释。

其功能有二：
1. 它们可以**用自然语言**告诉你某段代码的功能是什么(也可以告诉别人你的代码在做什么)；
2.在你想要**临时移除**一段代码时，可以用注解的方式将这段代码临时禁用

之所以说是临时，是因为在修改和重构代码的时候，不确定是否要删去这段代码，是故临时禁用。

> 练习部分

```Python
# A comment, this is so you can read your program later.

# Anything after the # is ignored by python.

print "I could have code like this. "

# and the comment after is ignored

#you can also use a comment to "disable" or comment out a piece of code:

#print "This won't run."

print "This will run."
```

"#"是一个特殊符号。

除此以外，我还发现了一个有趣的符号“@”，暂时还没琢磨清它的用法，以后再写一下。

***

## 习题 3: 数字和数学计算 
这一节是关于数学运算符号的。

> + `+` plus 加号 
- ` -` minus 减号
+ ` / `slash 斜杠 
+  `* `asterisk 星号
+ ` %` percent 百分号 
+ `>`greater-than 大于号 
+  `<` less-than 小于号
+ ` <=` less-than-equal 小于等于号
+ ` >=` greater-than-equal 大于等于号 

让我们通过练习掌握它们吧！

```
print "I will now count my chickens:" 
print "Hens", 25 + 30 / 6 
print "Roosters", 100 - 25 * 3 % 4 
print "Now I will count the eggs:" 
print 3 + 2 + 1 - 5 + 4 % 2 - 1 / 4 + 6 
print "Is it true that 3 + 2 < 5 - 7?" 
print 3 + 2 < 5 - 7 
print "What is 3 + 2?", 3 + 2 
print "What is 5 - 7?", 5 - 7 
print "Oh, that's why it's False." 
print "How about some more."  
print "Is it greater?", 5 > -2 
print "Is it greater or equal?", 5 >= -2 
print "Is it less or equal?", 5 <= -2 
```

**加分习题：**
1.使用 # 在代码每一行的前一行为自己写一个注解，说明一下这一行的作用。 
2.使用**浮点数**重写一遍 `ex3.py`,让它的计算结果更准确(提示: 20.0 是一个浮点数)。
```python
# 输出 I will now count my chickens:
print "I will now count my chickens:"
# 计算 Hens
print "Hens",25.0 + 30 /6
# 计算 Roosters
print "Roosters", 100.0 - 25 * 3 % 4
# 输出 How I will count the eggs:
print "How I will count the eggs:"
# 计算 eggs
print 3.0 + 2 + 1 - 5 + 4 % 2 - 1 / 4 + 6
# 输出 Is it true that 3 + 2 < 5 - 7 ?
print "Is it true that 3 + 2 < 5 - 7 ?"
# 判断 3 + 2 和 5 - 7 的关系，输出结果
print 3.0 + 2 < 5 - 7
# 计算 3 + 2
print "What is 3 + 2 ?",3.0 + 2
# 计算 5 - 7
print "What is 5 - 7 ?",5.0 - 7
# 输出 Oh,that's why it's False.
print "Oh,that's why it's False."
# 输出 How about some more.
print "How about some more."
# 进行判断
print "Is it greater ?",5 > -2
print "Is it greater or equal ?",5 >= -2
print "Is it less or equal ?",5 <= -2
```

> 注：Python中的数学运算符并不只是用于数学运算，它们也可以使用在字符串、列表、字典的操作中。**用处意外的大！**
> 比如'+' 可以用于拼接字符串。

```python
>> It' + ' ' + 'is' + 'an apple.'
'It is an apple.'
```

![image](http://img.blog.csdn.net/20180320192202172?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
