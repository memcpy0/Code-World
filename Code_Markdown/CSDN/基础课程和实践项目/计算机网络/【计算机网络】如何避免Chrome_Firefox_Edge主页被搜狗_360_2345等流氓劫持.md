真是蚌埠住了，一打开Firefox，主页就变成了这样：
![WZ7gSg.png](https://img-blog.csdnimg.cn/img_convert/f711466e6f5899fbf86a972625a7b698.png) 
也是奇怪，我又没有修改Firefox的主页设置：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210714124110474.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
然后分别打开Chrome和Edge，主页也被劫持了，除了搜狗还有360这个流氓：
 ![WZTHsA.png](https://img-blog.csdnimg.cn/img_convert/f03a0226dbe322a86d3eab0cb61e0c00.png) ![WZT7Md.png](https://img-blog.csdnimg.cn/img_convert/8ddbf351d5cb634ead4cbd422953ed4c.png) 

好家伙！真是给我整不会了……为了应付这些流氓，总结的方法有两种，一种治标一种治本。

治标的方法是：右键单击Firefox/Chrome/Edge的图标，打开属性窗口，可以发现 **目标(T)** 一栏被篡改了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210714124623493.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

不过如果直接删除掉后面的垃圾网址，可能会弹出 **无法将所做的改动保存到 'Firefox.Ink'** 这个弹窗：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210714124756666.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

应付的方法是，在 **属性窗口->常规** 一栏中，取消 **属性：只读** 的勾选，然后修改 **目标(T)**，删除后面的垃圾网址，最后重新勾选 **属性：只读** 。这样再打开就是自己设置的主页了。


治本的方法是修改注册表。结果又发现了2345的踪迹……彻底删掉 `2345.com, 2345Explorer, 360, 360Chrome, 360se6, 360WallPaperEver, SogouExplorer` 这些垃圾： ![在这里插入图片描述](https://img-blog.csdnimg.cn/20210714130507463.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

