@[toc]

学习AC自动机之前，建议熟练掌握KMP和字典树。

---
# 1. AC自动机简介
AC自动机 `Aho-Corasick automaton` 是KMP的升级版，是Trie字典树的变种，一种伪树形结构。

KMP是单模匹配算法，处理在一个文本字符串中查找一个模式串的问题。AC自动机是多模匹配算法，能够在一个文本串中**同时查找多个不同的模式串**。(多模匹配问题：给定一个长度为 `n` 的文本  `S` ，`k` 个平均长度为 `m` 的模式串 $P_1, P_2, \dots, P_k$ ，要求搜索这些模式串出现的位置。）

诚然，KMP也可以解决多模匹配问题，只是时间复杂度较高，需要对每个 $P_1, P_2, \dots, P_k$ 分别做一次KMP，总复杂度是 $\text{O((n+m)k)}$ 。而AC自动机不需要对 `S` 多次KMP，只搜索一遍 `S` ，在搜索时匹配所有的模式串。

> 突然想起来  [392. is Subsequence](https://leetcode-cn.com/problems/is-subsequence/) 判断子序列这道题目，如果我们只是判断一个模式字符串是否是文本串的子序列，直接比较即可，$\text{O(n)}$ 的时间。但是如果我们要**判断多个模式字符串是否是文本串的子序列**，就需要**对文本串进行处理**，记录每个字符出现的下一个位置，用于序列比较时直接跳转。

如何同时匹配所有的模式串 $P$ ？结合字典树+KMP算法，AC自动机出现了。KMP通过查找 $P$ 对应的 `next[]` 数组实现快速匹配，**如果把所有的 $P$ 做成一棵字典树，然后在匹配的时候查找这个 $P$ 对应的 `next[]` 数组，就实现了快速匹配**。

此时，复杂度：`k` 个字符串，平均长度为 `m` ，文本串长度为 `n` 。建立字典树 $\text{O(km)}$ ；建立 `fail` 指针 $\text{O(km)}$ ；模式匹配 $\text{O(nm)}$ ，乘以 `m` 的原因是：统计时需要顺着链回溯到 `root` 结点。总的复杂度是 $\text{O(km + km + nm)} = \text{O(km+nm)}$ ，对比简单使用KMP的复杂度 $\text{O((n + m)k)}$ ，当 $\text{m << k}$ 时，$(k + n)m << (m + n)k$ 。AC自动机的优势非常大。

---
# 2. 构建AC自动机
如何建立AC自动机，需要仔细学习以下内容。
两个优化：

1.类似并差集的路径压缩，把不存在的ch[u][c]全部指向ch[f[u]][c]（f是失配函数）

2.把有效失配函数用last指针保存。


---
# 3. 应用题目
[洛谷 P3808 【模板】AC自动机（简单版）](https://www.luogu.com.cn/problem/P3808)
[HDU 2222](http://acm.hdu.edu.cn/showproblem.php?pid=2222)
[HDU 2896](http://acm.hdu.edu.cn/showproblem.php?pid=2896)
[HDU 3065](http://acm.hdu.edu.cn/showproblem.php?pid=3065)
[HDU 3336](http://acm.hdu.edu.cn/showproblem.php?pid=3336)

下面的几道题难一些，要用到AC自动机+状态压缩DP：
[HDU 2243]()
[HDU 2825]()
[HDU 2296]() 
