# 复习Python的第四十二天
## 习题47: 自动化测试
确认游戏的功能是否正常而输入命令，这个
过程是很枯燥无味的。如果能写一小段代码用来测试你的代码岂不是更好？

然后如果我们对程序做了任何修改，或者添加了什么新东西，只要“跑一下你的测试”就行了。

这些自动测试能确认程序依然能正确运行，也不会抓到所有的 bug，但可以让你无需重复输入命令运行你的代码，从而为你节约很多时间。

> 从现在开始，你需要为自己写的所有代码写自动化测试，而这将让你成为一个更好的程序员。
> 这个写代码测试你写的其他代码的过程将强迫你清楚的理解你之前写的代码。这会让你更清晰地了解你写的代码实现的功能及其原理，而且让你对细节的注意更上一个台阶。
 
让无聊繁琐的东西自动化！这就是我学习编程的目的之一。

### 项目代码
从你的项目骨架创建一个叫做ex47的项目，确认该改名称的地方都有改过，
尤其是`tests/ex47_tests.py`这处不要写错，还有`ex47_tests.py`中应该改为`import ex47`，然后运行nosetest确认一下没有错误信息。

```python
#  ex47/game.py
class Room(object):
    def __init__(self, name, description): 
    # 初始化类属性，Room has-a name and description
        self.name = name
        self.description = description
        self.paths = { }  # 可以在实例化之后从外部添加
        
    def go(self, direction):
        return self.paths.get(direction, None)
        
    def add_paths(self, paths):
        self.paths.update(paths)
```
### 测试代码
```python
from nose.tools import *
from ex47.game import Room 
# import在ex47.game创建的Room类

def test_room():
	gold = Room("GoldRoom",
				"""This room has gold in it you can grab.
				There's a
				door to the north.""") # 传入name和description
	assert_equal(gold.name, "GoldRoom") # 测试是否相等
	assert_equal(gold.paths, {})        # 测试

def test_room_paths():
	center = Room("Center", "Test room in the center.")
	north = Room("North", "Test room in the north.")
	south = Room("South", "Test room in the south.")

	center.add_paths({'north': north, 'south': south})
	assert_equal(center.go('north'), north) # 测试方位
	assert_equal(center.go('south'), south) # 测试方位

def test_map():
	start = Room("Start", "You can go west and down a hole.")
	west = Room("Trees", "There are trees here, you can go
east.")
	down = Room("Dungeon", "It's dark down here, you can go
up.")
	
	start.add_paths({'west': west, 'down': down})
	west.add_paths({'east': start})
	down.add_paths({'up': start})

	assert_equal(start.go('west'), west)
	assert_equal(start.go('west').go('east'), start)  # 这里的双重go？？go(start,'west')，再然后go(west,'east')，返回start
	assert_equal(start.go('down').go('up'), start)
```
test_ 开头的测试函数是所谓的“测试用例(test case)”，每一个测试用例里面都有一小段代码，它们会创建一个或者一些房间，然后去确认房间的功能和你期望的是否一样。
**它测试了基本的房间功能，然后测试了路径，最后测试了整个地图。**

>**最重要的函数是assert_equal**，它保证了你设置的变量，以及你在Room里设置的路径和你的期望相符。
>如果你得到错误的结果的话，nosetests将会打印出一个错误信息，这样你就可以找到出错的地方并且修正过来。

测试结果：

![这里写图片描述](http://img.blog.csdn.net/20180419102659464?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 测试指南
1. 测试脚本要放到`tests/`目录下，并且命名为`NAME_tests.py`，否则 nosetests 就不会执行你的测试脚本了。这样做还有一个好处就是防止测试代码和别的代码互相混淆掉。
2. **为你的每一个模组写一个测试**。
3. 测试用例（函数）保持简短，但如果看上去不怎么整洁也没关系，测试用例一般都有点乱。
4. 就算测试用例有些乱，也要试着让他们保持整洁，把里边重复的代码删掉。创建一些辅助函数来避免重复的代码。
当你下次在改完代码需要改测试的时候，你会感谢我这一条建议的。重复的代码会让修改测试变得很难操作。
5. 最后一条是别太把测试当做一回事。有时候，更好的方法是把代码和测试全部删掉，然后重新设计代码。

### 加分习题
```python
>>> dir(nose)
['DeprecatedTest', 'SkipTest', '__all__', 
'__author__', '__builtins__', '__cached__', 
'__doc__', '__file__', '__loader__', 
'__name__', '__package__', '__path__', 
'__spec__', '__version__', '__versioninfo__', 

'case', 'collector', 'config', 'core', 
'exc', 'failure', 'importer', 'loader', 
'main', 'plugins', 'proxy', 'pyversion', 
'result', 'run', 'run_exit', 'runmodule',
 'selector', 'suite', 'tools', 'util', 'with_setup']

>>> dir(nose.tools)
['TimeExpired', '__all__', '__builtins__', 
'__cached__', '__doc__', '__file__', '__loader__', 
'__name__', '__package__', '__path__', '__spec__', 

'assert_almost_equal', 'assert_almost_equals', 
'assert_count_equal', 'assert_dict_contains_subset', 
'assert_dict_equal', 'assert_equal', 'assert_equals', 
'assert_false', 'assert_greater', 'assert_greater_equal', 
'assert_in', 'assert_is', 'assert_is_instance', 'assert_is_none', 
'assert_is_not', 'assert_is_not_none', 'assert_less', 
'assert_less_equal', 'assert_list_equal', 'assert_logs', 
'assert_multi_line_equal', 'assert_not_almost_equal', 
'assert_not_almost_equals', 'assert_not_equal', 
'assert_not_equals', 'assert_not_in', 'assert_not_is_instance', 
'assert_not_regex', 'assert_not_regexp_matches', 'assert_raises', 
'assert_raises_regex', 'assert_raises_regexp', 'assert_regex', 
'assert_regexp_matches', 'assert_sequence_equal', 
'assert_set_equal', 'assert_true', 'assert_tuple_equal', 
'assert_warns', 'assert_warns_regex', 

'eq_', 'istest', 'make_decorator', 
'nontrivial', 'nontrivial_all', 'nottest', 
'ok_', 'raises', 'set_trace', 'timed', 
'trivial', 'trivial_all', 'with_setup'] 
```
可以看到，nose中的工具是很多的。

关于Python的`doc test`，可以看一下这篇[文章](https://my.oschina.net/lionets/blog/268542)，还有这篇[文章](http://www.jb51.net/article/64049.htm)。