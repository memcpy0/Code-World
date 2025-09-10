> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个整数数组 `nums1` 和 `nums2` ，它们长度都为 `n` 。

两个数组的 **异或值之和** 为 `(nums1[0] XOR nums2[0]) + (nums1[1] XOR nums2[1]) + ... + (nums1[n - 1] XOR nums2[n - 1])` （**下标从 0 开始**）。

- 比方说，`[1,2,3]` 和 `[3,2,1]` 的 **异或值之和** 等于 `(1 XOR 3) + (2 XOR 2) + (3 XOR 1) = 2 + 0 + 2 = 4` 。

请你将 `nums2` 中的元素重新排列，使得 **异或值之和** **最小** 。

请你返回重新排列之后的 **异或值之和** 。

**示例 1：**
```js
输入：nums1 = [1,2], nums2 = [2,3]
输出：2
解释：将 `nums2` 重新排列得到 `[3,2] 。`
异或值之和为 (1 XOR 3) + (2 XOR 2) = 2 + 0 = 2 。
```
**示例 2：**
```js
输入：nums1 = [1,0,3], nums2 = [5,3,4]
输出：8
解释：将 `nums2 重新排列得到` `[5,4,3] 。`
异或值之和为 (1 XOR 5) + (0 XOR 4) + (3 XOR 3) = 4 + 4 + 0 = 8 。
```
**提示：**
- `n == nums1.length`
- `n == nums2.length`
- `1 <= n <= 14`
- `0 <= nums1[i], nums2[i] <= 10^7`

---
### 解法 状压DP
设数组 $nums_1$ 和 $nums_2$ 的长度为 $n$ ，我们可以用一个长度为 $n$ 的二进制数 $mask$ 表示数组 $nums_2$ 中的数被选择的状态：如果 $mask$ 从低到高的第 $i$ 位为 $1$ ，说明 $nums_2[i]$ 已经被选择，否则说明其未被选择。

这样一来，我们就可以使用动态规划解决本题。记 $f[mask]$ 表示当我们选择了数组 $nums_2$ 中的元素的状态为 $mask$，并且选择了数组 $nums_1$  的前 $\text{count}(\textit{mask})$ 个元素的情况下，可以组成的**最小的异或值之和**。
> 或者使用更加直观的 $f[i][mask]$ ，表示选择数组 $nums_1$ 中 $0, \dots, i$ 下标的元素、且选择数组 $nums_2$ 中元素的状态为 $mask$ 情况下的最小异或值之和。

这里的 $count(mask)$ 表示 $mask$ 的二进制表示中 $1$ 的个数。为了叙述方便，记 $c = \text{count}(\textit{mask})$ 。在进行状态转移时，我们可以枚举 $\textit{nums}_1[c-1]$ 与 $nums_2$ 中的哪一个元素进行了异或运算，假设其为 $nums_2[i]$ ，那么有状态转移方程：
$$f[\textit{mask}] = \min_{\textit{mask} ~二进制表示的第~ i ~位为~ 1} \big\{ f[\textit{mask} \backslash i] + (\textit{nums}_1[c-1] \oplus \textit{nums}_2[i]) \big\}$$

其中 $\oplus$ 表示异或运算，$\textit{mask} \backslash i$ 表示将 $mask$ 的第 $i$ 位从 $1$ 变为 $0$ 。最终的答案即为 $f[2^n - 1]$ 。
> 细节：$\textit{mask} \backslash i$ 可以使用异或运算 $\textit{mask} \oplus 2^i$ 实现；

判断 $mask$ 的第 $i$ 位是否为 $1$ ，等价于判断按位与运算 $\textit{mask} \wedge 2^i$ 的值是否大于 $0$ ；或者 $mask\ >>\ i\ \&\ 1$ 是否为 $1$ 。

由于我们需要求出的是最小值，因此可以将所有的状态初始化为极大值 $\infty$ ，方便进行状态转移。动态规划的边界条件为 $f[0]=0$ ，即未选择任何数时，异或值之和为 $0$ 。
```cpp
class Solution {
public:
    int minimumXORSum(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();
        vector<int> f(1 << n, INT_MAX);
        f[0] = 0;
        for (int mask = 1; mask < (1 << n); ++mask) {
            for (int i = 0; i < n; ++i) {
                if (mask & (1 << i)) {
                    f[mask] = min(f[mask], f[mask ^ (1 << i)] + (nums1[__builtin_popcount(mask) - 1] ^ nums2[i]));
                }
            }
        }
        return f[(1 << n) - 1];
    }
};
```
复杂度分析：
- 时间复杂度：$O(2^n \cdot n)$ ，其中 $n$ 是数组 $nums_1$ 和 $nums_2$ 的长度。状态的数量为 $O(2^n)$ ，每个状态需要 $O(n)$ 的时间计算结果，因此总时间复杂度为 $O(2^n \cdot n)$
- 空间复杂度：$O(2^n)$ ，即为状态的数量。

下面是记忆化搜索的代码：
```cpp
class Solution {
public:
    int minimumXORSum(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();
        vector<vector<int>> memo(n, vector<int>(1 << n, INT_MAX));
        // memo[i][j]表示在0~i在 j 代表的元素没有被选中时的最小异或值之和
        function<int(int, int)> f = [&](int i, int j) -> int {
            if (i < 0) return 0;
            int &ans = memo[i][j];
            if (ans != INT_MAX) return ans;
            for (int k = 0; k < n; ++k) 
                if ((j >> k) & 1) 
                    ans = min(ans, f(i - 1, j ^ (1 << k)) + (nums1[i] ^ nums2[k]));
            return ans;
        };
        return f(n - 1, (1 << n) - 1);
    }
};
```