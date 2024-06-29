# 复习Python的第四十九天
## 习题 49:  创建句子
[TOC]

从上一张的词汇扫描器中，我们得到的列表：
```python
>>> from ex48 import lexicon

>>> print lexicon.scan("go north")
[('verb', 'go'), ('direction', 'north')]

>>> print lexicon.scan("kill the princess")
[('verb', 'kill'), ('stop', 'the'), ('noun', 'princess')]

>>> print lexicon.scan("eat the bear")
[('verb', 'eat'), ('stop', 'the'), ('noun', 'bear')]

>>> print lexicon.scan("open the door and smack the bear in
the nose")
[('error', 'open'), ('stop', 'the'), ('noun', 'door'),
('error', 'and'),
('error', 'smack'), ('stop', 'the'), ('noun', 'bear'),
('stop', 'in'),
('stop', 'the'), ('error', 'nose')]
>>>
```
你的词汇扫描器应该能做到上面的样子，更要能通过测试才行。现在要将词汇列表转换为游戏能用的东西，一个Sentence类，它必须包含句子的基本结构。

句子基本结构：`主语(Subject)+谓语(动词Verb)+宾语(Object)`，如果我们将元组列表转换为这种具有基本结构的Sentence类就可以了。如何做，作者给了我们一些提示。

### 窥视(Peek)、匹配(Match) 、跳过(skip)
为了达到上面的结果，我们必须打造自己的函数工具箱。它能够做到：

- **窥视**(peek)下一个元组列表成员；
- 循环访问元组的列表；
- 对不同种类的词汇元组采用不同的方法(match)，即**匹配**；
- **跳过**(skip)我们不在乎的内容，如形容词和冠词等，也就是`stop`、`error`和`number`类型。

> 窥视
```python
# 传入词汇元组的列表，非空，则索引第一组单词元组，返回该元组类型值；空，则返回None
def peek(word_list):
    if word_list:
        word = word_list[0]
        return word[0]
    else:
        return None
```
> 匹配
```python
# 传入单词元组列表和期待值，非空，则删去单词元组列表第一组并返回其值，如为期待类型，则返回该词组；空，则返回None
def match(word_list, expecting): 
	if word_list:
		word = word_list.pop(0)
		if word[0] == expecting:
			return word
		else:
			return None
	else:
		return None
```
```python
# 传入单词元组列表和类型，窥视非空的返回类型值如等于跳过的类型，则匹配一下
def skip(word_list, word_type):
	while peek(word_list) == word_type:
	    match(word_list, word_type)
```
有了这几个提示，其实我们每个人都能写得出函数工具箱来，可以自己试一试，然后和作者比较一下。

### 关于异常(Exception)
这节的代码演示了如何`raise`定义的`ParserError`。注意ParserError是一个定义为`Exception`类型的`class`。我们的测试代码应该也要测试到这些异常。

### 本节代码
```python
class ParserError(Exception): # 
    pass
    
class Sentence(object): # 定义一个Sentence类，初识化属性有subject、verb和object
    def __init__(self, subject, verb, object):
        # remember we take ('noun', 'princess') tuples and convert them
        self.subject = subject[1] # 取词汇元组第二个，即实际词汇
        self.verb = verb[1]
        self.object = object[1] 
        
def peek(word_list):
    if word_list:
        word = word_list[0] # 取词汇元组并返回类型值
        return word[0]
    else:
        return None
    
def match(word_list, expecting): 
     if word_list:
        word = word_list.pop(0) # 删除相应词汇元组并返回该元组
            
        if word[0] == expecting: # 词汇类型检测
            return word
        else:
            return None
     else:
        return None
    
def skip(word_list, word_type):
     while peek(word_list) == word_type: # 词汇类型检测
        match(word_list, word_type) 
            
def parse_verb(word_list): # 处理动词
     skip(word_list, 'stop') # 停用词跳过
        
     if peek(word_list) == 'verb': # 动词则返回匹配的该词汇元组
        return match(word_list, 'verb')
     else:  # 否则抛出ParseError
        raise ParserError("Expected a verb next.") 
    
def parse_object(word_list): # 处理宾语
	 skip(word_list, 'stop') # 跳过停用词
	 next = peek(word_list) # 窥视下一个词汇元组
		
	 if next == 'noun': # 如为名词，返回匹配的词汇元组
		return match(word_list, 'noun')
	 if next == 'direction': # 方位词同上
		return match(word_list, 'direction')
	 else:
		raise ParserError("Expected a noun or direction next.")    
    
def parse_subject(word_list, subj): # 处理主语
     verb = parse_verb(word_list)
     obj = parse_object(word_list)
        
     return Sentence(subj, verb, obj)
        
def parse_sentence(word_list):
     skip(word_list, 'stop')
        
     start = peek(word_list)
        
     if start == "noun":
        subj = match(word_list, 'noun')
        return parse_subject(word_list, subj)   
     elif start == 'verb':
        # assume the subject is the player then
        return parse_subject(word_list, ('noun', 'player'))
     else:
        raise ParserError("Must start with subject, object, or verb not: %s" % start)
```

###  你应该测试的东西
为《习题 49》写一个完整的测试方案，确认代码中所有的东西都能正常工作，其中异常的测试——输入一个错误的句子它会抛出一个异常来。

使用`assert_raises`这个函数来检查异常，在`nose`的文档里查看相关的内容，学着使用它写针对“执行失败”的测试，这也是测试很重要的一个方面。而且学会了如何为别人的程序写测试代码也是一个非常有用的技能。
```	python
from nose.tools import *
from ex49 import ex49

def test_peek():
    assert_equal(ex49.peek([('direction', 'east')]), 'direction')
    assert_equal(ex49.peek([ ]), None)
    result = ex49.peek([('noun', 'east'),
                                   ('verb', 'go')])
    assert_equal(result, 'noun')
    
def test_match():
    assert_equal(ex49.match([('verb', 'kill')], 'verb'), ('verb', 'kill'))
    assert_equal(ex49.match([('noun', 'bear')], 'verb'), None)
    assert_equal(ex49.match([ ], 'noun'), None)

def test_skip():
    assert_equal(ex49.skip([('stop','the')], 'stop'), None)
    result = ex49.skip([('stop', 'in'), ('noun','bear')], 'stop')
    assert_equal(result, None)
    
def test_parse_verb():
    assert_raises(ex49.ParserError, ex49.parse_verb, [('stop','the'), ('stop','in')])
    assert_equal(ex49.parse_verb([('stop','the'), ('verb', 'go')]), ('verb', 'go'))
    assert_equal(ex49.parse_verb([('verb', 'eat'),('verb', 'kill')]), ('verb', 'eat'))
    
    assert_raises(ex49.ParserError, ex49.parse_verb, [('noun', 'door'), ('verb', 'go')])
    assert_raises(ex49.ParserError, ex49.parse_verb, [('direction', 'east'), ('verb', 'kill')])

def test_parse_object():
    assert_raises(ex49.ParserError, ex49.parse_object, [('stop','the'), ('stop','in')])
    assert_raises(ex49.ParserError, ex49.parse_object, [('stop','the'), ('verb', 'go')])
    
    assert_equal(ex49.parse_object([('noun', 'door'), ('verb', 'go')]), ('noun', 'door'))
    
    result = ex49.parse_object([('direction', 'north'), ('verb', 'eat')])
    assert_equal(result, ('direction', 'north'))
    
def test_parse_subject():
    assert_equal(ex49.parse_subject([('verb', 'kill'), ('noun', 'bear')], ('noun', 'doctor')), ex49.Sentence(('noun', 'doctor'), ('verb', 'kill'), ('noun', 'bear')))
    
    assert_equal(ex49.parse_subject([('stop','in'), ('verb', 'go'), ('stop','the'), ('direction', 'west')], ('noun', 'teacher')), ex49.Sentence(('noun', 'teacher'), ('verb', 'go'), ('direction', 'west')))
    
def test_parse_sentence():
    assert_raises(ex49.ParserError, ex49.parse_sentence, [('stop', 'at'), ('noun', 'door')])
    
    assert_equal(ex49.parse_sentence([('stop', 'in'), ('stop', 'at'), ('verb', 'kill'), ('noun', 'bear')]), ex49.parse_subject([('stop', 'in'), ('stop', 'at'), ('verb', 'kill'), ('noun', 'bear')], ('noun', 'player')))
    
    assert_raises(ex49.ParserError, ex49.parse_sentence, [('stop', 'in'), ('stop', 'at')])
    
    assert_equal(ex49.parse_sentence([('noun', 'door'), ('verb', 'open')]), ex49.Sentence(('noun', 'door'), ('verb', 'open'), None))
    
    assert_equal(ex49.parse_sentence([('verb', 'kill'), ('stop', 'the'), ('noun', 'bear')]), ex49.Sentence(('noun', 'player'), ('verb', 'kill'), ('noun', 'bear')))
    
```
很遗憾的是，我的代码无法检测类是否相等，会返回断言错误。一时半会我还想不到解决办法，以后再说吧。