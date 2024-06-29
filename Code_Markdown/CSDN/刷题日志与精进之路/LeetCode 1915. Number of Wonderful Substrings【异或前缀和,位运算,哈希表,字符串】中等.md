> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

如果某个字符串中 **至多一个** 字母出现 **奇数** 次，则称其为 **最美** 字符串。
- 例如，`"ccjjc"` 和 `"abab"` 都是最美字符串，但 `"ab"` 不是。

给你一个字符串 `word` ，该字符串由前十个小写英文字母组成（`'a'` 到 `'j'`）。请你返回 `word` 中 **最美非空子字符串** 的数目。_如果同样的子字符串在 `word` 中出现多次，那么应当对 **每次出现** 分别计数_。

**子字符串** 是字符串中的一个连续字符序列。

**示例 1：**
```java
输入：word = "aba"
输出：4
```
解释：4 个最美子字符串如下所示：
- "**a**ba" -> "a"
- "a**b**a" -> "b"
- "ab**a**" -> "a"
- "**aba**" -> "aba"

**示例 2：**
```java
输入：word = "aabb"
输出：9
```
**解释：** 9 个最美子字符串如下所示：
- "**a**abb" -> "a"
- "**aa**bb" -> "aa"
- "**aab**b" -> "aab"
- "**aabb**" -> "aabb"
- "a**a**bb" -> "a"
- "a**abb**" -> "abb"
- "aa**b**b" -> "b"
- "aa**bb**" -> "bb"
- "aab**b**" -> "b"

**示例 3：**
```java
输入：word = "he"
输出：2
```
**解释：** 2 个最美子字符串如下所示：
- "**h**e" -> "h"
- "h**e**" -> "e"

**提示：**
- `1 <= word.length <= 10^5`
- `word` 由从 `'a'` 到 `'j'` 的小写英文字母组成

---
与本题相似的题目（**前缀和+异或+字符串**）如下：
- [1177. 构建回文串检测](https://leetcode.cn/problems/can-make-palindrome-from-substring)
- [1371. 每个元音包含偶数次的最长子字符串](https://leetcode.cn/problems/find-the-longest-substring-containing-vowels-in-even-counts/)
- [1542. 找出最长的超赞子字符串](https://leetcode.cn/problems/find-longest-awesome-substring/) 

还有些使用前缀和的题目：
- [560. 和为 K 的子数组](https://leetcode-cn.com/problems/subarray-sum-equals-k/)
- [930. 和相同的二元子数组](https://leetcode-cn.com/problems/binary-subarrays-with-sum/)
- [974. 和可被 K 整除的子数组](https://leetcode-cn.com/problems/subarray-sums-divisible-by-k/)
- [1590. 使数组和能被 P 整除](https://leetcode-cn.com/problems/make-sum-divisible-by-p/)
### 解法 奇偶性
由于我们只关心每个字母出现次数的奇偶性，因此可以将「字母出现次数」转换成「字母出现次数的奇偶性」，这可以用一个长为 $10$ 的二进制串表示，二进制串的第 $i$ 位为 $0$ 表示第 $i$ 个小写字母出现了偶数次，为 $1$ 表示第 $i$ 个小写字母出现了奇数次。

考虑字母出现次数的前缀和，由于只考虑奇偶性，我们也可以将其视作一个长为 $10$ 的二进制串。此时计算前缀和由加法运算改为异或运算，这是因为异或运算的本质是在模 $2$ 剩余系中进行加法运算，刚好对应奇偶性的变化。

若有两个不同下标的前缀和相同，则这两个前缀和的异或结果为 $0$ ，意味着==这段子串的各个字母的个数均为偶数==，符合题目要求。因此可在求前缀和的同时，用一个长为 $2^{10}=1024$ 的 $cnt$ 数组，统计每个前缀和二进制串出现的次数，从而得到**相同前缀和的对数**，即**各个字母的个数均为偶数的子串**个数。

题目还允许有一个字母出现奇数次，这需要我们**寻找两个前缀和，其异或结果的二进制数中恰好有一个 $1$** ，意味着==这段子串的各个字母的个数仅有一个为奇数==。对此可以枚举当前前缀和的每个比特，将其反转，然后去 $cnt$ 中查找该前缀和的出现次数。将所有统计到的次数累加即为答案。
```cpp
class Solution {
public:
    long long wonderfulSubstrings(string word) {
        int n = word.size(), xor_sum = 0;
        long long ans = 0;
        int cnt[1024] = {0};
        cnt[0] = 1; // 初始前缀和为0,需将其计入出现次数
        for (char c : word) {
            xor_sum ^= (1 << (c - 'a')); // 计算当前前缀和
            ans += cnt[xor_sum]; // 所有字母均出现偶数次
            for (int j = 0; j <= 9; ++j) // 枚举其中一个字母出现奇数次
                ans += cnt[xor_sum ^ (1 << j)]; // 反转该字母的出现次数的奇偶性
            ++cnt[xor_sum]; // 更新前缀和出现次数
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(10⋅n)$ ，$n$ 为字符串 $word$ 的长度。
- 空间复杂度：$O(S)$ ，$S=1024$ 为十个小写字母的奇偶性映射为二进制串的状态数。