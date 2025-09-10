> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

在代号为 C-137 的地球上，Rick 发现如果他将两个球放在他新发明的篮子里，它们之间会形成特殊形式的磁力。Rick 有 `n` 个空的篮子，第 `i` 个篮子的位置在 `position[i]` ，Morty 想把 `m` 个球放到这些篮子里，使得任意两球间 **最小磁力** 最大。

已知两个球如果分别位于 `x` 和 `y` ，那么它们之间的磁力为 `|x - y|` 。

给你一个整数数组 `position` 和一个整数 `m` ，请你返回最大化的最小磁力。

**示例 1：**

![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2020/08/16/q3v1.jpg)

```java
输入：position = [1,2,3,4,7], m = 3
输出：3
解释：将 3 个球分别放入位于 1，4 和 7 的三个篮子，两球间的磁力分别为 [3, 3, 6]。最小磁力为 3 。我们没办法让最小磁力大于 3 。
```
**示例 2：**
```java
输入：position = [5,4,3,2,1,1000000000], m = 2
输出：999999999
解释：我们使用位于 1 和 1000000000 的篮子时最小磁力最大。
```
**提示：**
-   `n == position.length`
-   `2 <= n <= 10^5`
-   `1 <= position[i] <= 10^9`
-   所有 `position` 中的整数 **互不相同** 。
-   `2 <= m <= position.length`

---
### 解法 二分+排序+贪心
本题与LeetCode 2517. Maximum Tastiness of Candy Basket几乎完全一样，都是极大化极小。==如果题目中有「最大化最小值」或者「最小化最大值」，一般都是二分答案，请记住这个套路==。

为什么？对于本题来说，**任意两球间最小磁力越大，能放置的球越少，有单调性**，所以可以二分。定义 $f(d)$ 表示最小磁力至少为 $d$ 时，至多能放多少个球在这些篮子里。于是二分答案 $d$ ：
- 如果 $f(d)\ge m$ ，说明答案至少为 $d$ 。在区间左侧外面的 $d$ 都是 $f(d)\ge m$ 的。
- 如果 $f(d)< m$ ，说明答案至多为 $d-1$ 。在区间右侧外面的 $d$ 都是 $f(d)<m$ 的。
- 二分结束后，设答案为 $\textit{d}_0$ ，那么 $f(d_0)\ge m$ 且 $f(d_0+1)< m$ ，这就是满足题意「放 $m$ 个球到篮子里」，能得到的最大化的最小磁力 $d_0$ 。

二分上界可以取 $\left\lfloor\dfrac{\max(\textit{position})-\min(\textit{position})}{m-1}\right\rfloor$ 。这是因为**最小磁力（即使是最大化的也）不会超过平均值**。

```cpp
class Solution {
public:
    int maxDistance(vector<int>& position, int m) {
        sort(position.begin(), position.end());
        int n = position.size();
        int l = 0, r = (position[n - 1] - position[0]) / (m - 1);
        while (l < r) {
            int mid = (l + r + 1) >> 1;
            int cnt = 1;
            // for (int i = 0, j; i < n; i = j) {
            //     j = lower_bound(position.begin() + i + 1, position.end(),
            //         position[i] + mid) - position.begin();
            //     if (j != n) ++cnt; // 找到一个数position[j],和position[i]差>=mid
            // }
            for (int pre = position[0], i = 1; i < n; ++i) {
                if (position[i] >= pre + mid) {
                    pre = position[i];
                    ++cnt;
                }
            }
            if (cnt >= m) l = mid;
            else r = mid - 1;
        }
        return l;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n\log n + n\log U)$ ，其中 $n$ 为 $\textit{position}$ 的长度，$U=\left\lfloor\dfrac{\max(\textit{position})-\min(\textit{position})}{m-1}\right\rfloor$ 。
- 空间复杂度：$\mathcal{O}(1)$ ，忽略排序的空间，仅用到若干额外变量。