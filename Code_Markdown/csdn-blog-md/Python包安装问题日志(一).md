##  目录
[TOC]

我在安装Python工具包时总是遇到各种各样的问题，想必大家也都一样吧。有时往往会花很长时间在这上面。这对于新手来说太不友好了，什么热情都消磨殆尽。因此，我想**分享**一些我遇到的问题和我的解决方案。本文将**长期持续不定时**更新。

---
##一、兼顾2.x和3.x的包安装

Python的两个版本有时实在让人伤脑筋，虽然我坚定地站在python3的那一边，但有时也不得不承认Python2仍有不可替代之处。

比如说最近，我为了更近一步地学习机器学习和处理大规模数据，开始使用[Graphlab Create](https://turi.com/download/install-graphlab-create.html)这个工具包。

但是它是基于Python2.x的版本的。

> The GraphLab Create Launcher will download and install Anaconda with Python 2.7. Once complete, GraphLab Create will be installed into this environment.

然而我一直使用的是Python3.5和Anaconda3的版本，总不能让我再装一个Anaconda2吧。

该怎么做呢？

这个时候当然是用虚拟环境安装了，可以使用Anaconda3建立虚拟独立的python2.7环境。

做法如下：

 1. 打开Anaconda Prompt，输入如下命令，创建一个虚拟的环境名为gcpy2，并安装2.7版本的python。
 
 **` conda create -n gcpy2 python = 2.7` **

   ![如图](http://img.blog.csdn.net/20180212130500777?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 
 由于我已经创建，会显示如上图。刚创建的话等它安装完成即可。

 2. 激活虚拟环境。
 
 **` activate gcpy2`**

 3. 激活虚拟环境后的界面 
 
  ![](http://img.blog.csdn.net/20180212131631747?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

	gcpy2就是我之前命名的环境名称，可自行定义。激活后，前面会显示（gcpy2)，表示已经进入该虚拟环境。 至此大功告成，在虚拟环境下安装库均不会影响原来的版本，十分干净和方便。
 4.  使用 activate  gcpy2进入到该虚拟环境中的 python 中 , 使用deactivate 退出。
 
 ![](http://img.blog.csdn.net/20180212132442330?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

        注：可使用 conda info -e 查看虚拟环境中的 python 版本。

安装Graphlab Create并非本文所述重点，略去不提。 

---
## 二、Conda包管理命令总结

很多时候我们想用Conda来对包进行管理，却不记得命令，因此总结如下：

 1. 新安装 Anaconda之后需要conda upgrade –all 更新所有的包。

 2. conda install package_name   进行安装新包

 3. conda remove package_name  删除包 

 4. conda update package_name 更新包 

 5.  conda search search_item 进行模糊查找

 6. conda create -n env_name list of package 创建新的环境

 7. conda list             查看所有包