# 复习Python的第十二天
[TOC]

## 习题13：参数、解包、变量

这是另一种将变量传递给脚本的方法(所谓脚本，就是你写的.py 程序)。**我以前也没学过**。

![](http://img2.imgtn.bdimg.com/it/u=3670294539,2758241811&fm=27&gp=0.jpg)

作者给出了一点提示：
> 如果要运行 ex13.py，只要在命令行运行python ex13.py 就可以了。这句命令中的ex13.py 部分就是所谓的“参数 (argument)”。
> 我们现在要做的就是写一个可以接受参数的脚本。 

emmmm……还是摸不着头脑。
> 代码部分
```python
from sys import argv  # 从sys引入argv模块，即‘参数变量’
# 下面是一个链式赋值语句 
script, first, second, third = argv # 解包，‘=’将我们在命令行中传递给python的参数分别赋值给左边的变量
# 打印 
print "The script is called:", script 
print "Your first variable is:", first 
print "Your second variable is:", second 
print "Your third variable is:", third 
```
我运行的结果：

![](http://img.blog.csdn.net/20180327185141866?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

作者的解答：

- `argv`就是所谓的“参数变量(argument variable)”，是一个非常标准的编程术语。在其他的编程语言里你也可以看到它。这个变量包含了你通过命令行传递给Python的参数。

- `第3行`将argv“解包(unpack)”，与其将所有参数放到同一个变量下面，我们将每个参数赋予一个变量名：script, first, second, 以及third。这也许看上去有些奇怪，不过”解包”可能是最好的描述方式了。它的含义很简单:“把argv中的东西解包，将所有的参数依次赋予给左边的变量。

- 我们此后将引入的（import）功能叫做模组（module），或者是库（libraries）。
- 运行方法注意：

> 必须输入三个参数！
如：
` python ex13.py first 2nd 3rd `

>我们也可以将“first”、“2nd”、“3rd”替换成任意三样东西。
如：
`python ex13.py stuff I like 
python ex13.py anything 6 7 `

- 另外，有一个错误如下：
```python
python ex13.py first 2nd 
Traceback (most recent call last): 
  File "ex/ex13.py", line 3, in <module> 
    script, first, second, third = argv 
ValueError: need more than 3 values to unpack 
```
> 当你运行脚本时提供的参数的个数不对的时候，你就会看到上述错误信息 (这次我只用了 first 2nd 两个参数)。“need more than 3 values to unpack”这个错误信息告诉你参数数量不足。 

## 加分习题 
1. 给你的脚本三个以下的参数。看看会得到什么错误信息。试着解释一下。

![](http://img.blog.csdn.net/20180327185816009?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

错误是“ValueError”，它提示我需要**更多的命令行参数**来解包。

补充知识如下：
Python中，可以通过sys.argv获得命令行中所输入的全部的内容，获得的值是列表类型的变量。

列表中的值，分别是命令行中**通过空格分隔出来**的各个值。既然是列表变量，自然不能只有一个或者没有元素。

至于sys.avgv，先记住它，是另外的用法，和本节课代码无关。
sys和argv中间的点是一个运算符，**取sys库中的一个功能**。

2.再写两个脚本，其中一个接受更少的参数，另一个接受更多的参数，在参数解包时给它们取一些有意义的变量。
```python2
# ex13-1.py 
from sys import argv

life_is_short = raw_input()

script,life_is_short,use_python = argv

print "The script is called:",script

print "Your first variable is:",life_is_short

print "Your second variable is:",use_python

```

![](http://img.blog.csdn.net/20180327190737171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

人生苦短，我用Python！

```python2
# ex13-2.py
from sys import argv

script,I,love,the,peace = argv

print "The script is called:",script

print "Your first variable is:",I

print "Your second variable is:",love

print "Your third variable is:",the

print "Your forth variable is:",peace
```

![](http://img.blog.csdn.net/20180327190715872?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 补充知识：sys模组
sys是python标准库的一个模组，模组包含很多预先定义好的功能和变量。

本节课中讲的sys库中还有其他功能。标准库中也有其他的模组。

### sys模块具体模块介绍

 - sys.argv：命令行参数，**List类型**，第一个元素是程序本身路径名

 
 -  sys.modules.keys()：返回**所有已经导入的模块列表**
 
 - sys.exc_info()：获取**当前正在处理的异常类**，exc_type、exc_value、exc_traceback是当前处理的异常详细信息

 -  sys.exit(n)：**退出程序**，正常退出时exit(0)   
 
 -  sys.hexversion：获取Python**解释程序的版本值**，16进制格式如：0x020403F0   

 -  sys.version：获取Python**解释程序的版本信息** 

 -  sys.maxint：最大的Int值   
 
 -   sys.maxunicode：最大的Unicode值 
 
 -  sys.modules：返回**系统导入的模块字段**，key是模块名，value是模块  
 
 -   sys.path：返回**模块的搜索路径**，初始化时使用PYTHONPATH环境变量的值 

 -   sys.platform：**返回操作系统平台名称**   
 
 - sys.stdout：标准输出 sys.stdin；标准输入 sys.stderr
   
 - sys.exc_clear()：用来清除**当前线程所出现的当前的或最近的错误信息**   

 -  sys.exec_prefix：返回平台独立的python文件安装的位置   

 -  sys.byteorder：本地字节规则的指示器，big-endian平台的值是'big'，little-endian平台的值是'little'

 -  sys.copyright：记录python**版权相关**的东西 

 -  sys.api_version：解释器的C的API版本   

 -  sys.version_info 

 -  sys.displayhook(value)    

 -   sys.getdefaultencoding()：返回**当前所用的默认的字符编码**格式   

 -  sys.getfilesystemencoding()：返回将Unicode文件名转换成系统文件名的编码的名字   

 -  sys.setdefaultencoding(name)：说明：用来**设置当前默认的字符编码**，如果name和任何一个可用的编码都不匹配，抛出LookupError，这个函数只会被site模块的sitecustomize使用，一旦别site模块使用了，它会从sys模块移除。

 -  sys.builtin_module_names：Python解释器导入的模块列表 
 
 - sys.executable：Python解释程序路径

 -  sys.getwindowsversion()：**获取Windows的版本**   

 -  sys.stdin.readline()：从标准输入读一行，若sys.stdout.write("a")，则屏幕输出a    

 -  sys.startswith()：是**用来判断一个对象是以什么开头**的，比如在python命令行输入'abc'.startswith('ab')就会返回True。
 
 ![](http://img0.imgtn.bdimg.com/it/u=2913516883,3502762911&fm=27&gp=0.jpg)