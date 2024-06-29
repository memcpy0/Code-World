> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given 2 integers `n` and `start`. Your task is return **any** permutation `p` of `(0,1,2.....,2^n -1)` such that:
-   `p[0] = start`
-   `p[i]` and `p[i+1]` differ by only one bit in their binary representation.
-   `p[0]` and `p[2^n -1]` must also differ by only one bit in their binary representation.

**Example 1:**
```java
Input: n = 2, start = 3
Output: [3,2,0,1]
Explanation: The binary representation of the permutation is (11,10,00,01). 
All the adjacent element differ by one bit. Another valid permutation is [3,1,0,2]
```
**Example 2:**
```java
Input: n = 3, start = 2
Output: [2,6,7,5,4,0,1,3]
Explanation: The binary representation of the permutation is (010,110,111,101,100,000,001,011).
```
**Constraints:**
-   `1 <= n <= 16`
-   `0 <= start < 2 ^ n`

题意：给出两个整数 `n` 和 `start`。任务是返回任意 `(0,1,2,,...,2^n-1)` 的排列 `p`，并且满足：
-   `p[0] = start`
-   `p[i]` 和 `p[i+1]` 的二进制表示形式只有一位不同
-   `p[0]` 和 `p[2^n -1]` 的二进制表示形式也只有一位不同

---
### 解法 数学（循环码）
本题和[89. 格雷编码](https://leetcode.cn/problems/gray-code/solution/89-ge-lei-bian-ma-ti-jie-di-zeng-an-wei-nhqgn/)非常相似，区别在于89.格雷编码要求第一个整数是0，而本题要求第一个整数是 $\textit{start}$ 。

观察题目中的排列，可以发现，==它的二进制表示中，任意两个（包括首尾）相邻的数只有一位二进制数不同==。这种编码方式就是格雷码，我们在工程中会遇到的一种编码方式。

二进制码转换成二进制格雷码，其法则是**保留二进制码的最高位作为格雷码的最高位**，而**次高位格雷码为二进制码的高位与次高位相异或**，而格雷码其余各位与次高位的求法相类似。

假设某个二进制数表示为 $B_{n-1}B_{n-2}...B_2B_1B_0$ ，其格雷码表示为 $G_{n-1}G_{n-2}...G_2G_1G_0$ ​。最高位保留，所以 $G_{n-1} = B_{n-1}$ ；而其它各位 $G_i = B_{i+1} \oplus B_{i}$ ，其中 $i=0,1,2..,n−2$ 。因此，**对于一个整数 $x$ ，我们可以用 `x ^ (x >> 1)` 得到其格雷码**。

可以直接将 $[0,\dots, 2^n - 1]$ 这些整数转换成对应的格雷码数组，然后找到 $start$ 在格雷码数组中的位置，==将格雷码数组从该位置开始截取，再将截取的部分拼接到格雷码数组的前面==，就得到了题目要求的排列。这也是我一开始的代码：
- 时间复杂度：$O(2^n)$
- 空间复杂度：$O(2^n)$ 
```cpp
class Solution {
public:
    vector<int> circularPermutation(int n, int start) {
        vector<int> ans, circ;
        int pos, tot = 1 << n;
        for (int i = 0; i < tot; ++i) { // 这种算法下
            circ.push_back(i ^ (i >> 1)); // 从0到第tot个transnum形成循环码
            if (circ.back() == start) pos = i; // 找到start在这个循环码数组中的位置
        }
        for (int i = 0; i < tot; ++i) {
            ans.push_back(circ[pos]); 
            pos = (pos + 1) % tot; // 然后从该位置开始循环
        }
        return ans;
    }
};
```
对上述代码进行优化。由于 $gray(0) = 0$ ，那么 $gray(0) \oplus start = start$ ，而 $gray(i)$ 与 $gray(i−1)$ 只有一个二进制位不同，所以 **$gray(i) \oplus start$ 与 $gray(i-1) \oplus start$ 也只有一个二进制位不同**。因此，我们也可以直接将 $[0,..2^n - 1]$ 这些整数转换成对应的 $gray(i) \oplus start$ ，即可得到首项为 $start$ 的格雷码排列。
```java
class Solution {
    public List<Integer> circularPermutation(int n, int start) {
        int tot = (int)Math.pow(2, n);
        List<Integer> rs = new ArrayList<>(tot);
        // 递增遍历,利用格雷码特性,相邻两数的格雷码二进制只有一位不同
        for (int i = 0; i < tot; ++i) {
            int grayCode = i ^ (i >> 1);  // 求格雷码
            // 对start做异或，保证格雷码顺序以start值为起点而不是0
            rs.add(start ^ grayCode);
        }
        return rs;
    }
}
```






