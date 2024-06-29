# 复习Python的第二十七天

## 习题 29: 如果(if) 

```python
people = 20 
cats = 30 
dogs = 15 
  
if people < cats: 
    print "Too many cats! The world is doomed!" 
 
if people > cats: 
    print "Not many cats! The world is saved!" 
 
if people < dogs: 
    print "The world is drooled on!" 
 
if people > dogs: 
    print "The world is dry!" 
  
dogs += 5 
 
if people >= dogs: 
    print "People are greater than or equal to dogs." 
 
if people <= dogs: 
    print "People are less than or equal to dogs." 
  
if people == dogs: 
    print "People are dogs."
```
if(if-statements)语句，表示“如果”，后面接条件判断表达式，根据表达式的TRUE或FALSE来决定是否执行后续语句。

if语句主体需要缩进，如不缩进，会抛出`IndentationError: expected an indented block`异常。

上一题的布尔表达式放在if语句也能运行，因为它们同样返回True或False，当然，也不一定总是这两个定制词，有时也会返回对象。

因为在Python中，True表示任何不为空的对象，False表示任何为空的对象，如0、[ ]、{ }、' '等。因此有时会根据布尔逻辑返回相应的对象。

另外，布尔运算符和比较运算符能够组成相当复杂的条件表达式，放在if语句中，以判断复杂的分类。