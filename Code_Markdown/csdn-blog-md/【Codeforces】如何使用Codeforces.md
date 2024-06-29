@[toc]

本文介绍国外著名的编程竞赛平台[Codeforces](codeforces.com)。

---
# 1. Codeforces简介
Codeforces位于俄罗斯，最早是由俄罗斯的一群大学生维护的，因此具有以下特点：
- 强烈的开源精神。所有代码和题解都被公开，所有人都可以随意查看其它人的代码；
- 题目众多。各种难度等级的题目都有，而且题目的思维陷阱比较多，即思维题比较多。对数据结构和算法的考察相对弱一点——就算告诉你用什么算法，有的题你也不知道怎么做。
- 有自己的上分系统。每周都会举办一到两次在线算法比赛，比赛时长一般为两个小时，然后根据在比赛中的表现上分或减分。
- 比赛有档次划分。由于参赛选手实力不一，它的比赛都有几个档次 `div` ，不同层次的选手面对的题目难度也不一样。

---
# 2. Codeforces基本功能
以首页的banner为线索，本节简单介绍Codeforces的重要功能，如TOP、CONTESTS、GYM、PROBLEMSET，其他如GROUP、CALENDAR等功能，不是非常常用，就不多赘述了。![在这里插入图片描述](https://img-blog.csdnimg.cn/3d6d0cdeea924d1d86875ab5732c795f.png)
## 2.1 HOME
主页，报到即将开始的比赛，以及在赛后进行总结：
![在这里插入图片描述](https://img-blog.csdnimg.cn/0f229720f8a14e98b4230d4bee456835.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
有时也会显示一些讨论和重要的改进：
![在这里插入图片描述](https://img-blog.csdnimg.cn/336653d8913346d3a798a3d7f1988452.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
## 2.2 TOP
TOP中基本上是dalao们放出来的一些博客、题解和资料，还有很多讨论：
![在这里插入图片描述](https://img-blog.csdnimg.cn/4bbf57b53a2c4754a96b4618f9f71ac8.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
## 2.3 CONTESTS
CONTESTS是最重要的栏目，里面是即将到来、正在进行以及过去举办过的线上比赛。下图就是目前的比赛安排，包括比赛名、作者、开始时间、比赛时长等等，即将开始的比赛会有一个 `register` 按钮，让大家注册参加。
![在这里插入图片描述](https://img-blog.csdnimg.cn/923f03418f7c475b9e2acc89c09c9e39.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
比赛名称后，括号里 `div` 表示的是难度等级，`div1` 是最高难度，需要有 `1900` 以上的Rating才可以报名。`div2` 会简单很多，基本学过数据结构和算法，就可以进去做个一两题——前两题主要考察思维，还不会涉及什么算法，一般到C题之后，才会考察一些算法和数据结构。

注册报名页面如下所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/b80c4ee8ac0c42cb813e43ce54828098.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

Codeforces的比赛一般有六道题（也不一定）、两个小时的时间（看情况），像 `Codeforces Round # ` 和 `Educational Codeforces Round` 都是这种形式。一般人很难把六道题都做完，能做出来三题，Rating就不会跌了。不过涨跌情况还和当前Rating有关——如果是新手，做两道题就可能涨分；如果Rating已经很高，只做出两道题，那肯定是跌的。


在比赛时，还能实时看到每一题过了多少人，从而帮助选择题目。一般来说前面的题目比较简单，但也有例外。比如某些思维题中藏了一些陷阱，很多人没想到就挂在上面，也是常有的事。

比赛时经常出现一种情况，就是做了几道题后还有时间，但是后面的题目想不出来了。这时可以查看其它选手的代码——有一个栏目叫ROOM，在比赛的时候系统会随机分配房间。你可以查看同房间内、通过同样问题的人的代码。

Codeforces比赛中只会测试一小部分数据，真正的测试集放到赛后进行测试。所以赛中测试通过的代码，只通过了小数据验证，很可能有隐藏的问题。你通过了这道题之后，可以去查看其他通过人的代码，分析它们有没有问题。如果发现了Bug，可以构造一份数据HACK掉他的提交，成功后会获得分数奖励。所以到了比赛后期，能做的问题做的差不多之后，就进入了紧张刺激的互相HACK阶段。
![在这里插入图片描述](https://img-blog.csdnimg.cn/fba48cb08dc0469f8bac1ec05b3a7e78.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
下图是之前举办过的比赛，可以 `Enter` 点进去练习 `Practice` ，或者 `Virtual participation` 参加虚拟比赛，感觉和LeetCode的竞赛差不多。它还提供Replay功能，可以模拟当时比赛的情况。
![在这里插入图片描述](https://img-blog.csdnimg.cn/ecc57823251d4d6bbd4c7db9149bf4a4.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
## 2.4 GYM
健身房，顾名思义可以理解成练习题。不过看到ICPC这个字样了吗？这都是些专业的比赛题和ACM的练习题，比起CONTESTS来说题目难度更大，会有各种算法和数据结构的专题。虽说如此，GYM中的题目也是有区分度的，并不是每一题都很难，一般都可以做一两道。
![在这里插入图片描述](https://img-blog.csdnimg.cn/ccf251aa15bb48919c57d3c20402ea74.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
## 2.5 PROBLEMSET
题集，类似LeetCode的题库，可以自己照着某个标签刷题：
![在这里插入图片描述](https://img-blog.csdnimg.cn/afdd9dd912e44a83b21e92733b39358b.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)







