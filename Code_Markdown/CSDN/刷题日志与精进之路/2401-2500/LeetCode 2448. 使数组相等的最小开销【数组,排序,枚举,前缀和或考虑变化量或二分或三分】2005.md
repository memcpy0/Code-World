> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个下标从 **0** 开始的数组 `nums` 和 `cost` ，分别包含 `n` 个 **正** 整数。

你可以执行下面操作 **任意** 次：
- 将 `nums` 中 **任意** 元素增加或者减小 `1` 。

对第 `i` 个元素执行一次操作的开销是 `cost[i]` 。

请你返回使 `nums` 中所有元素 **相等** 的 **最少** 总开销。

**示例 1：**
```js
输入：nums = [1,3,5,2], cost = [2,3,1,14]
输出：8
解释：我们可以执行以下操作使所有元素变为 2 ：
- 增加第 0 个元素 1 次，开销为 2 。
- 减小第 1 个元素 1 次，开销为 3 。
- 减小第 2 个元素 3 次，开销为 1 + 1 + 1 = 3 。
总开销为 2 + 3 + 3 = 8 。
这是最小开销。
```
**示例 2：**
```js
输入：nums = [2,2,2,2,2], cost = [4,2,8,1,3]
输出：0
解释：数组中所有元素已经全部相等，不需要执行额外的操作。
```
**提示：**
- `n == nums.length == cost.length`
- `1 <= n <= 10^5`
- `1 <= nums[i], cost[i] <= 10^6`
- 测试用例确保输出不超过 $2^{53}-1$ 。

---
题目集合：
- [453. 最小操作次数使数组元素相等](https://leetcode.cn/problems/minimum-moves-to-equal-array-elements/)
- [456. 最小操作次数使数组元素相等 II](https://leetcode.cn/problems/minimum-moves-to-equal-array-elements-ii/)
- [2448. 使数组相等的最小开销](https://leetcode.cn/problems/minimum-cost-to-make-array-equal/)

这道题是 [462. 最小操作次数使数组元素相等 II](https://leetcode.cn/problems/minimum-moves-to-equal-array-elements-ii/submissions/) 的加强版本，给每个元素增加了一个权重，要求的是加权最小移动开销，但其实原理是一样的。
### 解法1 排序+枚举+两个前缀和
先按照 $nums[i]$ 为关键字，对 $nums[i], cost[i]$ 数对从小到大排序。枚举到第 $i$ 个数时，假设所有数都变成 $nums[i]$ ，那么总开销将会是：
$$\begin{aligned}
\sum_{0}^{i-1} (nums[i]-nums[j])\times cost[j] +\sum_{i+1}^{n-1} (nums[j]-nums[i])\times cost[j]
\end{aligned}$$
然后分别利用 $nums[i]$ 和 $nums[i] \times cost[i]$ 的前缀和，就可以轻松算出来当所有数都是 $nums[i]$ 时的总开销了。
```cpp
class Solution {
public:
    long long minCost(vector<int>& nums, vector<int>& cost) {
        typedef pair<int, int> pii;
        typedef long long ll;
        int n = nums.size();
        vector<pii> order;
        for (int i = 0; i < n; ++i) order.emplace_back(nums[i], cost[i]);
        sort(order.begin(), order.end(), [&](const pii &a, const pii &b) {
            return a.first < b.first;
        });
        // 将其他元素变为nums[i]
        // 小于nums[i]的代价为: sum_{0}^{i-1} (nums[i]-nums[j])*cost[j]
        // 大于nums[i]的代价为: sum_{i+1}^{n-1} (nums[j]-nums[i])*cost[j]
        // 注意nums[i]为确定的值
        // nums[i] * sum_{0}^{i-1} cost[j] -nums[i] * sum_{i+1}^{n-1} cost[j]
        // -sum_{0}^{i-1} nums[j]*cost[j] sum_{i+1}^{n-1} nums[j]*cost[j]
        vector<ll> d(n + 1); // cost[i]的前缀和
        vector<ll> q(n + 1); // nums[i]*cost[i]的前缀和
        ll ans = LLONG_MAX;
        for (int i = 0; i < n; ++i) {
            d[i + 1] = d[i] + order[i].second;
            q[i + 1] = q[i] + (ll)order[i].first * order[i].second;
        }
        for (int i = 0; i < n; ++i)
            ans = min(ans, (ll)order[i].first * (d[i] - d[n] + d[i + 1]) + (q[n] - q[i + 1] - q[i]));
        return ans;
    }
};
```
---
### 解法2 排序+枚举+考察变化量
同样将 $nums$ 和 $cost$ 捆在一起排序。这里的考虑是：首先计算使所有元素都为 $nums[0]$ 的总开销 $total$ ，以及所有 $cost$ 的和为 $sumCost$ 。然后考虑**使所有元素都等于 $nums[1]$ 时 $total$ 的变化量**为多少：
- $cost[0]$ 这么多的开销，要增加 $nums[1] - nums[0]$ 个；
- $sumCost - cost[0]$ 这么多的开销，要减少 $nums[1] - nums[0]$ 个。

因此 $total$ 减少了 $$(sumCost - 2cost[0]) \times (nums[1] - nums[0])$$
按照这个公式模拟后续 $nums[i]$ ，取所有 $total$ 最小值为答案。
```cpp
class Solution {
public:
    long long minCost(vector<int>& nums, vector<int>& cost) {
        typedef pair<int, int> pii;
        typedef long long ll;
        int n = nums.size();
        vector<pii> order;
        for (int i = 0; i < n; ++i) order.emplace_back(nums[i], cost[i]);
        sort(order.begin(), order.end(), [&](const pii &a, const pii &b) {
            return a.first < b.first;
        });
        ll total = 0, sumCost = 0;
        for (int i = 0; i < n; ++i) {
            total += (ll) order[i].second * (order[i].first - order[0].first);
            sumCost += order[i].second;
        }
        ll ans = total;
        for (int i = 1; i < n; ++i) {
            sumCost -= 2 * order[i - 1].second;
            total -= sumCost * (order[i].first - order[i - 1].first);
            ans = min(ans, total);
        }
        return ans;
    }
};
```



首先转化为数学形式。假设最终所有元素相等，且都为 $x$ ，则每个元素 $nums[i]$ 移动到 $x$ 的开销是 $cost[i] \cdot |x-nums[i]|$ ，总的开销是
$$f(x)=\sum_{i=0}^{n-1}cost[i] \cdot |x-nums[i]|$$ 
**绝对值函数**可以写成**分段的线性函数**，上面这个函数则可以在每一个 $nums[i]$ 处分段，写成一个 $n+1$ 段的线性函数，函数图像是一条折线。

考虑 $x$ 从 $-\infty$ 变化到 $+\infty$ 的过程中，作为线性函数的一次项系数的变化。
- 首先，在 $-\infty$ 处，系数为 $-\sum_{i=0}^{n-1}cost[i]$​
- 之后每次碰到一个数 $nums[i]$ 时，系数就会增加 $2cost[i]$ 
- 直到最终在 $+\infty$ 处增加到最大值 $\sum_{i=0}^{n-1}cost[i]$

​因此整个函数图像是一个向下凹的类抛物线形状，因此，只要找到系数由负变为正的转折点，就能得到最小值。

注意到这是一个凹函数，因此可以用二分来求极值，复杂度 O(nlog⁡U)O(n \log U)O(nlogU)。另一个比较简单的方法是排序，然后累加 costcostcost，直至累加的 costcostcost 值达到总和的一半，复杂度主要是排序的 O(nlog⁡n)O(n \log n)O(nlogn)。更快的方法是使用快速选择算法，可以把复杂度优化到 O(n)O(n)O(n)。


---
### 解法4 中位数贪心
把 cost[i]\textit{cost}[i]cost[i] 理解成 nums[i]\textit{nums}[i]nums[i] 的出现次数。

根据中位数贪心，把所有数变成中位数是最优的。

详细证明参考 462. 最小操作次数使数组元素相等 II。

代码实现时，仍然按照方法一那样排序，然后不断累加 cost[i]\textit{cost}[i]cost[i]，首次累加到 ≥sumCost2\ge\dfrac{\textit{sumCost}}{2}≥ 
2
sumCost
​
  时就找到了中位数。

由于 sumCost\textit{sumCost}sumCost 可能是奇数，所以要上取整，即首次累加到 ≥⌈sumCost2⌉\ge\left\lceil\dfrac{\textit{sumCost}}{2}\right\rceil≥⌈ 
2
sumCost
​
 ⌉ 时就找到了中位数。

Python3
Go
func minCost(nums, cost []int) (ans int64) {
	type pair struct{ x, c int }
	a := make([]pair, len(nums))
	sumCost := 0
	for i, c := range cost {
		a[i] = pair{nums[i], c}
		sumCost += c
	}
	sort.Slice(a, func(i, j int) bool { a, b := a[i], a[j]; return a.x < b.x })

	s, mid := 0, (sumCost+1)/2
	for _, p := range a {
		s += p.c
		if s >= mid {
			// 把所有数变成 p.x
			for _, q := range a {
				ans += int64(abs(q.x-p.x)) * int64(q.c)
			}
			break
		}
	}
	return
}

func abs(x int) int { if x < 0 { return -x }; return x }

作者：灵茶山艾府
链接：https://leetcode.cn/problems/minimum-cost-to-make-array-equal/solutions/1917771/by-ctysss-q5ek/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。