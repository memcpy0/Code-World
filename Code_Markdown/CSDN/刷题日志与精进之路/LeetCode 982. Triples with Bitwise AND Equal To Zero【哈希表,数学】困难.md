> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given an integer array nums, return _the number of **AND triples**_.

An **AND triple** is a triple of indices `(i, j, k)` such that:
-   `0 <= i < nums.length`
-   `0 <= j < nums.length`
-   `0 <= k < nums.length`
-   `nums[i] & nums[j] & nums[k] == 0`, where `&` represents the bitwise-AND operator.

**Example 1:**
```java
Input: nums = [2,1,3]
Output: 12
Explanation: We could choose the following i, j, k triples:
(i=0, j=0, k=1) : 2 & 2 & 1
(i=0, j=1, k=0) : 2 & 1 & 2
(i=0, j=1, k=1) : 2 & 1 & 1
(i=0, j=1, k=2) : 2 & 1 & 3
(i=0, j=2, k=1) : 2 & 3 & 1
(i=1, j=0, k=0) : 1 & 2 & 2
(i=1, j=0, k=1) : 1 & 2 & 1
(i=1, j=0, k=2) : 1 & 2 & 3
(i=1, j=1, k=0) : 1 & 1 & 2
(i=1, j=2, k=0) : 1 & 3 & 2
(i=2, j=0, k=1) : 3 & 2 & 1
(i=2, j=1, k=0) : 3 & 1 & 2
```
**Example 2:**
```java
Input: nums = [0,0,0]
Output: 27
```
**Constraints:**
-   `1 <= nums.length <= 1000`
-   `0 <= nums[i] < 2^16`

题意：给出一个数组 `nums` ，返回**AND triple**的数量，**AND triple**是按位与为零的三元组。

---
### 解法一 二重枚举+哈希表
由于 `nums.length` 最多为1000，三重暴力循环 $O(n^3)$ 会超时。注意到题目中还给了一个限制：数组 $\textit{nums}$ 的元素不会超过 $2^{16}$ 。**这说明，$\textit{nums}[i] ~\&~ \textit{nums}[j]$ 的值也不会超过 $2^{16}$** 。

因此，我们可以先用二重循环枚举 $i$ 和 $j$ ，并使用一个长度为 $2^{16}$ 的数组（或哈希表）存储每一种 $\textit{nums}[i] ~\&~ \textit{nums}[j]$ 以及它出现的次数。随后，再使用二重循环，其中的一重枚举记录频数的数组，另一重枚举 $k$ ，这样就可以将时间复杂度从 $O(n^3)$ 降低至 $O(n^2 + 2^{16} \cdot n)$ 。
```cpp
class Solution {
public:
    int countTriplets(vector<int>& nums) {
        vector<int> cnt(1 << 16);
        for (int x: nums) {
            for (int y: nums) {
                ++cnt[x & y];
            }
        }
        int ans = 0;
        for (int x: nums) {
            for (int mask = 0; mask < (1 << 16); ++mask) {
                if ((x & mask) == 0) {
                    ans += cnt[mask];
                }
            }
        }
        return ans;
    }
};
```
---
### 解法二 二重枚举+哈希表+子集优化
在方法一的第二个二重循环中，我们需要枚举 $[0, 2^{16})$ 中的所有整数。即使用哈希表代替数组，**在数据随机的情况下，$\textit{nums}[i] ~\&~ \textit{nums}[j]$ 也会覆盖 $[0, 2^{16})$ 中的大部分整数，使哈希表不会有明显更好的表现**。

这里介绍一个常数级别的优化。在第二个二重循环中枚举 $k$ 时，希望统计出所有与 $\textit{nums}[k]$ 按位与为 $0$ 的二元组数量。也就是说：
> **如果 $\textit{nums}[k]$ 的第 $t$ 个二进制位是 $0$ ，那么二元组的第 $t$ 个二进制位才可以是 $1$ ，否则一定不能是 $1$** 。

因此，可将 $\textit{nums}[k]$ 与 $2^{16}-1$（即二进制表示下的 $16$ 个 $1$ ）进行按位异或运算，得到数 $x$ 。这样一来，==「满足要求的二元组的二进制表示」中包含的 $1$ 必须是该数 $x$ 的子集==，例如 $x$ 是 $(100111)_2$ ，那么满足要求的二元组可以是 $(100010)_2$​ 或者 $(000110)_2$ ，但不能是 $(010001)_2$ 。

说的更直白点，==如果把二进制数看成集合的话，二进制从低到高第 $i$ 位为 $1$ 表示 $i$ 在集合中，为 $0$ 表示 $i$ 不在集合中==，例如  $a=1101_{(2)}$ 表示集合 $A=\{0,2,3\}$ 。

那么 $a\ \text{AND}\ b = 0$ 相当于集合 $A$ 和集合 $B$ 没有交集，也可以理解成 $B$ 是 $\complement_{U}A$  的子集，这里全集 $U=\{0,1,2,\cdots,15\}$ ，对应的数字就是 $\text{0xffff}$ 。一个数异或 $\text{0xffff}$ 就可以得到这个数的补集了。因此，上面的代码可以**优化成枚举 $m=\textit{nums}[k] \oplus \text{0xffff}$ 表示的集合 $M$ 的子集** $S$ 。$S \cap M = S, s\ \& \ m = s$ 。
 
要想得到所有该数的子集，当然可以从 $m$ 不断减1直到0，如果 $s\ \&\ m = s$ 就表示 $s$ 是 $m$ 的子集。

更高效的是使用「**二进制枚举子集**」的技巧，直接跳到下一个子集。这里给出对应的步骤：
- 记该数为 $m$ 。我们用 $s$ 表示当前枚举到的子集。初始时 $s=m$ ，**因为 $s$ 也是本身的子集**；
- 我们不断地令 $s = (s - 1) ~\&~ m$ 。这样就可以从大到小枚举 $m$ 的所有子集。当 $s = 0$ 时枚举结束。
- 这样做的正确性在于，$s-1$ 仅把 $s$ 最低位的 $1$ 改成了 $0$ ，比这个 $1$ 更低的 $0$ 全部改成了 $1$ ，因此下一个 $m$ 的子集（比 $s$ 小）一定是 $s-1$ 的子集，直接 $\&\ m$ ，就能得到下一个子集了。
- 最后，当 $s=0$ 时，由于 $−1$ 的二进制全为 $1$ ，所以 $\&\ m = m$ ，因此可以通过判断下一个子集是否又回到 $m$ ，来判断是否要退出循环。

枚举子集相当于**一个「压缩版」的二进制减法**，比如 `10101 -> 10100 -> 10001 -> 10000 -> 00101 -> ...` 比如 `10100 -> 10001` 这一步，普通的二进制减法会把最低位的 $1$ 变成 $0$ ，这个 $1$ 右边的 $0$ 变成 $1$ ，即 `10100 -> 10011` ，对于压缩版也是类似的，但它**只保留在 `10101` 中的 $1$** ，即 `10100 -> 10001` 。怎么保留？`& 10101` 就行。

> 注：这一技巧经常用于**子集状压DP**中。
```java
class Solution {
    public int countTriplets(int[] nums) {
        var rec = new HashMap<Integer, Integer>();
        for (int i : nums)
            for (int j : nums) 
                rec.put(i & j, rec.getOrDefault(i & j, 0) + 1);
        int ans = 0;
        for (int i : nums) {
            int m = i ^ 0xffff; // i的补集m
            int s = m;
            do { // 枚举m的子集
                ans += rec.getOrDefault(s, 0);
                s = (s - 1) & m; // 求下一个子集
            } while (s != m);
        }
        return ans;
    }
}
```
或者反过来，预处理每个 $\textit{nums}[k]$ 的补集的子集的出现次数$\textit{cnt}$ ，最后累加 $\textit{cnt}[\textit{nums}[i]\ \&\ \textit{nums}[j]]$ 。

可以粗略估计这样做优化的时间复杂度：==当数据随机时，$m$ 的二进制表示中期望有 $16/2=8$ 个 $1$ ，那么「二进制枚举子集」需要枚举 $2^8$ 次==。在优化前，我们需要枚举 $2^{16}$ 次，因此常数项就缩减到原来的 $\dfrac{1}{2^8}$ ​。但在最坏情况下，$m$ 的二进制表示有 $16$ 个 $1$ ，两种方法的表现没有区别。

---
### 解法三 快速沃尔什变换（最优）
复杂度 O(n+ClogC)。
```java
void fwt(int* seq, int n) {
    for (int i = 1;i < n;i *= 2)
        for (int j = 0;j < n;j += i)
            for (int k = j + i;j < k;++j)
                seq[j] += seq[j + i];
}

class Solution {
public:
    int countTriplets(const vector<int>& nums) {
        const int m = *max_element(nums.begin(), nums.end());
        const int l = m > 0 ? 1 << (32 - __builtin_clz(m)) : 1;
        int cnt[l];
        fill_n(cnt, l, 0);
        for (int e : nums)
            ++cnt[e];
        fwt(cnt, l);
        int ans = 0;
        for (int i = 0;i <= m;++i)
            ans += (1 - 2 * __builtin_parity(i)) * cnt[i] * cnt[i] * cnt[i];
        return ans;
    }
};
```

快速沃尔什变换容易使用 SIMD 优化，优化后速度极快。

```cpp
#include<immintrin.h>

__attribute__((target("avx2")))
void fwt(int* seq, int n) {
    const auto end = seq + n;
    for (int i = 1;i < 8 && i < n;i *= 2)
        for (auto j = seq;j != end;j += i)
            for (const auto k = j + i;j < k;++j)
                *j += j[i];
    const int m = n / 8;
    const auto l = reinterpret_cast<__m256i*>(seq);
    const auto r = reinterpret_cast<__m256i*>(end);
    for (int i = 1;i < m;i *= 2)
        for (auto j = l;j != r;j += i)
            for (const auto k = j + i;j < k;++j)
                *j = _mm256_add_epi32(*j, j[i]);
}

class Solution {
public:
    int countTriplets(const vector<int>& nums) {
        const int m = *max_element(nums.begin(), nums.end());
        const int l = m > 0 ? 1 << (32 - __builtin_clz(m)) : 1;
        alignas(32) int cnt[l];
        fill_n(cnt, l, 0);
        for (int e : nums)
            ++cnt[e];
        fwt(cnt, l);
        int ans = 0;
        for (int i = 0;i <= m;++i)
            ans += (1 - 2 * __builtin_parity(i)) * cnt[i] * cnt[i] * cnt[i];
        return ans;
    }
};
```
https://oi-wiki.org//math/poly/fwt/