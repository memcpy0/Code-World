> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个长度为 `n` 、下标从 **0** 开始的整数数组 `nums` ，表示收集不同巧克力的成本。每个巧克力都对应一个不同的类型，最初，位于下标 `i` 的巧克力就对应第 `i` 个类型。

在一步操作中，你可以用成本 `x` 执行下述行为：
- 同时修改所有巧克力的类型，将巧克力的类型 `ith` 修改为类型 `((i + 1) mod n)th`。

假设你可以执行任意次操作，请返回收集所有类型巧克力所需的最小成本。

**示例 1：**
```cpp
输入：nums = [20,1,15], x = 5
输出：13
解释：最开始，巧克力的类型分别是 [0,1,2] 。我们可以用成本 1 购买第 1 个类型的巧克力。
接着，我们用成本 5 执行一次操作，巧克力的类型变更为 [1,2,0] 。我们可以用成本 1 购买第 2 个类型的巧克力。
然后，我们用成本 5 执行一次操作，巧克力的类型变更为 [2,0,1] 。我们可以用成本 1 购买第 0 个类型的巧克力。
因此，收集所有类型的巧克力需要的总成本是 (1 + 5 + 1 + 5 + 1) = 13 。可以证明这是一种最优方案。
```
**示例 2：**
```cpp
输入：nums = [1,2,3], x = 4
输出：6
解释：我们将会按最初的成本收集全部三个类型的巧克力，而不需执行任何操作。因此，收集所有类型的巧克力需要的总成本是 1 + 2 + 3 = 6 。
```
**提示：**
- `1 <= nums.length <= 1000`
- `1 <= nums[i] <= 10^9`
- `1 <= x <= 10^9`

---
### 解法
#### 提示1
这道题有两个成本，一个是收集每种巧克力的成本数组 `nums` ；另一种是执行巧克力数组 *rotate* 的成本 `x` 。

不难想到，由于成本数组长度最多为 $1000$ ，则巧克力数组长度也不会超过 $1000$ ，最多 *rotate* $1000$ 次，就会回到原样。所以可以尝试所有可能——**枚举操作次数，从操作 $0$ 次到操作 $n−1$ 次。
#### 提示2

考虑简单情况：
- 如果不操作，第 $i$ 个巧克力必须花费 $\textit{nums}[i]$ 收集，总花费为所有 $\textit{nums}[i]$ 之和。例如示例 2 不操作是最优的。
- 如果只操作一次，第 $i$ 个巧克力可以在操作前购买，也可以在操作后购买，取最小值，即 $\min(\textit{nums}[i], \textit{nums}[(i+1)\bmod n])$ 。
- 如果操作两次，购买第 $i$ 个巧克力的花费为 $\min(\textit{nums}[i], \textit{nums}[(i+1)\bmod n], \textit{nums}[(i+2) \bmod n])$ 。例如示例 1，我们可以操作两次，这样每块巧克力都只需要 $1$ 的花费，总成本为 $2x+1+1+1=13$ 。

依此类推。
#### 提示3
如果暴力枚举操作次数，再枚举每个巧克力，再计算购买这个巧克力的最小花费，总的时间复杂度是 $\mathcal{O}(n^3)$ 。

一个初步的优化是，**用 $\mathcal{O}(n^2)$ 的时间预处理所有子数组的最小值，保存到一个二维数组中**。这样做需要 $\mathcal{O}(n^2)$ 的时间和空间。

但其实不需要预处理，还有更简单的做法：
1. 用一个长为 $n$ 的数组 $s$ 统计不同操作次数下的总成本，**$s[i]$ 统计操作 $i$ 次下的总成本**。
2. 写一个二重循环，枚举子数组的左端点 $i$ 和右端点 $j$ 。
3. 在枚举右端点的同时，维护从 $\textit{nums}[i]$ 到 $\textit{nums}[j]$ 的最小值 $\textit{mn}$ 。
4. 把 $\textit{mn}$ 加到 $s[j−i]$ 中，这是因为**长为 $j-i+1$ 的子数组恰好对应着操作 $j-i$ 次时要处理的子数组**，或者说区间 $[i,j]$ 的最小值对应操作 $j-i$ 次取巧克力的成本。
    1. 对每个区间 $[i,i]$ ，最小值就是 $nums[i]$ ，也就是操作 $0$ 次取每个巧克力的成本；
    2. 对每个区间 $[i,i+1]$ ，最小值是 $\min(nums[i], nums[i+1])$ ，也就是操作 $1$ 次时取每个巧克力的成本。
    3. ……
5. 最后输出 $\min(s)$ 。

```cpp
// cpp
class Solution {
public:
    long long minCost(vector<int>& nums, int x) {
        int n = nums.size();
        vector<long long> s(n); // s[k] 统计操作 k 次的总成本
        for (int i = 0; i < n; ++i)
            s[i] = (long long) i * x;
        for (int i = 0; i < n; ++i) { // 子数组左端点
            int mn = nums[i];
            for (int j = i; j < n + i; ++j) { // 子数组右端点（把数组看做环状）
                mn = min(mn, nums[j % n]); // 维护nums[i : j]的最小值
                s[j - i] += mn; // 累加操作 j-i 次时的成本
            }
        }
        return *min_element(s.begin(), s.end());
    }
};

// java
class Solution {
    public long minCost(int[] nums, int x) {
        int n = nums.length;
        long[] s = new long[n];
        for (int i = 0; i < n; ++i) 
            s[i] = (long) i * x;
        for (int i = 0; i < n; ++i) {
            int mn = nums[i];
            for (int j = i; j < n + i; ++j) {
                mn = Math.min(mn, nums[j % n]);
                s[j - i] += mn;
            }
        }
        long ans = Long.MAX_VALUE;
        for (long v : s) ans = Math.min(ans, v);
        return ans;
    }
}

// go
func minCost(nums []int, x int) int64 {
    n := len(nums)
    s := make([]int64, n)
    for i := range s {
        s[i] = int64(i) * int64(x)
    }
    for i, mn := range nums {
        for j := i; j < n + i; j++ {
            mn = min(mn, nums[j % n]);
            s[j - i] += int64(mn);
        }
    }
    return slices.Min(s)
}

// rust
impl Solution {
    pub fn min_cost(nums: Vec<i32>, x: i32) -> i64 {
        let n = nums.len();
        let mut s: Vec<i64> = (0..n).map(|i| i as i64 * x as i64).collect();
        for i in 0..n {
            let mut mn = nums[i];
            for j in i..(n + i) {
                mn = mn.min(nums[j % n]);
                s[j - i] += mn as i64;
            }
        }
        *s.iter().min().unwrap()
    }
}

// python
class Solution:
    def minCost(self, nums: List[int], x: int) -> int:
        n = len(nums)
        s = list(range(0, n * x, x))
        for i, mn in enumerate(nums):
            for j in range(i, n + i):
                mn = min(mn, nums[j % n])
                s[j - i] += mn
        return min(s)
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n^2)$，其中 $n$ 为 $\textit{nums}$ 的长度。
- 空间复杂度：$\mathcal{O}(n)$ 。
