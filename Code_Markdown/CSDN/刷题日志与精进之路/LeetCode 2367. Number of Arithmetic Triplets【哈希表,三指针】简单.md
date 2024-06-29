 >本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given a **0-indexed**, **strictly increasing** integer array `nums` and a positive integer `diff`. A triplet `(i, j, k)` is an **arithmetic triplet** if the following conditions are met:
-   `i < j < k`,
-   `nums[j] - nums[i] == diff`, and
-   `nums[k] - nums[j] == diff`.

Return _the number of unique **arithmetic triplets**._

**Example 1:**
```java
Input: nums = [0,1,4,6,7,10], diff = 3
Output: 2
Explanation:
(1, 2, 4) is an arithmetic triplet because both 7 - 4 == 3 and 4 - 1 == 3.
(2, 4, 5) is an arithmetic triplet because both 10 - 7 == 3 and 7 - 4 == 3. 
```
**Example 2:**
```java
Input: nums = [4,5,6,7,8,9], diff = 2
Output: 2
Explanation:
(0, 2, 4) is an arithmetic triplet because both 8 - 6 == 2 and 6 - 4 == 2.
(1, 3, 5) is an arithmetic triplet because both 9 - 7 == 2 and 7 - 5 == 2.
```
**Constraints:**
-   `3 <= nums.length <= 200`
-   `0 <= nums[i] <= 200`
-   `1 <= diff <= 50`
-   `nums` is **strictly** increasing.

题意：给出一个下标从 **0** 开始、**严格递增** 的整数数组 `nums` 和一个正整数 `diff` 。如果满足下述全部条件，则三元组 `(i, j, k)` 就是一个 **等差三元组** ：
-   `i < j < k` ，
-   `nums[j] - nums[i] == diff` 且
-   `nums[k] - nums[j] == diff`

返回不同 **等差三元组** 的数目。

---
### 解法1 哈希集合
> *arithmetic triplets* 应翻译成等差三元组（如 *arithmetic progression* 翻译成等差数列）。

==由于 $\textit{nums}$ 是严格递增的，即没有重复元素，对于一个**特定且唯一的** $\textit{nums}[j]$ ，如果它在等差三元组中，那么这样的等差三元组是唯一的==，即 $$(\textit{nums}[j]-\textit{diff},\ \textit{nums}[j],\ \textit{nums}[j]+\textit{diff})$$
可用哈希集合记录 $\textit{nums}$ 的每个元素，然后遍历 $\textit{nums}$ ，看 $nums[j]−diff$ 和 $nums[j]+diff$ 是否**都在**哈希集合中。
```cpp
class Solution {
public:
    int arithmeticTriplets(vector<int>& nums, int diff) {
        int ans = 0;
        unordered_set<int> rec(nums.begin(), nums.end());
        for (int i = 0, n = nums.size(); i < n; ++i)
            if (rec.find(nums[i] - diff) != rec.end()
                && rec.find(nums[i] + diff) != rec.end())
                ++ans;
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 为 $\textit{nums}$ 的长度。
- 空间复杂度：$O(n)$ 。

优化：等差三元组可以用 $nums[k]$ 表示：
$$(nums[k]−2⋅diff,nums[k]−diff,nums[k])$$
所以还可以一边查询哈希表中是否有 $nums[k]−2⋅diff$ 和 $nums[k]−diff$ ，一边把 $nums[k]$ 插入哈希表，从而做到**一次遍历**。
```cpp
class Solution {
public:
    int arithmeticTriplets(vector<int>& nums, int diff) {
        int ans = 0;
        unordered_set<int> rec;
        for (int i = 0, n = nums.size(); i < n; ++i) {
            if (rec.find(nums[i] - 2 * diff) != rec.end()
                && rec.find(nums[i] - diff) != rec.end())
                ++ans;
            rec.insert(nums[i]);
        }
        return ans;
    }
};
```

---
### 解法2 同向三指针
由于 $nums$ 是严格递增的，遍历 $k$ 时，$i$ 和 $j$ 只增不减，因此可以用类似同向双指针的做法来移动指针：
- 枚举 $x=nums[k]$ ；
- 移动 $j$ 直到 $nums[j]+diff≥x$ ；
- **如果 $nums[j]+diff=x$** ，则移动 $i$ 直到 $nums[i]+2⋅diff≥x$ ；
- 如果 $nums[i]+2⋅diff=x$ ，则找到了一对等差三元组。

注意，下面代码在循环时没有判断 $i< j$  和 $j<k$ ，**因为一旦 $j \ge k$ ，$nums[j]+diff≥x$ 必然成立，所以 $j<k$ 无需判断**，$i$ 也同理。
```cpp
class Solution {
public:
    int arithmeticTriplets(vector<int>& nums, int diff) {
        int ans = 0, i = 0, j = 1; 
        for (int x : nums) {
            while (nums[j] + diff < x) ++j; // 直到nums[j]+diff>=x
            if (nums[j] + diff != x) continue;
            while (nums[i] + diff < nums[j]) ++i;
            if (nums[i] + diff == nums[j]) ++ans;
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 为 $nums$ 的长度。虽然写了个二重循环，但 $i++$ 和 $j++$ 的执行次数不会超过 $n$ 次，所以总的时间复杂度为 $O(n)$ 。
- 空间复杂度：$O(1)$ ，仅用到若干额外变量。
