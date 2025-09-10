 >本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given a non-negative integer array `nums`. In one operation, you must:
-   Choose a positive integer `x` such that `x` is less than or equal to the **smallest non-zero** element in `nums`.
-   Subtract `x` from every **positive** element in `nums`.

Return _the **minimum** number of operations to make every element in_ `nums` _equal to_ `0`.

**Example 1:**
```java
Input: nums = [1,5,0,3,5]
Output: 3
Explanation:
In the first operation, choose x = 1. Now, nums = [0,4,0,2,4].
In the second operation, choose x = 2. Now, nums = [0,2,0,0,2].
In the third operation, choose x = 2. Now, nums = [0,0,0,0,0].
```
**Example 2:**
```java
Input: nums = [0]
Output: 0
Explanation: Each element in nums is already 0 so no operations are needed.
```
**Constraints:**
-   `1 <= nums.length <= 100`
-   `0 <= nums[i] <= 100`

---
### 解法1 贪心+桶排序+模拟
这道题要求计算**将非负整数数组 $nums$ 中的所有元素减少到 $0$ 的最少操作数**，用 $m$ 表示数组 $nums$ 中的最小非零元素，则每次操作可选择不超过 $m$ 的正整数 $x$ ，将数组中的每个非零元素减 $x$ 。**为了使操作数最少，应选择 $x = m$** ，理由如下。
- 当选择 $x = m$ 时，经过一次操作之后，数组中的所有元素 $m$ 都变成 $0$ ，且其余的所有非零元素都减少 $m$ 。
- 当选择 $x < m$ 时，经过一次操作之后，数组中的所有元素 $m$ 在减少 $x$ 之后仍大于 $0$ ，**为了使数组中的最小非零元素变成 $0$ ，至少还需要一次操作**，因此至少需要两次操作使数组中的所有元素 $m$ 都变成 $0$ ，且其余的所有非零元素都减少 $m$ 。

由于**当 $x < m$ 时使元素 $m$ 变成 $0$ 的操作数大于当 $x = m$ 时使元素 $m$ 变成 $0$ 的操作数**，且两种方案中，**使元素 $m$ 变成 $0$ 之后，剩余的最小非零元素相同（所有非零元素都减少 $m$ ）**，因此只有当 $x = m$ 时才能使操作数最少。根据上述分析，==应使用贪心策略使操作数最少，贪心策略为每次选择数组中的最小非零元素作为 $x$ ，将数组中的每个非零元素减 $x$== 。可以根据贪心策略模拟操作过程，计算最少操作数。

为了每次选择最小非零元素，我们可以进行排序，而本题中数组 $nums$ 的元素范围很小，所以使用桶排序。然后从左到右遍历桶数组 `cnt` ，对于每个遍历到的非零元素，该元素是此时数组中的最小非零元素，将该元素记为 $m$ ，将数组中的每个非零元素都减 $m$ （代码中用「记录减去值之和」来代替），将操作数加 $1$ 。遍历结束之后，即可得到最少操作数。
```cpp
class Solution {
public:
    int minimumOperations(vector<int>& nums) {
        int cnt[101];
        memset(cnt, 0, sizeof(cnt));
        for (int num : nums) ++cnt[num]; // 桶排序
        int step = 0, sum = 0;
        for (int m = 1; m <= 100; ++m) {
            if (cnt[m]) { // 每个遍历到的非零元素,都是此时数组中最小非零元素
                if (sum == 0) { // sum记录已经减去的值之和
                    sum = m; // 将数组中每个非零元素都减去m
                    ++step; // 操作数+1
                } else {
                    if (m - sum != 0) ++step; // 如果该元素没有被减为零,则要减去自己,操作数+1
                    sum += m - sum;
                }
            }
        }
        return step;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ 。其中 $n$ 是数组 $nums$ 的长度。排序需要 $O(n)$ 的时间，排序之后需要遍历数组一次。因此总体时间复杂度为 $O(n)$ 。
- 空间复杂度：$O(n)$ ，其中 $n$ 是数组 $nums$ 的长度。

---
### 解法2 哈希集合
我们观察到，每次操作都将数组中的所有非零元素减少一个相同的值，因此**数组中的相等元素减少到 $0$ 的操作数相等，数组中的不相等元素减少到 $0$ 的操作数不相等**。又由于使用贪心策略操作时，每次操作都会将数组中的最小非零元素减少到 $0$ ，因此**最少操作数等于数组中的不同非零元素的个数**。我们只需要统计数组 $nums$ 中有多少个不同的非零元素，即为最少操作数

```java
class Solution {
    public int minimumOperations(int[] nums) {
        Set<Integer> s = new HashSet<>();
        for (int num : nums) if (num > 0) s.add(num);
        return s.size();
    }
}
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是数组 $nums$ 的长度。需要遍历数组一次，每个非零元素加入哈希集合的时间是 $O(1)$ 。
- 空间复杂度：$O(n)$ ，其中 $n$ 是数组 $nums$ 的长度。哈希集合需要 $O(n)$ 的空间。