> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个字符串 `s`，请你对 `s` 的子串进行检测。

每次检测，待检子串都可以表示为 `queries[i] = [left, right, k]`。我们可以 **重新排列** 子串 `s[left], ..., s[right]`，并从中选择 **最多** `k` 项替换成任何小写英文字母。 

如果在上述检测过程中，子串可以变成回文形式的字符串，那么检测结果为 `true`，否则结果为 `false`。

返回答案数组 `answer[]`，其中 `answer[i]` 是第 `i` 个待检子串 `queries[i]` 的检测结果。

注意：在替换时，子串中的每个字母都必须作为 **独立的** 项进行计数，也就是说，如果 `s[left..right] = "aaa"` 且 `k = 2`，我们只能替换其中的两个字母。（另外，任何检测都不会修改原始字符串 `s`，可以认为每次检测都是独立的）

**示例：**
```java
输入：s = "abcda", queries = [[3,3,0],[1,2,0],[0,3,1],[0,3,2],[0,4,1]]
输出：[true,false,false,true,true]
解释：
queries[0] : 子串 = "d"，回文。
queries[1] : 子串 = "bc"，不是回文。
queries[2] : 子串 = "abcd"，只替换 1 个字符是变不成回文串的。
queries[3] : 子串 = "abcd"，可以变成回文的 "abba"。 也可以变成 "baab"，先重新排序变成 "bacd"，然后把 "cd" 替换为 "ab"。
queries[4] : 子串 = "abcda"，可以变成回文的 "abcba"。
```
**提示：**
- `1 <= s.length, queries.length <= 10^5`
- `0 <= queries[i][0] <= queries[i][1] < s.length`
- `0 <= queries[i][2] <= s.length`
- `s` 中只有小写英文字母

---
与本题相似的题目（前缀和+异或）如下，还使用了哈希表：
- [1371. 每个元音包含偶数次的最长子字符串](https://leetcode.cn/problems/find-the-longest-substring-containing-vowels-in-even-counts/)
- [1915. 最美子字符串的数目](https://leetcode.cn/problems/number-of-wonderful-substrings/) 
- [1542. 找出最长的超赞子字符串](https://leetcode.cn/problems/find-longest-awesome-substring/)，与本题十分相似，建议先做本题与上两题，然后会发现这道困难题很简单。
 
思考如下问题：
- 回文意味着什么？
- 题目允许重新排列字母，那么它要**如何排列**？偶数个 $a$ 要如何排列？奇数个 $a$ 要如何排列？
- 什么情况下**一定**要替换字母？要替换多少个？
- 如何快速求出子串中每种字母的个数？

不难发现：
- 回文意味着，从左往右第 $i$ 个字母和从右往左第 $i$ 个字母是相同的，即**回文串关于回文中心是对称的**。
- 如果有偶数个 $a$ ，那么可以均分成两部分，分别放置在字符串的中心对称位置上。例如有 $4$ 个 $a$ ，可以在字符串的最左边放置 $2$ 个 $a$ ，最右边放置 $2$ 个 $a$ ，这样字符串中的 $a$ 是回文的。其它字母如果出现偶数次，也同理。
- 如果有奇数个 $a$ ，多出的一个 $a$ 要单独拿出来讨论：
    - 假如**只有 $a$ 出现奇数次**，其它字母都出现偶数次。此时字符串的长度一定是奇数，那么可以把多出的这个 $a$ 放在字符串的中心，我们仍然可以得到一个回文串，**无需替换任何字母**。
    - 如果**有两种字母出现奇数次**（假设是字母 $a,b$ ），由于多出的一个 $a$ 和一个 $b$ 无法组成回文串，可以把一个 $b$ 改成 $a$（或者把一个 $a$ 改成 $b$ ），这样 $a$ 和 $b$ 就都出现偶数次了。
    - 如果**有三种字母出现奇数次**（假设是字母 $a,b,c$ ），把一个 $b$ 改成 $c$ ，就转换成只有 $a$ 出现奇数次的情况了。
- 一般地，**如果有 $m$ 种字母出现奇数次，只需替换其中 $\left\lfloor\dfrac{m}{2}\right\rfloor$ 个字母**。换句话说，如果第 $i$ 次询问有 $\left\lfloor\dfrac{m}{2}\right\rfloor\le k$ ，那么 $answer[i]$ 为真，反之为假。
- 最后要解决的问题是，如何**快速求出子串中每种字母的个数**？
    我们可以创建 $26$ 个前缀和数组，分别统计每种字母在前缀中的出现次数。以字母 $a$ 为例，在计算前缀和时，如果 $s[i]=a$ 就视作 $1$ ，否则视作 $0$ 。

### 解法1 前缀和（优化前）
预处理 $s$ 的长为 $i$ 的前缀中，每种字母各出现多少次。为方便后续优化，这里用 $n\times 26$ 的二维数组 $sum$ 存储前缀和，其中 $sum[i+1][j]$ 表示从 $s[0]$ 到 $s[i]$ 中，字母表第 $j$ 个小写字母的出现次数。

对于 $queries[i]$ ，利用前缀和计算出**子串中每种字母出现次数**，统计有多少种字母出现奇数次，设为 $m$ 。如果 $\left\lfloor\dfrac{m}{2}\right\rfloor\le k$ ，那么 $answer[i]$ 为真，反之为假。
 ```cpp
class Solution {
public:
    vector<bool> canMakePaliQueries(string s, vector<vector<int>>& queries) {
        int n = s.size();
        vector<array<int, 26>> sum(n + 1); // sum[n+1][26]
        for (int i = 0; i < n; ++i) {
            sum[i + 1] = sum[i];
            ++sum[i + 1][s[i] - 'a']; // 计算前缀中每个字母的出现次数
        }
        vector<bool> ans;
        for (vector<int> &q : queries) {
            int l = q[0], r = q[1], k = q[2], m = 0;
            for (int j = 0; j < 26; ++j)
                m += (sum[r + 1][j] - sum[l][j]) % 2; // 奇数+1,偶数+0
            ans.push_back(m / 2 <= k);
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O((n+q)⋅∣Σ∣)$ ，其中 $n$ 为 $s$ 的长度，$q$ 为 $queries$ 的长度，$|\Sigma|$ 为字符集合的大小。本题中字符均为小写字母，所以 $∣Σ∣=26|$ 。
- 空间复杂度：$O(n⋅∣Σ∣)$ 。返回值不计入。

### 解法2 一步步优化
==由于只关心每种字母出现次数的奇偶性，所以不需要在前缀和中存储每种字母的出现次数，只需要保存每种字母出现次数的奇偶性==。为方便计算，用 $0$ 表示出现偶数次，用 $1$ 表示出现奇数次。

注意，这时的前缀和中只有 $0, 1$ 。**如果要判断子串中每种字母出现次数的奇偶性，就只有奇数减偶数，或者偶数减奇数，才能得到奇数**。所以如果相减的结果不为 $0$（或者说相减的两个数不相等），就表示出现了奇数次。
```cpp
class Solution {
public:
    vector<bool> canMakePaliQueries(string s, vector<vector<int>>& queries) {
        int n = s.size();
        vector<array<int, 26>> sum(n + 1); // sum[n+1][26]
        for (int i = 0; i < n; ++i) {
            sum[i + 1] = sum[i];
            ++sum[i + 1][s[i] - 'a']; // 计算前缀中每个字母的奇偶性
            sum[i + 1][s[i] - 'a'] &= 1;
        }
        vector<bool> ans;
        for (vector<int> &q : queries) {
            int l = q[0], r = q[1], k = q[2], m = 0;
            for (int j = 0; j < 26; ++j)
                m += sum[r + 1][j] != sum[l][j];
            ans.push_back(m / 2 <= k);
        }
        return ans;
    }
};
```
由于异或运算满足 $1$ 和 $0$ 的结果是 $1$ ，而 $0$ 和 $0$ 、$1$ 和 $1$ 的结果都是 $0$ ，所以可以用异或替换上面的减法。
> 怎么想到异或的？异或可以视作「不进位加法（减法）」或者「模 $2$ 剩余系中的加法（减法）」，所以也==常常用来解决一些和奇偶性有关的问题==。
```cpp
class Solution {
public:
    vector<bool> canMakePaliQueries(string s, vector<vector<int>>& queries) {
        int n = s.size();
        vector<array<int, 26>> sum(n + 1); // sum[n+1][26]
        for (int i = 0; i < n; ++i) {
            sum[i + 1] = sum[i]; 
            sum[i + 1][s[i] - 'a'] ^= 1; // 奇数次变偶数次,偶数次变奇数次
        }
        vector<bool> ans;
        for (vector<int> &q : queries) {
            int l = q[0], r = q[1], k = q[2], m = 0;
            for (int j = 0; j < 26; ++j)
                m += sum[r + 1][j] ^ sum[l][j];
            ans.push_back(m / 2 <= k);
        }
        return ans;
    }
};
```
由于**长为 $26$ 的数组中只存储 $0$ 和 $1$** ，可以压缩到一个二进制数中，二进制数从低到高第 $i$ 个比特存储着 $0$ 和 $1$ 的信息。例如二进制 $10010$ 表示 $b$ 和 $e$ 出现奇数次，其余字母出现偶数次。

在计算前缀和时（准确地说是异或前缀和）：
- 修改 $a$ 出现次数的奇偶性，可以异或二进制 $1$ ；
- 修改 $b$ 出现次数的奇偶性，可以异或二进制 $10$ ；
- 修改 $c$ 出现次数的奇偶性，可以异或二进制 $100$ ；
- 依此类推。

此外，由于异或可以「并行计算」，==对前缀和中的两个二进制数直接异或，便得到了子串中每种字母出现次数的奇偶性==。再计算这个二进制数中的 $1$ 的个数，便得到了 $m$ 。

例如 $10010⊕01110=11100$ ，说明有 $3$ 种字母出现奇数次。
```cpp
class Solution {
public:
    vector<bool> canMakePaliQueries(string s, vector<vector<int>>& queries) {
        int n = s.size();
        vector<int> xor_sum(n + 1);
        for (int i = 0; i < n; ++i) // 对应字母的奇偶性:奇数变偶数,偶数变奇数
            xor_sum[i + 1] = xor_sum[i] ^ (1 << (s[i] - 'a')); 
        vector<bool> ans;
        for (vector<int>& q : queries) {
            int l = q[0], r = q[1], k = q[2]; 
            int m = __builtin_popcount(xor_sum[r + 1] ^ xor_sum[l]) - ((r - l + 1) & 1); 
            ans.push_back(m / 2 <= k);
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n+q)$ ，其中 $n$ 为 $nums$ 的长度，$q$ 为 $queries$ 的长度。
- 空间复杂度：$O(n)$ 。

 
 