> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

一个数组的 **最小乘积** 定义为这个数组中 **最小值** **乘以** 数组的 **和** 。
- 比方说，数组 `[3,2,5]` （最小值是 `2`）的最小乘积为 `2 * (3+2+5) = 2 * 10 = 20` 。

给你一个正整数数组 `nums` ，请你返回 `nums` 任意 **非空子数组** 的**最小乘积** 的 **最大值** 。由于答案可能很大，请你返回答案对  `109 + 7` **取余** 的结果。

请注意，最小乘积的最大值考虑的是取余操作 **之前** 的结果。题目保证最小乘积的最大值在 **不取余** 的情况下可以用 **64 位有符号整数** 保存。

**子数组** 定义为一个数组的 **连续** 部分。

**示例 1：**
```java
输入：nums = [1,2,3,2]
输出：14
解释：最小乘积的最大值由子数组 [2,3,2] （最小值是 2）得到。
2 * (2+3+2) = 2 * 7 = 14 。
```
**示例 2：**
```java
输入：nums = [2,3,3,1,2]
输出：18
解释：最小乘积的最大值由子数组 [3,3] （最小值是 3）得到。
3 * (3+3) = 3 * 6 = 18 。
```
**示例 3：**
```java
输入：nums = [3,1,5,6,4,2]
输出：60
解释：最小乘积的最大值由子数组 [5,6,4] （最小值是 4）得到。
4 * (5+6+4) = 4 * 15 = 60 。
```
**提示：**
- `1 <= nums.length <= 10^5`
- `1 <= nums[i] <= 10^7`

---
方法一：单调栈
提示 111

「最小乘积」的定义为「最小值」乘以「和」，由于「和」较难进行枚举，我们可以考虑枚举「最小值」。

提示 222

我们可以枚举数组中的每个元素 numsi\textit{nums}_inums 
i
​
  作为最小值。

由于数组中的元素均为正数，那么我们选择的包含 numsi\textit{nums}_inums 
i
​
  的子数组是越长越好的。

提示 222 解释

我们选择子数组的限制只有一点，那就是「numsi\textit{nums}_inums 
i
​
  必须是子数组中的最小值」。那么我们应当找到：

在 numsi\textit{nums}_inums 
i
​
 「之前」且严格小于 numsi\textit{nums}_inums 
i
​
  的元素，并且它离 numsi\textit{nums}_inums 
i
​
  最近，该元素的下标记为 lefti\textit{left}_ileft 
i
​
 ；

在 numsi\textit{nums}_inums 
i
​
 「之后」且严格小于 numsi\textit{nums}_inums 
i
​
  的元素，并且它离 numsi\textit{nums}_inums 
i
​
  最近，该元素的下标记为 righti\textit{right}_iright 
i
​
 。

如果不存在这样的元素，那么对应的 lefti=−1\textit{left}_i = -1left 
i
​
 =−1 或 righti=n\textit{right}_i = nright 
i
​
 =n，其中 nnn 是数组 nums\textit{nums}nums 的长度。

此时，闭区间 [lefti+1,righti−1][\textit{left}_i+1, \textit{right}_i-1][left 
i
​
 +1,right 
i
​
 −1] 即为包含 numsi\textit{nums}_inums 
i
​
  作为最小值且最长的子数组。

提示 333

我们可以使用单调栈来找出提示 222 中每一个 numsi\textit{nums}_inums 
i
​
  对应的 lefti\textit{left}_ileft 
i
​
  以及 righti\textit{right}_iright 
i
​
 。如果读者对「单调栈」不熟悉，或者不了解如何使用单调栈来求出这些值，可以先去尝试下面的两道题目：

496. 下一个更大元素 I
503. 下一个更大元素 II
提示 444

最终的答案即为

max⁡i=0n−1(numsi×∑j=lefti+1righti−1numsj) \max_{i=0}^{n-1} \left( \textit{nums}_i \times \sum_{j=\textit{left}_i+1}^{\textit{right}_i-1} \textit{nums}_j \right)
i=0
max
n−1
​
  
⎝
⎛
​
 nums 
i
​
 × 
j=left 
i
​
 +1
∑
right 
i
​
 −1
​
 nums 
j
​
  
⎠
⎞
​
 
其中的求和项可以通过预处理 numsj\textit{nums}_jnums 
j
​
  的前缀和数组来快速求出。

细节

下面的代码部分与上面的叙述有一些小差异：

代码中的数组 left\textit{left}left 和 right\textit{right}right 存放的是所有的 lefti+1\textit{left}_i+1left 
i
​
 +1 以及 righti−1\textit{right}_i-1right 
i
​
 −1，这样做的目的是在使用前缀和时的代码更加易读；

我们可以使用两次单调栈分别求出严格遵守定义的 lefti\textit{left}_ileft 
i
​
  和 righti\textit{right}_iright 
i
​
 。而下面的代码中只使用了一次单调栈，其中 lefti\textit{left}_ileft 
i
​
  是严格遵守定义的，而 righti\textit{right}_iright 
i
​
  是「在 numsi\textit{nums}_inums 
i
​
  之后且小于等于 numsi\textit{nums}_inums 
i
​
  并且离 numsi\textit{nums}_inums 
i
​
  最近」的元素下标。 这样的修改对答案是不会造成影响的：在严格遵守定义的条件下，答案对应的子数组中，每一个最小的元素都对应着正确的答案；而在 righti\textit{right}_iright 
i
​
  不严格遵守定义的条件下，答案对应的子数组中，只有最后一次出现的最小的元素对应着正确的答案。 由于我们需要求出的是「最大值」，因此只要有一个位置对应着正确的答案，就是没有问题的。

代码

C++
Python3
class Solution {
private:
    using LL = long long;
    static constexpr int mod = 1000000007;

public:
    int maxSumMinProduct(vector<int>& nums) {
        int n = nums.size();
        // 数组 left 初始化为 0，数组 right 初始化为 n-1
        // 设置为元素不存在时的特殊值
        vector<int> left(n), right(n, n - 1);
        // 单调栈
        stack<int> s;
        for (int i = 0; i < n; ++i) {
            while (!s.empty() && nums[s.top()] >= nums[i]) {
                // 这里的 right 是非严格定义的，right[i] 是右侧最近的小于等于 nums[i] 的元素下标
                right[s.top()] = i - 1;
                s.pop();
            }
            if (!s.empty()) {
                // 这里的 left 是严格定义的，left[i] 是左侧最近的严格小于 nums[i] 的元素下标
                left[i] = s.top() + 1;
            }
            s.push(i);
        }
        
        // 前缀和
        vector<LL> pre(n + 1);
        for (int i = 1; i <= n; ++i) {
            pre[i] = pre[i - 1] + nums[i - 1];
        }
        
        LL best = 0;
        for (int i = 0; i < n; ++i) {
            best = max(best, (pre[right[i] + 1] - pre[left[i]]) * nums[i]);
        }
        return best % mod;
    }
};
复杂度分析

时间复杂度：O(n)O(n)O(n)，其中 nnn 是数组 nums\textit{nums}nums 的长度。计算数组 left\textit{left}left 和 right\textit{right}right、前缀和以及答案都需要 O(n)O(n)O(n) 的时间。

空间复杂度：O(n)O(n)O(n)，即为单调栈和前缀和数组需要使用的空间。

作者：力扣官方题解
链接：https://leetcode.cn/problems/maximum-subarray-min-product/solutions/766091/zi-shu-zu-zui-xiao-cheng-ji-de-zui-da-zh-rq8r/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。