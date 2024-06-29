@[toc]

 
## 一、主要内容
### 问题的提出
- 同一个逻辑函数可以有多种表达形式；
- 一种形式的表达式，对应一种电路；
- 表达形式越复杂，则电路越复杂；

如何处理函数，以实现用尽量少的单元电路、尽量简单的电路类型来达到目的。即，逻辑函数要化简。

### 函数化简的目的 
- 逻辑电路所用门的数量少，每个门的输入端个数少，**降低成本**；
- 逻辑电路构成级数少，保证逻辑电路能可靠地工作，**提高电路的工作速度和可靠性**。

### 函数化简的方法：
- 代数化简法（公式法）
- 卡诺图化简法

## 二、代数法化简逻辑函数
代数法**化简逻辑函数的实质**是反复运用逻辑代数的公式和规则，消去表达式中的多余项和多余变量。在用代数法化简逻辑函数时， 往往要依靠经验和技巧 ，带有一定的**试凑性**。

方法：
-   并项：  利用 $AB + A \overline B = A$ 将两项并为一项，且消去一个变量 $B$；
- 消项：  利用 $A + AB = A$ 消去多余的项 $AB$；
- 消元：利用 $A + \overline AB = A + B$ 消去多余变量 $\overline A$；
- 配项：利用 $AB + \overline AC + BC = AB + \overline AC$ 和互补律、重叠律先增添项，再消去多余项。

函数表达式一般化简成 **与-或式** ，其最简应满足的两个条件：
1）表达式中“与”项的个数最少；
2）在满足1）的前提下，每个“与”项中的变量个数最少。
$e.g.$ 化简$F = A\overline C + ABC + AC\overline D + CD$。
$$\begin{aligned}
F &= A\overline C + ABC + AC\overline D + CD \\
&= A(\overline C + BC) + C(A\overline D + D)\\
&= A(\overline C + B) + C(A+D)\\
&= A\overline C + AB + AC + CD\\
&= A(\overline C + C) + AB + CD\\
&= A(1 + B) + CD = A +CD
\end{aligned}
$$

实际上，我们最经常使用的是卡诺圈法，代数法化简常常作为结果的印证，我也就不麻烦的输入公式了，直接截图吧。
<img src="https://img-blog.csdnimg.cn/20200328100458934.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "50%">

<img src="https://img-blog.csdnimg.cn/202003281005125.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "50%">

---
## 三、卡诺图法化简逻辑函数
### 1. 卡诺图
卡诺图：是真值表的图形表示：
-  将变量分成两组，构成二维表；
- **行列组合排列顺序为循环码**；
- 表中每个方格对应真值表中的一行，代表一个最小项。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200328102407424.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200328103542813.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200328103729830.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200328103746968.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70) $\blacksquare$ 逻辑相邻性
- **相邻方格**的**最小项**，具有 **逻辑相邻性** ，即有一个变量**互为反变量**；
- 具有**逻辑相邻性**的方格有
•  相接 —— **几何位置相邻**的方格
•  相对 —— **上下两边、左右两边**的方格
•  相重 —— 多变量卡诺图，以对称轴相折叠，叠在一起的方格（五变量）

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200328104242673.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200328104321656.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200328104412278.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

### 2. 卡诺图填图
卡诺图有什么用？为什么要用循环码？
- 循环码：相邻两个码字有一位相反； 
- 卡诺图：逻辑相邻项有一个变量互为反变量；
- 卡诺图中的两个逻辑相邻项相加可以消去一个变量。

用卡诺图法对逻辑函数进行化简时，首先要确定函数与卡诺图的关系，将函数用卡诺图的形式表现出来。**真值表、表达式、卡诺图都可以表达一个逻辑函数。**

#### 1. 真值表填卡诺图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200328110536552.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
#### 2. 表达式化为最小项表达式填卡诺图
对于 $F = \overline AB\overline C + ABD + AC$ 来说， 可以如下化简：
$$\begin{aligned}
F &= \overline AB\overline CD + \overline A B\overline C\ \overline D + ABCD + AB\overline CD + A\overline BC\overline D + A\overline BCD + ABC\overline D + ABCD\\
&= m_5 + m_4 + m_{15} + m_{13} + m_{10} + m_{11} + m_{14} + m_{15}\\
&= \sum m(4,5,10,11,13,14,15)
\end{aligned}
$$![在这里插入图片描述](https://img-blog.csdnimg.cn/20200328125959284.png) ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200328125921322.png)

#### 3. 表达式作为一般与或式填卡诺图
由一般与-或式填卡诺图示例1：三变量 $F = AB + \overline AC$ 
1.将所有满足 $A=1且B=1$的方格内填“1”
2.将所有满足 $A=0且C=1$ 的方格内填“1”
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200328130231658.png)

由一般与或式填卡诺图示例2：四变量 $F = BD + \overline B\ \overline D + A\overline B CD+ \overline B C \overline D$
1.在所有满足 $B=1且D=1$ 的方格内填“1”
2.将所有满足 $B=0且D=0$ 的方格内填“1”
3.在所有满足 $A=1且B=0且C=1且D=1$ 的方格内填“1”
4.在所有满足 $B=0且C=1且D=0$ 的方格内填“1”

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329101822678.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 3. 函数的卡诺图化简
化简依据：相邻最小项  $\rightarrow$ 提出公因子 $\rightarrow$ 消去互补变量
$e.g.\ \ AB + A\overline B = A(B +\overline B) = A$
$e.g.\ \ ABC + A\overline BC = AC(B + \overline B) = AC$

方法： 
1）填写函数卡诺图
2）对邻项方格画卡诺圈
3）消去互补变量，直接写出最简与-或式
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329102323986.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
#### 1. 典型卡诺图
$e.g.$ 二变量卡诺图的典型卡诺圈
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329102604912.png)

$e.g.$ 三变量卡诺图的典型卡诺圈
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329103940591.png)
$e.g.$ 四变量卡诺图的典型卡诺圈
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329131824969.png)
#### 2. 函数的卡诺图化简
无效圈示例1：不是矩形
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329132036534.png)

无效圈示例2：没有新变量，无效圈
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329132117974.png)

写出**每个卡诺圈**的**最小项表达式**：**保留**卡诺圈内**值不变的变量**。$F = AB + BC$：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329132410522.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
$F_1：F(A,B,C,D)= \sum m(0,2,3,5,6,8,9,10,11,12,13,14,15)$
<img src="https://img-blog.csdnimg.cn/20200329132724654.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329134653951.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
- 圈 $1$ ：**函数**的**与-或**表达式；
- 圈 $0$ ：**反函数**的**与-或**表达式(下面的左图：$\overline F=ABD$)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329134415808.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
- 圈 $1$ ：函数的**与-或**表达式；
- 圈 $0$ ：函数的**或-与**表达式（此时保持不变为0的变量用原变量，为1用反变量）




<img src="https://img-blog.csdnimg.cn/20200329134903359.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329142208162.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
需要注意的是：**不同的圈法，会得到不同的最简结果**：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329134758258.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 3. 含有任意项的逻辑函数的化简
#### 1. 任意项定义
一个逻辑函数，如果它的**某些输入取值组合**因**受特殊原因制约**而不会再现，或者虽然每种输入取值组合都可能出现，但此时函数取值为1还是为0无关紧要，那么这些**输入取值组合对应的最小项称为无关项或者任意项** 。 任意项用 “d”或者 “ × ” 表示 。

任意项可以加到函数表达式中，也可以不加到函数表达式中，并不影响函数的实际逻辑功能。其值可以取1，也可以取0。 

例1：十字路口红绿灯，设控制信号 $G=1 \rightarrow$ 绿灯亮；控制信号 $R=1 \rightarrow$ 红灯亮；则 $GR$ 可以为 $GR=00, 01,10$ ，但 $GR ≠ 11$ 。

例2：电动机正反转控制，设控制信号 $F=1 \rightarrow$ 正传；控制信号 $R=1 →$ 反转；则  $FR$ 可以为 $FR=00, 01, 10$ ，但 $FR ≠ 11$ 。

例3：$8421BCD$ 码中，从 $1010  ～ 1111$ 的六种编码不允许出现，可视为无关最小项。

#### 2. 带有任意项的逻辑函数的化简方法
例1： 给定某电路的逻辑函数真值表如右图，求 $F$ 的最简"与或"式：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329150038862.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329150309995.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)<img src="https://img-blog.csdnimg.cn/20200329150342198.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="39%">

例2：已知真值表如右图，用卡诺图化简：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020032915050462.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)<img src="https://img-blog.csdnimg.cn/20200329150527738.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">


## 四、练习
1.用代数法化简逻辑函数
$$\begin{aligned}
F &= AC + \overline BC + B\overline D + A(B + \overline C) + \overline AC\overline D + A\overline BDE\\
&= A(C + \overline C) + \overline BC + B\overline D + AB + \overline AC\overline D + A\overline BDE\\
&= A + \overline BC + B\overline D + \overline AC\overline D\\
&= A + (\overline A\ \overline BC\overline D + \overline A\ \overline BCD + A\overline BC\overline D + A\overline BCD) + (\overline AB\overline C\ \overline D + \overline ABC\overline D+AB\overline C\ \overline D+ABC\overline D) + (\overline ABC\overline D + \overline A\ \overline BC\overline D)\\
&= A + \overline BC + B\overline D
\end{aligned}
$$
2.用卡诺图化简一下函数：
（1）$F = BD + \overline AB\overline C + A\overline CD+ \overline ACD + \overline A\ \overline B$
答：根据下面的卡诺图，化简函数为 $F=\overline A\ \overline B + \overline A\ \overline C +B D + \overline CD$
<img src="https://img-blog.csdnimg.cn/20200329165019728.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">
（2）$F(A,B,C,D) = \sum m(0,1,2,4,5,8,9,10,12,13)$
答：根据卡诺图，化简函数为：$F = \overline C +\overline B\ \overline D$

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329211002112.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
（3）$F(A,B,C,D) = \sum m(0,1,2,9,12) + \sum d(4,6,10,11)$
答：根据下面的卡诺图，化简函数为：$F=\overline A\ \overline D +B \overline C\ \overline D +\overline B\ \overline CD$
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329211903323.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
3.将以下的函数分别化简为最简与或式和最简或与式。
$F(A,B,C,D) = \sum m(2,3,5,6,7,10,11,13,14,15)$
答：根据下图，可得最简与或式为：$F=C+BD$；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329212346159.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
根据下面的卡诺图，圈0得最简或与式为：$F=(C+D)(B+C)$
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200329213306222.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
