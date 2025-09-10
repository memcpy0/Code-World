> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始、长度为 `n` 的整数数组 `nums` ，以及整数 `indexDifference` 和整数 `valueDifference` 。

你的任务是从范围 `[0, n - 1]` 内找出  **2** 个满足下述所有条件的下标 `i` 和 `j` ：
- `abs(i - j) >= indexDifference` 且
- `abs(nums[i] - nums[j]) >= valueDifference`

返回整数数组 `answer`。如果存在满足题目要求的两个下标，则 `answer = [i, j]` ；否则，`answer = [-1, -1]` 。如果存在多组可供选择的下标对，只需要返回其中任意一组即可。

**注意：**`i` 和 `j` 可能 **相等** 。

**示例 1：**
```js
输入：nums = [5,1,4,1], indexDifference = 2, valueDifference = 4
输出：[0,3]
解释：在示例中，可以选择 i = 0 和 j = 3 。
abs(0 - 3) >= 2 且 abs(nums[0] - nums[3]) >= 4 。
因此，[0,3] 是一个符合题目要求的答案。
[3,0] 也是符合题目要求的答案。
```
**示例 2：**
```js
输入：nums = [2,1], indexDifference = 0, valueDifference = 0
输出：[0,0]
解释：
在示例中，可以选择 i = 0 和 j = 0 。 
abs(0 - 0) >= 0 且 abs(nums[0] - nums[0]) >= 0 。 
因此，[0,0] 是一个符合题目要求的答案。 
[0,1]、[1,0] 和 [1,1] 也是符合题目要求的答案。 
```
**示例 3：**
```js
输入：nums = [1,2,3], indexDifference = 2, valueDifference = 4
输出：[-1,-1]
解释：在示例中，可以证明无法找出 2 个满足所有条件的下标。
因此，返回 [-1,-1] 。
```
**提示：**
- `1 <= n == nums.length <= 10^5`
- `0 <= nums[i] <= 10^9`
- `0 <= indexDifference <= 10^5`
- `0 <= valueDifference <= 10^9`

---
### 解法 双指针+维护最大最小
不妨设 $i\le j - \textit{indexDifference}$ 。

类似 [121. 买卖股票的最佳时机](https://leetcode.cn/problems/best-time-to-buy-and-sell-stock/)，我们可以在枚举 $j$ 的同时，维护 $\textit{nums}[i]$ 的最大值 $\textit{mx}$ 和最小值 $\textit{mn}$ 。那么只要满足下面两个条件中的一个，就可以返回答案了。
- $\textit{mx} -\textit{nums}[j] \ge \textit{valueDifference}$
- $\textit{nums}[j] - mn \ge \textit{valueDifference}$ 

代码实现时，可以维护最大值的下标 $\textit{maxIdx}$ 和最小值的下标 $\textit{minIdx}$ 。
> 问：为什么不用算绝对值？如果 $\textit{mx} < \textit{nums}[j]$ 并且 $|\textit{mx} - \textit{nums}[j]| \ge \textit{valueDifference}$ ，不就错过答案了吗？
> 答：不会的，如果出现这种情况，那么一定会有 $\textit{nums}[j] - mn \ge \textit{valueDifference}$ 。

```cpp
class Solution {
public:
    vector<int> findIndices(vector<int>& nums, int indexDifference, int valueDifference) {
        int maxIdx = 0, minIdx = 0;
        for (int j = indexDifference; j < nums.size(); ++j) {
            int i = j - indexDifference;
            if (nums[i] > nums[maxIdx]) maxIdx = i;
            else if (nums[i] < nums[minIdx]) minIdx = i;

            if (nums[maxIdx] - nums[j] >= valueDifference) return {maxIdx, j};
            if (nums[j] - nums[minIdx] >= valueDifference) return {minIdx, j};
        }
        return {-1, -1};
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n)$ ，其中 $n$ 为 $\textit{nums}$ 的长度。
- 空间复杂度：$\mathcal{O}(1)$ 。