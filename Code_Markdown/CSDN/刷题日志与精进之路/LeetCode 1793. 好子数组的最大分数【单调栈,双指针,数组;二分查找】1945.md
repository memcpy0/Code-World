> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数数组 `nums` **（下标从 0 开始）**和一个整数 `k` 。

一个子数组 `(i, j)` 的 **分数** 定义为 `min(nums[i], nums[i+1], ..., nums[j]) * (j - i + 1)` 。一个 **好** 子数组的两个端点下标需要满足 `i <= k <= j` 。

请你返回 **好** 子数组的最大可能 **分数** 。

**示例 1：**
```js
输入：nums = [1,4,3,7,4,5], k = 3
输出：15
解释：最优子数组的左右端点下标是 (1, 5) ，分数为 min(4,3,7,4,5) * (5-1+1) = 3 * 5 = 15 。
```
**示例 2：**
```js
输入：nums = [5,5,4,5,4,1,1,1], k = 0
输出：20
解释：最优子数组的左右端点下标是 (0, 4) ，分数为 min(5,5,4,5,4) * (4-0+1) = 4 * 5 = 20 。
```
**提示：**
- `1 <= nums.length <= 10^5`
- `1 <= nums[i] <= 2 * 10^4`
- `0 <= k < nums.length`

---
单调栈：
- 42. 接雨水
- 84. 柱状图中最大的矩形
- 面试题17.21. 直方图的水量
- 407. 接雨水 II
- 1793. 好子数组的最大分数
### 解法 单调栈+数组
本题和42题、84题很相似，但也有不同之处。
> **提示 1**
> $\min \big\{ \textit{nums}[i], \cdots, \textit{nums}[j] \big\}$ 的取值情况最多只有 $n$ 种，其中 $n$ 是数组 $\textit{nums}$ 的长度。

> **提示 2**
> 如果我们将某个 $\textit{nums}[i]$ 作为区间中的最小值，那么要想「保持」这个最小值，区间的左右端点最远可以到哪里？

我们可以**枚举每一个下标 $i$ 对应的元素 $\textit{nums}[i]$ 作为区间的最小值**，那么区间的左端点即为「$i$ 左侧最近的那个严格大于 $\textit{nums}[i]$ 的元素下标的后一个位置」，右端点即为「$i$ 右侧最近的那个严格大于 $\textit{nums}[i]$ 的元素下标的前一个位置」，这里可以规定 $\textit{nums}[-1]=\textit{nums}[n]=\infty$ ，这样总能找到区间的左右端点。

如果 $\textit{nums}[i]$ 对应的最长区间的左端点为 $l_i$ ，右端点为 $r_i$ ，那么只要 $k \in [l_i, r_i]$ ，我们就可以使用 $\textit{nums}[i] \times (r_i-l_i+1)$ 来更新答案。

**找出左右端点**可以使用单调栈。单调栈的方法可以参考 503. 下一个更大元素 II。
> 使用两次单调栈，分别找出左右两个更大的元素的下标是可选的做法。不过实际上，我们**只需要一次单调栈**即可。

细节：注意到
- 左端点即为「$i$ 左侧最近的那个严格大于 $nums[i]$ 的元素下标的后一个位置」
- 右端点即为「$i$ 右侧最近的那个**严格大于** $nums[i]$ 的元素下标的前一个位置」

是等价于：
- 左端点即为「$i$ 左侧最近的那个严格大于 $nums[i]$ 的元素下标的后一个位置」
- 右端点即为「$i$ 右侧最近的那个**大于等于** $nums[i]$ 的元素下标的前一个位置」的。
> 提示：设 $\textit{nums}[i_0]=\textit{nums}[i_1]=\cdots=\textit{nums}[i_k]$ 并且 $i_0 < i_1 < \cdots < i_k$ 。在第一种定义下，$i_0, i_1, \cdots, i_k$​ 对应的左右端点均完全相同，因此只需要枚举到其中的任意一个下标即可。而在第二种定义下，**$i_k$ 对应的左右端点与第一种定义下的相同**，因此二者是等价的。

仿照84题的做法，代码如下：
```cpp
class Solution {
public:
    int maximumScore(vector<int>& nums, int k) {
        typedef pair<int, int> pii;
        stack<pii> st;
        int n = nums.size();
        vector<int> leftWidth(n);
        int ans = 0;
        for (int i = 0; i <= n; ++i) {
            int width = 0;
            int v = i < n ? nums[i] : 0;
            while (!st.empty() && v < nums[st.top().first]) { // 下标
                pii tmp = st.top(); st.pop();
                int rightWidth = width; // nums[i]右边的宽度
                width += tmp.second; // 宽度
                if (tmp.first - leftWidth[tmp.first] + 1 <= k && tmp.first + rightWidth >= k)
                    ans = max(ans, nums[tmp.first] * width); // 高度*总宽度
            }
            st.push({i, width + 1});
            if (i < n) leftWidth[i] = width + 1; // 更新nums[i]左边的宽度,包括nums[i]
        }
        return ans;
    }
};
```
但是我们还可通过单调栈中**栈顶的下标和后面将入栈的下标**进行计算：
```cpp
class Solution {
public:
    int maximumScore(vector<int>& nums, int k) {
        stack<int> st;
        int n = nums.size();
        int ans = 0;
        for (int i = 0; i <= n; ++i) {
            int width = 0;
            int v = i < n ? nums[i] : 0;
            while (!st.empty() && v < nums[st.top()]) {
                int idx = st.top(); st.pop();
                int l_idx = st.empty() ? 0 : (st.top() + 1);
                int r_idx = i - 1;
                if (l_idx <= k && r_idx >= k)
                    ans = max(ans, nums[idx] * (r_idx - l_idx + 1)); // 高度*总宽度
            }
            st.push(i);
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(n)$