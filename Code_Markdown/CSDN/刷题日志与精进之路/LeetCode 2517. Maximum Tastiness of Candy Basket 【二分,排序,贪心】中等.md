> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个正整数数组 `price` ，其中 `price[i]` 表示第 `i` 类糖果的价格，另给你一个正整数 `k` 。

商店组合 `k` 类 **不同** 糖果打包成礼盒出售。礼盒的 **甜蜜度** 是礼盒中任意两种糖果 **价格** 绝对差的最小值。

返回礼盒的 **最大** 甜蜜度。

**示例 1：**
```java
输入：price = [13,5,1,8,21,2], k = 3
输出：8
解释：选出价格分别为 [13,5,21] 的三类糖果。
礼盒的甜蜜度为 min(|13 - 5|, |13 - 21|, |5 - 21|) = min(8, 8, 16) = 8 。
可以证明能够取得的最大甜蜜度就是 8 。
```
**示例 2：**
```java
输入：price = [1,3,1], k = 2
输出：2
解释：选出价格分别为 [1,3] 的两类糖果。 
礼盒的甜蜜度为 min(|1 - 3|) = min(2) = 2 。
可以证明能够取得的最大甜蜜度就是 2 。
```
**示例 3：**
```java
输入：price = [7,7,7,7], k = 2
输出：0
解释：从现有的糖果中任选两类糖果，甜蜜度都会是 0 。
```
**提示：**
-   `1 <= price.length <= 10^5`
-   `1 <= price[i] <= 10^9`
-   `2 <= k <= price.length`

---
### 解法 二分+排序
礼盒的甜蜜度为「任意两种糖果价格绝对差的**最小值**」，则等价于「排序后，任意两种**价格相邻**的糖果之间价格绝对差的最小值」。==如果题目中有「最大化最小值」或者「最小化最大值」，一般都是二分答案，请记住这个套路==。

为什么？对于本题来说，**甜蜜度越大，能选择的糖果越少，有单调性**，所以可以二分。定义 $f(d)$ 表示甜蜜度至少为 $d$ 时，至多能选多少类糖果。二分答案 $d$ ：
- 如果 $f(d)\ge k$ ，说明答案至少为 $d$ 。在区间左侧外面的 $d$ 都是 $f(d)\ge k$ 的。
- 如果 $f(d)< k$ ，说明答案至多为 $d-1$ 。在区间右侧外面的 $d$ 都是 $f(d)<k$ 的
- 二分结束后，设答案为 $\textit{d}_0$ ，那么 $f(d_0)\ge k$ 且 $f(d_0+1)< k$ 
​
如何计算 $f(d)$ ？对 $\textit{price}$ 从小到大排序，贪心地计算 $f(d)$ ：从 $\textit{price}[0]$ 开始选；假设上一个选的数是 $\textit{pre}$ ，那么当 $\textit{price}[i] \ge \textit{pre}+d$ 时，就可以选 $\textit{price}[i]$ 。

二分下界可以取 $1$ ，上界可以取 $\left\lfloor\dfrac{\max(\textit{price})-\min(\textit{price})}{k-1}\right\rfloor$ 。这是因为**最小值不会超过平均值**（平均值指选了 $\textit{price}$ 最小最大以及中间的一些糖果，相邻糖果差值的平均值）。
```cpp
class Solution {
public:
    int maximumTastiness(vector<int>& price, int k) {
        sort(price.begin(), price.end());
        int n = price.size();
        int l = 0, r = (price[n - 1] - price[0]) / (k - 1);
        while (l < r) {
            int mid = (l + r + 1) / 2; // [l,r+1)
            int cnt = 1; // 以mid为礼盒最大甜蜜度
            // 枚举数组price,看能否找到相差至少为mid的k个数
            // 下面这种写法只超过8%
            // for (int i = 0, j; i < n; i = j) {
            //     j = lower_bound(price.begin() + i + 1, price.end(), price[i] + mid) - price.begin();
            //     if (j != n) ++cnt; // 找到一个数price[j],和price[i]差>=mid
            // }
            for (int pre = price[0], i = 1; i < n; ++i) {
                if (price[i] >= pre + mid) {
                    ++cnt;
                    pre = price[i];
                }
            }
            if (cnt >= k) l = mid; // 可满足最大甜蜜度为mid
            else r = mid - 1;
        }
        return l;
    }
}; 
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n\log n + n\log U)$ ，其中 $n$ 为 $\textit{price}$ 的长度，$U=\left\lfloor\dfrac{\max(\textit{price})-\min(\textit{price})}{k-1}\right\rfloor$ 。
- 空间复杂度：$\mathcal{O}(1)$ ，忽略排序的空间，仅用到若干额外变量。
 