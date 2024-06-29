# 复习Python的第二十三天
## 习题 24: 更多练习 

本节没有什么新的东西，是一个比较长的练习
> The Codes
```python
print "Let's practice everything." 
print 'You\'d need to know \'bout escapes with \\ that do \n newlines and \t tabs.' 
 
poem = """ 
\tThe lovely world 
with logic so firmly planted 
cannot discern \n the needs of love 
nor comprehend passion from intuition 
and requires an explanation 
\n\t\twhere there is none. 
""" 
 
print "--------------" 
print poem 
print "--------------" 
 
 
five = 10 - 2 + 3 - 6 
print "This should be five: %s" % five 
def secret_formula(started): 
    jelly_beans = started * 500 
    jars = jelly_beans / 1000 
    crates = jars / 100 
    return jelly_beans, jars, crates 
 
 
start_point = 10000 
# 调用secret_formula函数，返回三个值，分别赋予beans，jars，crates
beans, jars, crates = secret_formula(start_point) 
 
print "With a starting point of: %d" % start_point 
print "We'd have %d beans, %d jars, and %d crates." % (beans, jars, crates) 
 
start_point = start_point / 10 
 
print "We can also do that this way:" 
# 下面的函数返回值分别对三个格式化字符
print "We'd have %d beans, %d jars, and %d crates." % secret_formula(start_point) 
```

## 习题25：更多更多的练习 
这节比较有趣。
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
```
本节知识：

- import的时候不用加“.py”的后缀，此时作为模块的程序文件被编译成为字节码，后缀变为“.pyc”；
- 使用ex25调用函数时用. (dot, period)调用ex25模块中的函数，如：` ex25.break_words`；
- 列表是一组元素的有序集合，属于序列，用[ ]引起；
- sorted()是内置的排序函数，传入可迭代对象，reverse可以选择是否翻转序列；
```python
Help on built-in function sorted in module builtins:

sorted(iterable, /, *, key=None, reverse=False)
    Return a new list containing all items from the iterable in ascending order.

    A custom key function can be supplied to customize the sort order, and the
    reverse flag can be set to request the result in descending order.
```

- 错写成wrods的提示信息`NameError`很常见，这里不做叙述；
- pop()函数意为删除指定位置的元素并返回该元素；
```python2
Help on built-in function pop:

pop(...) method of builtins.list instance
    L.pop([index]) -> item -- remove and return item at index (default last).
    Raises IndexError if list is empty or index is out of range.
```
- 帮助文档是定义函数时放在 """ 之间的东西，一般跟在函数定义的下一行，它们也被称作 documentation comments （文档注解），我们使用help()时看到的文档就来自这里；
- from ex25 import * 会挤占命名空间，也会让人不清楚哪个函数出自哪个模块，会使程序逻辑不清晰，不建议使用。