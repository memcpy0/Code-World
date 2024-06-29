> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given an array `nums` of positive integers. In one operation, you can choose **any** number from `nums` and reduce it to **exactly** half the number. (Note that you may choose this reduced number in future operations.)

Return _the **minimum** number of operations to reduce the sum of_ `nums` _by **at least** half._

**Example 1:**
```java
Input: nums = [5,19,8,1]
Output: 3
Explanation: The initial sum of nums is equal to 5 + 19 + 8 + 1 = 33.
The following is one of the ways to reduce the sum by at least half:
Pick the number 19 and reduce it to 9.5.
Pick the number 9.5 and reduce it to 4.75.
Pick the number 8 and reduce it to 4.
The final array is [5, 4.75, 4, 1] with a total sum of 5 + 4.75 + 4 + 1 = 14.75. 
The sum of nums has been reduced by 33 - 14.75 = 18.25, which is at least half of the initial sum, 18.25 >= 33/2 = 16.5.
Overall, 3 operations were used so we return 3.
It can be shown that we cannot reduce the sum by at least half in less than 3 operations.
```
**Example 2:**
```java
Input: nums = [3,8,20]
Output: 3
Explanation: The initial sum of nums is equal to 3 + 8 + 20 = 31.
The following is one of the ways to reduce the sum by at least half:
Pick the number 20 and reduce it to 10.
Pick the number 10 and reduce it to 5.
Pick the number 3 and reduce it to 1.5.
The final array is [1.5, 8, 5] with a total sum of 1.5 + 8 + 5 = 14.5. 
The sum of nums has been reduced by 31 - 14.5 = 16.5, which is at least half of the initial sum, 16.5 >= 31/2 = 15.5.
Overall, 3 operations were used so we return 3.
It can be shown that we cannot reduce the sum by at least half in less than 3 operations.
```
**Constraints:**
-   `1 <= nums.length <= 105`
-   `1 <= nums[i] <= 107`

题意：给出一个正整数数组 `nums` 。每一次操作中，可以从 `nums` 中选择 **任意** 一个数并将它减小到 **恰好** 一半（注意，在后续操作中可以对减半过的数继续执行操作）。返回将 `nums` 数组和 **至少** 减少一半的 **最少** 操作数。

---
### 解法1 贪心+堆
```cpp
class Solution {
public:
    int halveArray(vector<int>& nums) {
        priority_queue<double> pq;
        double sum = 0, ans = 0;
        double reduced = 0;
        for (auto v : nums) {
            sum += v;
            pq.push(v);
        }
        while (reduced * 2 < sum) {
            double v = pq.top(); pq.pop();
            reduced += 1.0 * v / 2;
            ++ans;
            pq.push(1.0 * v / 2);
        }
        return ans;
    }
};
```


用中位数分治思想可以解决的类似题目：[1648. 销售价值减少的颜色球](https://leetcode-cn.com/problems/sell-diminishing-valued-colored-balls/)，[1833. 雪糕的最大数量](https://leetcode-cn.com/problems/maximum-ice-cream-bars/)，[2141. 同时运行 N 台电脑的最长时间](https://leetcode-cn.com/problems/maximum-running-time-of-n-computers/)，[2187. 完成旅途的最少时间](https://leetcode-cn.com/problems/minimum-time-to-complete-trips/)，以及[1792. 最大平均通过率](https://leetcode.cn/problems/maximum-average-pass-ratio/)。

