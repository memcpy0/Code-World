> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

提示 1
定理：在最优发工资方案下，至少有一名工人，发给他的工资恰好等于他的最低期望工资。

证明：反证法。假设不存在这样的工人，那么发给每名工人的工资都高于其最低期望工资，我们可以把每人的工资都下调，从而得到更优的方案，矛盾。

提示 2
枚举发了最低期望工资的那名工人，在满足题干中规则 1 的前提下，哪些工人可以满足规则 2？

如何快速地求出这些工人？

提示 3
定义 ri=wageiqualityir_i = \dfrac{\textit{wage}_i}{\textit{quality}_i}r 
i
​
 = 
quality 
i
​
 
wage 
i
​
 
​
 ，表示「每单位工作质量的工资」。

若以某人的 rir_ir 
i
​
  为基准发工资，那么对于 rrr 值不超过 rir_ir 
i
​
  的工人，发给他们的工资是不低于其最低期望工资的，因此这些工人是可以随意选择（雇佣）的。

按照 rir_ir 
i
​
  从小到大排序，我们便可以快速地求出这些工人。

要选哪 kkk 名工人呢？

提示 4
设这 kkk 名工人的 quality\textit{quality}quality 之和为 sumQ\textit{sumQ}sumQ，若以 rir_ir 
i
​
  为基准发工资，那么发的工资总额为 sumQ⋅ri\textit{sumQ}\cdot r_isumQ⋅r 
i
​
 ，因此 sumQ\textit{sumQ}sumQ 越小发的工资总额就越小。

因此我们需要在从小到大枚举 rir_ir 
i
​
  时，维护当前最小的 kkk 个 quality\textit{quality}quality 值。

如何高效地维护呢？

提示 5
用一个最大堆来维护。

按照 rir_ir 
i
​
  从小到大的顺序遍历工人，当堆中有 kkk 个元素时，如果 qualityi\textit{quality}_iquality 
i
​
  比堆顶小，则可以弹出堆顶，将 qualityi\textit{quality}_iquality 
i
​
  入堆，从而得到一个更小的 sumQ\textit{sumQ}sumQ，此时有可能找到一个更优解 sumQ⋅ri\textit{sumQ}\cdot r_isumQ⋅r 
i
​
 ，更新答案。

Python3
Java
C++
Go
JavaScript
Rust
class Solution:
    def mincostToHireWorkers(self, quality: List[int], wage: List[int], k: int) -> float:
        pairs = sorted(zip(quality, wage), key=lambda p: p[1] / p[0])  # 按照 r 值排序
        h = [-q for q, _ in pairs[:k]]  # 加负号变成最大堆
        heapify(h)
        sum_q = -sum(h)
        ans = sum_q * pairs[k - 1][1] / pairs[k - 1][0]  # 选 r 值最小的 k 名工人
        for q, w in pairs[k:]:  # 后面的工人 r 值更大
            if q < -h[0]:  # 但是 sum_q 可以变小，从而可能得到更优的答案
                sum_q += heapreplace(h, -q) + q  # 更新堆顶和 sum_q
                ans = min(ans, sum_q * w / q)
        return ans
复杂度分析
时间复杂度：O(nlog⁡n)\mathcal{O}(n\log n)O(nlogn)，其中 nnn 为 quality\textit{quality}quality 的长度。排序的时间复杂度为 O(nlog⁡n)\mathcal{O}(n\log n)O(nlogn)，后面和堆有关的时间复杂度为 O(nlog⁡k)\mathcal{O}(n\log k)O(nlogk)，由于 k≤nk\le nk≤n，总的时间复杂度为 O(nlog⁡n)\mathcal{O}(n\log n)O(nlogn)。
空间复杂度：O(n)\mathcal{O}(n)O(n)。由于 k≤nk\le nk≤n，空间复杂度主要取决于 nnn。
