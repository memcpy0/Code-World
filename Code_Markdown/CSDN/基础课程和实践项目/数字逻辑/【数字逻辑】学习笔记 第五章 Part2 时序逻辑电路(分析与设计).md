@[toc]


# 一、时序逻辑电路分析 
## 1. 同步时序逻辑电路分析   
### (1) 分析目的
分析时序逻辑电路在输入信号的作用下，其状态和输出信号**变化的规律**，进而确定电路的**逻辑功能**。

### (2) 分析方法
利用**驱动方程**、**次态方程**和**输出方程** ， 求出在任何给定的**输入变量状态和存储电路状态**下时序电路的**输出和次态**

- 驱动方程：触发器**输入端**的逻辑表达式
- 次态方程：将驱动方程代入对应**触发器特征方程**得到的逻辑表达式
- 输出方程 ：时序电路**输出信号**的逻辑表达式

### (3) 分析步骤
1.了解电路的组成：电路的输入、输出信号、触发器的类型等

2.根据给定的时序电路图，写出下列各逻辑方程式：
-  各触发器的**驱动方程**
-  各触发器的**次态方程**
-  时序电路的**输出方程**

3.列出状态转换表或**画出状态图**和波形图

4.确定电路的逻辑功能

---
## 2. 例题A
例1：分析以下电路的逻辑功能：
<div align="center">
<img src="https://img-blog.csdnimg.cn/20200425102901933.png" width="40%">

### (1) 写方程组 
 - 输出方程：$z = Q^n_3$
 - 驱动方程：
 	$$\left \{ 
 	\begin{aligned}
 	&J_1 = \overline {Q_3}, K_1= 1\\
 	&J_2 = Q^n_1, K_2 = Q_1\\
 	&J_3 = Q^n_1Q^n_2, K_3 = 1
	\end{aligned}
	\right.
	$$
- 次态方程：这是下降沿触发的J-K触发器，因此 $$Q^{n+1} = J \overline {Q^n} + \overline k Q^n$$

从而，可以得到以下的次态方程组：
$$\left \{
\begin{aligned}
&Q^{n+1}_1 = \overline {Q^n_3}\ \overline {Q^n_1} \\
&Q^{n+1}_2 = Q^n_1\overline {Q^n_2} + \overline {Q^n_1}Q^n_2 = Q^n_1 \oplus Q^n_2\\
&Q^{n+1}_{3} = Q^n_1Q^n_2\overline {Q^n_3}
\end{aligned}
\right.
$$
### (2) 根据方程组列出状态转移真值表 
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425105549818.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="43%">
 
### (3) 根据状态转移真值表画状态转移图 
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425110015714.png" width="35%">

### (4) 根据状态转移图画时序图 
<div align="center"> <img src="https://img-blog.csdnimg.cn/20200425110132183.png" width="35%">

### (5) 检查电路能否重启动
检查电路能否从 **非工作状态** （未出现在状态转移真值表中的状态） **进入工作状态**

非工作状态：$101,110,111$
分别代入次态方程组和输出方程： 
<div align="center"> <img src="https://img-blog.csdnimg.cn/20200425110402709.png" width="24%">

电路可以由非工作状态进入工作状态，故可以自启！
 
### (6) 画出完整的状态图 
<div align="center"> <img src="https://img-blog.csdnimg.cn/20200425110709524.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="35%">

### (7) 判断电路功能
由于时钟脉冲控制状态从 $000-001-010-011-100-000$，循环变换，逢五进一，可知，该电路是一个同步的五进制计数器

## 3. 例题B
分析以下电路的逻辑功能：
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425110918681.png" width="45%">

### (1) 写方程组
这也是下降沿触发的J-K触发器。
- 输出方程：$Z = (X \oplus Q^n_2) \cdot \overline {Q^n_1}$
- 驱动方程：
$$\left \{
\begin{aligned}
J_1 = X\oplus \overline {Q^n_2}, K_1 = 1\\
J_2 = X \oplus Q^n_1, K_2 = 1
\end{aligned}
\right.
$$
- 次态方程：$Q^{n+1} = J\overline {Q^n} + \overline K Q^n$

可得次态方程组：
$$\left \{
\begin{aligned}
Q^{n+1}_1 = (X \oplus \overline {Q^n_2}) \overline {Q^n_1} \\
Q^{n+1}_2 = (X \oplus Q^n_1)\overline {Q^n_2}
\end{aligned}
\right.
$$
 
### (2) 列状态转移真值表
① 当 $X=0$ 时，化简方程：
$$\left \{
\begin{aligned}
&Q^{n+1}_1 = (X \oplus \overline {Q^n_2}) \overline {Q^n_1}=\overline {Q^n_2}\ \overline {Q^n_1} \\
&Q^{n+1}_2 = (X \oplus Q^n_1)\overline {Q^n_2} = Q^n_1 \overline {Q^n_2}\\
&Z = Q^n_2 \overline {Q^n_1}
\end{aligned}
\right.
$$ 
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425131657880.png" width="35%"></img></div>

② 当 $X=1$ 时，化简方程：
$$\left \{
\begin{aligned}
&Q^{n+1}_1 = (X \oplus \overline {Q^n_2}) \overline {Q^n_1}= {Q^n_2}\ \overline {Q^n_1} \\
&Q^{n+1}_2 = (X \oplus Q^n_1)\overline {Q^n_2} = \overline{Q^n_1}\ \overline {Q^n_2}\\
&Z = \overline {Q^n_2}\ \overline {Q^n_1}
\end{aligned}
\right.
$$  
 <div align="center"><img src="https://img-blog.csdnimg.cn/20200425131716477.png" width="35%"></img></div> 
 
### (3) 画状态转移图
 
$X =0$ 时的状态转移图：
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425132958483.png" width="35%">

$X =1$ 时的状态转移图
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425133309747.png" width="35%">
 
### (4) 画时序图
 $Z = (X \oplus Q^n_2) \cdot \overline {Q^n_1}$ 
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425133448343.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="35%">

### (5) 检查是否能自启
非工作状态：$11$；分别代入次态方程组和输出方程，可得：
<div align="center"> <img src="https://img-blog.csdnimg.cn/20200425134030736.png" width="28%">

该电路能够自启。  
### (6) 画出完整的状态转换图
<div align="center"> <img src="https://img-blog.csdnimg.cn/2020042513414086.png" width="30%">
 
### (7) 判断电路功能
该电路共有三个状态：$00,01,10$
该电路是一个**加减可控**的三进制计数器

当时钟信号下降沿到来时：
- 若 $X=0$， 按照加 $1$ 规律从 $00→01→10→00$ 循环变化，并每当转换为 $10$ 状态(最大数)时， 输出 $Z=1$；
- $X=1$ 时， 按照减 $1$ 规律从
 $10→01→00→10$ 循环变化，并每当转换为 $00$ 状态(最小数)时， 输出 $Z=1$
 
  
 ---
## 4. 异步时序逻辑电路分析
### (1) 异步时序逻辑电路的特点
-  电路无统一的时钟信号；
-  **除了时钟脉冲外，没有其它输入信号**， 由时钟脉冲，**直接引起**电路状态的变化
-  由次态逻辑产生各触发器的驱动信号及时钟信号

### (2) 分析步骤
1.了解电路的组成：电路的输入、输出信号、触发器的类型等；

2.根据给定的时序电路图, 写出下列各逻辑方程式：
- 各触发器的**时钟方程**
-  各触发器的**驱动方程**
-  时序电路的**输出方程**
-  各触发器的**次态方程**

3.列出状态转换表、画出状态图和波形图
4.确定电路的逻辑功能

### (3) 注意
1.第一个触发器：每一次状态转换必须从输入信号所能触发的**第一个触发器**开始**逐级确定**；
2.是否出现**触发沿**。逐个分析各触发器的时钟信号是否出现触发沿：
-  出现触发沿，求出该触发器的次态
-  未出现触发沿，记为 `CP_n =0` ，该触发器保持原有状态

3.时间延迟
各个触发器的状态转换**存在一定的延迟**。只有当**全部触发器状态转换完毕**，电路才进入新的“稳定”状态，即次态


--- 
## 5. 例题C
分析以下电路的逻辑功能： 
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425143554813.png" width="35%">

### (1) 写方程组
电路基本情况：
• 采用两个**上升沿触发**的 $D$ 触发器
• **时钟脉冲信号作为输入信号**，不存在其他的输入信号
• 时钟 `CP` 的上升沿触发 $FF_0$
• $Q_0$ 由 $0→1$ 时触发 $FF_1$ ，要根据 $Q_0$ 的变化情况，确定是否更新 $Q_1$ 的次态

时钟方程：
$$\left \{
\begin{aligned}
&CP_0 = CP\uparrow \\
&CP_1 = Q_0 \uparrow
\end{aligned}
\right.
$$
 
驱动方程组：
$$\left \{
\begin{aligned}
&D_0 = \overline {Q_0^n}   \\
&D_1 =\overline {Q_1^n}  
\end{aligned}
\right.
$$

输出方程：$Z = \overline {Q^n_0 + Q^n_1} = \overline {Q^n_0}\ \overline {Q^n_1}$
 
 由于 $Q^{n+1} = D$ ，次态方程组：
$$\left \{
\begin{aligned}
&Q_0^{n+1} = D_0 = \overline {Q_0^n} \cdot CP_0  \\
&Q_1^{n+1} = D_1 = \overline {Q_1^n}  \cdot CP_1
\end{aligned}
\right.
$$ 

计算过程：
- 将一组现态值带入次态方程组，计算出 $Q_0^{n+1}$
- 根据 $Q^n_0 \rightarrow Q_0^{n+1}$ 的变化情况，即时钟 $CP_1$ 的状态，确定是否计算更新 $Q_1^{n+1}$ (时钟信号变化了)
- 若 $Q^n_0 \rightarrow Q_0^{n+1} : 0 \rightarrow 1$ ，则计算更新 $Q_1^{n+1}$ ，否则  $Q_1^{n+1} = Q^n_1$

### (2) 根据方程组列出状态转移真值表
只有在 $Q_0$ 处于上升沿时，$Q_1$ 才会变化： 
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425150930593.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="49%">
  
### (3) 根据状态转移真值表画出状态转移图
 <img src="https://img-blog.csdnimg.cn/20200425151700387.png" width="45%">

### (4) 画出系统时序图
<div align="center">  <img src="https://img-blog.csdnimg.cn/20200425151713606.png" width="43%">
  
### (5) 逻辑功能分析
该电路共有四个状态，在**时钟脉冲**作用下，按照减 $1$ 规律循环变化： $00→ 11→ 10→ 01→00$ ，是一个四进制减法计数器。其中， $Z$ 是**借位信号**。
 

接下来就是时序逻辑电路的设计问题了。

---
# 二、时序逻辑电路的设计
时序逻辑电路的设计相对来说很有难度，这里只要求掌握同步时序电路的设计。

## 1. 同步时序逻辑电路的设计
同步时序逻辑电路的**设计是分析的逆过程**，其任务是根据实际逻辑问题的要求，设计出能实现给定逻辑功能的电路。
 
### (1) 同步时序逻辑电路设计的一般步骤
① 逻辑抽象：建立原始状态图和状态表；② 状态化简；③ 状态分配；
④ 选择触发器类型；⑤ 确定驱动方程组和输出方程；⑥ 检查自启动能力；⑦ 画出逻辑电路图

其中，状态化简和状态分配不要求掌握。

### (2) 逻辑抽象
-  确定输入、输出变量及**电路的状态数**
-  定义输入、输出逻辑状态和电路状态的**含义**
-  按题意建立原始状态转换图和状态转移真值表

#### a. 例1
设计一个**五进制可逆计数器**。当输入 $x=0$ 时，进行加 $1$ 计数；当 $x=1$ 时，进行减 $1$ 计数。

(1) 确定输入、输出变量及电路**状态的数量**
- 输入变量：$1$ 个
- 输出变量：$1$ 个
- 电路状态数量：$5$ 个
 
(2) 定义输入、输出变量及电路状态的**含义**
- 输入变量：$x$ —— 控制电路的功能选择
  $x=0$：加 $1$ 计数
  $x=1$：减 $1$ 计数
- 输出变量：$y$ —— 指示电路处于**初始状态**
  $y=1$：电路处于 $S_0$ 状态
  $y=0$：电路处于其它状态
- 电路状态： $S_0-S_4$ —— 五进制计数器的 $5$ 种状态

(3)  列出状态转移真值表，画出状态转移图：
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425154005923.png" width="22%">

红色箭头：加1计数状态转换顺序；黑色箭头：减1计数状态转换顺序.
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425154505667.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="33%">
 
#### b. 例2 
设计一个串行数据检测器，在连续输入四个或四个以上的 $1$ 时输出 $1$ ，否则输出 $0$ 。

(1) 确定输入、输出变量及电路状态分析
- 输入变量：$1$ 个，表示输入数值串
- 输出变量：$1$ 个，表示检测结果
- 电路状态：
   $S_0$ ：未输入 $1$
   $S_1$ ：输入一个 $1$
   $S_2$ ：连续输入两个 $1$
   $S_3$ ：连续输入三个 $1$
   $S_4$ ：连续输入四个或四个以上的 $1$
   
(2) 画出状态转移图
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425154857867.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="35%">  

(3) 列出状态转移表
<div align="center"> <img src="https://img-blog.csdnimg.cn/20200425155033825.png" width="45%">  
 
### (3) 状态化简和分配
合并等效状态、 消去多余状态的过程，利用状态与状态间的等效关系来进行状态简化。

> 状态等效 ：
在**完全确定的状态表**中，若对所有可能的输入序列 ，分别从某两个状态(假设为 $S_1$ 与 $S_2$ )出发 ，所得到的输出序列都相同，且状态转移效果相同，则说这两个状态( $S_1$ 与 $S_2$ )等效。记做：( $S_1$ ，$S_2$ )。此时 $S_1$ 与 $S_2$ 可以合并为一个状态。


等效的传递性：若有状态 $S_1$ 和 $S_2$ 等效，状态 $S_2$ 和 $S_3$ 等效，则状态 $S_1$ 和 $S_3$ 也等效，记为：($S_1$ , $S_2$)，($S_2$ , $S_3$） → ($S_1$ , $S_3$)

等效类：所含状态都可以互相构成等效对的等效状态集合，即：
$(S_1,S_2 ,S_3)$ → $(S_1, S_2)$ $(S_2, S_3)$ $(S_1, S_3)$
 $(S_1, S_2)$ $(S_2, S_3)$ $(S_1, S_3)$ → $(S_1,S_2 ,S_3)$ 

最大等效类：一个原始状态表中，不能被其它等效类所包含的等效类.

 
状态分配就是给最小化状态表中的每个状态分配一个二进制代码；
电路的状态是触发器状态的组合。如果电路有 $N$ 个状态，$2^{n-1} <N\le {2^n}$  ，则可用 $n$ 个触发器来实现.

### (4) 确定驱动方程组和输出方程
在确定了触发器类型的条件下，再确定驱动方程组。最常用的触发器是 $D$ 触发器，其次是 $J-K$ 触发器和 $T$ 触发器。

由状态转移表得到驱动方程组和输出方程的过程：
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425164829574.png" width="48%">

#### c. 例3 
分别用 `D` 触发器、`J-K` 触发器和 `T` 触发器实现下表所示的逻辑功能，试求对应的驱动方程和输出方程。
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425165435433.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="45%">

(1) 绘制触发器**次态卡诺图**和**输出卡诺图**
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425165838572.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="45%">
 
(2) 求出次态方程和输出方程
$$\left \{
\begin{aligned}
&Q^{n+1}_{2} = \overline X\ \overline {Q^n_2} + X\overline {Q^n_1}\\
&Q^{n+1}_1 = XQ^n_2 + X\overline {Q_1^n} + Q^n_2\overline {Q^n_1}
\end{aligned}
\right.
$$
输出方程：$Z = XQ^n_2Q^n_1$

(3) 确定驱动方程组和输出方程(将次态方程代入触发器的特征方程中)
- 若选用 `D` 触发器，$Q_{n+1} =D$。对比，得到
$$\left \{
\begin{aligned}
&D_{2} = \overline X\ \overline {Q^n_2} + X\overline {Q^n_1}\\
&D_1 = XQ^n_2 + X\overline {Q_1^n} + Q^n_2\overline {Q^n_1}
\end{aligned}
\right.
$$ 
- 若选用 `J-K` 触发器，$Q^{n+1} = J\overline {Q^n} + \overline K Q^n$ ，可得：
$$\begin{aligned}
Q^{n+1}_{2} &= \overline X\ \overline {Q^n_2} + X\overline {Q^n_1}\\
&= \overline X\ \overline {Q^n_2} + X\overline {Q^n_1}(\overline {Q^n_2} + Q^n_2)\\
&= (\overline X + X \overline {Q^n_1}) \overline {Q^n_2} + X \overline {Q^n_1} Q^n_2 \\
&= (\overline X + \overline {Q^n_1}) \overline {Q^n_2}  + X\overline {Q^n_1} Q^n_2\\
Q^{n+1}_{1} &=  X {Q^n_2} + X\overline {Q^n_1} + {Q^n_2}\overline {Q^n_1}\\
&=  X   {Q^n_2} ( {Q^n_1} + \overline {Q^n_1})+ X\overline {Q^n_1} + {Q^n_2}\overline {Q^n_1}\\
&= (X +{Q^n_2} +  X {Q^n_2}) \overline {Q^n_1} + X   {Q^n_2} Q^n_1 \\
&= ( X + {Q^n_2}) \overline {Q^n_1}  + X  {Q^n_1} Q^n_2
\end{aligned}
$$
 从而：
 $$\left \{
 \begin{aligned}
J_2 &= \overline X + \overline {Q^n_1}\\
K_2 &= \overline {X  \overline {Q^n_1}} =  \overline  X  + {Q^n_1}   \\ 
\end{aligned}
\right.\\
\left \{
 \begin{aligned}
J_1 &=X + {Q^n_2}\\
K_1 &= \overline {X   {Q^n_2}} =  \overline  X  + \overline{Q^n_2}   \\ 
\end{aligned}
\right.
$$

- 若选用 `T` 触发器，$Q^{n+1} = T\overline {Q^n} + \overline T{Q^n}$ ，这个就有一定难度了。为了求出驱动方程，我们分析如下：  
 <img src="https://img-blog.csdnimg.cn/20200425204240152.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="30%">
T触发器要发生状态翻转，必须满足输入端信号 $T=1$，若选用T触发器假设两个T触发器的输入端信号分别为T1和T2，由状态转移表画T1和T2的卡诺图：
 <img src="https://img-blog.csdnimg.cn/20200425204506840.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="45%">  <img src="https://img-blog.csdnimg.cn/20200425204637852.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="45%">
   <img src="https://img-blog.csdnimg.cn/20200425205023845.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="33%">
对T1和T2的卡诺图进行化简，得到T1和T2的逻辑表达式：
 $$\left \{
 \begin{aligned}
T_2 &= \overline X + \overline  {Q^n_1} \ \overline {Q^n_2} +Q^n_2Q^n_1\\
T_1 &= \overline {X}  {Q^n_1} +   X\overline{Q^n_2}  + Q^n_2\overline {Q^n_1}   \\ 
\end{aligned}
\right.\\ 
$$


###  (5) 检查自启动功能
当所设计的电路中，**触发器能表示的状态数大于有效状态数时**，需要对所设计的电路进行实际工作状态讨论。

讨论两个问题 ：
(1)  电路万一进入无用状态，能否在时钟脉冲作用下进入有效状态，即**是否具有自恢复功能和自启动功能**。
(2)  电路万一处在无用状态，**是否会产生错误信号**。

一但出现以上两个问题都必须修改电路设计，否则将影响电路工作的可靠性和正确性。

#### d. 例4
用 `J-K` 触发器设计一个五进制同步计数器， 要求状态转移关系如右图所示。
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425184144881.png" width="30%"> 
解：

(1) 逻辑抽象，建立**原始状态图和状态表** 。
本例属于给定状态的时序电路设计问题，不存在逻辑抽象的问题，可以直接建立状态转移真值表。 
<div align="center"><img src="https://img-blog.csdnimg.cn/2020042520014026.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="49%"> 

(2) 选择触发器， 确定驱动方程和输出方程。根据状态转移真值表，画出各个次态的卡诺图。
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425200413113.png" width="50%">

若选用J-K触发器， 则由**次态卡诺图**求出其**状态方程**，再求出**驱动方程**：
$$\left \{
\begin{aligned}
&Q^{n+1}_{2} =Q_1^n \overline {Q^n_2} + \overline {Q^n_1}Q^n_2\\
&Q^{n+1}_1 = Q^n_0 \overline {Q_1^n} \\
&Q^{n+1}_0 = \overline {Q^n_2} = \overline {Q^n_2}\ \overline {Q^n_0} + \overline {Q^n_2} Q^n_0
\end{aligned}
\right.
\left \{
\begin{aligned}
&J_{2} =Q_1^n, K_2 = {Q^n_1}\\
&J_1 = Q^n_0, K_1  = 1\\
&J_0 = \overline {Q^n_2}, K_0 =   {Q^n_2} 
\end{aligned}
\right.
$$  
 
(3) 自启动检查：根据以上次态方程， 检查多余状态的转移情况。
状态转移表：
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425201325718.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="33%">

状态转移图：
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425201331415.png" width="24%">

**存在孤立状态，因此不能自启**。通过修改电路设计，可以避免不能自启动的问题。解决方法：**改变卡诺图的圈法**。如果希望能尽量使用任意项，那么只能对图( a )和图( c )的圈法作修改。 现选择对图( c )的圈法作修改，即仅改变 $Q_0$ 的转移，改进后的 $Q_0$ 卡诺图：
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425203322444.png" width="20%"> 

$$\left \{
\begin{aligned}
&Q^{n+1}_{2} =Q_1^n \overline {Q^n_2} + \overline {Q^n_1}Q^n_2\\
&Q^{n+1}_1 = Q^n_0 \overline {Q_1^n} \\
&Q^{n+1}_0 = \overline {Q^n_1}\ \overline {Q^n_0} + \overline {Q^n_2} Q^n_0
\end{aligned}
\right.
\left \{
\begin{aligned}
&J_{2} =Q_1^n, K_2 = {Q^n_1}\\
&J_1 = Q^n_0, K_1  = 1\\
&J_0 = \overline {Q^n_1}, K_0 =   {Q^n_2} 
\end{aligned}
\right.
$$   
能够自启动的状态转移图：
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425203549998.png" width="21%"> 
 
(4) 画出电路图
<div align="center"><img src="https://img-blog.csdnimg.cn/20200425201411713.png" width="30%"> 
 
---
# 三、练习题目
<img src="https://img-blog.csdnimg.cn/2020042814504139.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="80%">

这里题目出现了一定的错误，应该是异步置 $0$ 端 $\overline {R_D}$，异步置 $1$ 端 $\overline {S_D}$ 。当 $\overline {S_D} = 1, \overline {R_D} = 0$ 时，$Q = 0$；当 $\overline {S_D} = 0, \overline {R_D} = 1$ 时，$Q = 1$ 。因此 $Q$ 的波形图如下：
<img src="https://img-blog.csdnimg.cn/20200502164711249.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">

看了答案，将答案更新如下。有的地方我没有写完整，有的地方我和答案不太一样，但是应该是对的。
<img src="https://img-blog.csdnimg.cn/20200505112650748.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">


<img src="https://img-blog.csdnimg.cn/20200428145906987.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">   
<img src="https://img-blog.csdnimg.cn/20200502165317663.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%"> 

上面没有检查自启动：
<img src="https://img-blog.csdnimg.cn/20200505113144691.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%"><img src="https://img-blog.csdnimg.cn/2020050511321849.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="62%"> 
 
$17.$ 用 $J-K$ 触发器和门电路设计一个七进制加法计数器。 
<img src="https://img-blog.csdnimg.cn/20200502165034335.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%"><img src="https://img-blog.csdnimg.cn/20200502164359868.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

<img src="https://img-blog.csdnimg.cn/20200505113542591.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="64%">
<img src="https://img-blog.csdnimg.cn/20200505113558972.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="62%">

