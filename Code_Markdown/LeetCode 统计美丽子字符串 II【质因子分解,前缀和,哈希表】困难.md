> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个字符串 `s` 和一个正整数 `k` 。

用 `vowels` 和 `consonants` 分别表示字符串中元音字母和辅音字母的数量。

如果某个字符串满足以下条件，则称其为 **美丽字符串** ：
- `vowels == consonants`，即元音字母和辅音字母的数量相等。
- `(vowels * consonants) % k == 0`，即元音字母和辅音字母的数量的乘积能被 `k` 整除。

返回字符串 `s` 中 **非空美丽子字符串** 的数量。

子字符串是字符串中的一个连续字符序列。

英语中的 **元音字母** 为 `'a'`、`'e'`、`'i'`、`'o'` 和 `'u'` 。

英语中的 **辅音字母** 为除了元音字母之外的所有字母。

**示例 1：**
```js
输入：s = "baeyh", k = 2
输出：2
解释：字符串 s 中有 2 个美丽子字符串。
- 子字符串 "b_aeyh_"，vowels = 2（["a","e"]），consonants = 2（["y","h"]）。
可以看出字符串 "aeyh" 是美丽字符串，因为 vowels == consonants 且 vowels * consonants % k == 0 。
- 子字符串 "_baey_h"，vowels = 2（["a","e"]），consonants = 2（["b","y"]）。
可以看出字符串 "baey" 是美丽字符串，因为 vowels == consonants 且 vowels * consonants % k == 0 。
可以证明字符串 s 中只有 2 个美丽子字符串。
```
**示例 2：**
```js
输入：s = "abba", k = 1
输出：3
解释：字符串 s 中有 3 个美丽子字符串。
- 子字符串 "_ab_ba"，vowels = 1（["a"]），consonants = 1（["b"]）。
- 子字符串 "ab_ba_"，vowels = 1（["a"]），consonants = 1（["b"]）。
- 子字符串 "_abba_"，vowels = 2（["a","a"]），consonants = 2（["b","b"]）。
可以证明字符串 s 中只有 3 个美丽子字符串。
```
**示例 3：**
```js
输入：s = "bcdf", k = 1
输出：0
解释：字符串 s 中没有美丽子字符串。
```
**提示：**
- `1 <= s.length <= 5 * 10^4`
- `1 <= k <= 1$0$`
- `s` 仅由小写英文字母组成。

---
### 解法 分解质因子+前缀和+哈希表
把元音视作 $1$，辅音视作 $-1$ 。「元音字母和辅音字母的数量相等」就等价于：==找到一个和为 $0$ 的连续子数组==。注意**这种子数组的长度一定是偶数**，因为元音辅音数量相等。

设子数组的长度为 $L$ 。由于元音辅音数量相等，所以**元音辅音数量都等于 $\dfrac{L}{2}$​** ，所以「元音字母和辅音字母的数量的乘积能被 $k$ 整除」等价于
$$\left(\dfrac{L}{2}\right)^2 \bmod k = 0$$
这等价于
$$L^2 \bmod (4k) = 0$$
这个平方很烦人，如果能去掉平方就好做了。

**这里得来点数学**。我们来研究下，如果一个数 $L$ 的平方能被 $n$ 整除，意味着什么？
- **假设 $n$ 是一个质数**，例如 $3$，那么 $L$ 必须包含质因子 $3$ ，此时问题就变成了：判断 $L$ 能不能被 $3$ 整除。我们把平方去掉了！
- **如果 $n$ 是一个质数 $p 的 $e$ 次幂**呢？分类讨论：
    - 如果 $e$是偶数，比如 $n=3^4$ ，那么 $L$ 必须包含因子 $3^2$ ，才能使得 $L^2$ 能被 $n$ 整除。此时问题就变成了：判断 $L$ 能不能被 $3^2$ 整除。
    - 如果 $e$ 是奇数，比如 $n=3^5$ ，那么 $L$ 必须包含因子 $3^3$ ，才能使得 $L^2$ 能被 $n$ 整除。此时问题就变成了：判断 $L$ 能不能被 $3^3$ 整除。
   所以 $L$ 必须包含因子 $p^r$ ，其中 $r=\left\lceil\dfrac{e}{2}\right\rceil = \left\lfloor\dfrac{e+1}{2}\right\rfloor$ 。
- 如果 $n$ 可以分解出多个质因子，只需要把每个质因子及其幂次按照上面的方法处理，然后再相乘，就得到 $L$ 必须包含什么因子。

**继续**
把 $4$ 按照上述方法计算，设 $L$ 必须包含因子 $k'$ 。现在问题变成，有多少个和为 $0$ 的子数组，其长度是 $k'$ 的倍数？

设子数组的下标范围为 $[j,i)$ ，那么其长度 $L=i-j$ ，则有
$$(i-j)\bmod k' = 0$$
等价于
$$i \bmod k' = j\bmod k'$$
对于元素和来说，相当于 $s[i]-s[j] = 0$ ，即
$$s[i] = s[j]$$
我们需要同时满足这两个条件（下标模 $k'$ 相等，$s$ 值相等），这可以一并用哈希表解决。**哈希表的 $key$ 是一个 $pair$ ：$(i\bmod k', s[i])$ ，哈希表的 $value$ 是这个 $pair$ 的出现次数**。

代码实现时，**前缀和数组可以用一个变量表示**。

代码实现时，可以把 `aeiou` 压缩成一个二进制数，从而**快速判断字母是否为元音**。
> 问：为什么哈希表要在一开始插入一个 $(k'-1, 0)$ ？
> 答：前缀和的第一项是 $0$，由于代码中是从下标 $0$ 开始算第二个前缀和，所以相当于 $s[−1]=0$ ，而 $k'-1$ 和 $-1$ 关于 $k'$ 同余，所以插入 $(k′−1,0)$ 。

```cpp
class Solution {
private:
    int pSqrt(int n) {
        int ans = 1;
        for (int i = 2; i * i <= n; ++i) {
            int i2 = i * i;
            while (n % i2 == 0) { // 质因数分解
                ans *= i; // L必须包含一个i^1
                n /= i2;
            }
            if (n % i == 0) { // 如果n包含某个质数的奇次幂
                ans *= i;
                n /= i;
            }
        }
        if (n > 1) ans *= n; // 剩下的最后1个质因子
        return ans;
    }
    struct PairHash {
        template<typename T1, typename T2>
        size_t operator() (const pair<T1, T2> &p) const {
            auto v1 = std::hash<T1>{}(p.first);
            auto v2 = std::hash<T2>{}(p.second);
            return v1 ^ v2;
        }
    };
    unordered_map<pair<int, int>, int, PairHash> cnt; //ok
public:
    int beautifulSubstrings(string s, int k) {
        k = pSqrt(k * 4); // 4k的质因数分解，求出L必须包含的因子值（多个质因子的幂次的乘积）
        const int aeiouMask = 1065233;
        cnt[ pair<int, int>{ k - 1, 0} ] = 1; // k-1和1同余
        int sum = 0;
        int ans = 0;
        for (int i = 0; i < s.size(); ++i) {
            char c = s[i];
            int bit = aeiouMask >> (c - 'a') & 1;
            sum += bit * 2 - 1; // 1->1, 0->-1
            auto p = pair{i % k, sum};
            ans += (long long)cnt[p];
            ++cnt[p];
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n + \sqrt k)$ ，其中 $n$ 为 $\textit{nums}$ 的长度。
- 空间复杂度：$\mathcal{O}(n)$ 。


