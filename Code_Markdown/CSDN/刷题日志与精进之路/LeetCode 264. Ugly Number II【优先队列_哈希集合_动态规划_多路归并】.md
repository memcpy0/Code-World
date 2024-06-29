> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，这个截止期限可能是永远。
> <b></b>
> 在这一系列刷题文章中，不仅讲解多种解体思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时还会总结相应的算法模板。为了方便在PC上运行调试、分享代码文件，我还将建立相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目总结、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>An <strong>ugly number</strong> is a positive integer whose prime factors are limited to <code>2</code>, <code>3</code>, and <code>5</code>.</p>

<p>Given an integer <code>n</code>, return <em>the</em> <code>n<sup>th</sup></code> <em><strong>ugly number</strong></em>.</p>
 
<p><strong>Example 1:</strong></p>

```bash
Input: n = 10
Output: 12
Explanation: [1, 2, 3, 4, 5, 6, 8, 9, 10, 12] is the sequence of the first 10 ugly numbers. 
```

<p><strong>Example 2:</strong></p>

```bash
Input: n = 1
Output: 1
Explanation: 1 has no prime factors, therefore all of its prime factors are limited to 2, 3, and 5.
```
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= n &lt;= 1690</code></li>
</ul>


题意：给你一个整数 <code>n</code> ，请你找出并返回第 <code>n</code> 个 <strong>丑数</strong> 。<strong>丑数 </strong>就是只包含质因数&nbsp;<code>2</code>、<code>3</code> 和/或&nbsp;<code>5</code>&nbsp;的正整数。

---
最简单的方法是，遍历每个正整数，判断它是否是丑数，即只含有质因数 `2, 3, 5` ，然后找到第 `n` 个丑数。不过显然，**丑数在正整数数轴上的分布是相当稀疏的**，大部分正整数都不是丑数，这样一一检查浪费了太多功夫。我们应该转而关注且只关注**丑数的生成**。

根据丑数的定义，显然有：
- $1$ 是最小的丑数；
- 任意一个丑数 $x$ ，乘以任一质因数 $2, 3, 5$ ，其结果 $2x, 3x, 5x$ 必然也是丑数。或者说，除了 $1$ 以外的任一丑数，都是由更小的丑数乘以任一质因数 $2, 3, 5$ 得到的。

通过对定义的分析，我们了解了如何生成丑数，**关键在于保持遍历丑数的大小顺序**以得到第 $n$ 个丑数——原因是，通过当前丑数乘以 $2, 3, 5$ 生成的丑数不一定是最大的，它可能比已生成的丑数要小，例如通过第二个丑数 $2$ 乘以 $2$ 得到丑数 $4$ ，小于丑数 $5$ ，$4$ 才是第四个丑数而非早已生成的 $5$ 。**我们可能在得到很大的丑数后，才能取得想要的小的结果**。

### 解法1 优先队列+哈希集合
最简单的方法是使用优先队列：
- 先将 $1$ 这个最小丑数入队；
- 每次弹出当前最小的丑数 $x$ ，如果是第 $n$ 个最小的丑数，即为答案；
- 否则，将 $x$ 对应的三个丑数 $2x, 3x, 5x$ 入队。重复执行这两步，直到得到答案为止。

**为了防止同一丑数的重复进队，我们需要使用哈希集合**——此处可能存在疑问，是否能用布尔数组或者 `bitset` 代替呢？试试暴力遍历所有数来计算第 $1690$ 个丑数，你就会打消这个念头，还会发现需要使用 `long long` ——当前丑数乘以 $2, 3, 5$ 得到的新丑数可能溢出 `int` 范围。

这一做法的时间复杂度为 $O(n\log n)$ ，空间复杂度为 $O(n)$ 。具体代码如下，相当简单：
```cpp
//C++ version
class Solution {
public:
    int nthUglyNumber(int n) {
        using ll = long long;
        priority_queue<ll, vector<ll>, greater<ll>> pq;
        unordered_set<ll> st;
        pq.push(1);
        st.insert(1);
        int ps[3] = {2, 3, 5};
        while (n-- > 0) {
            ll u = pq.top(); pq.pop();
            if (n == 0) return (int)u;
            for (int i = 0; i < 3; ++i) {
                ll v = u * ps[i];
                if (st.find(v) == st.end()) { //不存在这个丑数时
                    pq.push(v);
                    st.insert(v);
                }
            }
        }
        return 0; //never
    }
};
//执行用时：112 ms, 在所有 C++ 提交中击败了22.58% 的用户
//内存消耗：29.9 MB, 在所有 C++ 提交中击败了15.55% 的用户
```
```java
//Java version
class Solution {
    public int nthUglyNumber(int n) {
        PriorityQueue<Long> pq = new PriorityQueue<>();
        Set<Long> st = new HashSet<>();
        pq.add(1L);
        st.add(1L);
        int[] ps = new int[]{2, 3, 5};
        for (int i = 1; i <= n; ++i) {
            long u = pq.poll();
            if (i == n) return (int)u;
            for (int p : ps) {
                long v = u * p;
                if (!st.contains(v)) {
                    pq.add(v);
                    st.add(v);
                }
            }
        }
        return 0;
    }
}
//执行用时：49 ms, 在所有 Java 提交中击败了24.20% 的用户
//内存消耗：38.2 MB, 在所有 Java 提交中击败了5.01% 的用户
```
---
### 解法2 动态规划

---
### 解法3 多路（三路）归并


