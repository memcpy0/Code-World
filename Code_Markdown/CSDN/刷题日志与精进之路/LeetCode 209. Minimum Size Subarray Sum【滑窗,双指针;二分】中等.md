> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given an array of `n` positive integers and a **positive** integer `s`, find the **minimal** length of a **contiguous** subarray of which the sum `≥ s`. If there isn't one, return `0` instead.

**Example**: 
```cpp
Input: s = 7, nums = [2,3,1,2,4,3]
Output: 2
Explanation: the subarray [4,3] has the minimal length under the problem constraint.
```
**Follow up**: If you have figured out the `O(n)` solution, try coding another solution of which the time complexity is `O(nlogn)`. 

题意：给出一个由正整数组成的长为 `n` 的区间，找到最短的连续区间——其区间和满足 `>= s` 的条件。

---
### 解法1 
和之前做过的“红魔馆爆炸了”几乎一样，所以一上手就这么写了。通过二分搜索长度，然后用固定长度的滑动窗口搜索数组，得到最大的区间和，如果可以满足 `>=s` 的条件，则 `hi = mid` ，因为要找到的是最短的连续区间；不满足则 `lo = mid + 1` 。

代码1：
```cpp
class Solution {
public:
    int minSubArrayLen(int s, vector<int>& nums) {
        if (nums.empty()) return 0; 
        int lo = 0, hi = nums.size(); 
        while (lo < hi) { 
            int mid = lo + (hi - lo) / 2, maxWin = 0, nowWin = 0;
            for (int i = 0; i < nums.size(); ++i) {
                if (i < mid) maxWin += nums[i];
                else {
                    //求出固定为mid长度的区间的最大区间和
                    nowWin = (i == mid) ? maxWin : nowWin;
                    nowWin = nowWin + nums[i] - nums[i - mid];
                    maxWin = max(nowWin, maxWin);
                }
            } //求出第一个满足>=s的连续区间长度
            if (maxWin >= s) hi = mid;
            else lo = mid + 1;
        }
        if (lo >= nums.size()) {
            int sum = 0;
            for (int i = 0; i < nums.size(); ++i) 
                sum += nums[i];
            if (sum < s) lo = 0;
        }
        return lo;
    }
};
```
效率不高，但是能过：
```
执行用时：24 ms, 在所有 C++ 提交中击败了28.25% 的用户
内存消耗：10.3 MB, 在所有 C++ 提交中击败了12.50% 的用户
```
 虽然是 $\text{O(nlogn)}$ ，但是反复统计区间和浪费了太多的效率，用前缀和优化一下。代码2如下：
```cpp
class Solution {
public:
    int minSubArrayLen(int s, vector<int>& nums) {
        if (nums.empty()) return 0; 
        int lo = 0, hi = nums.size(); 
        vector<int> preSum(nums.size() + 1, 0);
        for (int i = 0; i < nums.size(); ++i) preSum[i + 1] = preSum[i] + nums[i];
        while (lo < hi) { 
            int mid = lo + (hi - lo) / 2, maxWin = 0;
            for (int i = mid; i < preSum.size(); ++i) 
                maxWin = max(preSum[i] - preSum[i - mid], maxWin);
            //求出第一个满足>=s的连续区间长度
            if (maxWin >= s) hi = mid;
            else lo = mid + 1;
        } 
        if (preSum.back() < s) lo = 0; //整个区间和都无法满足条件
        return lo;
    }
};
```
效率：
```
执行用时：20 ms, 在所有 C++ 提交中击败了45.59% 的用户
内存消耗：10.4 MB, 在所有 C++ 提交中击败了12.50% 的用户
```
要不先前缀和，再二分？

---
### 解法2 滑动窗口
前面的滑动窗口都是固定长度的；如果我们允许窗口扩张和收缩，就能得到 $\text{O(n)}$ 的解法。
```cpp
class Solution {
public:
    int minSubArrayLen(int s, vector<int>& nums) {
        if (nums.empty()) return 0; 
        int winSum = 0, left = 0, right = 0, minLen = INT_MAX;
        while (right < nums.size()) {
            winSum += nums[right++]; //right不断右移
            while (winSum >= s) { //满足条件
                minLen = min(minLen, right - left); //修改长度
                winSum -= nums[left++]; //区间收缩
            }
        } //整个区间和都无法满足条件
        return (minLen == INT_MAX) ? 0 : minLen;
    }
};
```
效率：
```
执行用时：16 ms, 在所有 C++ 提交中击败了77.76% 的用户
内存消耗：10.3 MB, 在所有 C++ 提交中击败了12.50% 的用户
```
为什么内存消耗还是这样大？不明白。而且似乎没有优化的地方了，但是还只是击败了 `77%` 的用户？？？