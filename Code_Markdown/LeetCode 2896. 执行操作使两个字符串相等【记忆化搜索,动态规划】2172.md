> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个下标从 **0** 开始的二进制字符串 `s1` 和 `s2` ，两个字符串的长度都是 `n` ，再给你一个正整数 `x` 。

你可以对字符串 `s1` 执行以下操作 **任意次** ：
- 选择两个下标 `i` 和 `j` ，将 `s1[i]` 和 `s1[j]` 都反转，操作的代价为 `x` 。
- 选择满足 `i < n - 1` 的下标 `i` ，反转 `s1[i]` 和 `s1[i + 1]` ，操作的代价为 `1` 。

请你返回使字符串 `s1` 和 `s2` 相等的 **最小** 操作代价之和，如果无法让二者相等，返回 `-1` 。

**注意** ，反转字符的意思是将 `0` 变成 `1` ，或者 `1` 变成 `0` 。

**示例 1：**
```js
输入：s1 = "11$0$11$0$", s2 = "0101001010", x = 2
输出：4
解释：我们可以执行以下操作：
- 选择 i = 3 执行第二个操作。结果字符串是 s1 = "110_11_11$0$" 。
- 选择 i = 4 执行第二个操作。结果字符串是 s1 = "1101_00_1$0$" 。
- 选择 i = 0 和 j = 8 ，执行第一个操作。结果字符串是 s1 = "_0_1010010_1_0" = s2 。
总代价是 1 + 1 + 2 = 4 。这是最小代价和。
```
**示例 2：**
```js
输入：s1 = "10110", s2 = "$0$11", x = 4
输出：-1
解释：无法使两个字符串相等。
```
**提示：**
- `n == s1.length == s2.length`
- `1 <= n, x <= 500`
- `s1` 和 `s2` 只包含字符 `'0'` 和 `'1'` 。

---
一个错误的贪心思路：对于两个需要反转的位置 $i$ 和 $j$ ，如果 $i+1$ 到 $j-1$ 都不需要反转，且 $j-i<x$ ，那么不断使用第二种操作直到 $i$ 和 $j$ 都反转，否则用第一种操作。这是不对的，比如
```js
s1=1$0$0011$0$001
s2=$0$$0$$0$$0$00
```
更优的做法是反转首尾的 $1$ ，再反转相邻的 $1$ 。

不论做法如何，首先需要统计 $s_1$ 和 $s_2$ 中不同字符的位置，**如果总不同位置数是奇数，则无法通过操作让二者相等，直接返回 $-1$** 。这比较明显。
> 或者注意到，反转两个做法：
> - 如果都是 $0$ 或者都是 $1$，那么反转后 $1$ 的个数会加 $2$ 或者减 $2$ ；
> - 如果一个是 $0$ 另一个是 $1$，那么反转后 $1$ 的个数不变。
> <b></b>
> 所以，无论是哪种操作，都不会改变 $s_1$ 中的 $1$ 的个数的奇偶性。那么只要 $s_1$  和 $s_2$ 的 $1$ 的个数一个是奇数一个是偶数，就直接返回 $-1$ 。
> <b></b>
> 否则，哪怕只用第二种操作（或只用第一种），都一定可以让 $s_1=s_2$ 。
### 解法1 记忆化搜索
考虑如何进行操作使总代价最小。对于两个不同位置 $i$ 和 $j$ ，要不然直接将两个位置反转，代价为 $x$ ；要不然使用第二种方法从 $i$ 反转到 $j - 1$ ，代价为 $j - i$ 。

注意到 $n≤500$ ，考虑**维护位置数组 $pos$ 的最左边 $l$ 和最右边 $r$ 两个位置，则有以下三种情况**：
- 反转 $l$ 和 $r$
- 反转 $l$ 和 $l + 1$
- 反转 $r - 1$ 和 $r$

也不细想了，直接记忆化搜索吧：
```cpp
class Solution {
public:
    int minOperations(string s1, string s2, int x) {
        vector<int> pos;
        for (int i = 0; i < s1.size(); ++i) if (s1[i] != s2[i]) pos.push_back(i);
        int n = pos.size();
        if (n & 1) return -1; // 不同位置数是奇数
        vector<vector<int>> dp(n, vector<int>(n, -1));
        function<int(int, int)> dfs = [&](int l, int r) {
            if (l >= r) return 0;
            if (dp[l][r] != -1) return dp[l][r];
            int a = min(x, pos[r] - pos[l]) + dfs(l + 1, r - 1);
            int b = min(x, pos[l + 1] - pos[l]) + dfs(l + 2, r);
            int c = min(x, pos[r] - pos[r - 1]) + dfs(l, r - 2);
            return dp[l][r] = min(a, min(b, c));
        };
        return dfs(0, n - 1);
    }
};
```
复杂度分析：
- 时间复杂度：$O(n^2)$
- 空间复杂度：$O(n^2)$

其实只维护一个位置即可，仔细想来着实妙极。即单独先消去一个 $idx$ 下标位置的元素，其代价认为是 $\frac{x}{2}$ ；消去idx和idx + 1，代价为 $pos[idx + 1] - pos[idx]$ 。
代码实现时，可以将代价先乘以 $2$ ，最后再除以 $2$ 。
```py
class Solution:
    def minOperations(self, s1: str, s2: str, x: int) -> int:
        pos = [i for i in range(len(s1)) if s1[i] != s2[i]]
        n = len(pos)
        if n % 2:
            return -1

        @lru_cache(None)
        def dfs(idx):
            if idx == n:
                return 0

            res = x + dfs(idx + 1)
            if idx != n - 1:
                res = min(res, (pos[idx + 1] - pos[idx]) * 2 + dfs(idx + 2))
            return res

        ans = dfs(0) // 2
        dfs.cache_clear()
        return ans
```
---
### 解法2 $O(n^2)$ 动态规划（免费反转次数）
做题时，可以先想 DP，再想贪心。毕竟 DP 就是对暴力搜索的优化，贪心还得思考它的正确性（或者看题目的数据范围比较小，可以往 DP 上想）。

考虑 $s_1$ 和 $s_2$ 的最后一对字符（也可以考虑第一对字符），相当于只考虑 $s_1$ 中的一个位置，不像上面一次性考虑两个不同位置：
- 如果相同，那么无需修改。
- 如果不同：
    - 选择第一种操作，支付代价后，相当于后面可以「免费」反转一个字符。
    - 选择第二种操作，那么下一个字符要把 $0$ 看作 $1$ ，把 $1$ 看作 $0$ 。

所以除了知道**当前下标 $i$** ，还需要知道**免费反转次数 $j$** ，以及**上一个字符是否选择了第二种操作 $\textit{preRev}$** 。

定义 $\textit{dfs}(i,j,\textit{preRev})$ ，参数含义如上，返回值是在这种状态下的最小操作代价之和。分类讨论：
- 如果 `(s1[i] == s2[i]) == (not pre_rev)` ，表示 $s_1[i]$ 和 $s_2[i]$ 是相等的（或者 `(s1[i] == s2[i] && !pre_rev) || (s1[i] != s2[i] && pre_rev)`），无需操作，返回 $\textit{dfs}(i - 1, j, \text{false})$ 。
- 否则：
    - 选择第一种操作：$\textit{dfs}(i - 1, j + 1, \text{false}) + x$ 。
    - 选择第二种操作：$\textit{dfs}(i - 1, j, \text{true}) + 1$ 。
    - 如果 $j>0$ ，免费反转一次：$\textit{dfs}(i - 1, j-1, \text{false})$ 。
    这三种情况取最小值。

递归边界，当 $i<0$ 时：
- 如果 $j>0$ 或者 $\textit{preRev}$ 为真，那么不合法，返回 $\infty$ 。
- 否则返回 $0$ 。

递归入口：$\textit{dfs}(n1,0,\text{false})$ ，即答案。
```js
class Solution {
public:
    int minOperations(string s1, string s2, int x) {
        if (count(s1.begin(), s1.end(), '1') % 2 != count(s2.begin(), s2.end(), '1') % 2) {
            return -1;
        }
        int n = s1.length();
        int memo[n][n + 1][2];
        memset(memo, -1, sizeof(memo)); // -1 表示没有计算过
        function<int(int, int, bool)> dfs = [&](int i, int j, bool pre_rev) -> int {
            if (i < 0) {
                return j || pre_rev ? INT_MAX / 2 : 0;
            }
            int &res = memo[i][j][pre_rev]; // 注意这里是引用
            if (res != -1) { // 之前计算过
                return res;
            }
            if ((s1[i] == s2[i]) == !pre_rev) { // 无需反转
                return dfs(i - 1, j, false);
            }
            res = min(dfs(i - 1, j + 1, false) + x, dfs(i - 1, j, true) + 1);
            if (j) { // 可以免费反转
                res = min(res, dfs(i - 1, j - 1, false));
            }
            return res;
        };
        return dfs(n - 1, 0, false);
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n^2)$ ，其中 $n$ 为 $s_1$ 的长度。由于每个状态只会计算一次，动态规划的时间复杂度 === 状态个数 $\times$ 单个状态的计算时间。本题中状态个数等于 $\mathcal{O}(n^2)$ ，单个状态的计算时间为 $\mathcal{O}(1)$ ，所以动态规划的时间复杂度为 $\mathcal{O}(n^2)$ 。
- 空间复杂度：$\mathcal{O}(n^2)$ 。

---
方法二：O(n) DP
把 s1[i]≠s2[i]s_1[i]\ne s_2[i]s 
1
​
 [i]

=s 
2
​
 [i] 的下标统计到数组 ppp 中。如果 ppp 的长度是奇数，那么和上面一样，无法操作，返回 −1-1−1。

设 f[i]f[i]f[i] 表示修改 ppp 的前 iii 个位置的最小花费，那么对于 p[i]p[i]p[i]，有两种方案：

第一种操作：花费 xxx，那么对于 p[i]p[i]p[i] 相当于花费 x2\dfrac{x}{2} 
2
x
​
 ，因此

f[i]=f[i−1]+x2f[i] = f[i-1] + \dfrac{x}{2}
f[i]=f[i−1]+ 
2
x
​
 
注意这个转移一定会发生偶数次，因为 ppp 的长度是偶数，并且第二种操作每次反转两个数，所以第一种操作一定会反转偶数个下标。

第二种操作：需要不断用相邻的位置操作，把 p[i]p[i]p[i] 和 p[i−1]p[i-1]p[i−1] 都反转，那么需要操作 p[i]−p[i−1]p[i]-p[i-1]p[i]−p[i−1] 次，因此

f[i]=f[i−2]+p[i]−p[i−1]f[i] = f[i-2] + p[i]-p[i-1]
f[i]=f[i−2]+p[i]−p[i−1]
两者取最小值，即

f[i]=min⁡(f[i−1]+x2,f[i−2]+p[i]−p[i−1])f[i] = \min(f[i-1] + \dfrac{x}{2}, f[i-2] + p[i]-p[i-1])
f[i]=min(f[i−1]+ 
2
x
​
 ,f[i−2]+p[i]−p[i−1])
代码实现时，为了方便处理 x2\dfrac{x}{2} 
2
x
​
 ，可以先在计算过程中把所有数都乘 222，最后返回答案的时候再除以 222。所以转移方程就是

f[i]=min⁡(f[i−1]+x,f[i−2]+(p[i]−p[i−1])⋅2)f[i] = \min(f[i-1] + x, f[i-2] + (p[i]-p[i-1]) \cdot 2)
f[i]=min(f[i−1]+x,f[i−2]+(p[i]−p[i−1])⋅2)
初始值 f[−1]=0f[-1]=0f[−1]=0，f[0]=xf[0]=xf[0]=x，答案为 f[m−1]2\dfrac{f[m-1]}{2} 
2
f[m−1]
​
 ，这里 mmm 是数组 ppp 的长度。你可以把 fff 数组的下标都加一来避免负数，也可以用两个变量滚动计算。

Python3
Java
C++
Go
class Solution {
public:
    int minOperations(string s1, string s2, int x) {
        if (s1 == s2) return 0;
        vector<int> p;
        for (int i = 0; i < s1.size(); i++)
            if (s1[i] != s2[i])
                p.push_back(i);
        if (p.size() % 2) return -1;
        int f0 = 0, f1 = x;
        for (int i = 1; i < p.size(); i++) {
            int new_f = min(f1 + x, f0 + (p[i] - p[i - 1]) * 2);
            f0 = f1;
            f1 = new_f;
        }
        return f1 / 2;
    }
};
复杂度分析
时间复杂度：O(n)\mathcal{O}(n)O(n)，其中 nnn 为 s1s_1s 
1
​
  的长度。
空间复杂度：O(n)\mathcal{O}(n)O(n)。
思考题
如果第一种操作限制 ∣i−j∣≥k|i-j|\ge k∣i−j∣≥k 呢？

作者：灵茶山艾府
链接：https://leetcode.cn/problems/apply-operations-to-make-two-strings-equal/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。