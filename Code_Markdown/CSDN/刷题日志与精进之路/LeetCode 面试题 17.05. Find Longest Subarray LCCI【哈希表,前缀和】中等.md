> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given an array filled with letters and numbers, find the longest subarray with an equal number of letters and numbers.
<p>Return the subarray. If there are more than one answer, return the one which has the smallest&nbsp;index of its left endpoint. If there is no answer, return an empty arrary.</p>
<strong>Example 1:</strong>
```swift
Input: ["A","1","B","C","D","2","3","4","E","5","F","G","6","7","H","I","J","K","L","M"]

Output: ["A","1","B","C","D","2","3","4","E","5","F","G","6","7"]
```
<strong>Example 2:</strong>
```swift
Input: ["A","A"]

Output: []
```
<strong>Note: </strong> <code>array.length &lt;= 100000</code></li> 

题意：给定一个放有字符和数字的数组，找到并返回最长的子数组，其中包含的字符和数字的个数相同。若存在多个最长子数组，返回左端点最小的。若不存在这样的数组，返回一个空数组。

> 相似题目（前缀和+哈希表）推荐按照顺序完成。
> - 560. 和为 K 的子数组
> - 974. 和可被 K 整除的子数组
> - 1590. 使数组和能被 P 整除
> - 523. 连续的子数组和
> - 525. 连续数组、

---
### 解法 前缀和+哈希表
**是哪个字母或数字并不重要**，我们把该问题简化为只包含 `A` 和 `B` 的数组。寻找具有相同数量的 `A` 和 `B` 的最长子数组。这等价于「 `A` 的个数减去 `B` 的个数等于0」。

我们构建两个数组，从头开始计算到当前位置为止 `A` 和 `B` 的个数并保存。很明显，到当前位置 `i` 时如果 `A` 和 `B` 的个数相同，则整个子数组区间 `[0, i]` 中的 `A` 和 `B` 的个数相等。

以此为基础，查找不以索引 `0` 开始的、符合条件的子数组，其实很简单。假设索引 `i` 满足 `countA[i] = 3, countB[i] = 8` ，这意味着 `B` 比 `A` 多五个。如果在后面发现某位置 `j` 具有相同的差值 `countB[j] - countA[j] = 5` ，则表示子数组区间 `[i, j)` 中有相同数量的 `A` 和 `B` 。为了快速查询，这里需要使用哈希表记录前面的 **`A, B` 数量差值**和**位置下标**。实际代码如下：
```cpp
class Solution {
public:
    vector<string> findLongestSubarray(vector<string>& array) {
        if (array.empty()) return {};
        int n = array.size(), left = -1, right = -1, maxLen = 0;
        //count[i]表示从头开始的长度为i的数组中, 数字/字符出现的次数
        vector<int> countA(n + 1), countB(n + 1); 
        for (int i = 0; i < n; ++i)
            countA[i + 1] = countA[i] + isdigit(array[i][0]), countB[i + 1] = countB[i] + !isdigit(array[i][0]);
        unordered_map<int, int> rec;
        for (int i = 0; i <= n; ++i) {
            int diff = countB[i] - countA[i];
            if (rec.find(diff) != rec.end()) { //存在同样的差值
                int l = rec[diff], len = i - l;
                if (len > maxLen) { //更新符合条件的子数组的最大长度
                    maxLen = len;
                    left = l, right = i; //[left,right)
                }
            } else rec[diff] = i; //记录这个差对应的位置
        }
        if (maxLen == 0) return {};
        return vector<string>(array.begin() + left, array.begin() + right); 
    }
};
```
运行效率如下：
```cpp
执行用时：296 ms, 在所有 C++ 提交中击败了74.79% 的用户
内存消耗：77.5 MB, 在所有 C++ 提交中击败了46.64% 的用户
```
改进一下，只使用一个前缀和——将数字看做 `-1` ，字符看做 `1` ，于是问题转换为**检测有没有子数组区间和为 `0`** ，这等价于「两个前缀和之差等于0」，进而等价于「两个前缀和相同」。完整的表述为：从前缀和 $presum$ 中找到两个相同的数 $presum[i]$ 和 $presum[j]$ ，计算 $i-j$ 的最大值。

为此，遍历前缀和 $presum$ 的同时，用一个数组或哈希表 $rec$ 记录 $presum[i]$ 首次出现的下标，我们需要计算的就是 $i - rec[presum[i]]$ 的最大值。
```cpp
class Solution {
public:
    vector<string> findLongestSubarray(vector<string>& array) {
        if (array.empty()) return {};
        int n = array.size();
        vector<int> presum(n + 1); //前缀和数组: 数字为-1,字符为1
        for (int i = 0; i < n; ++i) presum[i + 1] = presum[i] + (isdigit(array[i][0]) ? -1 : 1);
        //sum[j]-sum[i-1]是子数组[i,j]区间的和,如果和为0,则说明这段区间内字符和数字的个数相同 
        unordered_map<int, int> rec;
        int left = -1, right = -1, maxLen = 0; //[left,right)
        for (int i = 0; i <= n; ++i) {
            if (rec.find(presum[i]) != rec.end()) { //找到区间和为0的子数组
                int l = rec[presum[i]], r = i, len = r - l;
                if (len > maxLen) { //如果新的子数组区间长度>原最长长度,则更新
                    maxLen = len;
                    left = l, right = r;
                }
            } else rec[presum[i]] = i; 
        }
        if (maxLen == 0) return {};
        return vector<string>(array.begin() + left, array.begin() + right); 
    }
};
```
运行效率如下：
```cpp
执行用时：276 ms, 在所有 C++ 提交中击败了84.87% 的用户
内存消耗：75.6 MB, 在所有 C++ 提交中击败了68.49% 的用户
```
进一步优化，**用 `(c >> 6 & 1) * 2 - 1` 代替判断字母-数字的分支语句**。
> 对于任意小写/大写英文字母字符，其ASCII码的二进制都形如 `01xxxxxx` ；对于任意数字字符，其ASCII码的二进制都形如 `0011xxxx` 。根据这一特点，可以根据二进制从低到高第 $6$ 位（设二进制最低位是第 $0$ 位）是 $0$ 还是 $1$ 来判断：如果是 $1$ 就是小写/大写英文字母字符，如果是 $0$ 就是数字字符。把字符的二进制右移 $6$ 位再 `AND 1` 就可以得到这个比特值。然后再通过 `× 2 − 1` 的操作，把 $1$ 转换成 $1$ ，$0$ 转换成 $−1$ 。

更重要的是，**前缀和数组可以简化成一个整数变量 `partial_sum`** ，一边遍历给出的字符串数组、一边计算 `partial_sum` 。同时注意到， `partial_sum` 不会超过 $[-n,n]$ 的范围，哈希表可以用一个长为 $2n+1$ 的数组代替（数组比哈希表快）。数组可以初始化成 $−1$ ，表示相应的值没有遇到过。此外，为避免出现负数下标，`partial_sum`  应初始化成 $n$ 。
```cpp
class Solution {
public:
    vector<string> findLongestSubarray(vector<string>& array) {
        int n = array.size(), left = 0, right = 0;
        int partial_sum = n, rec[n + n + 1];
        memset(rec, -1, sizeof(rec));
        rec[partial_sum] = 0; // partial_sum[0]=0
        for (int i = 1; i <= n; ++i) {
            partial_sum += ((array[i - 1][0] >> 6) & 1) * 2 - 1;
            int j = rec[partial_sum];
            if (j < 0) rec[partial_sum] = i;
            else if (i - j > right - left) left = j, right = i; 
        }
        return {array.begin() + left, array.begin() + right};  
    }
};
```