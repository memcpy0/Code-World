偶然刷知乎，发现了一个宝藏资源：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210629211608856.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

进去作者说的网址[http://dec41.user.srcf.net/notes/](http://dec41.user.srcf.net/notes/)后，画风如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210629211803241.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

据作者说，有4266页笔记、204847行代码、1610863个单词、10611307个字符：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021062921224944.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

我们可以直接看HTML版的剑桥数学课程笔记，还有PDF版、精简PDF版、PDF限定义版、PDF限定理版、PDF限定理和证明版、TEX代码、例题表等等。如果一个个下载就太慢了，我们直接前往作者的[GitHub](https://github.com/dalcde/cam-notes)。使用Git将整个仓库下载下来：
```bash
git clone https://github.com/dalcde/cam-notes.git 
```
然后使用VS Code打开对应的文件夹：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210629213824464.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
接着开始编译其中的 $\TeX$ 文件。不知道怎么做的，可以看[Windows10下VS Code配置LaTeX语言环境](https://memcpy0.blog.csdn.net/article/details/118217633)这篇文章。由于 `header.tex`（列示了所有需要的 `package` ）存储在根文件夹中，并通过符号链接指向每个子文件夹——只是Windows不允许这种做法，所以我们必须先手动将每个子目录中的 `header.tex` ，替换为实际的根目录下的 `header.tex` 。最后是苦力活，一个个地编译课程 $\TeX$ 文件，就可以得到PDF笔记。一个示例如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021062922433797.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


