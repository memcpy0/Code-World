> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given an array `nums` of integers, a _move_ consists of choosing any element and **decreasing it by 1**.

An array `A` is a _zigzag array_ if either:

-   Every even-indexed element is greater than adjacent elements, ie. `A[0] > A[1] < A[2] > A[3] < A[4] > ...`
-   OR, every odd-indexed element is greater than adjacent elements, ie. `A[0] < A[1] > A[2] < A[3] > A[4] < ...`

Return the minimum number of moves to transform the given array `nums` into a zigzag array.

**Example 1:**
```java
Input: nums = [1,2,3]
Output: 2
Explanation: We can decrease 2 to 0 or 3 to 1.
```
**Example 2:**
```java
Input: nums = [9,6,1,6,2]
Output: 4
```
**Constraints:**
-   `1 <= nums.length <= 1000`
-   `1 <= nums[i] <= 1000`

题意：给定一个整数数组 `nums` ，一个 `move` 代表选择任一元素并将其减1。一个数组 `A` 是一个 *zigzag* ，要么其中偶数位置的值都大于邻近元素，要么奇数位置的值都大于邻近元素。返回最小的 `move` 次数，使得给出的数组 `nums` 变为一个 *zigzag* 。

---
### 解法 贪心
解法很简单，使数组变为 *zigzag* 形式只有两种做法，遍历构造奇数峰或偶数峰，选其中移动数最少的那个即可。**构造过程中，要将当前元素 `nums[i]` 左右邻近的、不符合要求的元素都降低到 `nums[i] - 1`** 。于是最初的代码如下所示：
- 每次都不会实际记录 `nums[i]` 峰值元素左侧修改后的值。
- 每次都会实际记录 `nums[i]` 峰值元素右侧修改后的值，因为后面可能还要修改。
- 时间复杂度：$O(n)$ 
- 空间复杂度：$O(1)$ 
```cpp
class Solution {
public:
    int movesToMakeZigzag(vector<int>& nums) {
        if (nums.size() == 1) return 0;
        int evenIndexedGreater = 0, oddIndexedGreater = 0; // 表示两种做法下各自需要的move数
        int temp1 = nums[0], temp2 = nums[1];
        for (int i = 0, n = nums.size(); i < n; ++i) {
            if (i & 1) { // 让奇数下标位置元素更大,要让两边元素减小
                if (temp1 >= nums[i]) // nums[i-1]已经小于nums[i-2],让它变得更小不影响后面的计数
                    oddIndexedGreater += temp1 - nums[i] + 1;
                if (i + 1 < n) {
                    if (nums[i + 1] >= nums[i]) { // 这里nums[i+1]变动会影响下一个的计数
                        oddIndexedGreater += nums[i + 1] - nums[i] + 1;
                        temp1 = nums[i] - 1;
                    } else temp1 = nums[i + 1];
                }
            } else { // 让偶数下标位置元素更大,要让两边元素减小
                if (i && temp2 >= nums[i])
                    evenIndexedGreater += temp2 - nums[i] + 1;
                if (i + 1 < n) {
                    if (nums[i + 1] >= nums[i]) {
                        evenIndexedGreater += nums[i + 1] - nums[i] + 1;
                        temp2 = nums[i] - 1;
                    } else temp2 = nums[i + 1];
                }
            }
        }
        return min(evenIndexedGreater, oddIndexedGreater);
    }
};
```
提取其中重复的代码为lambda，发现：上述做法要将两侧元素减少，比较繁琐；如果将当前元素减少到「比两侧元素中最小的一个 `min(nums[i - 1], nums[i + 1])` 」更小1（相当于上一种做法中，一次承受了左右两边峰值元素可能要求的修改），就构建出一个山谷，两侧就是峰值，**此时不必记录 `nums[i]` 修改后的值，它不会影响到下一次操作**。代码如下所示：
```cpp
class Solution {
public: 
    int movesToMakeZigzag(vector<int>& nums) {
        auto helper = [&](int start) -> int {
            int moves = 0;
            for (int i = start, n = nums.size(); i < n; i += 2) {
                int minVal = INT_MAX;
                if (i - 1 >= 0) minVal = min(minVal, nums[i - 1]);
                if (i + 1 < n) minVal = min(minVal, nums[i + 1]);
                if (minVal <= nums[i]) moves += nums[i] - minVal + 1; // 记录改变值
            }
            return moves;
        };
        return min(helper(1), helper(0));
    }
};
```