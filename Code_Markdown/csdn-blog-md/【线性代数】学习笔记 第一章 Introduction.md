@[toc]

沉浸式线性代数的作者是：`J. Ström`, `K. Åström` , and `T. Akenine-Möller` 。
[Immersive linear algebra](immersivemath.com/ila/index.html)
[线性代数就该这么学]



---
# Preface (A few words about this book.)
序言部分讲述一些关于这本书的话：
> ( `A picture says more than a thousand words` ) “一图胜千言”是一个常见的谚语。对于教科书而言，通常一个图形或一张插图也可以代替大量的单词。不过我们还相信，一张**交互式插图**可以说明更多，这就是为什么我们决定围绕这样的插图，建立属于我们自己的线性代数书。我们相信这些图片将使人们更容易、更快地消化和学习线性代数（这对于其他许多数学书籍也是如此）。此外，我们在书中增加了一些新的功能（例如，**弹出有关常用线性代数术语的窗口**），我们相信这些功能也会使阅读和理解变得更容易、更快。 
> <b></b>
> 我们准备写一本线性代数书，我们认为这将使学习和教授线性代数变得更加容易。此外，移动设备和网络浏览器的技术已经超过了一定的门槛，所以这本书可以以一种非常新颖和创新的方式（我们认为）来组合在一起。我们的想法是，在每一章的开头都有一个直观的具体例子，通过交互式插图实际展示数学是如何工作的。之后将引入更为形式化的数学，概念被概括，有时变得更抽象。我们相信，在每一章的开头用一个简单而具体的例子，把线性代数的整个主题牢牢地固定在读者的脑海中，这样就更容易理解线性代数的整个主题。
 
 
 
# Chapter 1: Introduction 简介
> How to navigate, notation, and a recap of some math that we think you already know.

这一章很短，主要是介绍本书的一些基本符号 `basic notation` ，回顾一些用于其他部分的数学内容，不会作为教学材料，也不会解释相关公式（可以阅读有关三角学 `Trigonometry` 的书）。读者可以尽快从第二章开始实际的数学内容。
## 1.1 Brief Note on Notation
一个实数 $n \in \R$ ，在本书中常被称为一个标量 `scalar` 或者标量值 `scalar value` 。它们常用小写字母表示，比如 $j, s, t, a, m$ ，还可能有下标，比如 $k_1$，$k_2$，$k_3$  。如果写成 $\sum^3_{i=1}k_i$ 则表示从 $k_1$ 加到 $k_3$ 。

一个标量 $k$ 的绝对值写成 $|k|$ ，定义如下：
$$|k| =\begin{cases}
 k, \quad &if\ k \ge 0,  \\
-k, \quad &if\ k \lt 0
\end{cases}
$$

诸如此类的简单内容等等……
## 1.2 Some Trigonometry 
这一节将介绍一些用在本书中的三角学知识。

`cosine, sine, tangent` 

---
# Chapter 2: Vectors
The concept of a vector is introduced, and we learn how to **add and subtract vectors**, and more.
![在这里插入图片描述](https://img-blog.csdnimg.cn/834780a6645c452e9672f23e84e012b0.png)


---
# Chapter 3: The Dot Product

A powerful tool that takes two vectors and produces a scalar.

---
# Chapter 4: The Vector Product

In three-dimensional spaces you can produce a vector from two other vectors using this tool.

---
# Chapter 5: Gaussian Elimination

 A way to solve systems of linear equations.


 

---
# Chapter 6: The Matrix

Enter the matrix.


矩阵是线性代数中的核心内容，矩阵的运算及理论贯穿线性代数的始终。矩阵是一个数表；矩阵不仅能描述事物间的相互联系，也能进行相关的运算。

## 1. 矩阵
由 $m \times n$ 个数 $a_{ij}\ (i = 1, 2, \dots, m,\ j = 1, 2, \dots, n)$ 排成的 $m$ 行 $n$ 列矩形表：
$$|A| = 
\begin{vmatrix}
a_{11}\quad a_{12}\quad  \dots  \quad a_{1n} \\ 
a_{21}\quad  a_{22}\quad   \dots \quad a_{2n} \\
\vdots  \qquad  \vdots  \qquad \ \ \qquad \vdots \\
a_{m1}\quad  a_{m2} \quad  \dots  \quad a_{mn} \\
\end{vmatrix} 
$$
称为 $m$ 行 $n$ 列的**矩阵**。简记为 $A = (a_{ij})_{m\times n}$ ，其中 $a_{ij}$ 称为 $A$ 的第 $i$ 行第 $j$ 列**元素**。当 $m = n$ 时，称 $A$ 为**方阵**，或简称为 $n$ 阶矩阵。

$R^{m\times n}$ 为全实数元素 $m\times n$ 矩阵的全体（集合）。特别地 $1\times n$ 的矩阵称为**向量**，记为 $R^n$ 。

> 矩阵和行列式的区别如下：
> - 矩阵是一个表，行数和列数可以不等；行列式是一个数，行数和列数必须相等（**方阵才有行列式**）
> - 两个矩阵相等是指矩阵中对应元素相等；两个行列式相等不一定对应元素相等，甚至阶数都可以不相等；
> - 两个矩阵相加是将其对应元素相加；两个行列式相加，在某种特殊情况下（比如有 $n - 1$ 行相同）只能将一行（列）对应元素相加，其余元素照写
> - 数乘矩阵是指数乘矩阵的每一个元素；数乘行列式，只能用该数乘行列式的某一行（列）
> - 矩阵经过初等变换，其秩不变；行列式经过初等变换（这是行列式的性质），其值可能要变，如 $A = (a_{ij})_{n\times n},\ A\to$ 

## 2. 几种特殊矩阵
（1）零矩阵
（2）单位矩阵
（3）对角矩阵
（4）数量矩阵

## 3. 逆矩阵
（1）逆矩阵
若有矩阵 $B$ 使 $AB = BA = I$ ，则称矩阵 $A$ 可逆（又称非奇异、满秩），且称矩阵 $B$ 为 $A$ 的逆矩阵，记为 $A^{-1}$ 。
（2）逆矩阵的性质：
- $(A^{-1})^{-1} = A$ 
- $(A^T)^{-1} = (A^{-1})^T$
- $(AB)^{-1} = B^{-1}A^{-1}$ 
- $|A^{-1}| = \frac{1}{|A|}$ 
- $(\alpha A)^{-1} = \frac{1}{\alpha} A^{-1}$（$\alpha$ 为非零常数）

（3）矩阵可逆的充要条件
$n$ 阶方阵 $A$ 可逆 $\Leftrightarrow AB = I$ 或 $BA = I$ 
$\Leftrightarrow |A| \ne 0$（$A$ 非奇异）
$\Leftrightarrow r(A) = n$（$A$ 满秩）
$\Leftrightarrow A$ 的行（列）向量线性无关
$\Leftrightarrow Ax = 0$ 方程组只有零解
$\Leftrightarrow A$ 经过初等变换化为单位矩阵
$\Leftrightarrow A$ 可以表示成有限个初等方阵的乘积
$\Leftrightarrow A$ 的特征值全不为零
$\Leftrightarrow Ax = \beta$ 对任意 $\beta$ 有唯一解
注：$A$ 可逆、$A$ 非奇异、$A$ 是满秩这几种说法是一致的



---
# Chapter 7: Determinants

A fundamental property of square matrices.

> 行列式是一个重要的数学工具，在线性代数中应用较多，**要正确理解 $n$ 阶行列式的概念，掌握行列式的性质**，熟练使用行列式的性质及其展开，计算3阶、4阶和简单的 $n$ 阶行列式。

## 1. 行列式的定义
行列式的定义众多，较为直接的（构造性的）定义是：
$$|A| = 
\begin{vmatrix}
a_{11}\quad a_{12}\quad  \dots  \quad a_{1n} \\ 
a_{21}\quad  a_{22}\quad   \dots \quad a_{2n} \\
\vdots  \qquad  \vdots  \qquad \ \ \qquad \vdots \\
a_{n1}\quad  a_{n2} \quad  \dots  \quad a_{nn} \\
\end{vmatrix}
= \sum_{(i_1i_2\dots i_n)} (-1)^{\tau(i_1i_2\dots i_n)} a_{1i_1} a_{2i_2} \dots a_{ni_n}
$$

这里的 $(i_1 i_2\dots i_n)$ 是数字 $1,2,\dots, n$ 的任一排列，$\tau(i_1i_2\dots i_n)$ 为排列 $(i_1i_2\dots i_n)$ 的逆序数。

矩阵（方阵）$A$ 的行列式常记为 $detA$ 或简记为 $|A|$ 或 $|a_{ij}|$ 。

## 2. 行列式的性质
（1）行列式中行、列互换（行变列、列变行），其值不变，即 $|A| = |A^T|$ ，$|A^T|$ 表示 $A$ 的转置；
（2）交换行列式中两行（或两列）的位置，行列式的值变号；
（3）某数乘以行列式中一行（或列）的诸元素，等于该数乘以行列式的值；（从面积理解）
（4）将某行（或列）的倍数加到另一行（或列），行列式的值不变；
（5）行列式中若两行（或列）的对应元素成比例，则行列式的值为零。（从面积理解）

## 3. 拉普拉斯定理 `Laplace Theorem` 拉普拉斯展开

如何证明行列式的拉普拉斯定理？ - 不改名更不了答案的回答 - 知乎
https://www.zhihu.com/question/313725710/answer/697867199![在这里插入图片描述](https://img-blog.csdnimg.cn/cf5d3d9f6d0142f0b15deda12d69d203.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/5e1b5d4dd669448194a73176186ddd97.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)![在这里插入图片描述](https://img-blog.csdnimg.cn/e1d3b6984ec2426987a6bf00b821e011.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)![在这里插入图片描述](https://img-blog.csdnimg.cn/1f2d612338eb4627a874b4fed071072c.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/14b742ff7e03435581b889940967b3d3.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)


一个行列式按指定的k行展开，指的是：先找出这k行的全部k阶子式，然后让这些k阶子式都乘以它们的代数余子式，再将所有的这些乘积求和，最后这个和就等于原来那个行列式。这就是行列式按k行进行拉普拉斯展开的定义。

行列式可以按某一行（或列）展开，且：
$$\sum^n_{k=1} a_{ik} A_{jk} = \delta_{ij} |A|$$
其中 $\delta_{ij}$ 称为 `Kronecker` 符号：
$$\delta _{ij} = \begin{cases}
1,\ i = j, \\
0,\ i \ne j.
\end{cases}\quad  (1 \le i, j \le n).
$$
特别地，$|A| = \sum^n_{k=1} a_{ik} A_{ik}$（$i=1,2,\dots, n$）

## 4. 行列式计算方法归类
- 用行列式的定义计算行列式。一般用于**零元素较多时**的行列式计算。
- 用行列式的性质计算行列式。一般将行列式化为（上/下/次）**三角形行列式**的形式。
- 用行列式的性质及 **展开定理（降阶法）** 计算行列式。一般将行列式的某一行（列）化- 为**零元素较多**，再将其展开达到降阶的效果。
- 用递推法计算行列式。适用于按某一行或某一列展开后，得到 **与原行列式模式相同（但阶数低一阶）** 的行列式。
- 数学归纳法。适用同递推法，但要合理使用**第一归纳法**与**第二归纳法**。
- 用范德蒙行列式计算。
- 加边法（升阶法）。
- 分析线性因子法。

## 5. 特殊行列式
### 5.1 上/下三角行列式等于其主对角线上元素的乘积
$$
\begin{vmatrix}
a_{11}\qquad a_{12}\qquad  \dots  \qquad a_{1n} \\ 
0 \qquad \ a_{22}\qquad  \dots \qquad a_{2n} \\
\vdots  \qquad  \vdots  \qquad  \qquad \vdots \\
0\qquad  0 \qquad  \dots  \qquad a_{nn} \\
\end{vmatrix}
$$


---
# Chapter 8: Rank

Discover the behaviour of matrices.

---
# Chapter 9: Linear Mappings

Learn to harness the power of linearity...

---
# Chapter 10: Eigenvalues and Eigenvectors
## 1. 特征的特征向量
### (1) 特征值与特征向量
若 $A$ 是 $n$ 阶矩阵，$\lambda_0$ 是数，若存在非零列向量 $\alpha$ 使得 $A\alpha = \lambda_0\alpha$ ，则称 $\lambda_0$ 为 $A$ 的一个**特征值**/根，$\alpha$ 称为 $A$ 相对于 $\lambda_0$ 的**特征向量**

### (2) 特征多项式和特征方程
多项式 $f(\lambda) = | \lambda I - A|$ 称为 $A$ 的**特征多项式**，$f(\lambda) = 0$ 称为**特征方程**。

### (3) 矩阵化为对角型
若矩阵 $A$ 有 $n$ 个线性无关的特征向量 $x_1, x_2, \dots, x_n$ ，又记为 $x = (x_1, x_2, \dots, x_n)$ ，则 $x^{-1} Ax  = diag\{\lambda_1, \lambda_2, \dots, \lambda_n\}$ 
其中 $\lambda_i\ (i = 1, 2, \dots, n)$ 为 $A$ 相应于 $x_i$ 的特征值
注：结论的条件是充要的。

### (4) *凯莱-哈密顿定理 `Cayley-Hlamilton` 
若 $A\in R^{n\times n},\ f(\lambda) = det(A - \lambda I)$ ，则 $f(A) = O$ 。

## 2. 特征值的性质
（1）任意 $n$ 阶矩阵 $A$ 与 $A^T$ 具有相同的特征值
（2）若 $\lambda$ 是可逆阵 $A$ 的特征值，则 $\frac{1}{\lambda}$ 是 $A^{-1}$ 的特征值，也是 $\frac{|A|}{\lambda} A^*$ 的特征值。
（3）若 $\lambda$ 是 $A$ 的特征值，则 $\lambda^m$ 是 $A^m$ 的特征值，$k\lambda$ 是 $kA$ 的特征值。
（4）若 $\lambda$ 是 $A$ 的特征值，则对任意多项式 $\varphi(x)$ ，$\varphi(\lambda)$ 是 $\varphi(A)$ 的特征值。

## 3. 特征向量的性质
（1）属于不同特征值的特征向量线性无关；
（2）属于同一特征值的向量的线性组合仍属于该特征值的特征向量？
（3）属于不同特征值的特征向量 $\alpha, \beta$ 之和 $\alpha + \beta$ 不是其特征向量

矩阵 $A = (a_{ij})_{m\times n}$ 是正交矩阵的充要条件 $\delta_{ii}$ 称为 `Kronecker` 记号

This chapter has a value in itself.

# 线性空间
## 1. 线性空间
定义了**加法**与**数乘**的集合 $L$ ，对 $\forall a, b \in L$ 及数 $\forall \lambda, \mu \in R$ 均有：
（1）$a + b \in L, \lambda a \in L$ ，即**对加法和数乘封闭**；
（2）$a + b = b + a$ ，即**加法满足交换律**；（加法中）有零元 $0$ ；（加法中）$a$ 有负元 $-a$
（3）$(\lambda\mu) a = \lambda(\mu a)$ ；$1a = a$ 有幺元 $1$ 
（4）$(\lambda + \mu) a = \lambda a + \mu a$ ，$\lambda (a + b) = \lambda a + \lambda b$ 
则称 $L$ 为线性空间。

具体的，线性空间包括：$n$ 维向量空间；$m\times n$ 阶矩阵空间，函数空间等。

因此，判断一个集合 $V$ 是否构成线性空间：
1. 验证集合对元素的加法及数乘运算封闭，即 $\forall a, b \in V \Rightarrow a + b \in V$ ，$\forall  a \in V,\ \forall \lambda \in R \Rightarrow \lambda a \in V$ ；
2. 验证 $V$ 中的运算是否满足**八条运算规律**，若全部满足，则 $V$ 是线性空间

若要否定一个集合是线性空间，只需指出一条不成立即可。



## 2. 基与坐标
若向量空间 $L$ 中的任何向量 $x$ 均可以用 $L$ 中一组**线性无关向量** $\alpha_1, \alpha_2, \dots, \alpha_n$ 的**线性组合表示**，即：
$$x = \lambda_1 \alpha_1 +  \lambda_2 \alpha_2 + \dots +  \lambda_n \alpha_n = \sum^n_{i = 1} \lambda_i a_i$$

则称 $\alpha_1, \alpha_2, \dots, \alpha_n$ 为 $L$ 的**一组基** `basis` ，而 $(\lambda_1, \lambda_2, \dots, \lambda_n)$ 称为 $x$ 关于**基** $(\alpha_1, \alpha_2, \dots, \alpha_n)$ 的**坐标**。

$e_1 = (1, 0, \dots, 0), e_2 = (0, 1, 0, \dots, 0), \dots, e_n = (0, \dots, 0, 1)$ 称为 $n$ 维向量空间的**标准基**。

一般地，对于 $L$ 的一组基 $\alpha_1, \alpha_2, \dots, \alpha_n$ ，若满足 $(a_i, a_j) = \delta_{ij} (1 \le i, j\le n)$ ，称该基为**标准正交基**。其中 $\delta_{ij}$ 是 `Kronecker` 符号，当 $i = j$ 时 $\delta_{ij} = 1$ ，当 $i \ne j$ 时 $\delta_{ij} = 0$ 。


