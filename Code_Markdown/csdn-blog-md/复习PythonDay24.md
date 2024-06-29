# 复习Python的第二十四天
## 习题 26: 恭喜你，现在可以考试了
作者的话：
> 当你成为程序员以后，你将需要经常面对别的程序员的代码，也许还有他们的傲慢态度，他们会经常说自己的代码是完美的。 
这样的程序员是自以为是不在乎别人的蠢货。优秀的科学家会对他们自己的工作持怀疑态度，同样，优秀的程序员也会认为自己的代码总有出错的可能，他们会先假设是自己的代码有问题，然后用排除法清查所有可能是自己有问题的地方， 最后才会得出“这是别人的错误”这样的结论。

而本节的习题就是修改现有的代码。
错误排除清单：

- 随机删掉的一些字符，以及添加的一些错误；有
- 大部分的错误可以通过Python执行告知； **最可行**
- 还有一些算术错误要自己找出来； 已找出
- 再剩下来的就是格式和拼写错误。 有

接下来的是本节的代码：
1. [原代码](http://learnpythonthehardway.org/exercise26.txt)
2. 修改后的代码：
```python2
def break_words(stuff):
    """This function will break up words for us."""
    words = stuff.split(' ')
    return words

def sort_words(words):
    """Sorts the words."""
    return sorted(words)

def print_first_word(words):
    """Prints the first word after popping it off."""
    word = words.pop(0)
    print word

def print_last_word(words):
    """Prints the last word after popping it off."""
    word = words.pop(-1)
    print word
########  
def sort_sentence(sentence):
    """Takes in a full sentence and returns the sorted words."""
    words = break_words(sentence)
    return sort_words(words)

def print_first_and_last(sentence):
    """Prints the first and last words of the sentence."""
    words = break_words(sentence)
    print_first_word(words)
    print_last_word(words)

def print_first_and_last_sorted(sentence):
    """Sorts the words then prints the first and last one."""
    words = sort_sentence(sentence)
    print_first_word(words)
    print_last_word(words)

########
print "Let's practice everything."
print 'You\'d need to know \'bout escapes with \\ that do \n newlines and \t tabs.'

poem = """
\tThe lovely world
with logic so firmly planted
cannot discern \n the needs of love
nor comprehend passion from intuition
and requires an explantion
\n\t\twhere there is none.
"""


print "--------------"
print poem
print "--------------"
##### 
five = 10 - 2 + 3 - 5 # 6
print "This should be five: %d" % five
#####
def secret_formula(started):
    jelly_beans = started * 500
    jars = jelly_beans / 1000
    crates = jars / 100
    return jelly_beans, jars, crates


start_point = 10000
beans, jars, crates = secret_formula(start_point)

print "With a starting point of: %d" % start_point
print "We'd have %d jeans, %d jars, and %d crates." % (beans, jars, crates)
##### 
start_point = start_point / 10

print "We can also do that this way:"
print "We'd have %d beans, %d jars, and %d crabapples." % secret_formula(start_point)

##### 
import ex25
sentence = "All god\tthings come to those who weight."

words = ex25.break_words(sentence)
sorted_words = ex25.sort_words(words)

print_first_word(words)
print_last_word(words)
print_first_word(sorted_words)
print_last_word(sorted_words)
sorted_words = ex25.sort_sentence(sentence)
print sorted_words

print_first_and_last(sentence)

print_first_and_last_sorted(sentence)
```
比起手动找错误，我更想学pdb自动测试啊，还有调试代码的技巧。

本节的测试很简单，细心点就可以了，本来我以为自己看一遍就差不多了，没想到还是漏了几个错误（汗颜）。