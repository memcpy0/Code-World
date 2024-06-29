@[toc]

 

# 一、计数器
## 1. 计数器概念和分类
计数： 累计输入脉冲的个数构成：
- $1$ 个触发器有 $2$ 个状态，可计 $2$ 个数( $1$ 位二进制数)
- $n$ 个触发器有 $2^n$ 个状态，可计 $2^n$ 个数( $n$ 位二进制数)

应用：**分频、定时**、产生节拍脉冲和脉冲序列、进行数字运算等.

分类如下：
按数制分：
- 二进制计数器
- 十进制计数器
- $N$ 进制(任意进制)

计数器按计数方式分：
- 加法计数器
- 减法计数器
- 可逆计数 (`Up-Down Counter`，既能够做加法也能够做减法)

按时钟控制分：
- 同步计数器 (`Synchronous `)
- 异步计数器 (`Asynchronous`)

我们的重点在于同步集成计数器。同步计数器的特点：在同步计数器内部， **各个触发器都受同一时钟脉冲**——**输入计数脉冲**(计数就是**统计累计输入脉冲的个数**)的控制， 因此， 它们**状态的更新几乎是同时**的， 故被称为 “同步计数器”。

下面就将介绍常用的同步计数器。

## 2. 双时钟可逆十六进制计数器74LS193
### (1) 74LS193的引脚示意图和逻辑功能图
双时钟是指存在两个时钟信号，可逆指的是既可以做加法计数，也可以做减法计数。
<img src="https://img-blog.csdnimg.cn/20200514162634533.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="52%">
 
为了使用74LS193，我们必须掌握其引脚的功能：
- `Vcc` ：外接电源；
- `GND` ：接地；
- $CP_D$ ：减法(down)脉冲输入信号；
- $CP_U$ ：加法(up)脉冲输入信号；
- $\overline {CO}$ ：进位输出端，carry output，==多个74LS193级联的时候有用处==；
- $\overline {BO}$ ：借位输出端，borrow output，==多个74LS193级联的时候有用处==；
- $D_3D_2D_1D_0$ ：外部输入数据，用于异步置数；
- $Q_3Q_2Q_1Q_0$ ：计数器输出数据；
- $CR$ ：异步清零端；
- $\overline {LD}$ ：异步置数控制端；

### (2) 74LS193 的逻辑功能表
<img src="https://img-blog.csdnimg.cn/20200514163859618.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">

- $CR=1$ 时，**异步清零**，计数器输出数据 $Q_3Q_2Q_1Q_0$ 为 $0000$；
- $CR=0，\overline {LD}=0$时，**异步置数** $Q_3Q_2Q_1Q_0$ 为 $D_3D_2D_1D_0$ ；
- $CR=0，\overline {LD}=1$，既不清零也不置数，此时如果 $CP_{U} \uparrow$ 且 $CP_D = 0$ 时，**加 $1$ 计数**；当加法计数到达 `1111` 时，进位输出 $\overline {CO}$ 输出一个脉宽等于 $CP_{U}$ 的低电平部分的低电平脉冲；当下一个 $CP_{U}$ 上升沿到来时，$\overline {CO}$ 变为高电平，由此产生一个 $\overline {CO}$ 的上升沿；多个74LS193级联时，正好将低位的  $\overline {CO}$ 端与高位的 $CP_{U}$ 端连接起来；
- $CR=0，\overline {LD}=1$，既不清零也不置数，此时如果 $CP_D \uparrow$ 且 $CP_U = 0$ 时，**减 $1$ 计数**；当减法计数到达 `0000` 时，借位输出 $\overline {BO}$ 输出一个脉宽等于 $CP_{U}$ 的低电平部分的低电平脉冲；当下一个 $CP_{D}$ 上升沿到来时，$\overline {BO}$ 变为高电平，由此产生一个 $\overline {BO}$ 的上升沿；多个74LS193级联时，正好将低位的  $\overline {BO}$ 端与高位的 $CP_{D}$ 端连接起来；
-  $CR=0，\overline {LD}=1$，且 $CP_D,CP_U$ 均无变化时，**状态保持**。

---
## 3. 用集成计数器实现任意进制计数
若已有 $N$ 进制计数器，现在要实现 $M$ 进制计数器：
 - `M < N` ：反馈清零法或反馈置数法
- `M > N` ：多个芯片级联

### (1) 反馈清零法和反馈置数法 (M < N)
在 $N$ 进制计数器的顺序计数过程中， 若设法使之跳过 $N - M$ 个状态 ， 就可以得
到 $M$ 进制计数器 ， 其方法有清零法(复位法)和置数法(置位法)。注意，两种方法的使用是有条件的。

#### a. 反馈清零法
**清零法** 适用于**有清零输入端**(`异步` 或 `同步`）的计数器。例如74LS193。

异步清零法和同步清零法存在一些差异，但基本思想都是：计数器从全 $0$ 状态 $S_0$ 开始计数，计满 $M$ 个状态后产生清零信号，使计数器恢复到初态 $S_0$ ，然后再重复上述过程。
<img src="https://img-blog.csdnimg.cn/20200514165822828.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="25%">

**反馈清零法-异步清零**： $S_M$ 状态进行译码产生置零信号并反馈到异步清零端，74LS193中是 $CR$ ，使计数器立即返回 $S_0$ 状态。$S_M$ 状态只在极短的瞬间出现，通常称它为“暂态”。 
<img src="https://img-blog.csdnimg.cn/20200514180705314.png" width="25%">
步骤：
(1) 写出状态 $S_M$ 的二进制代码；
(2) 求归零逻辑：$S_M$ 中所有 $1$ 对应的端口求与(**异步清0端高电平有效**)或者是求与-非(低电平有效)；
(1) 画出电路连线图。

**反馈清零法-同步清零**：当计数器处于 $S_{M-1}$ 状态时，产生同步清零信号，在下一个时钟脉冲使芯片转换到零状态。
<img src="https://img-blog.csdnimg.cn/20200514181024847.png" width="22%">
步骤：
(1) 写出状态 $S_{M-1}$ 的二进制代码；
(2) 求归零逻辑：$S_{M-1}$ 中所有 $1$ 对应的端口求与(**同步清0端高电平有效**)或者是求与-非(低电平有效)；
(3) 画出电路连线图

例. 用74LS193构成一个十二进制计数器。
答：74LS193是十六进制计数器，要构成十二进制计数器，可采用反馈清零法。**利用异步清零端即可**。
步骤：
(1) 写出S M 的二进制编码：$S_M = S_{12} =1100$
(2) 求归零逻辑：$CR = Q^n_3Q^n_2$
(3) 画出电路图：
<img src="https://img-blog.csdnimg.cn/20200514194909155.png" width="25%"><img src="https://img-blog.csdnimg.cn/20200514195122327.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="42%">

#### b. 反馈置数法
清 $0$ 法中，计数器每次都是从全 $0$ 状态 $S_0$ 开始计数；
置数法可以通过预置功能使计数器从某个预置状态 $S_i$ 开始计数， 计满 $M$ 个状态后产生置数信号，使计数器又进入预置状态 $S_i$ ， 然后再重复上述过程。
<img src="https://img-blog.csdnimg.cn/20200514195715550.png" width="22%">
**反馈置数法-异步置数**：在暂态 $S_{i+M}$ 产生置数信号，并马上将预置数—— $i$ 的二进制代码，通过**异步置数端**置于计数器；适合74LS193；

**反馈置数法-同步置数**：在状态 $S_{i+M-1}$ 产生置数信号，要等下一个 `CP` 到来时，才将预置数置入计数器，故无暂态。置数完成后，计数器从被置入的状态重新开始计数。

例. 采用反馈置数法，用74LS193构成一个十进制计数器。
分析：74LS193是异步置数计数器，共有16个状态，要构成十进制计数器，可从中选取10个连续的状态。选定十个状态 $Q_3 Q_2 Q_1 Q_0$ 为：$0011→1100$ 。
- 写出反馈态 $S_{i+M}$ 的二进制代码， 即 $S_{i+M} =1101$；
- 求置数逻辑：$\overline {LD} = \overline {Q_3 Q_2 Q_0}$；
- 预置态 $S_i =0011$， 画出电路图：
<img src="https://img-blog.csdnimg.cn/20200514200103767.png" width="22%">

### (2) 多个芯片级联(M>N)
若要求实现的计数器计数值 $M$ 超过单片计数器的计数范围， 则必须将多片计数器级联。
#### a. 分解法
若 $M$ 可分解为 $M = M_1 × M_2 × … M_n$ ， 则用 $n$ 片计数器分别组
成 $M_1$, $M_2$ … $M_n$ 进制计数器 ， 然后再将它们 **级联** 而成。

级联方法：
- 并行进位法：所有芯片共用一个时钟信号，**低位芯片的进位输出**控制相邻高位芯片的**使能端**(同步方式)；
- 串行进位法：**低位芯片的进位输出**作为相邻高位芯片的`CP`(异步方式)；适合74LS193.
 
#### b. 扩展法
先将 $n$ 片计数器 **级联** 组成最大计数值 $N＞M$的计数器 ， 然后采用 **整体清0** 或 **整体置数** 的方法实现模 $M$ 计数器 。
 
例. 用74LS193设计一个 $256$ 进制的计数器。
分析：74LS193是 $16$ 进制计数器，两片74LS193级联可以实现 $256$ 进制计数器。
<img src="https://img-blog.csdnimg.cn/20200514201817262.png" width="36%">
当低位计数器计数为 $1111$ 时，低位计数器的进位输出端产生一个**低电平脉冲**信号；当下一个时钟脉冲上升沿到来时，低位计数器的进位输出端变为**高电平值**，相当于高位计数器的时钟端有一个**上升沿**，高位计数器计数增加 $1$ 。

---
# 二、寄存器
## 1. 寄存器概念和分类
和计数器一样，寄存器也是时序逻辑电路的最常见的应用之一。

寄存：把二进制数据或代码**暂时存储**；
功能特点：暂存数据或代码，一般**不对存储内容进行处理**；
结构特点：通常由 $D$ 触发器构成；
分类：
- 基本寄存器（数码寄存器）
- 移位寄存器

输入输出方式：
- 并行方式：每一位数据对应一个输入端/输出端，在 $CP$ 作用下，各位**同时输入/输出**；
- 串行方式：只有一个输入端/输出端，$CP$ 作用下，各数码**逐位输入/输出**。

## 2. 基本寄存器结构和功能
<img src="https://img-blog.csdnimg.cn/20200514170824815.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="36%">
<img src="https://img-blog.csdnimg.cn/20200514202118603.png" width="50%">

右图是具有**异步清零端**的基本寄存器。

功能说明：
- 当 $\overline {CR} = 0$ 时，**寄存器清零**：$Q^{n+1}_3Q^{n+1}_2Q^{n+1}_1Q^{n+1}_0 = 0000$；
- 当 $\overline {CR} = 1$ 时，时钟脉冲 `CP` 上升沿到来，**寄存器置数**。加在**并行数据输入端**的数据 $D_3$ ~ $D_0$ ，就立即被送到触发器输出端，$Q^{n+1}_3Q^{n+1}_2Q^{n+1}_1Q^{n+1}_0 = D_3D_2D_1D_0$ ；
- 当 $\overline {CR} = 1$ 时，除了时钟上升沿之外的其他时间，寄存器的状态保持不变。

## 3. 集成的基本寄存器
### (1) 由多个边沿D触发器组成的集成寄存器
下图是 $4$ 位上升沿集成触发器74LS175：
<img src="https://img-blog.csdnimg.cn/20200514202838183.png" width="20%">

引脚说明：
- $D_0$～$D_3$ ：并行数据输入端
- $Q_0$～$Q_3$ ：并行数据输出端
- $\overline {R_D}$ ：异步清零控制端

74LS175集成触发器，上升沿时送数，异步清零：
<img src="https://img-blog.csdnimg.cn/202005142031586.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="35%">

### (2) 具有输入使能功能的锁存型寄存器
<img src="https://img-blog.csdnimg.cn/20200514203511936.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="45%"> 

上图的集成寄存器在时钟上升沿锁存数据，其他时间保存数据；可以异步清零，可以使能。功能表如下：
<img src="https://img-blog.csdnimg.cn/20200514203651918.png" width="45%"> 

### (3) 具有输出缓冲功能的寄存器
<img src="https://img-blog.csdnimg.cn/20200514204445223.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="45%"> 

当 $\overline {OE}=0$ 时，**锁存**在触发器的数据才会传到输出端。

---
## 4. 移位寄存器
移位： 将寄存器所存储的各位数据，在每个移位脉冲的作用下， 向左或向右移动一位。根据移位的方向，分成：左移、右移和双向移位。

### (1) 单向移位寄存器
<img src="https://img-blog.csdnimg.cn/20200514204721944.png" width="43%"> 

- $D_I$ ：**串行**数据输入端；
- $D_O$ ：**串行**数据输出端；
- $CP$ ：时钟端；
- $Q_0$  ~ $Q_3$ ：**并行**数据输出端；
- $\overline R$ ：异步清零端

触发器的驱动方程：$D_0=D_I，D_1 =Q_0^n，D_2 =Q_1^n，D_3 =Q_2^n$；
代入 $D$ 触发器的特征方程：$Q^{n+1}  = D$ ；
得到：$Q_0^{n+1} =D_I，Q_1 ^{n+1} =Q_0^n，Q_2^{n+1} =Q_1^{n} ，Q_3^{n+1} =Q_2^n$
 
### (2) 双向移位寄存器
<img src="https://img-blog.csdnimg.cn/20200515015647273.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="43%"> 

- 移位控制端：$M$
- 左移串行输入端：$D_{SL}$
- 右移串行输入端：$D_{SR}$
 
次态方程：
$$
\left \{
\begin{aligned}
&Q^{n+1}_0 = \overline {M}D_{SR} + MQ^n_1\\
&Q^{n+1}_1 = \overline MQ^n_0 + MQ^n_2\\
&Q^{n+1}_2 = \overline MQ^n_1 + MQ^n_3\\
&Q^{n+1}_3 = \overline MQ^n_2 + MD_{SL}
\end{aligned}
\right.
$$

$M=0$ 时，右移，$Q_0^{n+1} = D_{SR}，Q_1 ^{n+1} =Q_0^n，Q_2^{n+1} =Q_1^{n} ，Q_3^{n+1} =Q_2^n$；
$M=1$ 时，左移，$Q_0^{n+1} = Q^{n}_1，Q_1^{n+1} = Q_2^n，Q_2^{n+1} =Q_3^n，Q_3^{n+1} = D_{SL}$；

### (3) 4位双向移位寄存器74LS194
<img src="https://img-blog.csdnimg.cn/20200515021010314.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="45%"> 

74LS194的逻辑功能表如下：
<img src="https://img-blog.csdnimg.cn/20200515021125480.png" width="48%"> 
例. 利用两片集成移位寄存器74LS194扩展成一个 $8$ 位移位寄存器
分析如下：
- 74LS194受统一的时钟脉冲控制，$8$ 位移位寄存器也应该同步运行；
- 74LS194的清零端应该一起接受控制；
- 工作方式控制段也应该统一被控制；
- 为了左移，必须将右边那片74LS194的 $Q_4$ 指向左边74LS194的 $D_{SL}$ 输入端；同理可以右移：
<img src="https://img-blog.csdnimg.cn/20200515122852161.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="44%"> 

### (4) 移位寄存器的应用——实现数码串/并转换
**串/并转换**是指将**串行输入的数据**，经转换电路之后变成**并行输出**，常用于**计算机通信中的数据接收方**。 
<img src="https://img-blog.csdnimg.cn/20200514223254741.png" width="48%">

Step1：$\overline {CR}$ 端加低电平，异步清零，$Q_7 = 0，M_1M_0 = 11$ ，寄存器处于**置数**工作状态；
<img src="https://img-blog.csdnimg.cn/2020051422384466.png" width="50%">
Step2：$\overline {CR}$ 端加高电平，当第一个 `CP` **上升沿**到来时，输出端：$Q_0Q_1 Q _2 Q_3 Q_4 Q_5 Q_6 Q_7 =01111111$ ，$Q_7 =1，M_1 M_0 =01$ ，寄存器处于**串行右移**工作方式；

Step3：当第二个 `CP` 上升沿到来时，**输出端数据**右移 $1$ 位：$Q_0 Q_1 Q_2 Q_3 Q_4 Q_5 Q_6 Q_7 =D_0 0111111$ ，$Q_7 =1，M_1 M_0 =01$ ，寄存器处于**串行右移**工作方式；
 
Step4：在后续的 $6$ 个 `CP` 上升沿到来时，输出端数据依次右移 $1$ 位：$Q_0 Q _1 Q_2 Q_3 Q_4 Q_5 Q_6 Q_7 =D_6 D_5 D_4 D_3 D_2 D_1 D_0 0$ ，此时，**串行输入的数据已被转换成并行输出**。


数据串/ 并转换电路的逻辑功能表：
<img src="https://img-blog.csdnimg.cn/20200514222949769.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="52%">

---
# 三、多谐震荡电路(补充)
## 1. 多谐振荡器
多谐振荡器是一种**自激**振荡器，在接通电源后，**不需要外加触发信号**，便能自动产生矩形脉冲。多谐振荡器在工作过程中没有稳定状态，故称为**无稳态电路**。

## 2. 555定时器
### (1) 电路结构
<img src="https://img-blog.csdnimg.cn/20200514205848707.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="40%">

### (2) 工作原理 
a. $\overline {R_D} = 0$ 时，三极管导通：
<img src="https://img-blog.csdnimg.cn/20200514210033733.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="38%">

b. $\overline {R_D} = 1，TH > {2\over 3} V_{CC}，\overline {TR} > {1\over 3} V_{CC}$时，$\overline R = 0，\overline S = 1$，三极管导通：
<img src="https://img-blog.csdnimg.cn/20200514210111448.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="38%">

c. $\overline {R_D} = 1，TH < {2\over 3} V_{CC}，\overline {TR} < {1\over 3} V_{CC}$时，$\overline R = 1，\overline S = 0$，三极管截止：
<img src="https://img-blog.csdnimg.cn/20200514210420428.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="38%">

d. $\overline {R_D} = 1，TH < {2\over 3} V_{CC}，\overline {TR} > {1\over 3} V_{CC}$时，$\overline R = 1，\overline S = 1$，三极管保持：
<img src="https://img-blog.csdnimg.cn/20200514210456322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="38%">

e. $\overline {R_D} = 1，TH > {2\over 3} V_{CC}，\overline {TR} < {1\over 3} V_{CC}$时，$\overline R = 0，\overline S = 0$，无意义。

### (3) 工作原理 
功能表总结如下：
<img src="https://img-blog.csdnimg.cn/20200514210517520.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="38%">

---
## 3. 用555定时器构成多谐振荡器
### (1) 电路结构
 <img src="https://img-blog.csdnimg.cn/2020051422275575.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="47%">
 
### (2) 工作原理
a. 第一暂稳态。接通 $V_{CC}$  后：
开始时 $TH = \overline {TR} = u_C \approx 0$ ，$u_O$ 为高电平，放电管截止，$V_{CC}$  经 $R_1,R_2$ 向 $C$  充电 ，$u_C$ 上升，这时电路 处于暂稳态 $I$ 。
 <img src="https://img-blog.csdnimg.cn/20200515125631136.png" width="22%">

b. 第二暂稳态。当 $u_C$ 上升到 $TH = \overline {TR}  = u_C ≥ {2 \over 3} V_{CC}$  时，$u_O$ 跃变为低电平，同时放电管 $T$ 导通，$C$ 经 $R_2$ 和 $T$ 放电，$u_C$ 下降，电路进入暂稳态 $Ⅱ$ 。
 <img src="https://img-blog.csdnimg.cn/20200515125646183.png" width="22%">
  
c. 第三暂稳态。当 $u_C$ 下降到：$TH = \overline {TR}  = u_C \le {1 \over 3} V_{CC}$  时，$u_O$ 重新跃变为高电平，同时放电管 $T$ 截止，$C$ 又被充电，$u_C$ 上升，电路又返回到暂稳态 $Ⅰ$ 。
 <img src="https://img-blog.csdnimg.cn/20200515125707621.png" width="22%"> 

### (3) 振荡频率计算 
电容 $C$  如此循环充电和放电，使电路产生振荡，输出矩形脉冲。
 <img src="https://img-blog.csdnimg.cn/20200515125728751.png" width="22%"> 
$t_{PH} \approx 0.7(R_1 +R_2)C$
$t_{PL} \approx 0.7R_2C$
周期：$T = t_{PH} + t_{PL} = 0.7(R_1+2R_2)C$
频率：$$f = {1 \over {(t_{PL} + t_{PH})}} \approx {1.43 \over (R_1 +2R_2)C}$$
占空比：$$q = {t_{PH} \over T} = {{R_1+R_2}\over {R_1 +2R_2}}$$

