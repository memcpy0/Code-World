> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

我们有一个 `n` 项的集合。给出两个整数数组 `values` 和 `labels` ，第 `i` 个元素的值和标签分别是 `values[i]` 和 `labels[i]`。还会给出两个整数 `numWanted` 和 `useLimit` 。

从 `n` 个元素中选择一个子集 `s` :
-   子集 `s` 的大小 **小于或等于** `numWanted` 。
-   `s` 中 **最多** 有相同标签的 `useLimit` 项。

一个子集的 **分数** 是该子集的值之和。

返回子集 `s` 的最大 **分数** 。

**示例 1：**
```java
输入：values = [5,4,3,2,1], labels = [1,1,2,2,3], numWanted = 3, useLimit = 1
输出：9
解释：选出的子集是第一项，第三项和第五项。
```
**示例 2：**
```java
输入：values = [5,4,3,2,1], labels = [1,3,3,3,2], numWanted = 3, useLimit = 2
输出：12
解释：选出的子集是第一项，第二项和第三项。
```
**示例 3：**
```java
输入：values = [9,8,8,7,6], labels = [0,0,0,1,1], numWanted = 3, useLimit = 1
输出：16
解释：选出的子集是第一项和第四项。
```
**提示：**
-   `n == values.length == labels.length`
-   `1 <= n <= 2 * 10^4`
-   `0 <= values[i], labels[i] <= 2 * 10^4`
-   `1 <= numWanted, useLimit <= n`

---
### 解法 贪心+排序+哈希表
首先将元素按照 $\textit{values}$ 的值进行降序排序。待排序完成后，依次遍历每个元素并判断是否选择。如果已选择的元素个数超过了 `numWanted` 就退出，还用一个哈希表记录每一种标签已选择的元素个数（键表示标签，值表示该标签已经选择的元素个数）。

如果当前元素的标签在哈希表中对应的值等于 $\textit{useLimit}$ ，我们忽略这个元素，否则我们选择这个元素，并更新哈希表以及答案。
> 实现细节：由于题目中的 $\textit{values}$ 和 $\textit{labels}$ 是分成两个数组给出的，直接排序会比较困难。**可额外开辟一个同样长度为 $n$ 的数组，存储下标，并直接在该数组上进行排序**。
```cpp
class Solution { 
public:
    int largestValsFromLabels(vector<int>& values, vector<int>& labels, int numWanted, int useLimit) { 
        vector<int> id;
        int n = labels.size();
        for (int i = 0; i < n; ++i) id.push_back(i);
        sort(id.begin(), id.end(), [&](int a, int b) {
            return values[a] > values[b];
        }); 
        int ans = 0;
        unordered_map<int, int> hasUsed;
        for (int i = 0; i < n && numWanted; ++i) {
            int label = labels[id[i]];
            if (hasUsed[label] >= useLimit) continue; 
            ans += values[id[i]]; // 加上可选的最大的那个分数
            ++hasUsed[label];
            --numWanted;
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n\log⁡n)$
- 空间复杂度：$O(n)$ ，即为存储下标的数组以及哈希表需要使用的空间。
 