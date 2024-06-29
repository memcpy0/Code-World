@[toc] 

本章小结：
- 二种方法： 分析、设计方法
- 六种电路 ： 译码器 、数据选择 器

本章要求： 
- **熟练掌握**组合电路的分析方法和设计方法
- **熟练掌握**两种芯片的主要功能和基本应用：`74LS138`, `74LS151`
- 了解组合电路中的竞争与险象

---
# 一、常用组合逻辑电路
## 1. 译码器
**译码**： 将具有**特定含义**的**二进制代码**变换( (翻译) 成一定的输出信号，以表示二进制代码的**原意**，这一过程称为译码。实现译码功能的组合电路称为译码器。

译码是编码的**逆过程**，即将某个二进制代码翻译成电路的某种状态。常见的译码器有二进制译码器、二-十进制译码器、显示译码器。

 
### (1) 二进制译码器 74LS138(3/8译码器)
二进制译码器把输入的 $n$ 位二进制代码翻译为 $2^n$ 个输出的高低电平信号，其中**只有一个为有效电平**，其编号对应于输入的二进制代码。
 
 常见的**二进制**译码器有 `2-4` 译码器、`3-8` 译码器和 `4-16` 译码器。

**3线-8线译码器**见下图：
<img src="https://img-blog.csdnimg.cn/20200411010620695.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="35%">
<img src="https://img-blog.csdnimg.cn/20200411083833671.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="46%">

注意：译码器是**多输入、多输出**组合逻辑电路，==每个输出对应一个 $n$ 变量最小项——也称**最小项发生器**==。 当然，这里的符号和真值表只是一个简化的情形，详细的看后面。
#### a. 一般符号和图形符号
 <img src="https://img-blog.csdnimg.cn/20200411010700132.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="30%"><img src="https://img-blog.csdnimg.cn/20200411010706463.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="30%">
 
==注意，这里凡是有上划线的信号，都是低位有效的，比如 $ST_A$ 是高位有效的，$\overline {Y_0}$ 是低位有效的；同时，凡是有上划线的信号，输出的信号线都有三角符号或者是空心圆符号，同样表示低位有效。==
#### b. 74LS138功能表
下表有：
- $3$ 个输入端：$A,B,C$
- $8$ 个输出端：$\overline {Y_0} ... \overline {Y_7}$ (低电平有效)
- $3$ 个使能端：$ST_A, \overline {ST_B},\overline {ST_C}$
 <img src="https://img-blog.csdnimg.cn/2020041108460644.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="43%">
 
功能表中，要注意的是：==对于 $n=3$ 个输入信号，有 $2^n=8$ **个**输出信号，输出信号为 $0$ 表示有效，比如 $\overline {Y_1} = 0$ 表示 $A,B,C$ 二进制转换为十进制为 $1$。此外，输出信号有大量无效的状态，为了避免这些状态，就使用了使能端，其中 $ST_A$ 为 $0$ 时，全部输出为无效信号；$\overline {ST_B}+\overline {ST_C} = 1$ 或者说 $\overline {ST_B} = 1 \vee \overline {ST_C} = 1$ 时，全部输出为无效信号；只有 $ST_A=1$，且 $\overline {ST_B} + \overline {ST_C}=0$ 或者说 $\overline {ST_B}=0 \wedge \overline {ST_C} = 0$ 时，才有有效输出。==

#### c. 两片 `74LS138` 构成 `4-16` 译码器：
<img src="https://img-blog.csdnimg.cn/20200411010759577.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="55%">

$A_3A_2A_1A_0$ ：译码输入，对应的是 $16$ 个输出信号；

这里用最高位地址作为片选信号：
- $A_3 =0$ 时，片 $1$  低位输出工作，为了让片 $1$ 工作，我们将 $A_3$ **取反送入**片 $1$ 的 $ST_A$ 中，此时 $A_3 = 0$ 不会让高位输出工作。$A_3A_2A_1A_0$ ：$0000-0111$ ，有效输出产生于：$\overline {Y_0}- \overline{Y_7}$ 之中；
- $A_3 =1$ 时，片 $2$  高位输出工作，为了让片 $2$ 工作，我们可以将 $A_3$ **直接送入**片 $2$ 的 $ST_A$ 中，此时 $\overline {A_3} = 0$ 不会让低位输出工作。$A_3A_2A_1A_0$ ：$1000-1111$ ，有效输出产生于：$\overline{Y_8}-\overline {Y_{15}}$； 
 
#### d. 用 `74LS138` 实现函数 
二进制译码器的输出分别对应一个 **最小项** (高电平译码) 或一个 **最小
项的非** (低电平译码)，所以附加适当门，可实现任意函数。

特点 ：方法简单，无须简化，工作可靠。 

<img src="https://img-blog.csdnimg.cn/20200411010823187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="32%">

如上图，实现的逻辑函数如下：
$$\begin{aligned}
F&=\overline {\overline {Y_0}\ \overline{Y_3}\ \overline{Y_4}\ \overline{Y_7}} \\ &= Y_0 + Y3 +Y_4 + Y_7 \\ &= m_0 + m_3 + m_4 + m_7 \\ &= \sum {(0,3,4,7)}\end{aligned}$$


 
 #### e. 9片 `74LS138` 扩展构成6-64线译码器
用一片 `74LS138` 进行高 $3$ 位译码，$8$ 个输出分别用来控制 $8$ 片译码器进行低 $3$ 位译码。最后，将这 $8$ 片的共 $64$ 个输出作为总译码的输出结果。
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200412014004474.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
 
 ---
### (2) 二—十进制译码器 74LS42 
二—十进制译码器把输入的 $4$ 位 $BCD$ 码翻译为 $10$ 个输出的高低电平信号 ，其中**有一个为有效电平**， 其编号对应于输入的 $BCD$ 码。

看起来和前面的二进制译码器很像，要做好区分。

#### a. 一般符号 
<img src="https://img-blog.csdnimg.cn/20200411010830783.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="23%">

#### b. 74LS42功能表
<img src="https://img-blog.csdnimg.cn/20200411010850658.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">

功能表中，要注意的是：
|  | 74LS138二进制译码器 | 74LS42二-十进制译码器
|:-|:-|:-|
| 输入和输出 | $n$ 个输入信号，有 $2^n$ **个**输出信号  | $4$ 个输入信号，有 $10$ **个**输出信号，不是 $16$ 个，因为是 `BCD` 码，有些变量的组合不被使用
| 使能端 | 存在使能端 $ST_A,\overline{ST_B}, \overline{ST_C}$ ，只有 $ST_A=1$ 且 $\overline {ST_B} + \overline {ST_C}=0$ 时，才有有效输出 | 没有使能端
| 输出变量 | 低位有效 | 低位有效
| 举例 | $\overline {Y_1} = 0$ 表示 $A,B,C$ 二进制转换为十进制为 $1$ | $\overline {Y_1} = 0$ 表示 $A_3A_2A_1A_0$ `BCD`码转换为十进制为 $1$


### (3) 数字显示译码器 74LS48
在数字系统中，常需**把结果用十进制数码显示**出来，数字显示电路包括两部分——译码驱动电路和数码显示器。

举个例子，`8421BCD` 显示译码电路框图如下：
<img src="https://img-blog.csdnimg.cn/20200411010910573.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="45%">

我们要介绍的是七段显示译码器 `74LS48`。也就是七段数码管(每一段由一个发光二极管组成)，顺时针从 $a\rightarrow g$ 编码。
 <img src="https://img-blog.csdnimg.cn/20200411010930584.png" width="15%">
 
- 共阴极：高电平亮
- 共阳极：低电平亮

 
输入：二—十进制代码
输出：译码结果，可驱动相应的七段数码管显示正确的数字。


<img src="https://img-blog.csdnimg.cn/20200411010935967.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="23%">

#### a.  BCD七段字符显示译码器74LS48功能表

<img src="https://img-blog.csdnimg.cn/2020041101095663.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">
 
注意，这里的输出**不是低位有效的**，而且是**多位有效**。一个例子，要输出数字 $4$，需要 $b,c,f,g$ 段发光，因此，$Y_b,Y_c, Y_f, Y_g$ 都为 $1$，其他位为 $0$ 。


---
## 2. 数据选择器
定义：能从**多个数据信号**中**选择一个数据信号**传送到输出端的电路。
- 输入：$2^n$ 路数据和 $n$ 位地址，因此有 $2^n+n$ 个输入变量；
- 输出：$1$ 位数据；
- 地址：控制选择哪个数据的信号。
<img src="https://img-blog.csdnimg.cn/20200411011037126.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="30%">
 
数据选择器类似一个多掷开关。选择哪一路信号由相应的**一组控制信号**控制。

### (1) 4选1数据选择器
我们常用的是 $4$ 选 $1$ 数据选择器：
- 输入数据：$a_3,a_2,a_1,a_0$
- 控制地址：$A_1, A_0$
- 输出数据：$F$
- 功能：根据输入地址，选择输入数据中的一个，送到输出端

**真值表**：
| $A_1$ | $A_0$ | $F$
|:-:|:-:|:-:|
| $0$ | $0$ | $a_0$
| $0$ | $1$ | $a_1$
| $1$ | $0$ | $a_2$
| $1$ | $1$ | $a_3$

表达式： 
$$\begin{aligned}
F &= \overline {A_1}\ \overline {A_0} a_0 + \overline {A_1} {A_0} a_1 + {A_1}\ \overline {A_0} a_2 + A_1A_0a_3\\
&= m_0 \cdot a_0 + m_1 \cdot a_1 + m_2 \cdot a_2 + m_3 \cdot a_3\\
&= \sum_{i=0}^{2^n-1} m_i \cdot a_i \\
&= \sum_{i=0}^3 m_i\cdot a_i 
\end{aligned}
$$
 
$n$ 位地址变量，有 $2^n$ 个数据通道，实现 $2^n$ 选 $1$ 功能。
<img src="https://img-blog.csdnimg.cn/20200411011423663.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="40%">

### (2) 集成数据选择器: 8选1数据选择器74LS151
$8$ 选 $1$： $74LS151$
$16$ 选 $1$： $74LS150$
双 $4$ 选 $1$： $74LS153$
四 $2$ 选 $1$： $74LS157$，$74LS158$
<img src="https://img-blog.csdnimg.cn/20200411011451178.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="40%">

#### a. 74LS151功能表
$8$ 选 $1$ 数据选择器 $74LS151$ 的功能表

<img src="https://img-blog.csdnimg.cn/20200411011508879.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="37%">
 
 使能时，输出 $Y$ 和输入的逻辑关系：
 <img src="https://img-blog.csdnimg.cn/20200411171726634.png" width="30%">
 
#### b. 74LS151一般符号和电路
- 数据输入端：$D_7 \to D_0$；
- 地址端：$A_2 \to A_0$
- 输出端：$Y$，$\overline W$ 
 <img src="https://img-blog.csdnimg.cn/20200411012023964.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">


---
# 二、竞争和险象
## 1. 基本概念
理想情况：
- 逻辑门连线**无延迟**
- 多个信号**同时瞬间变化**

实际情况：
- 信号变化：过渡时间
- 信号通过逻辑门： 响应时间
- 多个信号变化：有先有后

**竞争**：信号经不同路径到达某一点时，**所用的时间不同**，这个时间差称为竞争；
**险象**：由竞争引起电路输出发生**瞬间错误**的现象，表现为输出端出现了原设计中没有的窄脉冲(毛刺)，称为险象。

> 一般来说，时延对数字系统是有害的，它会降低系统的工作速度，还会产生竞争冒险现象。
> 竞争和险象是对电路的，而不是针对函数的。


## 2. 险象分类
依据输入信号**变化前后**输出
信号的**变化情况**，分为：
- 静态险象：本应**不变**而发生了**变化**；
- 动态险象：本应**一次变化**而发生了**多次变化**

依据导致输出信号发生变化
的**输入变量个数**，分为：
- 逻辑险象：**一个输入变量**发生变化导致的险象
- 功能险象：**多个输入变量**发生变化导致的险象
 
两两组合起来，就是 $4$ 种险象。我们主要关注的是静态逻辑险象！

此外，根据输出的错误，还分为：
- $0$ 型险象： 产生**低电平**错误
- $1$ 型险象： 产生**高电平**错误

可以继续依次细分为：
<img src="https://img-blog.csdnimg.cn/20200411012051839.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="45%">
### (1) 静态功能险象 

 产生的原因：
- 多个输入变量的值不可能严格地“同时”变化

产生的条件：
- **$K$ 个 ($K>1$) 输入信号**同时发生变化
- 输入信号变化前、后的**稳态输出值相同**
- 变化的 $K$ 个变量的取值组合，对应在卡诺图上所占有的 $2^K$ 个方格中，必定既有 $1$ ，又有 $0$

 
$e.g.$ 分析逻辑函数 $F=B\overline C+AC$ ，说明当输入信号 $ABC$ 由 $010$ 变化到 $111$ 时，是否有险象发生。
分析：
输入信号 $A$ 和 $C$ 发生变化，从 $00$ 变为 $11$ ； $B=1$ ，$A$ 和 $C$ 发生变化的 $4$ 个最小项了中既有 $0$ 也有 $1$，可能发生险象。
<img src="https://img-blog.csdnimg.cn/20200411012131569.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="42%">
<img src="https://img-blog.csdnimg.cn/20200411012158134.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="45%">
 
**功能险象**是**逻辑函数**的功能所**固有**的，无法通过改变设计来消除，只能通过**控制输入信号的变化顺序**来避免。
### (2) 静态逻辑险象
产生的原因：逻辑**器件**固有的时延；
产生的条件：
-  一对逻辑变量 $A$ 和反变量 $\overline A$ 同时出现，且在某些取值条件下，逻辑表达式可写成 $F = A\cdot \overline A$ 或者 $F=A+\overline A$
- 一个输入变量 $A$ 发生变化
-  输入变量**发生变化前、后**的**稳态输出值相同**
 
$e.g.$ $F=B\overline C+ AC$.
分析：当 $A=B=1$ 时，$F=C+\overline C$. 当 $C$ 由 $1$ 变为 $0$ ，会出现险象。 
 <img src="https://img-blog.csdnimg.cn/20200411134402688.png" width="25%">
 

## 3. 险象的判别
### (1) 逻辑表达式判别法
如果电路中**存在出现险象的可能性**，则其逻辑表达式有如下特点：
- 当某一变量**同时以原变量和反变量的形式**出现在逻辑表达式中，则该变量就具备了竞争的条件；
- 保留被研究变量，用某些定值**消去其它变量**；
- 若得到的表达式为下列形式之一，则有险象存在：
$F = A + \overline A$：$0$ 险象（如 $A$ 从 1 → 0）
$F = A \cdot \overline A$：$1$ 险象（如 $A$ 从 0 → 1）
 
$e.g.$ $F=(A+B+\overline C)(C+D)(\overline B+\overline D)$.
分析：式中变量 $B,C , D$ 均以原变量、反变量形式出现在表达式中，具备竞争条件：
- 当 $ABC=010$ 或 $110$ 时，表达式为 $F= D\cdot \overline D$，如果 $D$ 从 $0→1$，则存在1险象。
- 当 $ABD=000$ 时，表达式为 $F=C\cdot \overline C$ ，如果 $C$ 从 $0→1$，则存在 $1$ 险象。
- 当 $ACD=011$ 时，表达式为 $F = B\cdot \overline B$， 如果 $B$ 从 $0→1$，则存在 $1$ 险象。

### (2) 卡诺图判别法
对于“与-或”电路：在卡诺图中，如果两个圈 $1$ 的卡诺圈存在着**部分相切** ，且这个相切部分又**没有被其它的圈 $1$ 卡诺圈包含**，则该电路**必然存在险象**：
$F= \overline A\ \overline C + AB$：
 <img src="https://img-blog.csdnimg.cn/2020041101225654.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">  
 
对于“或-与”电路： 在卡诺图中，如果两个圈 $0$ 的卡诺圈存在着**部分相切**，且这个相切部分又**没有被其它的圈 $0$ 卡诺圈包含**，则该电路**必然存在险象**：
$F = (A+C)(B+\overline C)$：
 <img src="https://img-blog.csdnimg.cn/20200411012328189.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%"> 
 
## 4. 险象的消除
### (1) 加选通脉冲
对下面的电路图，
(1)  先使 $CP ＝ 0$ ，**关闭与门**；
(2)  等 $A$ 、$\overline A$ 信号都来到后，让 $CP ＝ 1$，得到可靠的 $F$
<img src="https://img-blog.csdnimg.cn/20200411012341181.png" width="34%">
<img src="https://img-blog.csdnimg.cn/20200411012402420.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="35%">
 
### (2) 加冗余卡诺圈 
在相切部分加上冗余的卡诺圈：
$\quad F = A\overline B + BC \qquad  \qquad \ \ \ F = (A+B)(\overline B +C)$
<img src="https://img-blog.csdnimg.cn/20200411012423316.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">
$\quad F = A\overline B + BC +AC \quad  \quad F = (A+B)(\overline B +C)(A+C)$

