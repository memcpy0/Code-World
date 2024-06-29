# 复习Python的第三十一天

## 习题 34: 访问列表的元素
```python2
animals = ['bear', 'tiger', 'penguin', 'zebra']
bear = animals[0]
```
列表的每一个元素都应该有一个地址，或者一个index（索引），或者是offset(数据偏移值)，而最好的方式是使用以0开头的索引。
`ordinal == 有序，以 1 开始；cardinal == 随机选取, 以 0 开始`

***
## 习题 35: 分支和函数
我们已经学会了if语句 、函数，还有列表，可以写一个小的文字游戏了。
```python
from sys import exit

def gold_room():
    print "This room is full of gold .How much do you take?"
    
    next = raw_input(">")
    if "0" in next or "1" in next :
        how_much = int(next)
    else:
        dead("Man, learn to type a number.")
        
    if how_much < 50:
        print "Nice, you're not greedy, you win."
        exit(0)
    else:
        dead("You greedy bastard(渣渣)!")
        
def bear_room():
    print "There is a bear here."
    print "The bear has a bunch of honey."
    print "The fat bear is in front of another door."
    print "How are you going to move the bear?"
    bear_moved = False
    
    while True:
        next = raw_input(">")
        
        if next == "take honey":
            dead("The bear looks at you then slaps(拍打) your face off.")
        elif next == "taunt bear" and not bear_moved: # taunt 嘲笑；奚落
            print "The bear has moved from the door.You can go through it now."
            bear_moved = True
        elif next == "taunt bear" and bear_moved:
            dead("The bear gets pissed off and chews your leg off.")
        elif next == "open door" and bear_moved:
            gold_room()
        else:
            print "I got no idea what that means."

def cthulhu_room():
    print "Here you see the great evil Cthulhu."
    print "He, it, whatever stares at you and you go insane."
    print "Do you flee for your life or eat your head?"
    
    next = raw_input(">")
    
    if "flee" in next:
        start()
    elif "head" in next:
        dead("Well that was tasty!")
    else:
        cthulhu_room()

def dead(why):
    print why,"Good job!"
    exit(0)
    
def start():
    print "You are in a dark room."
    print "There is a door to your right and left."
    print "Which one do you take?"
    
    next = raw_input(">")
    
    if next == "left":
        bear_room()
    elif next == "right":
        cthulhu_room()
    else:
        dead("You stumble(摔得) around the room until you starve.")
        
start()
```
尴尬的是有些英文单词我不认识，果然，英语不好游戏都玩不了(手动滑稽)。

```flow
st=>start: start( )
e=>end: 结束
op1=>operation: 输入left或right或其他
op2=>operation: bear_room()
op3=>operation: cthulhu_room()
cond=>condition: 确认？
op=>operation: 我的操作

st->op1->cond
cond(yes)->op2
cond()->op
```
算了，先不写了，CSDN的流程图语法我还不熟。

## 习题36：设计和调试
一些设计和调试的小技巧。
#### if的规则
1.  每一个“if 语句”必须包含一个  else。
2. 如果这个else永远都不应该被执行到，因为它本身没有任何意义，那你必须在else 语句后面使用一个叫做die的函数，让它打印出错误信息并且死给你看，这
和上一节的习题类似，这样你可以找到很多的错误。
3. “if 语句”的嵌套不要超过 2 层，最好尽量保持只有 1 层。 这意味着如果你在 if 里边又有了一个 if（即二次嵌套），那你就需要把第二个 if 移到另一个函数里面。
4. 将“if 语句”当做段落来对待，其中的每一个 if, elif, else组合就跟一个段落
的句子组合一样。在这种组合的最前面和最后面留一个空行以作区分。
5. 你的布尔测试应该很简单，如果它们很复杂的话，你需要将它们的运算事先放到一个变量里，并且为变量取一个好名字。

第一、四点很简单，第三、五点不一定会注意到。

#### 循环的规则

1.  **只有在循环永不停止时使用“while 循环”**，这意味着你可能永远都用不到。这条只有Python 中成立，其他的语言另当别论。
2. 其他类型的循环都使用“for 循环”，尤其是在循环的对象数量固定或者有限的情况下。

#### debug小技巧

1.  不要使用 “debugger”。 Debugger 所作的相当于对病人的全身扫描。你并不会得到某方面的有用信息，而且你会发现它输出的信息态度，而且大部分没有用，或者只会让你更困惑。
2. **最好的调试程序的方法是使用 print在各个你想要检查的关键环节将关键变量打印出来**，从而检查哪里是否有错。
3. **让程序一部分一部分地运行起来**。不要等一个很长的脚本写完后才去运行它。写一点，运行一点，再修改一点。

这些原则的确很重要，不重视的话会吃亏的，就像我。

