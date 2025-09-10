> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个字符串 `s` ，每个字符是数字 `'1'` 到 `'9'` ，再给你两个整数 `k` 和 `minLength` 。

如果对 `s` 的分割满足以下条件，那么我们认为它是一个 **完美** 分割：

- `s` 被分成 `k` 段互不相交的子字符串。
- 每个子字符串长度都 **至少** 为 `minLength` 。
- 每个子字符串的第一个字符都是一个 **质数** 数字，最后一个字符都是一个 **非质数** 数字。质数数字为 `'2'` ，`'3'` ，`'5'` 和 `'7'` ，剩下的都是非质数数字。

请你返回 `s` 的 **完美** 分割数目。由于答案可能很大，请返回答案对 `109 + 7` **取余** 后的结果。

一个 **子字符串** 是字符串中一段连续字符串序列。

**示例 1：**

**输入：**s = "23542185131", k = 3, minLength = 2
**输出：**3
**解释：**存在 3 种完美分割方案：
"2354 | 218 | 5131"
"2354 | 21851 | 31"
"2354218 | 51 | 31"

**示例 2：**

**输入：**s = "23542185131", k = 3, minLength = 3
**输出：**1
**解释：**存在一种完美分割方案："2354 | 218 | 5131" 。

**示例 3：**

**输入：**s = "3312958", k = 3, minLength = 1
**输出：**1
**解释：**存在一种完美分割方案："331 | 29 | 58" 。

**提示：**

- `1 <= k, minLength <= s.length <= 1000`
- `s` 每个字符都为数字 `'1'` 到 `'9'` 之一。

 


定义 f[i][j]f[i][j]f[i][j] 表示把 sss 的前 jjj 个字符分割成 iii 段的方案数（每段需要满足题目的后两个要求）。

定义 jjj 为分割点，等价于 s[j]s[j]s[j] 不是质数且 s[j+1]s[j+1]s[j+1] 是质数。

如果 jjj 是分割点，那么可以考虑枚举第 i−1i-1i−1 段与第 iii 段的分割点 j′j'j 
′
 ，需满足 j−j′≥minLengthj-j'\ge \textit{minLength}j−j 
′
 ≥minLength。

累加所有 f[i−1][j′]f[i-1][j']f[i−1][j 
′
 ]，记作 sum\textit{sum}sum，那么 f[i][j]=sumf[i][j]=\textit{sum}f[i][j]=sum。

每个 f[i][j]f[i][j]f[i][j] 都要这样累加就太慢了，需要优化。

我们可以从小到大同时遍历 j′j'j 
′
  和 jjj，一边更新 sum\textit{sum}sum，一边计算 f[i][j]f[i][j]f[i][j]，具体见代码。

为方便计算，定义初始值 f[0][0]=1f[0][0] = 1f[0][0]=1，表示空串的 000 个分割算作一种方案。因为这个原因，要把所有下标 jjj 向后移动一位。

答案为 f[k][n]f[k][n]f[k][n]，这里 nnn 为 sss 的长度。

还有一些剪枝和循环次数优化的小技巧，具体见代码。

Python3
Java
C++
Go
class Solution {
    const int MOD = 1e9 + 7;

    bool is_prime(char c) {
        return c == '2' || c == '3' || c == '5' || c == '7';
    }

    // 判断是否可以在 j-1 和 j 之间分割（开头和末尾也算）
    bool can_partition(string &s, int j) {
        return j == 0 || j == s.length() || !is_prime(s[j - 1]) && is_prime(s[j]);
    }

public:
    int beautifulPartitions(string &s, int k, int l) {
        int n = s.length();
        if (k * l > n || !is_prime(s[0]) || is_prime(s[n - 1])) // 剪枝
            return 0;
        int f[k + 1][n + 1]; memset(f, 0, sizeof(f));
        f[0][0] = 1;
        for (int i = 1; i <= k; ++i) {
            int sum = 0;
            // 优化：枚举的起点和终点需要给前后的子串预留出足够的长度
            for (int j = i * l; j + (k - i) * l <= n; j++) {
                if (can_partition(s, j - l)) sum = (sum + f[i - 1][j - l]) % MOD; // j'=j-l 双指针
                if (can_partition(s, j)) f[i][j] = sum;
            }
        }
        return f[k][n];
    }
};
复杂度分析
时间复杂度：O(k(n−kl))O(k(n-kl))O(k(n−kl))，其中 nnn 为 sss 的长度。
空间复杂度：O(kn)O(kn)O(kn)。注：也可以用滚动数组优化至 O(n)O(n)O(n)。
相似题目
1977. 划分数字的方案数

作者：灵茶山艾府
链接：https://leetcode.cn/problems/number-of-beautiful-partitions/solutions/1981346/dong-tai-gui-hua-jian-ji-xie-fa-xun-huan-xyw3/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。


在没想清楚或者抓牢dp数组的含义时，为后面字符留位置（即内层枚举中`for (int j = i * l; j + (k - i) * l <= n; j++)`中 j 的上限的给出是不好思考的，有点断层）所以建议大家自己写的时候直接 `j <= n;`，ac了之后再回头思考这里的优化。（当然我感觉给前面预留位置比较好想，而给后面留，最开始没想清转移过程时就理不清）

为后面字符子串预留位置是在枚举划分为 i 段时（i < k），其实是在为后面枚举划分为 i + 1 ~ k 段提供转移的基础，而我们知道在 minLength 的限制下为保证最后一/几段能划分出来，那么前几段的最后一个字符能到的地方是有限制的，而灵佬这里 `j + (k - i) * minLength <= n` 就是在做这件事，这样能防止冗余的计算。 另外，看了灵佬的视频自己写完~~中间调大半天~~，发现可以不要 `can_partition`,（虽然好像没啥优化作用）分享一下思考：

```cpp
for (int i = 1; i <= k; ++i) {
    long tot = 0;
//    for (int j = mL * i; j <= n; ++j) {
    for (int j = mL * i; j + (k - i) * mL <= n; ++j) {
        /* 
        s[j - mL] 表示第 i 段的第一个字符
        可以不判断是否能划分，因为下面质数处不会转移，所以 s[j - mL - 1] 是质数时，其对应dp值是0，对累加值无影响。
        而且不能是 !isPrime(s[j - mL - 1]) ，因为这不能保证分出来的第一个字符是质数
        而 isPrime(s[j - mL]) 可以保证第一个字符是质数，在上面推理的基础上转移有效
        */
        if (j - mL == 0 || j - mL >= 1 && isPrime(s[j - mL])) {
            tot += dp[i-1][j - mL], tot %= mod;
        }
        if (!isPrime(s[j-1]))
            dp[i][j] = (dp[i][j] + tot) % mod;
    }
}
```
对于j的每一个j0是让s的第j0到第j个字符能够作为一个分割区间

j的取值范围是[minLength*i, n]（后面没优化），可能作为分割点j0的范围是[(i-1)*minLength + 1, n - minLength + 1]

因为j0一定在j左边，而且取值的区间恰好差一个minLength+1，所以可以直接在遍历j的过程中，再用一个指针指向j0一起遍历

因为当前的j0如果满足条件需要加到j的答案中，所以要先判断j0的合理性更新sum，再判断j的合理性更新f[i][j]

j0的合理性只需要让s[j0-1]是质数

j的合理性只需要让s[j-1]是非质数


这题计算 `sum` 的思想其实和上上周的 [2463. 最小移动总距离](https://leetcode.cn/problems/minimum-total-distance-traveled/) 没啥本质区别，如果这题没想清楚的话，可以先做做 2463 找找感觉。


首先，定义 f[i][j]f[i][j]f[i][j] 为分割字符串的前 iii 字符为 j+1j+1j+1 段（也就是有 jjj 个分割点）的方案数。 则根据题意，f[i][j]=∑k=0i−minLength(f[k][j−1])\displaystyle{f[i][j] = \sum_{k=0}^{i-minLength}(f[k][j-1])}f[i][j]= 
k=0
∑
i−minLength
​
 (f[k][j−1])，其中 kkk 满足 s[k]∉’2357’,s[k+1]∈’2357’s[k] \notin \texttt{'2357'}, s[k+1] \in \texttt{'2357'}s[k]∈
/
’2357’,s[k+1]∈’2357’。

利用前缀和可以优化复杂度为 O(nk)O(nk)O(nk)。 注意特判：当一个字符串的第一个字符是质数，最后一个不是质数时，才有可能分割。否则返回 000。

const int mod = 1e9 + 7;

class Solution {
public:
    int beautifulPartitions(string s, int k, int minLength) {
        int n = s.size(),  ok[256];
        memset(ok, 0, sizeof(ok));
        ok['2'] = ok['3'] = ok['5'] = ok['7'] = 1;
        if(!(ok[s[0]] && !ok[s[n-1]])) return 0;
        
        int f[n][k];
        memset(f, 0, sizeof(f));
        for(int i = minLength-1; i < n; ++i)
            f[i][0] = 1;

        for(int j = 1; j < k; ++j) {
            for(int i = 1, sum = 0; i < n; ++i) {
                if(i - minLength >= 0 && !ok[s[i-minLength]] && ok[s[i-minLength+1]])
                    sum += f[i-minLength][j-1], sum %= mod;
                f[i][j] = sum;
            }
        }
        
        return f[n-1][k-1];
    }
};

作者：newhar
链接：https://leetcode.cn/problems/number-of-beautiful-partitions/solutions/1981398/dong-tai-gui-hua-you-hua-by-newhar-i0bs/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。