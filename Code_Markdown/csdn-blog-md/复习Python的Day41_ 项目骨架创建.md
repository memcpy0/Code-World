# 复习Python的第四十一天

## 习题 46: 一个项目骨架
本节我们将学习如何创建一个项目骨架，这是做项目必经之路。尽管作者的示范不一定是最合适最新最好的，但也能成为一个入门的帮助。

这个骨架目录**具备让项目跑起来的所有基本内容**。它里边会包含

- 你的项目文件布局
- 自动化测试代码
- 模组
- 安装脚本

### 骨架内容
```DOS
~ $ mkdir projects  # 原来这里有个-p，没什么用，就删了 
~ $ cd projects/
~/projects $ mkdir skeleton # 前三行可以使用mkdir
~/projects $ cd skeleton
~/projects/skeleton $ mkdir bin NAME tests docs 
```
注意，里面的**~是省略的路径名，$是提示符**，我们可以自由选择把这个骨架创建在哪个盘里的某个路径。

另外，书中的命令都是在OS X下的命令，有些在Windows中和DOS命令含义一样，但有的就不行。

关于Windows下的DOS命令的使用有些值得注意的地方，我会[总结一下的](https://blog.csdn.net/myRealization/article/details/79833508)。

```python
~/projects/skeleton $ touch NAME/__init__.py 
~/projects/skeleton $ touch tests/__init__.py
```
请注意，这两行命令是OS X环境下的。不幸的是Windows中的DOS没有同样的命令。我们其实在一开始就应该使用git。在git中可以使用这些命令，即使在Windows中。

当然，git中的命令使用也有一些坑。

![](http://img.blog.csdn.net/20180418235805920?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```python
# setup.py
# try-except语句，异常处理
try: 
    from setuptools import setup
except ImportError: 
# 当遇到ImportError时，不终止程序，而执行except下的语句
    from distutils.core import setup

"""
字典，包含项目描述，作者，URL，下载地址，作者邮件，版本，安装要求，包，脚本，项目名等
"""
config = {
          'description' : 'My Project',
          'author' : 'My name',
          'url' : 'URL to get it at.',
          'download_url' : 'Where to download it.',
          'author_email' : 'My email.',
          'version' : '0.1',
          'install_requires' : ['nose'],
          'packages' : ['NAME'],
          'scripts' : [ ],
          'name' : 'projectname'
          }
"""
解包参数，**会以键值对的形式解包一个字典使其成为一个独立的关键字参数
"""
setup(**config)  
```
```python
# test/NAME_tests.py
from nose.tools import *
import NAME 

def setup():
    print "SETUP!"
    
def teardown():
    print "TEAR DOWN!"
    
def test_basic():
    print "I RAN!"
```
setup.py和NAME_tests.py应该不会出现什么问题，如果有些不懂的地方，看一下注释。要注意的是，setup.py存储位置无所谓，但是NAME_tests.py必须保存在tests目录下面。

### Python软件包的安装
Windows PowerShell里面非常简单，遵循如下命令行形式即可。
```
pip install 软件包名称
```
shell会自动下载软件包之后自动安装。
要下载的包有以下这些：`distribute`、`nose`、`virtualenv`。

### 测试你的配置
在git下进入虚拟环境bbpy2，是Python2.7的环境配置，运行测试，结果如下：

![](http://img.blog.csdn.net/20180419003436815?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 使用这个骨架
以后每次我们要新建一个项目时，只要做下面的事情就可以了：
1. **拷贝这份骨架(skeleton)目录，把skeleton改成你新项目的名字**。照样可以放在projects文件夹中，因为这里是用来存放我们的各个项目的地方，而skeleton则是各个项目的基础目录。
2. **再将NAME模组更名为你需要的名字**，它可以是你项目的名字，当然别的名字也行。
3. **编辑setup.py让它包含你新项目的相关信息**。
4. **重命名`tests/NAME_tests.py`**，让它的名字匹配到你模组的名字。
5. **使用`nosetests`检查有无错误**。
6. 开始写代码吧。

### 加分习题