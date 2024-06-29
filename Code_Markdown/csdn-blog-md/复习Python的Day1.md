# 复习Python的Day1
![python](http://img.blog.csdn.net/20161210130124726?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQV9sUGhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
作为大一的学生，虽然面临从早到晚的各种课程(水课)，但是我还想好好学一次Python。

说实话，我不是零基础的。以前学过一点C，还在大一上学期学了一点Python的网络课程。但由于我当时“人心不足蛇吞象”，竟然妄想将更高级的一些课程一起学完，例如Python机器学习和全栈开发……

最后可想而知，在社团、课程考试以及一次性学这么多门课的疲惫之下，我……花了半期却没取得什么成果……

但是A站都可以说出“I'll be back!”，我也不会就这么自我放弃，决心扎实的复习几次。

担心枯燥的内容难以坚持下来，我就希望通过写博客的方式督促自己复习和学习，一有空就更新博客。很多大牛通过更新自己的网站或更新博文的方式传播技能知识，令我深受启发。比如[廖雪峰老师](https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2)。而这也是我第一次用这种方式学习一门知识，对于自己来说是一种全新的体验，也可以作为学习笔记，日后也留有足迹。我的学习资料暂定为 《笨办法学 Python(第四版）》。

如果有新手看到此文章，希望只当参考中的参考，我们可以共同学习和进步。如有错误，还请大家指正！

---
## 准备工作

我使用的是Windows64bit系统，选择的文本编辑器是Python自带的IDLE，对新手友好，既有交互式界面，也可以Ctrl + N 创建新文件，F5运行。


![IDLE](http://img.blog.csdn.net/20180310225324363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


## 习题一：第一个程序
"Hello world"，无论什么语言，入门的一定是这个程序。因此很多人挑挑拣拣，学这学那，最终会写几十门语言的"hello world"，但这有什么用呢？以此自警！！！

```python2
print "Hello World!"
print "Hello Again" 
print "I like typing this." 
print 'Yay! Printing.' 
print "I'd much rather you 'not'." 
print 'I "said" do not touch this.'
```
![the result](http://img.blog.csdn.net/20161111122206907)

用F5快捷键保存，命名为ex1.py，然后Run。

或者打开CMD命令台，输入`python ex1.py`来运行这段代码。

当然，现在我更加习惯在**Jupyter Notebook**中运行代码。

> 保存的文件以.py为后缀。

### 摘自《笨办法学python》的几句话:

> 如果你看到类似如下的错误信息：

```python
python ex/ex1.py    
   File "ex/ex1.py", line 3
     print "I like typing this. 
SyntaxError: EOL while scanning string literal
```

> 这些内容你应该学会看懂的，这是很重要的一点，因为你以后还会犯类似的错误。让我们一行一行来看。
>  首先我们在命令行终端输入命令来运行 ex1.py 脚本。 
>  Python 告诉我们 ex1.py 文件的第 3 行有一个错误。 
>   然后这一行的内容被打印了出来。 注意到少了一个 " (双引号，double-quote) 符号了吗？
>    最后，它打印出了一个“语法错误(SyntaxError)”告诉你究竟是什么样的错误。通常这些错误信息都非常难懂，不过你可以把错误信息的内容复制到搜索引擎里，然后你就能看到别人也遇到过这样的错误， 而且你也许能找到如何解决这个问题。


确实，运行代码的时候会出现各种意想不到的问题，而且提示信息还很难看懂。这就是考验我们信息素养的时候了。将错误信息复制到搜索引擎中查找……也不一定能解决问题。但至少能了解到出了什么错误。**慢慢积累**，就能看懂这些错误提示了。


但很多时候，错误不会像上述例子一样简单易懂，我们需要更加合理的查找方法。这里是王树义老师的一篇文章[《Python编程遇问题，文科生怎么办？》](https://www.jianshu.com/p/af6201d2f0c6)。对我们文科生算是很友好的了。(笑)

书中还有一个提示，关于ASCII编码的错误，在脚本的最上面加入：

```python
 # -*- coding: utf-8 -*-
```

对我们而言，这其实是一个使用中文的声明，就可以使用中文注释，且不提示错误。

> 关于编码问题可以参考[廖雪峰老师的Python教程](https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/001431)，的确讲的很详细透彻。

什么时候把这个教程也看一下。

![image](http://img.blog.csdn.net/20180310233757824?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)