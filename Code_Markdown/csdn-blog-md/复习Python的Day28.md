# 复习Python第二十八天

## 习题 30: Else 和 If
```python2
people = 30 
cars = 40 
buses = 15 
  
if cars > people: 
    print "We should take the cars." 
elif cars < people: 
    print "We should not take the cars." 
else: 
    print "We can't decide." 
 
if buses > cars: 
    print "That's too many buses." 
elif buses < cars: 
    print "Maybe we could take the buses." 
else: 
    print "We still can't decide." 
 
if people > buses: 
    print "Alright, let's just take the buses." 
else: 
    print "Fine, let's stay home then." 
```
这节加入了elif (else if) 和else语句分支，用于双分支和多分支的判断任务。

事实上，这两个语句都是可选的，就算只用if语句（多个）就可以进行复杂的判断，但是用elif和else组成一个语句块会更合适。

## 习题 31: 作出决定 
以前我们学习的程序工作流程都是顺序结构，直线式进行，但现在可以开始创建包含条件判断的脚本了。 

> 这节的脚本中，你将需要向用户提问，依据用户的答案来做出决定。

```python2
# 你将进入一间有两扇门的黑暗房间，选择1或2？
print "You enter a dark room with two doors.  Do you go through door #1 or door #2?" 
# 输入 
door = raw_input("> ") 
# 是1的话 
if door == "1": 
    print "There's a giant bear here eating a cheese cake.  What do you do?" 
    print "1. Take the cake." 
    print "2. Scream at the bear." 
 
    bear = raw_input("> ") 
 
    if bear == "1": 
        print "The bear eats your face off.  Good job!" 
    elif bear == "2": 
        print "The bear eats your legs off.  Good job!" 
    else: 
        print "Well, doing %s is probably better.  Bear runs away." % bear 
 
elif door == "2": 
    print "You stare into the endless abyss at Cthulhu's retina."  # 克总的视线
    print "1. Blueberries." 
    print "2. Yellow jacket clothespins." 
    print "3. Understanding revolvers yelling melodies." 
 
    insanity = raw_input("> ") 
     
    if insanity == "1" or insanity == "2": 
        print "Your body survives powered by a mind of jello.  Good job!" 
    else: 
        print "The insanity rots your eyes into a pool of muck.  Good job!"  # 疯狂使你的双眼腐烂
 
else: 
    print "You stumble around and fall on a knife and die.  Good job!"
```
本节重点是嵌套的if语句，其中的一个分支将引向另一个分支的子分支。
上面的程序流程图如下：

![](https://img-blog.csdn.net/20180412000203606?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


