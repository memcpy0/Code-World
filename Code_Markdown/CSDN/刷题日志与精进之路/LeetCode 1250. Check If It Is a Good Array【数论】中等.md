> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given an array `nums` of positive integers. Your task is to select some subset of `nums`, multiply each element by an integer and add all these numbers. The array is said to be **good** if you can obtain a sum of `1` from the array by any possible subset and multiplicand.

Return `True` if the array is **good** otherwise return `False`.
<p><strong class="example">Example 1:</strong></p>
<pre><strong>Input:</strong> nums = [12,5,7,23]
<strong>Output:</strong> true
<strong>Explanation:</strong> Pick numbers 5 and 7.
5*3 + 7*(-2) = 1
</pre>
<p><strong class="example">Example 2:</strong></p>
<pre><strong>Input:</strong> nums = [29,6,10]
<strong>Output:</strong> true
<strong>Explanation:</strong> Pick numbers 29, 6 and 10.
29*1 + 6*(-3) + 10*(-1) = 1
</pre>
<p><strong class="example">Example 3:</strong></p>
<pre><strong>Input:</strong> nums = [3,6]
<strong>Output:</strong> false
</pre>

<strong>Constraints:</strong>
<ul>
	<li><code>1 &lt;= nums.length &lt;= 10^5</code></li>
	<li><code>1 &lt;= nums[i] &lt;= 10^9</code></li>
</ul>
题意：给出一个正整数数组 `nums` ，任务是选出 `nums` 的任一可能子集，将子集中每个元素乘以一个整数并求和，如果和为1，则说明 `nums` 是个好数组。

---
### 视角1 裴蜀定理
本题涉及数论中的裴蜀定理（具体证明可参考[「裴蜀定理」百度百科](https://leetcode.cn/link/?target=https%3A%2F%2Fbaike.baidu.com%2Fitem%2F%E8%A3%B4%E8%9C%80%E5%AE%9A%E7%90%86%2F5186593)），其内容为：**对于不全为零的任意整数 $a, b$ ，记 $g=gcd(a,b)$ 为 $a,b$ 的最大公约数，则对于任意整数 $x, y$ 都满足 $ax + by$ 是 $g$ 的倍数，特别地存在整数 $x, y$ 满足 $ax+by=g$** 。这应该很好理解，$g$ 为 $a,b$ 最大公约数，则存在 $m,n$ 满足 $mg =a,ng=b$ ，所以对于任意整数 $x,y$ 都有 $ax+by=g(mx+ny)$ 是 $g$ 的倍数。

推广到多个整数的情况：**对于不全为零的任意整数 $a_1,a_2,\dots,a_n$ ，记 $g$ 为这 $n$ 个数的最大公约数，则对于任意 $n$ 个整数 $x_1,x_2,\dots,x_n$ 都满足 $a_1x_1+\dots + a_nx_n$ 是 $g$ 的倍数，特别地存在整数 $x_1,\dots, x_n$ 满足 $a_1x_1+\dots +a_nx_n=g$** 。

一个重要推论是：**正整数 $a_1,\dots,a_n$ 的最大公约数是 $1$（充分必要条件）$\Leftrightarrow$ 存在 $n$ 个整数 $x_1,\dots,x_n$ 满足 $a_1x_1+\dots+a_nx_n =1$** 。

### 视角2 辗转相减法
求两个数的最大公因数，可以使用辗转相减法。回忆下述递归过程，不难发现：这一过程压缩后等价于，==两个数 $a, b$ 通过同时乘以两个特定整数，再求和可得它们的最大公因数==。因此，**如果它们的最大公因数为1，则说明存在两个特定整数 $x,y$ 使得 $ax+by=1$** 。反过来说，**如果存在两个特定整数 $x,y$ 使得 $ax+by=1$ ，则 $gcd(a,b)=1$**（说明 $a,b$ 通过辗转相减可得1，即 $a,b$ 最大公因数为1）。
```cpp
int gcd(a, b) {
	if (a > b) swap(a, b); // 使得a<=b
	if (a == 0) return b; // 如果a为0,则gcd(0,b)=b
	return gcd(a, b - xa); 
	// x是使得xa最大、但xa<=b的某一正整数
	// b-xa其实就是b%a
}
```
从而，如果一个互质数组的最大公因数是1，那就一定存在一组整数可以让它们相乘求和得到1。反过来说也可行。

对于 `nums` 来说，==如果存在一个数组子集满足题意（即存在一组整数使它们互乘再求和为1），则说明该数组子集是互质的，从而 `nums` 一定是互质的==。如果 `nums` 不是互质的，则 `nums` 就不是好数组。

### 其他视角
从一些简单的例子入手来思考本题。示例三给的 `nums = [3, 6]` ，可以先朴素地列出所有的可能性，其子集为 `{}, {3}, {6}, {3, 6}` 。
- 空集显然不符合要求。
- `{3}, {6}` 显然也不符合要求。因为对于任意不为1的正整数 $x$ ，乘以任意整数 $a$ 的结果 $ax$ 都不可能为1。当且仅当 $x=1$ 时，才存在唯一的整数 $a=1$ 能使 $ax$ 满足 $ax = 1$ 。
- `{3, 6}` 也不符合要求。假设 $y$ 和 $z$ 是任意整数，我们需要找到使得 $3y+6z=1$ 的那对 $(y, z)$ 。由于 $3y+6z=3(y+2z)$ ，而 $(y + 2z)$ 可看作是一个整体的整数，由上一点的分析可知，不存在这样的整数 $a = (y + 2z)$ 使得 $3a = 1$ 。3是3和6的最大公约数，因此可以大胆猜测==最大公约数应在本题中占很重要的位置==。

因此对两个正整数 $a, b$ ，假设其最大公约数为 $g$ 使得 $a=mg, b=ng$ ，考虑任意整数 $x,y$ ，则 $ax + by=g(mx + ny)$ ，可知 $mx+ny$ 必定是整数，如要找到某一对 $x,y$ 使得 $g(mx+ny)=1$ 成立，则唯一的可能性是 $g=1$ 且 $(mx+ny)=1$ 。也就是 $a,b$ 的最大公因数必须是1。

### 解法 数论
由裴蜀定理可得，题意等价于：如果 `nums` 中全部整数的最大公约数为1（即为互质数组），则 `nums` 为好数组。否则不是。
```python
class Solution:
    def isGoodArray(self, nums: List[int]) -> bool:
        return reduce(gcd, nums) == 1
```
