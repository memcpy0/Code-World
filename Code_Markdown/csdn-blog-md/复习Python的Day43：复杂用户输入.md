# 复习Python的第四十三天
## 习题 48:  更复杂的用户输入
看完这一节，真心觉得处理用户输入是一门学问，不过在玩自己写的东西时也察觉到这一点了，如果是其他人来输入的话，说不定就不用玩了。

>每一个房间都需要一套自己的语句，而且只有用户完全输入正确后才能执行。

所以，我们需要对用户的输入进行扫描，不管用户以什么方式输入词汇，都能执行房间代码。甚至如果用户的输入和常用英语很接近也应该是可以的，而你的游戏要识别出它们的意思。像下面的都要能够识别出来。
```python
  open door
  open the door
  go THROUGH the door
  punch bear
  Punch The Bear in the FACE
```
当然，前提是假设玩这个游戏的是一个正常人(手动滑稽) 。

作者的建议是专门写一个模组，搭配若干类，互相配合，接受用户输入，并将其转化成游戏可以识别的命令。

首先是英文的简单格式：

-  单词由空格隔开
-  句子由单词组成
-  语法控制句子的含义

我们要最先处理用户输入的词汇，并判断出来它们是什么。

### 词汇处理
游戏里创建了下面这些语汇：
```python
  表示方向: north, south, east, west, down, up, left, right, back.
  动词: go, stop, kill, eat.
  修饰词: the, in, of, from, at, it
  名词: door, bear, princess, cabinet.
  数词: 由 0-9 构成的数字
```
#### 断句
有了词汇表，为了分析句子的意思，我们需要找到一个断句的方法，因为我们对于句子的定义是“空格隔开的单词”，所以用split方法就好了。当然，这是英文，处理中文的话还需要第三方库。
```python
stuff = raw_input('> ')
words = stuff.split()
```
#### 语汇元组
先将句子转化成词汇列表，剩下的就是**逐一检查这些词汇，看它们是什么类型，并且对它执行指令**。
使用元组(tuple)保存词汇和其类型，它是一种不能原地修改的序列类型，这样使得用户输入的词汇不会改变。
```python
# (TYPE, WORD)
first_word = ('direction', 'north')
second_word = ('verb', 'go')
sentence = [first_word, second_word] # 元组的列表
```
> 接受用户输入，用split将其分隔成单词列表，然后分析这些单词，识别它们的类型，最后重新组成一个句子。

### 扫描输入
下面是自己要写的扫描器代码：
```python
"""
direction : north, south, east, west, down, up, left, right, back;
verb : go, stop, kill, eat;
stop : the, in, of, from, at, it;
noun : door, bear, princess, cabinet;
number : 0-9 number
"""
def scan(stuff):
	stuff = stuff.lower() # 为识别任意大小写的词汇，将其统一转换为小写
    words = stuff.split()
    direction = ['north', 'south', 'east', 'west', 'down', 'up', 'left', 'right', 'back']
    verb = ['go', 'stop', 'kill', 'eat']
    stop = ['the', 'in', 'of', 'from', 'at', 'it']
    noun = ['door', 'bear', 'princess', 'cabinet']
    result = [ ]
    for word in words:
        if word in direction:
            result.append(('direction', word))
        elif word in verb:
            result.append(('verb', word))
        elif word in stop:
            result.append(('stop', word))
        elif word in noun:
            result.append(('noun', word))
        elif convert_number(word):
            result.append(('number', int(word)))
        else:
            result.append(('error',word))
    return result 
            
def convert_number(num): # 如果没有作者的提醒，这里不一定会想到
    try:
        return int(num)
    except ValueError:
        return None
```
#### 测试代码
```python
from nose.tools import *
from ex48 import lexicon 

def test_upper_lower():  # 更新代码
	assert_equal(lexicon.scan("NORth"), [('direction', 'north')])
	result = lexicon.scan("souTH Kill EaT")
	assert_equal(result, [('direction', 'south'),
						  ('verb', 'kill'),
						  ('verb', 'eat')])

def test_directions(): # 测试表中方位词
    assert_equal(lexicon.scan("north"), [('direction', 'north')])
    result = lexicon.scan("north south east")
    assert_equal(result, [('direction', 'north'),
                          ('direction', 'south'),
                          ('direction', 'east')])
                                      
def test_verbs():   # 测试表中动词
    assert_equal(lexicon.scan("go"), [('verb', 'go')])
    result = lexicon.scan("go kill eat")
    assert_equal(result, [('verb', 'go'),
                          ('verb', 'kill'),
                          ('verb', 'eat')])
    
def test_stop():   # 测试表中停用词
    assert_equal(lexicon.scan("the"), [('stop', 'the')])
    result  = lexicon.scan("the in of")
    assert_equal(result, [('stop', 'the'),
                          ('stop', 'in'),
                          ('stop', 'of')])
                                      
def test_nouns():  # 测试表中名词
    assert_equal(lexicon.scan("bear", [('noun', 'bear')])
    result = lexicon.scan("bear princess")
    assert_equal(result, [('noun', 'bear'),
                          ('noun', 'princess')])

def test_number(): # 测试数字
    assert_equal(lexicon.scan("1234"), [('number', 1234)])
    result = lexicon.scan("3 91234")
    assert_equal(result, [('number', 3),
                          ('number', 91234)])
                                      
def test_errors(): # 测试错误
    assert_equal(lexicon.scan("ASDFADFASDF"), [('error', 'asdfadfasdf')])
    result = lexicon.scan("bear IAS princess")
    assert_equal(result, [('noun', 'bear'),
                          ('error', 'ias'),
                          ('noun', 'princess')])   
```
#### 测试结果
![](http://img.blog.csdn.net/20180419224638973?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 本节知识：异常与数字
学会使用`异常(exceptions)`。

异常指的是你运行某个函数时得到的错误。你的函数在碰到错误时，就会`提出(raise)`一个异常，如果不使用异常处理的话，程序就会终止。所以，我们要学会去处理(handle)异常。

Python使用`try-except`来做异常处理，我们会把要试着运行的代码放到`try`的区段里，再将出错后要运行的代码放到`except`区段里。

在扫描数字的时候，用`int()`来将字符串转换为数字，如果不是数字的话，该函数就会抛出`ValueError`异常。我们要做的就是中间出了错，就抓住这个错误，然后返回`None`，这样`if语句`判断时就是False了。