> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个正整数 `n` ，你需要找到一个下标从 **0** 开始的数组 `powers` ，它包含 **最少** 数目的 `2` 的幂，且它们的和为 `n` 。`powers` 数组是 **非递减** 顺序的。根据前面描述，构造 `powers` 数组的方法是唯一的。

同时给你一个下标从 **0** 开始的二维整数数组 `queries` ，其中 `queries[i] = [lefti, righti]` ，其中 `queries[i]` 表示请你求出满足 `lefti <= j <= righti` 的所有 `powers[j]` 的乘积。

请你返回一个数组 `answers` ，长度与 `queries` 的长度相同，其中 `answers[i]`是第 `i` 个查询的答案。由于查询的结果可能非常大，请你将每个 `answers[i]` 都对 `109 + 7` **取余** 。

**示例 1：**

**输入：**n = 15, queries = [[0,1],[2,2],[0,3]]
**输出：**[2,4,64]
**解释：**
对于 n = 15 ，得到 powers = [1,2,4,8] 。没法得到元素数目更少的数组。
第 1 个查询的答案：powers[0] * powers[1] = 1 * 2 = 2 。
第 2 个查询的答案：powers[2] = 4 。
第 3 个查询的答案：powers[0] * powers[1] * powers[2] * powers[3] = 1 * 2 * 4 * 8 = 64 。
每个答案对 109 + 7 得到的结果都相同，所以返回 [2,4,64] 。

**示例 2：**

**输入：**n = 2, queries = [[0,0]]
**输出：**[2]
**解释：**
对于 n = 2, powers = [2] 。
唯一一个查询的答案是 powers[0] = 2 。答案对 109 + 7 取余后结果相同，所以返回 [2] 。

**提示：**

- `1 <= n <= 10^9`
- `1 <= queries.length <= 10^5`
- `0 <= starti <= endi < powers.length`

---
```cpp
class Solution {
private:
    int fastPow(int a, int b, int p) {
        int ans = 1;
        while (b) {
            if (b & 1) ans = ((long long)ans * a) % p;
            a = ((long long)a * a) % p;
            b >>= 1;
        }
        return ans;
    }
public:
    vector<int> productQueries(int n, vector<vector<int>>& queries) {
        vector<long long> p2;
        const int mod = 1e9 + 7;
        for (int i = 0; i < 32; ++i) {
            if ((n >> i) & 1) {
                if (!p2.empty()) p2.push_back(i + p2.back());
                else p2.push_back(i);
                cout << p2.back();
            }
        }
        
        vector<int> ans;
        for (vector<int> &q : queries) {
            if (q[0] == 0) {
                ans.push_back( fastPow(2, p2[q[1]] , mod) );
            }
            else ans.push_back( fastPow (2, p2[q[1]] - p2[q[0] - 1] , mod) );
        }
        return ans;
    }
};
```