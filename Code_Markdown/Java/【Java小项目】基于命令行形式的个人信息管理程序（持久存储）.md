@[toc]

> 本文的实验和代码基于[【Java小项目】基于命令行形式的个人信息管理（PIMCmd）程序](https://memcpy0.blog.csdn.net/article/details/125141324)。

（必做）PIM with I/O：改写基于命令行形式的个人信息管理（PIMCmd）程序，要求如下：
1) 将第5章实验内容2中的数据存储形式改为文件就行持久存储。
2) 保持命令行交互命令格式不变。
3) 按需要可适当改写原有实现。
![](https://img-blog.csdnimg.cn/a4a77c8978794932839e49c032a9cab9.png)

---
# 一、实验目的
1. 熟悉 File 对象的创建、文件名和目录操作，文件信息的获取与测试；
2. 掌握典型的流式输入输出（文件流、缓存流、数据流、标准输入输出流），典型的流接口的使用。
3. 了解 java.util.Scanner 类以及输入输出的重定向方法。
4. 掌握对象序列化的一般方法，以及可序列化类的定义方法。

# 二、实验过程
1. 实验环境
操作系统：Windows 11
集成开发环境：Eclipse IDE for Enterprise Java and Web Developers (includes
Incubating components) 2022-03 (4.23.0)
2. 题目分析
个人信息管理程序通过命令行形式实现四种功能：List（打印所有的 PIM 对象）、Create（添加一个新的 PIM 对象）、Save（保存整个 PIM 对象列表到文件）、Load（从存储中读取整个 PIM 对象列表）。本次实验要求实现后面两种功能，在以前的个人信息管理程序中添加持久存储功能。之前是将数据保存在内存中的，这样一旦断电，数据就会全部消失，为了能在重启之后依然保留原来的数据，需要将其持久化存储在磁盘中。
根据题目提示，我没有选择利用 PIMEntity 对象中的 fromString 方法解析字符串、利用 toString 方法生成并将字符串保存到文件中，而是创建可序列化的对象、并使用对象输入输出流来实现对象的持久化存储。
3. 代码实现
为了让一个对象能够序列化、串行化、永久化，我们必须为对象实现 Serializable 接口（该接口仅为标记接口，不包含任何方法定义）。因此，让抽象父类 PIMEntity 实现 Serializable 接口，这样其子类 PIMContact、PIMAppointment、PIMTodo 和 PIMNote 也可以被序列化： ![](https://img-blog.csdnimg.cn/09b2a11f3aae4f0aad16c9fc83fc6d74.png)
之前在操作类 PIMManager 中，我定义了一个用来保存 PIMEntity 对象数据的字符串数组 itemList。现在，由于打算使用对象输入输出流来进行对象持久化，这里改为 PIMEntity 对象数组，并在 saveData 方法中将对象数组保存到文件中：
	![](https://img-blog.csdnimg.cn/4fe20287db7e4e2ca8deaf4ab7dcb8aa.png)
	完成了对象集合的保存后，再次启动程序时，需要将对象集合重新反序列化，从文件中提取对象数据。具体的 saveData 方法如下所示：
![](https://img-blog.csdnimg.cn/3f8075f05548442089abe60502bd1a2b.png)
	程序主类 PIMManager 的 main 方法中，只是稍作修改：	![](https://img-blog.csdnimg.cn/c46b859a56d846a4bdd30230d0d69da0.png)
	输入 Save 和 Load 命令时，调用相关方法，完成对象的序列化和反序列化：	![](https://img-blog.csdnimg.cn/3c505ce9893146408a81dee342e5e62f.png)

# 三、实验结果分析
完成程序编写后，在 Eclipse 中点击 Run，输入如下信息：
![](https://img-blog.csdnimg.cn/cffcc83dfd78473280a4031cc3a590d0.png)

……最后输入 Save 命令，用来持久化 PIMEntity 对象集合。再次点击 Run，运行程序。发现之前输入的信息完整保存了下来……用记事本打开 PIMDatabase.dat，看到的是一片乱码：……

可见，实验结果是正确的。

# 四、实验小结
本次实验学习了对象序列化和反序列化的相关操作，这部分内容是我之前不是很明白的，为此我还把《Java 核心技术卷 II》第 2 章输入与输出仔细看了一遍，自我感觉获益良多，尤其是对 ObjectInputStream 类的使用。

---
本实验的完整代码和报告见个人的GitHub：[https://github.com/memcpy0/Mini-Java-Projects/tree/main/PIMCmd-Persistence](https://github.com/memcpy0/Mini-Java-Projects/tree/main/PIMCmd-Persistence)。
