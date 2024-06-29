> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given an array of strings `names`, and an array `heights` that consists of **distinct** positive integers. Both arrays are of length `n`.

For each index `i`, `names[i]` and `heights[i]` denote the name and height of the `ith` person.

Return `names` _sorted in **descending** order by the people's heights_.

**Example 1:**
```java
Input: names = ["Mary","John","Emma"], heights = [180,165,170]
Output: ["Mary","Emma","John"]
Explanation: Mary is the tallest, followed by Emma and John.
```
**Example 2:**
```JAVA
Input: names = ["Alice","Bob","Bob"], heights = [155,185,150]
Output: ["Bob","Alice","Bob"]
Explanation: The first Bob is the tallest, followed by Alice and the second Bob.
```
**Constraints:**
-   `n == names.length == heights.length`
-   `1 <= n <= 103`
-   `1 <= names[i].length <= 20`
-   `1 <= heights[i] <= 10^5`
-   `names[i]` consists of lower and upper case English letters.
-   All the values of `heights` are distinct.

题意：对于每个下标 `i`，`names[i]` 和 `heights[i]` 表示第 `i` 个人的名字和身高。请按身高 **降序** 顺序返回对应的名字数组 `names` 。

---
### 解法1 对下标数组排序
通用做法是创建一个下标数组，对下标数组排序，这样既不会打乱输入的数组，又保证了 $names[i]$ 和 $heights[i]$ 的对应关系。
```cpp
class Solution {
public:
    vector<string> sortPeople(vector<string> &names, vector<int> &heights) {
        int n = names.size(), id[n];
        iota(id, id + n, 0);
        sort(id, id + n, [&](const auto &i, const auto &j) {
            return heights[i] > heights[j];
        });
        vector<string> ans(n);
        for (int i = 0; i < n; ++i)
            ans[i] = names[id[i]];
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n\log n)$
- 空间复杂度：$O(n)$
 
---
### 解法2 哈希表
由于身高各不相同，所以可以使用哈希表记录每个身高所对应的下标，最后从高到低收集对应名字即可：
```cpp
class Solution {
public:
    vector<string> sortPeople(vector<string> &names, vector<int> &heights) {
        int n = names.size(), maxn = *max_element(heights.begin(), heights.end());
        int rec[maxn + 1]; memset(rec, -1, sizeof(rec));
        for (int i = 0; i < n; ++i) rec[heights[i]] = i;
        vector<string> ans;
        for (int i = maxn; i >= 1; --i)
            if (rec[i] != -1) ans.push_back(names[rec[i]]);
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(maxn)$ 
- 空间复杂度：$O(maxn)$
