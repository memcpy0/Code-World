
> 本文属于「算法学习」系列文章的汇总目录。之前的「数据结构和算法设计」系列着重于基础的数据结构和算法设计课程的学习，与之不同的是，这一系列主要用来记录大学课程范围之外的**高级算法学习、优化与应用的全过程**，同时也将归纳总结出简洁明了的算法模板，以便记忆和运用。在本系列学习文章中，为了透彻理解算法和代码，本人参考了诸多博客、教程、文档、书籍等资料，由于精力有限，恕不能一一列出，这里只列示重要资料的不完全参考列表：
> - 算法导论 第三版
> - 算法竞赛进阶指南，李煜东著，河南电子音像出版社，[GitHub Tedukuri社区](https://github.com/lydrainbowcat/tedukuri)以及[个人题解文章汇总目录](https://memcpy0.blog.csdn.net/article/details/121280121)
> - 算法 第四版 *Algorithm Fourth Edition*，[美] `Robert Sedgewick, Kevin Wayne` 著，谢路云译，[配套网站](https://algs4.cs.princeton.edu/)
> - 数据结构（C语言版）、数据结构题集（C语言版）严蔚敏，清华大学出版社
> - 算法 第四版
> - 数据结构（C++）邓俊辉
>  <b></b>
> 为了方便在PC上运行调试、分享代码，我还建立了相关的[仓库](https://github.com/memcpy0/Algorithm-Templates)。在这一仓库中，你可以看到算法文章、模板代码、应用题目等等。由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏本文以作备忘。

着重于基础的数据结构和算法设计课程的学习

@[toc]
  
> 为了方便在PC上运行调试、分享代码，我还建立了相关的仓库：[https://github.com/memcpy0/Data-Structure-and-Algorithm.git](https://github.com/memcpy0/Data-Structure-and-Algorithm.git)。 

 
相信许多朋友或多或少遇到过这种情况，使用过的很多算法，如果不及时将学习过程文字化，过上一段时间就会遗忘关键细节，带来诸多不便。为此，我在博客上写过一些介绍数据结构和算法的文章……然后就有两个问题了！这些文章零零散散地分布，有时我都不大记得是否写过相关文章，有时又想把某些文章合并或者删除……但是查找和修改起来都有点麻烦……**出于优化文章管理、抵抗遗忘噩梦的目的，我创建了这一目录，持续收录和更新这些算法学习文章及其链接**。

 
---
# 零、内容概述
数据结构课程的内容框架如下所示，主要包括**基础数据结构**和**应用数据结构**，基础数据结构包括**线性结构**（线性表、栈和队列、串）和**非线性结构**（数组、广义表、树和二叉树、图），应用数据结构包括查找、排序（内部排序和外部排序）、文件~~和动态存储管理~~（可能放在操作系统中介绍）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/ab27fbafbf9041d28e9a8150204b14ed.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)



---
# 一. 数据结构学习
## 1. 绪论
绪论主要介绍以下内容：
- 什么是数据结构
- 基本概念和术语
- 抽象数据类型的表示、实现
- 算法、算法设计要求、算法效率分析（可能放到算法设计中介绍）

具体内容见此处：[【数据结构和算法设计】数据结构学习(1) 绪论]
## 2. 线性表
具体内容见此处：[【数据结构和算法设计】数据结构学习(2) 线性表]
## 3. 栈和队列
## 4. 串
## 5. 数组、广义表
## 6. 树和二叉树
## 7. 图
## 8. 查找
## 9. 内部排序
## 10. 外部排序
## 11. 文件
# 2. 数据结构实现
【数据结构和算法设计】动态数组实现：C++/Java
https://editor.csdn.net/md/?articleId=104541515

【数据结构Java实现】单链表【最基本的动态数据结构】
https://editor.csdn.net/md/?articleId=104794021

【数据结构Java实现】链表和递归
https://editor.csdn.net/md/?articleId=104818281

【数据结构Java实现】栈 【数组栈、链栈】
https://editor.csdn.net/md/?articleId=104783374

【数据结构Java实现】队列【数组队列、循环队列和链队】
https://editor.csdn.net/md/?articleId=104785958

【数据结构Java实现】树专题之二叉搜索树
https://editor.csdn.net/md/?articleId=104827774

【数据结构Java实现】集合【链表和二叉搜索树实现】
https://editor.csdn.net/md/?articleId=105106124

【数据结构Java实现】映射【链表和二叉搜索树实现】
https://editor.csdn.net/md/?articleId=105119733

【数据结构Java实现】线段树【数组实现】
https://editor.csdn.net/md/?articleId=105130003

---
一部分包括工程实现（算法第四版、邓俊辉）、注重细节、分类（标签？指向理论？Java！C++），一部分关注理论（算法导论、算法设计沙特？）、注重快速使用、使用学习编号（指向工程的标签）。

暴力
分枝限界法
分治
贪心
动态规划
图论
计算几何
字符串
数论
树
散列表
排序
# 3. DSA理论设计
Data Structure and Algorithm Definition、Proof、Design
包括学习DSA的定义、理论证明和各种设计方法、快速编写的算法实现
里面包括：
算法时间复杂度、各个设计方法提纲挈领，为前几篇，用作后面的标签？
后面包括算法技巧、各个算法的定义、理论证明、快速算法！

[算法学习笔记 全源最短路径Johnson算法（用于稀疏图和有负边的图）](https://blog.csdn.net/myRealization/article/details/124190455)



# 4. DSA工程实现

![在这里插入图片描述](https://img-blog.csdnimg.cn/c2919dcbb26347b29feb62f7751c5ce8.png)

【算法学习】递归定义、递推式计算、递归算法设计方法、递归到非递归算法的转化
https://editor.csdn.net/md/?articleId=108546095 

【算法基础】 数学预备知识2——证明方法
https://editor.csdn.net/md/?articleId=105416313

【算法基础】 数学预备知识1——集合、关系和函数
https://memcpy0.blog.csdn.net/article/details/104899890

【算法基础】学习笔记11 NP完全问题
https://editor.csdn.net/md/?articleId=104907802

【算法基础】第十章 回溯——有效的搜索算法
https://editor.csdn.net/md/?articleId=104906140
【算法基础】第五章 归纳法【基数排序、生成排列】等
https://editor.csdn.net/md/?articleId=104901175
【算法基础4】堆和不相交集数据结构
https://editor.csdn.net/md/?articleId=104900959

【算法基础】动态规划
https://editor.csdn.net/md/?articleId=104904564
 
【算法学习】 贪心算法——求解最优化问题的有效算法
https://editor.csdn.net/md/?articleId=104905272
 
【算法学习】分治法
https://editor.csdn.net/md/?articleId=104901472
 
【算法学习】枚举与暴力法
https://editor.csdn.net/md/?articleId=104838761 

【算法学习】算法概念、非递归和递归算法时空复杂度渐进分析、计算复杂度理论和图灵机
https://editor.csdn.net/md/?articleId=108505969 


【算法基础】基础数据结构
https://editor.csdn.net/md/?articleId=104900804


---
极大化极小 https://en.wikipedia.org/wiki/Minimax
[算法学习笔记(0) 对拍技巧、考场预处理宏、快速输入输出](https://memcpy0.blog.csdn.net/article/details/107560442)
[算法学习笔记(1) 蓄水池抽样（在线随机抽样算法）](https://memcpy0.blog.csdn.net/article/details/120088851)
[算法学习笔记(2) 悬线法](https://memcpy0.blog.csdn.net/article/details/119908788)   **有题没做完！**
[算法学习笔记() 最短路算法（Floyd-Warshall、Bellman-Ford、SPFA、Dijkstra、Johonson全源最短路算法）](https://editor.csdn.net/md?articleId=120465933)

[算法学习笔记(3) 图的概念和存储](https://memcpy0.blog.csdn.net/article/details/106799151)
[算法学习笔记(4) ]
[算法学习笔记(5) 传递闭包](https://memcpy0.blog.csdn.net/article/details/120174902)
[算法学习笔记(6) 快速幂和矩阵快速幂](https://memcpy0.blog.csdn.net/article/details/108410952) **有题没做完！**  **待修改**，修缮**矩阵快速幂的模板**，增加**高精度指数**的快速幂

[算法学习笔记()（一维和二维）前缀和的实现与应用](https://memcpy0.blog.csdn.net/article/details/104470754) **有题没做完！**
[[算法学习笔记()（一维和二维）差分的实现与应用](https://memcpy0.blog.csdn.net/article/details/104594255) **有题没做完！** 在前面
[算法学习笔记(7)  离散化](https://memcpy0.blog.csdn.net/article/details/104987097)
[算法学习笔记(8) 接受-拒绝抽样]
[算法学习笔记(9) 生成函数]

[算法学习笔记(11) 二维偏序]
[算法学习笔记(12) 快速傅里叶变换]


算法学习笔记() 平衡树之AVL树
https://editor.csdn.net/md/?articleId=107943759
算法学习笔记() 平衡树之替罪羊树
https://editor.csdn.net/md/?articleId=107924986
算法学习笔记() 平衡树之FHQ Treap树(无旋Treap)
https://editor.csdn.net/md/?articleId=107924345
[算法学习笔记() 平衡树之笛卡尔树和Treap](https://editor.csdn.net/md/?articleId=107948539)：[【PAT】2021年秋季PAT甲级题解 Sorted Cartesian Tree (30 分)](https://memcpy0.blog.csdn.net/article/details/121863753)

算法学习笔记() 平衡树之Splay（伸展树和自顶向下伸展树）
https://editor.csdn.net/md/?articleId=107919037
[算法学习笔记(13) 线段树]
[算法学习笔记() DFS序、树链剖分及其应用](https://memcpy0.blog.csdn.net/article/details/121885004)  前置知识：线段树
[算法学习笔记() 扫描线算法](https://editor.csdn.net/md/?articleId=108219307) 前置知识：线段树
[算法学习笔记() 可持久化数据结构——可持久化数组/并查集/Trie/线段树](https://editor.csdn.net/md/?articleId=121918202)，前置知识：Splay，线段树


[算法学习笔记(14) 一、二、三维树状数组及其拓展应用](https://memcpy0.blog.csdn.net/article/details/107896533)
[算法学习笔记(15) 摩尔投票、拓展摩尔投票、线段树上摩尔投票] 指向线段树
[[算法学习笔记(16) 割边、割点、强连通分量]
[算法学习笔记(18) 快速傅里叶变换]
[算法学习笔记() 数论之素数判断和计数——埃式筛、欧拉线性筛、区间素数](https://memcpy0.blog.csdn.net/article/details/117432214) **有题没做完**！

划分树]
[算法学习笔记(20) 数位DP（递推和记忆化搜索实现）]
算法学习笔记() 图的遍历和连通性(基础)
https://editor.csdn.net/md/?articleId=107603374
算法学习笔记(22) 拓扑排序（BFS和DFS实现）
https://editor.csdn.net/md/?articleId=107607404 
[算法学习笔记() 无向/有向连通图的欧拉路径/回路（DFS和BFS实现）](https://memcpy0.blog.csdn.net/article/details/107837324) **有题没做完！**
[算法学习笔记() 无向图连通性（割点和割边、点/边双连通分量）、有向图连通性（强连通分量、Kosaraju、Tarjan、Garbow算法）](https://editor.csdn.net/md?articleId=120425368） **有题没做完！** 
[算法学习笔记(17) 布尔可满足式与2-SAT]https://editor.csdn.net/md?articleId=120219894

算法学习笔记() 极角排序
[算法学习笔记() 极角排序](https://memcpy0.blog.csdn.net/article/details/122006212)：前置 计算几何基础模板
凸包：前置  极角排序

算法学习笔记() 平衡树之红黑树
https://editor.csdn.net/md/?articleId=107943780

[算法学习笔记 快速平方根倒数算法详解]()



算法学习笔记() 公平组合游戏
# 算法技巧   
[算法学习笔记(7)  离散化](https://memcpy0.blog.csdn.net/article/details/104987097)


算法学习笔记 基本算法专题 位运算
https://editor.csdn.net/md/?articleId=108907656
 
算法学习笔记 位运算专题 奇妙位操作总结3
https://editor.csdn.net/md/?articleId=106984129 
算法学习笔记 位运算专题 奇妙位操作总结1
https://memcpy0.blog.csdn.net/article/details/106751078

算法学习笔记 位运算专题 奇妙位操作总结2
 https://memcpy0.blog.csdn.net/article/details/106935731



# 搜索算法


 
算法学习笔记 搜索专题 排列、组合和子集生成
https://memcpy0.blog.csdn.net/article/details/104334808
 
算法学习笔记 搜索专题 深度优先搜索
https://editor.csdn.net/md/?articleId=110025409
算法学习笔记 搜索专题 双向广搜
https://editor.csdn.net/md/?articleId=108354320

# 高级数据结构

算法学习笔记 二叉树专题 线索二叉树
https://memcpy0.blog.csdn.net/article/details/109348219 
 
算法学习笔记 区间数据结构 线段树
https://editor.csdn.net/md/?articleId=105132050 

算法学习笔记 高级数据结构 一、二、三维树状数组及其拓展应用
https://memcpy0.blog.csdn.net/article/details/107896533
 
【算法学习笔记】Sparse Table
https://memcpy0.blog.csdn.net/article/details/108374025 
 
算法学习笔记 高级数据结构3 带权并查集
https://editor.csdn.net/md/?articleId=108326386

算法学习笔记 分块 块状数组
https://editor.csdn.net/md/?articleId=108306431

【数据结构】优先队列专题 左式堆
https://editor.csdn.net/md/?articleId=107902798

算法学习笔记 高级数据结构2 种类并查集
https://memcpy0.blog.csdn.net/article/details/108287224
 
算法学习笔记 高级数据结构1 并查集
https://memcpy0.blog.csdn.net/article/details/106536865
 
算法学习笔记 高级数据结构2 单调队列/单调栈
https://editor.csdn.net/md/?articleId=106724231  
P1950 长方形
 
P2032 扫描
 
P1440 求m区间内的最小值
 
P1886 滑动窗口 /【模板】单调队列
 
P3143 [USACO16OPEN]Diamond Collector S
 
P1638 逛画展
 
P1102 A-B 数对
 
P1419 寻找段落
 
P2882 [USACO07MAR]Face The Right Way G
 
P1147 连续自然数和
 
P4086 [USACO17DEC]My Cow Ate My Homework S
 
P3124 [USACO15OPEN]Trapped in the Haybales S
 
P1496 火烧赤壁
 
P1884 [USACO12FEB]Overplanting S 


【数据结构】优先队列专题 二项队列
https://editor.csdn.net/md/?articleId=107968337

【数据结构】跳跃表和确定性跳跃表
https://editor.csdn.net/md/?articleId=108007880

【数据结构】树专题 K-D树
https://editor.csdn.net/md/?articleId=108025427




# 字符串算法
算法学习笔记 字符串专题 基本操作和字符串哈希
https://memcpy0.blog.csdn.net/article/details/108344760
 
算法学习笔记 字符串 AC自动机
https://editor.csdn.net/md/?articleId=108306159
 
算法学习笔记 字符串 扩展KMP算法
https://editor.csdn.net/md/?articleId=108310067
算法学习笔记 字符串 字典树Trie
https://memcpy0.blog.csdn.net/article/details/103209817 
算法学习笔记 字符串 KMP算法、next数组性质
https://memcpy0.blog.csdn.net/article/details/107873647

算法学习笔记 字符串专题 Manacher算法
https://editor.csdn.net/md/?articleId=107848059

算法学习笔记 字符串 Rabin-Karp和Sunday算法
https://editor.csdn.net/md/?articleId=107633708  
算法学习笔记 字符串专题 后缀树和后缀数组
https://editor.csdn.net/md/?articleId=108350076
# 数论算法
算法学习笔记 数论 阶乘、阶乘中质因子个数、组合数及取模
https://editor.csdn.net/md/?articleId=117342064
 
算法学习笔记 数论 最小公倍数、最大公约数（几何理解）与扩展欧几里得算法
https://editor.csdn.net/md/?articleId=117433581
 


素数和质因子分解
算法学习笔记 浮点数的比较宏
https://memcpy0.blog.csdn.net/article/details/116680535 
算法学习笔记 数论1 反素数
https://memcpy0.blog.csdn.net/article/details/107585213

算法学习笔记 数论专题 快速幂和矩阵快速幂
https://memcpy0.blog.csdn.net/article/details/108410952
 
算法学习笔记 数论专题 GCD、LCM、扩展欧几里得算法与二元一次方程的整数解
https://editor.csdn.net/md/?articleId=108550983
 
算法学习笔记 组合数学 特殊计数1 Catalan数
https://editor.csdn.net/md/?articleId=108426687
 
# 动态规划 
[算法学习笔记(2) 悬线法](
https://memcpy0.blog.csdn.net/article/details/119908788) 
算法学习笔记 动态规划 背包专题 01和完全背包
https://editor.csdn.net/md/?articleId=109850350
经典动态规划问题及其类似问题，大部分子序列的动态规划是「区间DP」：
- 最长回文子串([5. 最长回文子串](https://leetcode-cn.com/problems/longest-palindromic-substring/)，[214. 最短回文串](https://leetcode-cn.com/problems/shortest-palindrome/))，回文子串计数([647. 回文子串](https://leetcode-cn.com/problems/palindromic-substrings/))和输出；最长回文子序列([516. 最长回文子序列](https://leetcode-cn.com/problems/longest-palindromic-subsequence/))，回文子序列计数([730. 统计不同回文子序列](https://leetcode-cn.com/problems/count-different-palindromic-subsequences/))和输出；
- 最长公共子串，公共子串计数和输出；最长公共子序列([1143. 最长公共子序列](https://leetcode-cn.com/problems/longest-common-subsequence/)，[1035. 不相交的线](https://leetcode-cn.com/problems/uncrossed-lines/)，[712. 两个字符串的最小ASCII删除和](https://leetcode-cn.com/problems/minimum-ascii-delete-sum-for-two-strings/)，[583. 两个字符串的删除操作](https://leetcode-cn.com/problems/delete-operation-for-two-strings/)，[72. 编辑距离](https://leetcode-cn.com/problems/edit-distance/))，公共子序列计数和输出；
- 最长上升子数组([674. 最长连续递增序列](https://leetcode-cn.com/problems/longest-continuous-increasing-subsequence/))，上升子数组计数和输出；最长上升子序列([300. 最长递增子序列](https://leetcode-cn.com/problems/longest-increasing-subsequence/)，[334. 递增的三元子序列](https://leetcode-cn.com/problems/increasing-triplet-subsequence/)，[646. 最长数对链](https://leetcode-cn.com/problems/maximum-length-of-pair-chain/)，[354. 俄罗斯套娃信封问题](https://leetcode-cn.com/problems/russian-doll-envelopes/))，上升子序列计数([673. 最长递增子序列的个数](https://leetcode-cn.com/problems/number-of-longest-increasing-subsequence/))和输出；
- 最长湍流子数组([978. 最长湍流子数组](https://leetcode-cn.com/problems/longest-turbulent-subarray/))，湍流子数组计数和输出；最长湍流子序列([376. 摆动序列](https://leetcode-cn.com/problems/wiggle-subsequence/))，湍流子序列计数和输出；
- 最长等差子数组，等差子数组计数([413. 等差数列划分](https://leetcode-cn.com/problems/arithmetic-slices/))和输出；最长等差子序列([1027. 最长等差数列](https://leetcode-cn.com/problems/longest-arithmetic-subsequence/)，[368. 最大整除子集](https://leetcode-cn.com/problems/largest-divisible-subset/submissions/))，等差子序列计数([446. 等差数列划分 II - 子序列](https://leetcode-cn.com/problems/arithmetic-slices-ii-subsequence/))和输出；
- ……

# 图论算法
(3)

[算法学习笔记(3) 图的概念和存储](https://memcpy0.blog.csdn.net/article/details/106799151)
[算法学习笔记(4) Floyd]
算法学习笔记() 拓扑排序(BFS+DFS)
https://editor.csdn.net/md/?articleId=107607404 
算法学习笔记() 图的遍历和连通性(基础)
https://editor.csdn.net/md/?articleId=107603374
[算法学习笔记() 无向/有向连通图的欧拉路径/回路（DFS和非递归DFS实现）](https://memcpy0.blog.csdn.net/article/details/107837324)
[算法学习笔记() 无向图连通性（割点和割边、点/边双连通分量）、有向图连通性（强连通分量、Kosaraju算法、Tarjan算法）]()

算法学习笔记 树算法 最近公共祖先
https://editor.csdn.net/md/?articleId=107230384
算法学习笔记 图算法 网络流
https://editor.csdn.net/md/?articleId=107032309
算法学习笔记 树专题 树的重心
https://editor.csdn.net/md/?articleId=113003945
算法学习笔记 图论专题 判断有向图中环的存在、环的输出
https://editor.csdn.net/md/?articleId=110393422 
算法学习笔记 图论专题 判断无向图中环的存在、环的输出
https://memcpy0.blog.csdn.net/article/details/107812221
算法学习笔记 图论专题 关键路径
https://memcpy0.blog.csdn.net/article/details/109348124
https://memcpy0.blog.csdn.net/article/details/107585653 
算法学习笔记 树专题 树的直径解法
https://editor.csdn.net/md/?articleId=108002589
 
算法学习笔记 图论 旅行商问题(用遗传算法求解)
https://editor.csdn.net/md/?articleId=107820730
# 网络流


 

# 组合数学
[组合数高效计算、避免溢出]()

# 离散数学
[算法学习笔记(5) 传递闭包](https://memcpy0.blog.csdn.net/article/details/120174902)

2-SAT问题

二维偏序
# 计算几何

# 线性代数
[算法学习笔记(6) 快速幂和矩阵快速幂](https://memcpy0.blog.csdn.net/article/details/108410952)

# 多项式
# 博弈论
算法学习笔记 博弈论 取石子系列
https://editor.csdn.net/md/?articleId=107300408 


# 随机化
[算法学习笔记(1) 蓄水池抽样（在线随机抽样算法）](https://memcpy0.blog.csdn.net/article/details/120088851)
[算法学习笔记() 接受-拒绝抽样]
[478. 在圆内随机生成点](https://leetcode-cn.com/problems/generate-random-point-in-a-circle/)
 
算法学习笔记 随机化算法 随机数生成器和mt19937
https://editor.csdn.net/md/?articleId=107970502
 



 
STL next_permutation和prev_permutation 算法原理和自行实现
https://memcpy0.blog.csdn.net/article/details/104803834 


算法学习笔记 康托展开(全排列中某一排列的字典序计算问题)  https://memcpy0.blog.csdn.net/article/details/104556462


算法竞赛入门经典笔记 https://editor.csdn.net/md/?articleId=86662648





算法学习笔记 部分年度NOIP普及组试题分析
https://editor.csdn.net/md/?articleId=107045664


【算法引论】读书笔记 第一章
https://editor.csdn.net/md/?articleId=105776905

 
算法学习笔记 C和C++实现MD5算法
 https://editor.csdn.net/md/?articleId=118098764
 
算法学习笔记 数学专题 有理数类模板
https://memcpy0.blog.csdn.net/article/details/111304844 
 
 

算法学习笔记 排序专题 后缀排序(倍增法)
https://editor.csdn.net/md/?articleId=108353364 

 
算法学习笔记 排序算法 基数排序
https://editor.csdn.net/md/?articleId=108327811

 
算法学习笔记 分块算法 莫队2：回滚莫队和不删除莫队
https://editor.csdn.net/md/?articleId=108225262
 

 
算法学习笔记 分块算法 莫队
https://editor.csdn.net/md/?articleId=108209328



