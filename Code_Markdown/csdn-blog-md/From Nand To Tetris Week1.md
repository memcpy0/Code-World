@[toc]


---
依据基本原理构建现代计算机：从与非门到俄罗斯方块（基于项目的课程）
- 数字电路
- 离散数学
- 汇编语言
- 编译原理
- 操作系统
- ……

# General Course Information
## 阅读材料: Course Overview
Welcome to the Nand to Tetris voyage of discovery! You’re joining many learners who took the course in the past, as well as thousands of learners currently enrolled. People take this project-centered course for a variety of reasons. Programmers take the course in order to better understand the underlying hardware that executes their programs; hackers and computer aficionados take the course in order to connect with the soul of the machine; people with no technical background take the course because they are curious about how computers work; and many take the course simply for the kick of building a modern computer system from the ground up.

Whichever is your background or motivation, welcome aboard!

To get started, we suggest reading the [course overview](https://www.coursera.org/learn/build-a-computer/supplement/4DqXs/course-overview), where you will find important information about the course contents, workload, and grading. Also, you may want to introduce yourself in the [Meet and Greet forum](https://www.coursera.org/learn/build-a-computer/forum/PIireTfrEeWddiIAC9pDDA/discussions?sort=lastActivityAtDesc&page=1) of the course.

If you have questions about the course content, you may post them in the relevant course forums. All questions will be answered promptly by the course staff and other learners taking the course. For technical problems with the Coursera platform, visit the Learner Help Center.

We wish you a great Nand to Tetris learning experience!

-- Noam and Shimon

## 阅读材料: Textbook
This course is completely self-contained and requires no additional sources of information or reference. Students who wish to do so can refer to the book The Elements of Computing Systems, written by the course instructors and published by MIT Press. This course covers the first half of this book (hardware); the second part (software) will be covered by a subsequent course.  

## 阅读材料: FAQ
First of all, you should probably consult the [HDL survival guide](http://www.nand2tetris.org/software/HDL%20Survival%20Guide.html). It has some good explanations on various aspects of the HDL language and specifications.

edit HDL files: using your favorite text editor. Once they are ready to be tested, you should open them using the supplied Hardware Simulator.

- When I design chips, do I need to do it using only NAND gates?
You should **use chips you previously built** when designing later chips. What we mean by "using only NAND gates" is that **these are the elementary building blocks**. But once you arranged these elementary building blocks into more sophisticated chips, you are encouraged to **use the sophisticated chips in the future**.

- Can I use loops to create multi-bit chips?
There are no looping constructs in the HDL language. If you want to use several chips, you must explicitly write each one of them. 


- 我收到错误消息“芯片名称与 HDL 名称不匹配”。
确保您的文件名和芯片第一行中的芯片名称相同。名称区分大小写，因此“chipname”与“ChipName”不同。

- 我收到消息“正在加载芯片...”，但文件无法加载。
如果您尝试在其内部使用芯片，则可能会发生这种情况。例如，如果您尝试使用 And.hdl 文件的 PARTS 部分中的 And 芯片。这构成了一个递归循环。

- 规则是您只能使用已经构建的部件。您可以在 Not.hdl 中使用的唯一部分是 Nand。一旦你编写并测试了你的 Not，你就会有 Nand 和 Not 可用，这样你就可以在编写 And.hdl 时同时使用它们。然后，您将拥有 Nand、Not 和 And 可用于编写 Or。  

- 我收到错误消息“在工作文件夹和内置文件夹中找不到 Chip YourChipName”。

- 这意味着您尝试使用未包含在您提交中的芯片。如果您想使用辅助芯片，您必须提交一个包含它们的 zip 文件。

- 我收到错误消息“指定的子总线不在总线范围内”。
这可能意味着您尝试访问总线 [16]。总线线路从 0 开始编号，因此 16 位总线将从 bus[0] 开始并以 bus[15] 结束。尝试访问总线 [16] 超出范围。

- 我收到错误消息“可能无法使用内部节点的子总线”。
阅读HDL 生存指南中的“子总线”和“多输出”部分。

 

- 我收到错误消息“in is not a pin in Nand”。
Nand的界面是： 
  Nand(a= ,b= ,out= );  
它有 2 个输入引脚 (a, b) 和 1 个输出引脚 (out)。您应该在这些分配的右侧填写非门的输入和输出值。

- 我收到 Mux4way16 的比较错误。
硬件位从右到左排序。所以 sel[0] 是数字最右边的数字。如果 sel=01，则 sel[0]=1 且 sel[1]=0。您可以在HDL 生存指南的“位编号和总线语法”部分阅读更多相关信息。

 

- 我收到错误消息“无法将门的输出引脚连接到零件”。
阅读[HDL 生存指南](http://www.nand2tetris.org/software/HDL%20Survival%20Guide.html)中的“多输出”部分。  

以下是您希望最终输出来自 MyChip 并重用某些输出时的示例：
```cpp
MyChip(a=a, b=b, out=out, out[0..7]=smallerInternalBus, out[0]=internalNode);
```

- 如何将常量值（0 或 1）连接到引脚？
```cpp
MyChip(..., a=false, ...);
MyChip(..., a=true, ...);
```

- 当我在模拟器上运行我的“填充”程序时，如果按键被按下超过一秒钟，HACK 屏幕开始闪烁。
这可能是操作系统的击键设置有问题。

- Mult.asm 返回一个比较错误。
确保在程序开始时将 R2 设置为零。

- 我收到错误消息“第一个文件较短”
程序末尾可能有空行。

## 视频: 课程视频Promo Video 
从与非门到俄罗斯方块 第一部分 
任何人都可以选择这门课
- 这个课程包括六个一周项目作业，每个分别关注构建一个不同的计算机硬件模块
- 在每个课程作业中 你将要构建一系列的芯片 对于每一个芯片 我们都会提供一份完整的技术参数 然后你将要解决的是如何利用你之前构建的芯片 来完成这些功能 
- 你将使用硬件模拟器来构建所有这些芯片 就像硬件工程师在实际工作中所做的那样
- 在课程的头三个项目作业中 我们从与非门（NAND）这个最基本的逻辑门开始 利用详尽的课程材料 指导你构建 一个芯片 一个中央处理单元 以及一个内存单元 
- 在接下来的两个项目作业里 我们将装配这些你构建好的芯片 在此之后 我们将构建一台通用目的计算机 我们称它为 HACK ，然后我们将这台计算机接入键盘和屏幕，这台机器就运行起来了 
- 在最后的项目作业里 我们将开发一个汇编器 它可以让我们用符号命令来编写程序 而不是二进制代码 
这将允许我们使用HACK系统 来运行所有类型的酷程序 比如 俄罗斯方块 或者任何你想得到的程序 

这就是 在六个星期里 从与非门到俄罗斯方块 
这门课程每周需要五到十个小时的时间，并且不需要任何计算机科学和工程学的背景知识，任何用以完成这门课程、以及构建一台计算机所需要的知识，都将包含在这门课程里 

 如果你想了解一台计算机是如何工作的 以及如何被设计出来的  

---
# Module 0: Introduction Roadmap
这需要大约 10 min。完成后， 请继续学习并尝试提前完成学习内容。


In a nutshell: course introduction and overview, the roles of abstraction and implementation in systems design, the road ahead.

## WATCH
Unit 0.0: Introduction 
### Unit 0.1: The Road Ahead 
每周我们都会担心单一的抽象级别。 我们将较低的级别视为给定的。 我们只记得它做了什么，我们忘记了我们是怎么做的。 我们实现了更高级别的抽象和 下一层抽象。 我们测试它是否有效。

 以非常基本的构建块开始的计算机，称为 Nand 门， 这是非常非常简单的逻辑门。 并以一台几乎可以运行任何程序的计算机结束

多谈谈前面的路线图，它告诉我们 首先，我们将在本课程的艰苦工作部分做些什么。 我们要做的主要步骤是什么。 然后还有哪些主要步骤将在第二阶段完成
### Unit 0.2: From Nand to Hack 
![在这里插入图片描述](https://img-blog.csdnimg.cn/14df05b4b57f4b4abec5bce2076481db.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_12,color_FFFFFF,t_70,g_se,x_16)
我们抽象这个硬件并专注于在我们能想到的最基本的逻辑门上，它叫做 Nand。 你知道，Nand 是我可以在十秒钟内描述的东西 我将在以下单元之一中进行，但现在，让我们假设它只是一个基本的逻辑门。
我们采用这个与非门，并从中使用一种称为组合逻辑的艺术，我们将构建一整套基本逻辑门，例如 AND 或异或等等。 然后我们将使用这些门，我们将同时使用它们。
组合逻辑和时序逻辑，是一个不同的设计，它考虑了和时间和时钟，从中我们将构建诸如寄存器和 RAM 单元以及 CPU 之类的东西。 
然后，我们将使用我们构建的这个芯片组， 我们将根据它设计一个完整的计算机架构，称为 Hack。

为了写可以在这台机器上执行的程序，在这台机器上做以方便的方式，我们还将引入一个汇编程序 我们还将为 Hack 机器语言开发一个汇编程序。

今天的硬件工程师不会赤手空拳地做任何事情。 他们使用计算机开发计算机。 特别是他们使用一种叫做硬件模拟器的东西来设计和 测试和调试他们想要构建的硬件

您可以从我们的网站免费下载的一款软件。 你要把它安装在你的电脑上，你会做的 本课程中的所有项目都使用您的计算机和我们将提供的软件。

您在此处看到的是对 Xor 操作方式的模糊描述。 所以基本上你要接受这个障碍，以及我们将提供的各种提示和指导。 
我们将提出一些逻辑图，使您能够 使用您之前构建的较低级别的门构建 Xor。
然后，您将获取此图表，并使用我们将使用的语言指定它HDL

数字逻辑（组合逻辑、时序逻辑）和离散数学、CSAPP和From Nand to Tetris

 在第一周，我们将构建一些基本的逻辑门。 总共15个门。 正如您在幻灯片中看到的那样。 在第二周，我们将构建一个算术逻辑单元， 一个 ALU，它是我们稍后将构建的 CPU 的核心部件。 在第三周，我们将建立记忆系统，你知道的， 从寄存器开始，一直到 RAM 和 ROM 单元。 在第五周，我们将拿走我们制造的所有Chips， 并用它来设计一台真正的计算机。
 
在第 4 周，在我们构建计算机之前，我们将用 Hack 机器语言编写一些程序以感受这台计算机将要做什么。

在课程的最后一周，我们将介绍一个汇编程序 Hack 机器语言，实际上以两种不同的方式开发它。一种适合具有编程背景的人。 一种是为那些不这样做的人准备的。

### Unit 0.3: From Hack to Tetris 
在第一部分，在本课程中， 正如您所听到的，您将从非常简单的 Nand Gate 开始。 然后你就可以找到一个可以运行的工作计算机系统。我们还将构建一种您可以在其上运行的计算机语言，但是 它是一种非常低级的汇编语言

在课程的第二部分，我们将处理软件 层次结构，这正是我们要实现的。 我们将介绍一种高级语言，称为 Jack。 我们将为它编写一个编译器

### Unit 0.4: Project 0 Overview 
零项目由两个任务组成。 首先，您必须下载 Nand2Tetris 软件套件和 把它放在你的电脑上。 这将需要大约两、三分钟的时间。 然后，您必须通过 Coursera 提交 HDL 程序，只是为了确保您知道如何使用用户界面等

您将拥有一个名为 Nand2Tetris 的文件夹。 让我们来看看这个文件夹，看看里面有什么。
- 基本上，这个文件夹有两个子文件夹。 其中一种称为工具，另一种称为项目。
- 在工具文件夹中，您会找到我们要走的一整套软件工具 在本课程和尚未提供的第二门课程中使用。 所以，基本上，我们将使用前三个工具， 本课程中的硬件模拟器、CPU 模拟器和汇编器。 其余工具将用于 Nand2Tetris 第二部分。 
- 如果您查看项目子文件夹，您会发现其中有 13 个项目，或者 十四个项目，包括这个，实际上是零项目。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2f8a1af16d5342c8af3b00d343f153bf.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
将 Nand2Tetris 软件套件下载到您的计算机上。该软件套件包括将在整个课程中使用的所有软件工具和项目文件。下载后，在您的计算机上的 nand2tetris/projects/00 文件夹中找到 file.txt。无需了解或修改此文件的内容，此作业的唯一目的是验证您是否知道如何使用我们的课程平台提交文件。为了为下一个项目做好准备，您需要将文件压缩为名为 project0.zip 的 zip 文件，然后提交此 zip 文件。您可以在此处找到有关制作 zip 文件的说明。确保 zip 文件只包含 file.txt，并且 file.txt位于zip 文件的顶层，而不是位于文件夹内. （教师在第 0.4 单元中对“按原样上传文件”的评论意味着您不应修改文件的内容。因此，您将按原样上传文件，但要在 zip 文件中，如此处所述）。

## DO 
[Download the Nand2Tetris Software Suite](http://nand2tetris.org/software.php)  to your computer. The software suite includes all the software tools and project files that will be used throughout the course. After downloading, **locate file.txt in the nand2tetris/projects/00 folder on your computer**. There is no need to understand or modify the contents of this file, the only purpose of this assignment is to verify that you know how to submit files using our course platform. To prepare you for the next projects, you will need to compress the file into a zip file named project0.zip, and submit this zip file. You can find instructions on making zip files here. Make sure that the zip file only contains file.txt, and that file.txt is in the top level of the zip file and **not located inside a folder**. (The instructor's comment in unit 0.4 to "upload the file as is" meant that you are not supposed to modify the file's contents. So, you are to upload the file as is, yet within a zip file, as instructed here).

If you are taking the course as an auditor, just check that you can locate file.txt in the nand2tetris/projects/00 folder. If you are taking the certificate option, [submit](https://www.coursera.org/learn/build-a-computer/programming/Mezhu/project-0) project0.zip, containing the file nand2tetris/projects/00/file.txt

![在这里插入图片描述](https://img-blog.csdnimg.cn/15e4c2a2bede4878908016a7c6cc4be0.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)


## **READ** (optional):
Introduction chapter of The Elements of Computing Systems (the course's textbook). 

## **GET HELP:**
[Module 0 Discussion Forum](https://www.coursera.org/learn/build-a-computer/module/mFLIt/discussions?sort=lastActivityAtDesc&page=1)

 
---
# Module 1: Boolean Functions and Gate Logic Roadmap
**In a nutshell**: We will start with **a brief introduction** of **Boolean algebra**, and learn how **Boolean functions** can be physically implemented **using logic gates**. We will then learn how to **specify gates and chips using a Hardware Description Language (HDL)**, and how to **simulate the behaviour of the resulting chip specifications using a hardware simulator**. 

This background will set the stage for Project 1, in which **you will build, simulate, and test 15 elementary logic gates**. The chipset that you will build this module **will be later used to construct the computer's Arithmetic Logic Unit (ALU) and memory system**. This will be done in **modules 2 and 3, respectively.**

**Key concepts**: 
- Boolean algebra, Boolean functions, gate logic, 
- elementary logic gates, Hardware Description Language (HDL), hardware simulation.

## **WATCH:**
数字逻辑、离散数学（命题逻辑的联结词、（极小）全功能联结词集合、常见逻辑等价式和等价关系的证明、主析取范式、代数系统的布尔代数）、
### Unit 1.1: Boolean Logic
我们总是可以看到 x AND y，无论 x 和 y 是什么，都完全等于 y AND x。 这称为交换律。 对于 x OR y 等于 y OR x，我们有相同的类似现象。 所以这些就像交换律。即布尔代数。

使用，众所周知的结合律。 如果你有 x AND y AND z，如果你先做 x AND y，或者你先做 y AND z 都是一样的。 类似的事情发生在所有人身上。 另一个著名的定律，著名的定律是分配法。 如果你有 x AND y OR z，结果是完全相等的， 等于 x AND y，所有这些，OR x 和 z。
![在这里插入图片描述](https://img-blog.csdnimg.cn/ac66c9f9ff794600803576329e1eb40d.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
现在这被称为幂等定律
### Unit 1.2: Boolean Functions Synthesis
谈论我们如何构建 来自更原始操作的布尔函数

我们现在要做的恰恰相反。 你从一个函数的描述开始， 假设作为真值表给出，并且 我们的挑战是想出一个计算相同布尔函数的公式。

这正是我们设计计算机时必须做的事情。 

从作为真值表给出的布尔函数的规范开始。 我们如何构建它？

为它构建一个析取范式公式。
现在我们基本上已经构建了仅使用 and、not 和 or 的布尔表达式。

但是 我们实际上可以用许多不同的方式写出相同的表达式。 其中一些会比其他人短，一些，其中一些可能是，可能是 就我们何时真正在计算机中实现它们而言，效率更高。 但关键是，从逻辑上讲，它们都是完全等价的。

您实际上如何找到最短的或 与我们刚刚推导出的公式等效的最有效公式？ 嗯，一般来说这不是一个容易的问题。 这对人类来说并不容易，也没有任何算法可以有效地做到这一点。

 事实上，这是一个NP-hard问题 实际上找到与给定表达式等效的最短表达式
 
是我们真的证明了非常了不起的数学定理 任何布尔函数，实际上有多少变量和 布尔函数是什么， 可以仅使用 AND、OR 和 NOT 运算在表达式中表示。

然而，由于我们生活在有限的世界，布尔代数， 布尔函数只能用 AND、OR 和 NOT 表示。 这正是赋予我们实际构建的基本力量的原因 只有这些可能的门的计算机， 只有这些可能的操作，AND、OR、NOT。 但我们真的需要所有这些吗？ 好吧，如果你愿意，这里有一个更好的定理。

只需使用 AND 和 NOT，我们就可以构造任何布尔函数。我们实际上可以计算一个 OR 带有 AND 和 NOT 门。

有一些布尔函数，当你输入 0 时会给你 1 作为输出，所以 AND 本身是不够的。

但事实证明，还有另一个操作本身 足以实际计算所有内容，所以让我介绍 NAND 函数。

好，好的是我们可以证明下面的定理，如果你只 有与非门，你已经可以计算每个布尔函数，你已经可以 仅使用这些与非门

我们知道如果你能做NOT，如果你能做AND，你可以做任何事情。 所以我们只需要展示如何使用与非门做 NOT以及如何做 AND。
$\lnot x = (x \uparrow x)$
$(x \land y) = \lnot (x \uparrow y)$ 

所以此时， 我们刚刚完成了关于布尔逻辑的抽象观点。 现在我们正在做一个非常有趣的视角转变 从抽象的逻辑操作到实际的门

到目前为止，我们要求您将一切都视为抽象的布尔逻辑。 从现在开始，我们要开始要求你考虑一切 作为计算机中计算我们想要的功能的实际小部件。

 
### Unit 1.3: Logic Gates
从本单元开始，我们将讨论我们实际上如何 使用硬件实现这些布尔函数

这是一种技术 使用逻辑门实现布尔函数。逻辑门是一个独立的芯片
一个非常简单的芯片或一个基本的芯片，旨在提供明确定义的功能

什么是复合逻辑门？ 复合逻辑门是由基本逻辑门和 其他复合逻辑门

在本课程中，我们将开发诸如多路复用器和加法器之类的东西

显然，你们中的大多数人不知道这些术语的含义。 别担心。 你会熟悉他们并且 在课程的接下来几周内实际构建它们。

![在这里插入图片描述](https://img-blog.csdnimg.cn/fe5ef8e2e5f0429292152c08fc47b95e.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_19,color_FFFFFF,t_70,g_se,x_16)
应该只有一种独特的方式来描述门应该做什么
 同时，可能有几种不同的实现 实现相同的Abstraction
 它们可能使用更少的能源，它们可能更便宜或更贵，等等
 
让我们谈谈称为电路实现的东西。 如果我愿意，我可以使用这些门来实现 像这样的硬接线电路。 这里有某种图形语言 描述了什么  
### Unit 1.4: Hardware Description Language
在之前的单元中， 我们讨论了如何使用逻辑门来实现布尔函数。 在本单元中，我们将讨论如何实际构建和 使用硬件描述语言或 HDL实现逻辑门

一旦你在 HDL 中构建了一个逻辑门，你就可以实际模拟它，测试它。 最后，正如我们现在要描述的那样，在硬件中构建它
 作为门建筑师，您要做的第一件事是要求一个完整的 所需门的行为的完整描述

我们用一些文档启动 HDL 程序或 HDL 文件，你可以在那里写任何你想写的东西， 它描述了门应该做什么。 然后我们指定芯片的名称。 芯片输入的名称和芯片输出的名称。

您将在其中描述该芯片的实际设计方式。 现在当你使用这样的芯片，这样的异或芯片， 到目前为止我们所看到的称为门接口。 请注意，这是我使用异或门所需的一切。

但是，如果我必须建造这扇门，那又是另一回事了。 现在，我必须打开黑匣子并实际设计它。

我们可以做的第一件事就是画边界 芯片图或门图。 所以我们使用这个破折号来概述这个目的 留在边界之外的是用户对这个门的看法。 换句话说，门界面。 所有用户都知道他或她有一个有两个输入的门， a 和 b，一个输出被调用。

通常，当您编写 HDL 代码时，您可以接受任何信号。 并根据需要分发此信号的任意数量的副本 进入任意数量的目的地。 这个布线或这些调度是同时进行的。 

继续前进，请注意我们正在使用一些现成的门而不是和或。 现在每当你使用一个现成的门时， 您必须使用门输入和输出的名称。称为门的签名或门的 API 。

现在的规则是这些连接中的每一个都必须命名并 想出合理的名字

我们将重点关注此文件的实现部分。


所以这个HDL图无非是 门图的结构描述。实现不是唯一的， 相同的接口通常可以以许多不同的方式实现。我们可以仅使用三个逻辑门来实现 Xor 芯片。

与我们通常在其他编程语言中所做的非常相似。 我们必须担心我们的 HDL 程序的良好文档，我们必须想出好的描述性名称。我们还使用了缩进。

还有一些东西是 HDL 真正独特的。 首先，HDL 是一种函数式或声明式语言。 没有进行任何程序。 没有程序执行。 它只不过是对门图的静态描述。实际上可以按照我们希望的任何顺序编写这些 HDL 语句。  但通常习惯上开始描述 您的图表从左到右，这也使代码更具可读性。

一般的硬件描述语言。 其中有很多，但最流行的两个 至少我知道的 VHDL HDL 是 Veri、Verilog
然而，它是这些 HDL 的一个非常小和简单的版本。 出于这个原因，你可以在一小时内掌握它 阅读教程并开始编写您自己的一些 HDL 代码。 
最重要的是，我们的 **HDL** 以及我们的**硬件模拟器**为您提供 构建本课程中描述的计算机**所需的一切**。

- 如果您想阅读有关 HDL 的更多信息，并且您应该阅读。 查看教科书中的附录 A 并阅读Nand2Tetris 网站上的 HDL 生存指南。 两者实际上都可以在网站上找到。这是为您提供 HDL 入门的单元
- 你可能还想学习硬件模拟器教程， 并学习如何实际阅读 HDL 描述，以及 使用仿真来执行这些 HDL 的底层逻辑。
### Unit 1.5: Hardware Simulation
描述 如何进行 HDL 设计和 在硬件模拟器的环境中使它们栩栩如生。

 我们不知道我们放在一起的这个架构是否能提供 我们寻求设计的芯片的预期结果。 所以在本单元中，我们将缩小这一差距
，尽我们最大的能力验证该程序或 HDL 文件提供底层芯片的预期功能。

鉴于我们有一个特定的 HDL 文件并且我们想要测试它， 我们可以将其加载到称为硬件模拟器的特殊程序中。这个程序恰好是用 Java 编写的。 该程序旨在模拟和测试 HDL 文件。我们把它加载到这个程序中。 我们交互式地测试该芯片中的各种操作并 我们将这种测试模式称为交互式模拟。

或者可以编写另一个文件，然后使用我们设计的一种特殊语言，我们称之为测试语言， 你可以在几分钟内学会的东西。 将一组预先确定的可复制测试放在一起，不必以交互方式做事。提前考虑如何系统地测试底层芯片，并写在所谓的测试脚本中 。然后我们将两个单独的文件加载到模拟器中。 我们加载 HDL 代码。 我们加载测试脚本，然后发送到模拟器工作。 模拟器完成测试脚本和主题中的每一步 HDL 代码到此提供的脚本中的指定测试。 我们称这种操作模式为基于脚本的模拟。

最后，如果我们愿意，我们可以记录 模拟的输出称为输出文件。 我们甚至可以将模拟结果与所需的输出进行比较， 它存储在另一个名为比较文件的文件中。

这个单元的目的是，通过逐步引导您完成所有这些新信息 测试一个将伴随我们整个单元的工作示例的过程。

实际上只有一种软件工具， 硬件模拟器。 在自己的电脑上调用这个硬件模拟器 确保我们所做的也可以在您的机器上完成。我们在讲座中所做的一切也可以由您完成。

如果你想改变芯片逻辑，您必须使用一些外部文本编辑器来编辑 HDL 文件。 保存新文件，将其重新加载到模拟器中，然后 重新运行测试以测试新的芯片设计。

总的来说，我们将尝试始终使用测试脚本。 好消息是你真的不必担心如何编写测试 脚本，因为我们将向您提供您需要的所有测试脚本 为了测试您将在 hdl 中设计的芯片

我们可以做的另一件事是跟踪模拟的输出。 特别是可以增加使用类似于以下命令的测试脚本。我们可以在测试开始时初始化事物，我们可以指示模拟器创建一个输出文件， which in this example, we happen to call Xor.out.  
在每次测试结束时， 我们告诉模拟器向输出文件输出一组值， 这是在脚本的序言中确定的。 因此，正如您看到的第三行脚本所说，放置的输出是 a、b 和 out。 所以每当模拟器看到指令输出时， 它将写入输出文件，a、b 和 out 的当前值。 在模拟结束时， 我们可以简单地检查输出文件的结果并说服我们自己 该芯片实际上已按照预期运行。
我们有一个测试脚本，可以在右侧窗格中看到。 这是某种多用途的 GUI 区域， 我们用于不同的目的。 在我们加载脚本之后， 我们可以指示模拟器实际执行脚本。 我们通过单击此控件和模拟器来运行模拟器。 最后，如果我们愿意，我们可以检查生成的输出文件。

如果你想学习如何使用它， 有几个地方可以查阅可用的文档。 如果您访问 nand2tetris .org 网站，您可以找到完整的章节， 我认为是附录 a 和附录，实际上是附录和附录 b。 在我们的书中描述了如何使用硬件模拟器和 这些，这些章节也可以在网站上免费获得，并且 你应该做的另一件事是去硬件模拟器教程， 这是一堆交互幻灯片，解释了如何使用模拟器 以及您在本次讲座中看到的所有示例。


 现在，在模拟的每一步中，每当我们输出一组 选定的引脚值，模拟器将比较输出 比较文件中相应行的一组值。

在这个行为模拟的概念中，这一点非常有趣。 而且，有趣的是芯片 逻辑可以以设计者希望的任何方式实现。 所以我们可以用Java或其他一些高级语言来编写这个芯片逻辑。我们可以运行程序。 我们可以生成一个输出文件，然后再一次， 将其作为比较文件提供。 这种使用或编写门逻辑的技术 使用高级语言实现门逻辑， 让你有能力计划和 在编写一行 HDL 代码之前测试您的硬件
![在这里插入图片描述](https://img-blog.csdnimg.cn/2b36d2a7ff0c48acaec874a51e71c34d.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
我们，诺姆和我，课程讲师， 我们扮演系统架构师的角色， 你们正在扮演实际硬件开发人员的角色。
### Unit 1.6: Multi-Bit Buses
如何一起处理一堆位
本周，我们将构建某种芯片，将两个二进制数相加。 这些二进制数中的每一个都有一堆位。 实际上，在我们的实现中是 16 位。 但我们想，将它们视为数字。 所以我们的两个数字相加的芯片将被描述为有两个输入。 一个输入，它是 16 位。 和 B 输入， 这是 16 位，一个输出，即 out，也有 16 位。

您有时可能希望将总线分解为子总线。 所以这里的第一个例子显示， 如果我们想要组成一个总线，一个 16 位总线会发生什么。 来自两个八位总线。 所以我们，所以这个例子表明我们在我们的输入中有两个 8 位总线，两个 8 位 总线称为 lsb（最低有效字节）和 msb（最高有效字节）。 如果我们想将它们一起插入一个 AND 16 门， 我们可以只取总线的前 8 位并将 lsb 插入其中。 和第二个 8 位位并将 msb 插入其中。
![在这里插入图片描述](https://img-blog.csdnimg.cn/142e82f0e74b43f182007e794b58d8ce.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
我们的另一个约定是内部总线。 只是它们的宽度完全由你插入它们的东西推导出来。 因此，您无需指定内部 bu 内部引脚的宽度
![在这里插入图片描述](https://img-blog.csdnimg.cn/c2e7ed636c2c4093b28e1993e847e580.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

### Unit 1.7: Project 1 Overview
既然您已经熟悉了 HDL 和硬件仿真， 我们已经准备好亲自动手并实际开发一些芯片。
我们将在此构建的黑客计算机 当然由大约 30 个不同的芯片组成。 我们将一步一步地构建它们。 在第一个项目中，我们从 Nand 开始，这是上帝赐予的，或者是我们所赐的， 或者您可以将其视为可以假设的某种公理。

没有门时如何实现门？
![在这里插入图片描述](https://img-blog.csdnimg.cn/27c3617a3bcf4c74a3ff17f398d751d0.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

这些门包括 构建我们的计算机所需的所有基本逻辑门。 因此，项目 1 奠定了我们在后续项目中将要使用的基础 ，因为在其他项目中，我们将使用这些芯片， 为了构建更复杂和更有趣的功能。

![在这里插入图片描述](https://img-blog.csdnimg.cn/eecbd032ab2c4f2c9127b27f4c0c05a5.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
So what is a multiplexor? A multiplexor is a gate that operates as follows. There are two inputs coming in, and we know them a and b. And there's a sel input coming in, so-called, from the bottom. So three inputs come in, a, b, and sel. What does the multiplexor do? Well, if sel = 0, the multiplexor outputs a. If sel = 1 the multiplexor outputs b. That's it, that's the desired behavior of a multiplexor.
We have three inputs, so we have eight different possibilities.
![在这里插入图片描述](https://img-blog.csdnimg.cn/e490d1ab97d54f82a59c3c87f03fa787.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
多路复用逻辑是如何发挥作用的 在构建有时称为可编程门
Example: using mux logic to build a programmable gate
![在这里插入图片描述](https://img-blog.csdnimg.cn/1219026b47c74cfa85fd9e3673906c2b.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
我们如何构建多路复用器？ 我们从系统架构师那里得到所有这些信息，我们还得到了一个文件，以及我们必须做的事情， 当然是写HDL代码。 事实证明，我们可以使用以下方法构建多路复用器 三个门，And、Or 和 Not，我们将它们连接起来，某种巧妙的方法，结果是所需的 Mux 逻辑。 因此，由您决定如何进行接线和 实际实现所需的多路复用器。


 多路分解器看起来像多路复用器的反面。 它接收单个输入，并根据选择位， 它要么将输入引导至 a 输出，要么引导至 ab 输出。 好的，所以它是一种价值的分配器 进入几个可能的渠道之一。 在这个例子中，我们有一个双向多路复用器
DMux 基本上是一个逆 对于多路复用器，DMux 是多路复用器的逆。它再次分发 单个输入到几个可能的输出通道之一。
![在这里插入图片描述](https://img-blog.csdnimg.cn/a60d03be78db40bc83c1fff04b957aa1.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

![在这里插入图片描述](https://img-blog.csdnimg.cn/10c981d5e45b4eeea148067da50511ff.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
DMux 和 Mux 逻辑组合在一起使我能够编织或交错几个 单条通信线路上的消息，这可能非常昂贵且 
因此，将其用于多条消息是值得的。 顺便说一下，这个计划的另一个吸引人的地方是 是它可以完全异步，对吗？ 它们不必根据主时钟运行。这两个操作中的每一个，编码和 解码操作可以单独完成。它们是迄今为止最重要的 您必须构建的基本逻辑门中的复杂形状。

所以这是 关于您实现的 16 位变体，我想说些什么。 最后我想讨论一个多路变体的例子
![在这里插入图片描述](https://img-blog.csdnimg.cn/3528a6635c9c475197abb3fba794a85d.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
 再一次，如果您的芯片产生了我们指定的预期行为 然后你就完成了这个芯片，你可以继续开发下一个。 所以对于你必须开发的每一个芯片， 我们将为您提供存根文件、测试脚本和比较文件。 

当你构建一个像这样的特定门图时， 你将使用各种芯片部件。 在这种情况下，两个端门将单个或 门和两个转换器或两个非门。 问题是你怎么知道 是输入和输出文件的名称，对不起输入和 您要使用的每个门的输出引脚？ 答案是，如果您使用的门是所谓的现成门，或 如果它们是黑客芯片组 API 的一部分， 我们提供有关如何使用它们的所有必要信息。 特别是，在 nand2tetris 网站上有一个文档 这为您提供了 Hack 芯片组中所有芯片的 API。 该文档实际上是 HDL Survival Guide 的一部分，因此 每当您不确定时，输入的名称和 输出特定芯片部件的引脚。 请查阅此文档

So this gives us tremendous flexibility because it means that in principle we can build these chips in any order that we please although we recommend that you build the chips in the given order that they are listed in Project 1. 

![在这里插入图片描述](https://img-blog.csdnimg.cn/501df7757ba04bde84f9063af49ea023.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)总的来说，就像我们在计算机科学中所做的那样， 尽量少用零件， 尝试创建一个优雅、可读且做得好的实现。 
![在这里插入图片描述](https://img-blog.csdnimg.cn/2c0e2ada31384f29a26b3515c8fc03c5.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
HDL 中没有递归

Norm 解释了多位总线的整个概念， 我想在这次讨论中补充的一件事是， 这些总线从右到左编入索引。 所以如果 A 是 16 位总线，那么 A[0] 代表 最右边的位，有时称为最低有效位或 LSB。 而 A[15] 代表最左边的位， 也称为最高有效位或 MSB。 因此，这只是您在索引时必须牢记的约定
### Unit 1.8: Perspectives
在本课程的第一周， 我们构建了一套 15 个基本逻辑门。
 
在课程的下周，我们将使用这个基本功能和 使用它来构建一些更有趣和 功能强大的芯片，如加法器和算术逻辑单元。

So the first question of the day is is it possible to build a computer starting with a logic gate other than NAND?

you can use another elementary gate called NOR, which stands for not or, and base your entire computer on this atomic building block. Likewise, it is quite natural to start with the suite consisting of and, or and not gates and use them to build a system. 

事实证明与非门 在硬件系统的物理实现中非常流行。 因为有许多集成电路技术，其中相当便宜或 构建与非门相对便宜

下一个问题是， 如果你真的必须建造一个与非门，你会怎么做？
那已经是物理学或电子学，而不是计算机科学。

本课程中重要的是，我们不关心技术。 我们不关心这个实现或 如果有任何其他实现。 我们想把它抽象出来，远离那些变得真实和 false 并输出 true 和 false，我们不关心具体如何。

本课程和本周中使用的 HDL 如何， 它与硬件工程师通常使用的真实 HDL 语言相比如何？ 首先，我们的 HDL 是一种非常真实的语言，因为您可以使用它来 设计计算机和模拟计算机，这就是 HDL 的全部内容。 同时显然，工业强度的 HDL 语言，如 Verilog 和 VHDL 远比我们的 HDL 复杂和强大。
## DO:

[Project 1: Elementary Logic Gates](http://nand2tetris.org/01.php).

**Before starting to work on Project 1, make sure you read Appendix A of the book, and the HDL Survival Guide**  (links to these resources are given below).  **There is no need to read these documents back to back**. Rather, **use them as technical references, on a need-to-know basis.**  That is, if you have an HDL problem, **consult these references first** before posting a question in the forum.

**The outcome of your work will be a set of text files**, **containing the HDL code of the 15 chips** that you are expected to build. If you succeed building all 15 chips, great. If you manage to build only some of the chips, you can still submit them for partial credit.

To ensure proper testing and feedback, **the names of the HDL files that you submit must be the exact file names that appear in the nand2tetris/projects/01 folder** on your computer. Note that case matters: **a file named and.hdl or AND.hdl may fail our testing** and grading procedures (the correct name must be And.hdl). You can save yourself all this hustle by neither creating nor naming any files: **simply edit the `*.hdl` files in your nand2tetris/projects/01 folder using any text editor**, write your HDL code in them, and **save them using their given names**.

You should pack all the *.hdl files that **you wrote as one zip file named project1.zip** (pack the files themselves, don't put them inside any folders), and submit it. If you build helper chips that were not specified by the course, **you should include them in the zip file**. You have an unlimited number of submissions, and **the grade will be the maximum of all your submissions**, so you can't lose points by submitting again.

If you are taking the course as an auditor, you can check your work yourself, using the tests described here. **If you are taking the certificate option, submit your project zip file here**.

## READ:

[Chapter 1](https://b1391bd6-da3d-477d-8c01-38cdf774495a.filesusr.com/ugd/44046b_f2c9e41f0b204a34ab78be0ae4953128.pdf) of The Elements of Computing Systems.
[HDL Guide](https://drive.google.com/file/d/1dPj4XNby9iuAs-47U9k3xtYy9hJ-ET0T/view) (except for A.2.4)
[Hack Chip Set](https://drive.google.com/file/d/1IsDnH0t7q_Im491LQ7_5_ajV0CokRbwR/view) (when writing your HDL programs, you can **copy-paste chip-part signatures from here**)
[FAQ](https://www.coursera.org/learn/build-a-computer/supplement/Dkosx/faq), containing frequently asked questions by students. If you get an error message you can't solve, you should try looking here.

GET HELP: [Module 1 Discussion Forum](https://www.coursera.org/learn/build-a-computer/module/4UT5w/discussions?sort=lastActivityAtDesc&page=1)

  
