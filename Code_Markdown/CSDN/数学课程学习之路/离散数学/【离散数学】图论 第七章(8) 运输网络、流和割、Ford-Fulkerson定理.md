> 本文属于「离散数学」系列文章之一。这一系列着重于离散数学的学习和应用。由于内容随时可能发生更新变动，欢迎关注和收藏[离散数学系列文章汇总目录](https://memcpy0.blog.csdn.net/article/details/119997004)一文以作备忘。此外，在本系列学习文章中，为了透彻理解数学知识，本人参考了诸多博客、教程、文档、书籍等资料。以下是本文的不完全参考目录，在后续学习中还会逐渐补充：
> - （**国外经典教材**）离散数学及其应用 第八版 `Discrete Mathematics and Its Applications Eighth Edition` ，作者是 `Kenneth H.Rosen` ，袁崇义译，机械工业出版社
> - 离散数学 第二版，武波等编著，西安电子科技大学出版社，2006年
> - 离散数学 第三版，方世昌等编著，西安电子科技大学出版社，2013年
> - （经典参考书及其题解）离散数学/离散数学——理论•分析•题解，左孝凌、李为鉴、刘永才编著，上海科学技术文献出版社
> - 离散数学习题集：数理逻辑与集合论分册，耿素云；图论分册，耿素云；抽象代数分册， 张立昂。北京大学出版社

@[toc]

---
# 8. 运输网络
## 8.1 运输网络的定义
**运输网络**是对各类运输系统中 **物资从起点 $s$ 出发、经过多个中转站点、到达终点 $t$** 这类现象的数学抽象，可以用有向图进行描述。

定义8.1 设 $G = \langle V, E, \omega \rang$ 是一个**连通 边赋权 有向 简单图**，若 $G$ 中恰有一个入度为零的结点和一个出度为零的结点，则称 $G$ 为**运输网络** `transport network` 。入度为零的结点称为**源点** `source` ，出度为零的结点称为**汇点** `sink` 。对于任一边 $e \in E$ 上的权值，$\omega(e)$ 称为 $e$ 的**容量**。

下图8.1中，源点 $s = a$ ，汇点 $t = f$ ，边 $\lang a, b\rang$ 的容量 $\omega(a, b) = 4$ ，边 $\lang a, d\rang$ 的容量 $\omega(a, d) = 5$ ，边 $\lang c, f\rang$ 的容量 $\omega(c, f) = 4$ ，边 $\lang d, c \rang$ 的容量 $\omega(d, c ) = 2$ ，以此类推。对于这张图，我们可以把它认为是一个*输油管道网络*的有向图，原油在码头 $a$ 卸下并通过管道泵送到炼油厂 $f$ ，结点 $a, b, c, d, e$ 表示中间泵站，有向边表示输油管道、并标明了原油的流动方向，边上的数字表示<u>该管道单位时间内的最大输油量</u>。
![在这里插入图片描述](https://img-blog.csdnimg.cn/cf7b9c552524473fa8b68d8862f562c9.png)

在运输网络中，人们关心的问题是求解**最大运输量**，公路铁路运输系统、供水供电系统、传送数据流的计算机网络等，都可以使用运输网络来建模。**物资从源点到汇点的实际流通量**，除了与**每条边的容量**相关外，还与**每个结点的转运能力**相关。

## 8.2 流
定义8.2.1 在**运输网络** $G = \lang V, E, \omega \rang$ 中，设 $\phi$ 是从 $E$ 到非负实数集合上的一个函数，如果满足以下条件，则称 $\phi$ 为该运输网络中的一个**流**：
- 任取 $\lang u, v\rang \in E$ ，$\phi (u, v) \le \omega (u, v)$ ，即**单位时间内每条边的流量不能超过边的容量**；
- 源点 $s$ 和汇点 $t$ 满足 $$\sum_{\lang s, u \rang \ \in \ E} \phi (s, u) = \sum_{\lang v, t\rang\ \in \ E} \phi (v, t)$$ 即**单位时间内源点的流入量等于汇点的流出量**；
- 除源点 $s$ 和汇点 $t$ 以外，其余每一结点 $v$ 均满足 $$\sum_{\lang u, v\rang\ \in \ E} \phi(u, v) = \sum_{\lang v, w\rang \ \in \ E}\phi (v, w)$$
即**每个结点的流入量等于流出量**

值 $\phi(u, v)$ 称为流 $\phi$ 在边 $\lang u, v\rang$ 的**流量**，而 $\sum_{\lang s, u\rang \ \in \ E}\phi (s, u)$ 称为流 $\phi$ 在整个运输网络 $G$ 的**流量** `value` ，记为 $\phi_G$ 。对于边 $\lang u, v\rang$ 来说，如果 $\phi(u, v) = \omega(u, v)$ ，称边 $\lang u, v\rang$ 是**饱和的**；如果 $\phi(u, v) \lt \omega(u, v)$ ，称边 $\lang u, v\rang$ 是**非饱和的**。

仍以8.1中的输油管道网络为例，第一个条件的含义是**单位时间内每条管道的通油量不能超过管道的容量**；第二个条件的含义是**单位时间内泵送到炼油厂的总油量等于从码头卸下并输进管道的总油量**；第三个条件的含义是**每个中间泵站处油的流入量等于其流出量**，即原油在这些中间泵站既无消耗也无补充。

下图8.2给出了图8.1所示**运输网络**的一个**流**。如果边 $e$ 的容量是 $x$ ，而流量是 $y$ ，则边 $e$ 上标记 $x, y$ 。例如边 $\lang a, b\rang$ 的标记为 $4, 0$ ，表示其容量等于 $4$ 、当前流量等于 $0$ ；边 $\lang d, c\rang$ 的标记为 $2, 1$ ，表示其容量等于 $2$ 、当前流量等于 $1$ ；边 $\lang b, c\rang$ 的标记为 $3, 2$ ，表示其容量等于 $3$ 、当前流量等于 $2$ 。
![在这里插入图片描述](https://img-blog.csdnimg.cn/deb9276a34194e46a0f539c6d0ba6760.png)

**一个运输网络中能承载多种流，其中流量值最大的流称为最大流 `maximum flow`**。最大流也可能不止一个，即可能会有多个不同的流都具有最大流量。给定运输网络求其最大流，是一个很有现实意义的问题，可以用来确定**怎样才能使网络的运输效率最高**，即**单位时间内网络中物资的运输量最大**。

为了解决这个问题，先要引入**割**的概念和相关定理。

## 8.3 割
定义8.3.1 设图 $G = \lang V, E, \omega\rang$ 是一个运输网络，$X \subset V$ ，记为 $\overline X = V - X$ 。若 $s \in X$ 且 $t \in \overline X$ ，则称集合 $$(X, \overline X) = \{ \lang u, v\rang \ | \ u\in X, v \in \overline X, \lang u, v\rang \in E\}$$

为运输网络 $G$ 的一个**割**，令 $$\omega(X, \overline X) = \sum_{\lang u, v\rang\ \in \ (X, \overline X)} \omega(u, v)$$

则称 $\omega (X, \overline X)$ 为割 $(X, \overline X)$ 的**容量**。

显然，**对运输网络的一个割来说，每条从源点到汇点的有向路，至少要通过割中的一条边**。

例1 图8.3.1中的虚线给出了图8.1所示运输网络的一个割，该割将结点集合分为 $X = \{a, b\}$ 和 $\overline X = \{c, d, e, f\}$ 。割 $(X, \overline X) = \{\lang a, d\rang , \lang b, c\rang\}$ ，该割的容量是 $\omega(X, \overline X) = 5 + 3 = 8$ 。
![在这里插入图片描述](https://img-blog.csdnimg.cn/b1472e607132467f961690152d04465d.png)

 https://www.cxybb.com/article/aiweiluan5095/102214905

定理8.3.1 在运输网络中**任意一个流的流量**均小于等于该运输网络中**任意一个割的容量**。
证明：设 $G = \lang V, E, \omega\rang$ 是一个运输网络，$\phi$ 是该运输网络的任意一个流，$(X, \overline X)$ 是该运输网络的任意一个割。对于源点 $s$ ，有：
$$\sum_{\lang s, u\rang\ \in\ E} \phi(s, u)= \phi_G$$
对于 $X$ 中不是源点的结点 $p$ 均有：
$$\sum_{\lang u, v\rang\ \in \ E} \phi(u, v) = \sum_{\lang v, w\rang \ \in \ E}\phi (v, w)$$

由这两式可得：

下面举例说明这一定理的含义。如果从北京到上海的所有货物，都要经过武汉长江大桥或南京长江大桥，即使是用足这两座大桥的通行能力，从北京发往上海的单位时间内的货物量，也不能超过这两座大桥单位时间内货物的最大通行量。

考虑如图8.3.2(a)所示的一条路，这条路是图8.2.1所示的运输网络中的一条从源点 $s = a$ 到汇点 $t = f$ 的路。
![在这里插入图片描述](https://img-blog.csdnimg.cn/e3fc465f282a4491b02198897b1cb1b3.png)

定义8.3.2 设 $G = \lang V, E, \omega\rang$ 是以 $s$ 为源点、以 $t$ 为汇点的运输网络，称结点序列 $s = v_0, v_1, \dots, v_{n - 1}, v_n = t$ 为 $G$ 中的**一条不考虑边方向的从 $s$ 到 $t$ 的道路**。如下图8.3.2所示，若 $\lang v_i, v_{i + 1}\rang \in E$ ，则称 $\lang v_i, v_{i+1}\rang$ 为一条**前向边** `forward edge` ；若 $\lang v_{i+1}, v_i\rang \in E$ ，则称 $\lang v_{i+1}, v_i\rang$ 为一条**后向边** `backward edge` 。
![在这里插入图片描述](https://img-blog.csdnimg.cn/381ca87297164935b86d97db7d18b970.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/3d48633085b341c2a540f3df0fcc18b2.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/f0e3fbc106e348f6b812781f4f82ebdd.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/79c4bfaa855b415182b2a15f1eee5235.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/7e79029ea5044333bdbbf9b80e4584e5.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/a7d896919c6840f88bf6bd64699fb75b.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/d50a34ef1b15466cb63f089904c5310f.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/97c277d8a81743c088e77c31d258df35.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/f0d6ecb24d234297abaad4f2275ffa35.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/f653de7641f747de9b31105340bf6d25.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/cdc32f7de5ed4266a2f92a120fcd301b.png)

现实中，还有很多运输网络可能有多个源点和多个汇点。此时只需要在原运输网络中添加**一个虚拟的源点 $s'$** 和**一个虚拟的汇点 $t'$** ，再添加**以虚源 $s'$ 为起点、到原运输网络各个源点的有向边**，容量均设置为 $+\infin$ ，添加**以原运输网络各个汇点为起点、到虚汇 $t'$ 的有向边**，容量均设为 $+\infin$ ，即可将多源多汇问题转化为本节介绍的单源单汇问题。 
