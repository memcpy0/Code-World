@[toc]



---
# 0. Wireshark官方网站及资料
[Wireshark官网页面](https://www.wireshark.org/index.html#download)如下所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/cb72867a0c9e4e8c8ca73aea3c113bcc.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

[Wireshark文档页面](https://www.wireshark.org/docs/)如下所示，建议至少把 `User's Guide` 过一遍：
![在这里插入图片描述](https://img-blog.csdnimg.cn/39ee453119e14d24a6ce4d6d7c7a0623.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
[Wireshark的Learn页面](https://www.wireshark.org/index.html#learnWS)还有许多学习课程：
![在这里插入图片描述](https://img-blog.csdnimg.cn/1b978cddcc2648059453b7e90ad1f6fe.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

---
# 1. 下载安装Wireshark
在[https://www.wireshark.org/index.html#download](https://www.wireshark.org/index.html#download)下载页面中，选择合适的版本，下载Wireshark：
![在这里插入图片描述](https://img-blog.csdnimg.cn/1ffa207d876b47689d9ebd925be125b1.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
启动安装软件，一路 `Next` 即可：
![在这里插入图片描述](https://img-blog.csdnimg.cn/3ded7f8aa01048f7a23a28b0219dbdec.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)![在这里插入图片描述](https://img-blog.csdnimg.cn/b37f4bb2966e43d2b558a6a77922ca2f.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/17ec9e267ffa49cd8c222dcdf5edf772.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/b7d020af39de4dc0bb842ae830601ca3.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/38d6bf56b0e240dfb5a0af0ceea72641.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
需要安装 `Npcap, WinPcap, USBcap` ，勾选即可：
![在这里插入图片描述](https://img-blog.csdnimg.cn/354f7bb86e574419b7cb89033766282c.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/c84ad9f3b138400da456dfe928eca864.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
最后开始安装过程：
![在这里插入图片描述](https://img-blog.csdnimg.cn/004ad3336cc843d793f3a4eb5c8e5a9e.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
中途蹦出来 `Npcap` 的安装界面：
![在这里插入图片描述](https://img-blog.csdnimg.cn/aa76455bc5a94b96b8696def6107f89d.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/3634dd19899b4ed3b4df5361f3668a9a.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/e922c65b55134bae8d0796fbcd49459f.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
然后是 `USBPcap` 的安装界面：
![在这里插入图片描述](https://img-blog.csdnimg.cn/d6789d739ed5440a914c92e7320e55bc.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_19,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/7bc760013f9d4bb589f3a6c6d3a399d0.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_19,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/a4bb9fcab5f84675896296c6aa023d20.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_19,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/ddea4378b905405bb12b4d008432de89.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_19,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/1fa57e941a654898976c595f4430b3e4.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_19,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/a4d10ef3ce5141cab6939bfbb28e20f2.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
最后，要重启电脑以完成安装：
![在这里插入图片描述](https://img-blog.csdnimg.cn/37abfde0bfeb471d82f5c1fb11b978d1.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)



---
# 2. Wireshark的完整工作流程
现在本文的重点来了，下面简单介绍一下Wireshark的使用过程，通常这一过程分为这么几个步骤：
- 选择合适的网卡；
- 开始捕获数据包；
- 过滤掉无用的数据包；
- 将捕获到的数据包保存为文件。

以浏览器访问 `www.wireshark.org` 网站的过程为一次观察目标，演示使用Wireshark的完整工作流程，不会涉及到复杂的操作。

## 2.1 选择合适的网卡
启动Wireshark后，首先面对的界面如下所示，红框标记出了我们要考虑的第一个相当重要的问题：**这次操作应该使用哪个网卡**？
![在这里插入图片描述](https://img-blog.csdnimg.cn/2a5bac78a31846d5a9c1218099dd867c.png)
我们的个人计算机通常有两个以上的网卡，比如无线和有线两块网卡。如果计算机安装了虚拟机软件，则又会多出一些虚拟网卡。特殊用途的计算机，如同时连接到两个网络的服务器也会安装多个网卡。**我们如何才能选择合适的网卡，来捕获数据包呢**？

启动界面上只是列出了**当前PC上所有网卡设备的名称**和**流经该网卡的数据包数量的曲线图**（没有数据包经过时则是一条直线），没有更多详细信息，如MAC地址、IP地址等。**我们应该选择的是，有数据包经过的活动网卡**。比如下图的WLAN：
![在这里插入图片描述](https://img-blog.csdnimg.cn/8b6319943d45451484f4687fb57acff2.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
如果希望查看网卡的IP地址等信息，在工具栏上点击「捕获选项」按钮，打开Wireshark捕获窗口，可以看到IP地址，比如WLAN使用的地址是 `192.168.31.250` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/625e0f5be1244e7083601678e4568c62.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
在这一操作窗口中，选中要使用的网卡，单击下面的「开始/OK」按钮，就可以捕获数据包了。

## 2.2 开始捕获数据包
此刻开始，Wireshark将捕获所有的**流经选定网卡设备**的数据包，包括**从Wireshark所在主机发出的**和**发往Wireshark所在主机的**，并将这些数据包都展示在Wireshark工作界面中：
![在这里插入图片描述](https://img-blog.csdnimg.cn/ac3b7478750747648264d84a2b0c7ca4.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
**不要关掉Wireshark**，启动浏览器如Firefox，在地址栏输入目标地址 `www.wireshark.org` ，


---

  




