> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given two 2D integer arrays, `items1` and `items2`, representing two sets of items. Each array `items` has the following properties:

-   `items[i] = [valuei, weighti]` where `valuei` represents the **value** and `weighti` represents the **weight** of the `ith` item.
-   The value of each item in `items` is **unique**.

Return _a 2D integer array_ `ret` _where_ `ret[i] = [valuei, weighti]`_,_ _with_ `weighti` _being the **sum of weights** of all items with value_ `valuei`.

**Note:** `ret` should be returned in **ascending** order by value.

**Example 1:**
```java
Input: items1 = [[1,1],[4,5],[3,8]], items2 = [[3,1],[1,5]]
Output: [[1,6],[3,9],[4,5]]
Explanation: 
The item with value = 1 occurs in items1 with weight = 1 and in items2 with weight = 5, total weight = 1 + 5 = 6.
The item with value = 3 occurs in items1 with weight = 8 and in items2 with weight = 1, total weight = 8 + 1 = 9.
The item with value = 4 occurs in items1 with weight = 5, total weight = 5.  
Therefore, we return [[1,6],[3,9],[4,5]].
```
**Example 2:**
```java
Input: items1 = [[1,1],[3,2],[2,3]], items2 = [[2,1],[3,2],[1,3]]
Output: [[1,4],[2,4],[3,4]]
Explanation: 
The item with value = 1 occurs in items1 with weight = 1 and in items2 with weight = 3, total weight = 1 + 3 = 4.
The item with value = 2 occurs in items1 with weight = 3 and in items2 with weight = 1, total weight = 3 + 1 = 4.
The item with value = 3 occurs in items1 with weight = 2 and in items2 with weight = 2, total weight = 2 + 2 = 4.
Therefore, we return [[1,4],[2,4],[3,4]].
```
**Example 3:**
```java
Input: items1 = [[1,3],[2,2]], items2 = [[7,1],[2,2],[1,4]]
Output: [[1,7],[2,4],[7,1]]
Explanation:
The item with value = 1 occurs in items1 with weight = 3 and in items2 with weight = 4, total weight = 3 + 4 = 7. 
The item with value = 2 occurs in items1 with weight = 2 and in items2 with weight = 2, total weight = 2 + 2 = 4. 
The item with value = 7 occurs in items2 with weight = 1, total weight = 1.
Therefore, we return [[1,7],[2,4],[7,1]].
```
**Constraints:**
-   `1 <= items1.length, items2.length <= 1000`
-   `items1[i].length == items2[i].length == 2`
-   `1 <= valuei, weighti <= 1000`
-   Each `valuei` in `items1` is **unique**.
-   Each `valuei` in `items2` is **unique**.

题意：给出整数数组 `items1` 和 `items2` ，表示两个物品集合。每个数组有以下特质：
-   `items[i] = [valuei, weighti]` ，`valuei` 表示第 `i` 件物品的 **价值** ，`weighti` 表示第 `i` 件物品的**重量** 。
-   `items` 中每件物品的价值都是 **唯一的** 。

返回一个二维数组 `ret`，其中 `ret[i] = [valuei, weighti]`， `weighti` 是所有价值为 `valuei` 物品的**重量之和** 。`ret` 应该按价值 **升序** 排序后返回。

---
### 解法 有序集合
由于 `items` 数组中每件物品的价值是唯一的，我们可以用价值 $k$ 作为 `map` 的键，来累计所有价值为 $k$ 的物品的重量：
```java
class Solution {
public:
    vector<vector<int>> mergeSimilarItems(vector<vector<int>>& items1, vector<vector<int>>& items2) {
        map<int, int> rec;
        for (const auto& v : items1) rec[v[0]] += v[1];
        for (const auto& v : items2) rec[v[0]] += v[1];
        vector<vector<int>> ans;
        for (const auto& it : rec) ans.push_back({it.first, it.second});
        return ans;
    }
};
```
当然也可以对 `items1, items2` 分别排序，然后双指针合并，只是我不太想修改传入的数组。