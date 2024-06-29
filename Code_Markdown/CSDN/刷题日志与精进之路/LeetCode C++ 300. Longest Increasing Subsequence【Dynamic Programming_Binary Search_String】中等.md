<p>Given an integer array <code>nums</code>, return the length of the longest strictly increasing subsequence.</p>

<p>A <strong>subsequence</strong> is a sequence that can be derived from an array by deleting some or no elements without changing the order of the remaining elements. For example, <code>[3,6,2,7]</code> is a subsequence of the array <code>[0,3,1,6,2,2,7]</code>.</p>

 
<p><strong>Example 1:</strong></p>

```css
Input: nums = [10,9,2,5,3,7,101,18]
Output: 4
Explanation: The longest increasing subsequence is [2,3,7,101], therefore the length is 4.
```

<p><strong>Example 2:</strong></p>

```css
Input: nums = [0,1,0,3,2,3]
Output: 4 
```

<p><strong>Example 3:</strong></p>
 

```cpp
Input: nums = [7,7,7,7,7,7,7]
Output: 1
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums.length &lt;= 2500</code></li>
	<li><code>-10<sup>4</sup> &lt;= nums[i] &lt;= 10<sup>4</sup></code></li>
</ul> 
 
<p><b>Follow up:</b></p>

<ul>
	<li>Could you come up with the <code>O(n<sup>2</sup>)</code> solution?</li>
	<li>Could you improve it to <code>O(n log(n))</code> time complexity?</li>
</ul> 

题意：给出一个整数数组 `nums` ，找到其中**最长严格递增子序列**的长度。

子序列是由数组派生而来的序列，删除（或不删除）数组中的元素而不改变其余元素的顺序。例如，`[3,6,2,7]` 是数组 `[0,3,1,6,2,2,7]` 的子序列。
 
---
### 解法1 动态规划
经典题目，经典解法，其中状态 `dp[i]` 意为以 `nums[i]` 结尾的最长子序列的长度：
```cpp
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        int n = nums.size(), maxLen = 0;
        vector<int> dp(n, 1); //以nums[i]结尾的最长子序列的长度
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j)
                if (nums[j] < nums[i]) //构成严格递增子序列
                    dp[i] = max(dp[i], dp[j] + 1); //得到最长的长度
            maxLen = max(maxLen, dp[i]);
        }
        return maxLen;
    }
};
```
$O(n^2)$ 的时间复杂度，运行效率如下：
```cpp
执行用时：400 ms, 在所有 C++ 提交中击败了14.57% 的用户
内存消耗：10.7 MB, 在所有 C++ 提交中击败了22.07% 的用户
```

---
### 解法2 二分查找
通过改变状态的含义，可以得到 $O(n\log n)$ 的解法。下面的 `dp[i]` 表示所有长度为 `i + 1` 的递增子序列中，最小的子序列尾部元素。以 `nums = [4, 5, 6, 3]` 为例，不同长度的递增子序列和对应的 `dp[]` 值如下：
```cpp
//长度为1
[4], [5], [6], [3]      =>  dp[0] = 3
//长度为2
[4, 5], [5, 6], [4, 6]  =>  dp[1] = 5
//长度为3
[4, 5, 6]               =>  dp[2] = 6
```
容易证明，`dp[]` 是一个递增的数组：
-  由状态定义可知，`dp[0]` 一定是所有元素中最小的那个数字 `min1` ，因为长度为 `1` 的所有递增子序列中最小的尾部数字，就是整个序列中最小的数字。
- 长度为 `2` 的所有递增子序列中，最小结尾元素一定大于 `min1` 。首先因为是递增子序列，第二个元素（结尾元素）一定大于第一个元素；假设某个序列的结尾元素小于 `min1` ，则 `dp[0]` 在第一次操作中会被更新为这个值，而非 `min1` 。
- 长度为 `3` 的所有递增子序列中，假设 `dp[]` 已经得到的前两个结尾最小元素为 `[a, b]` ，长度为 `3` 的所有递增子序列中最小的结尾数字为 `c3` ，显然 `c3 > b` 。证明：设 `[c1, c2, c3]` 为其中的一个递增子序列，且 `c3 < b` ，则必然有 `c2 < b` ，和之前的结论——`b` 是长度为 `2` 的所有递增子序列结尾最小元素相互矛盾。
- 依次类推，一步步地反证，可知 `dp[]` 一定是一个递增数组。

因此算法步骤是：
- 对每个 `nums[i]` ，二分查找 `dp[]` 中**第一个大于等于 `nums[i]` 的元素的位置**；
- 如果 `nums[i]` 比所有的 `dp[]` 都大，说明 `nums[i]` 可以放在最长子序列的末尾，形成新的最长递增子序列，最长递增子序列的长度加一；
- 如果 `dp[i - 1] < nums[i] <= dp[i]` ，说明 `nums[i]` 需要替换后面的那个 `>= nums[i]` 的数字，以保证 `dp[i]` 是**所有长度为 `i + 1` 的递增子序列中最小的子序列尾部元素**，于是更新 `dp[i]` ；
- 这样就可以维护一个 `dp[]` 数组，最后的答案就是它的真实长度

```cpp
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        int n = nums.size(), maxLen = 0;
        vector<int> dp(n);
        //dp[i]表示nums中所有长度为i+1的递增子序列中,最小的序列尾部元素值
        for (int i = 0; i < n; ++i) {
            int v = nums[i], lo = 0, hi = maxLen;
            while (lo < hi) { //找到第一个>=v的数的位置
                int mid = lo + (hi - lo) / 2;
                if (dp[mid] >= v) hi = mid;
                else lo = mid + 1;
            }
            dp[lo] = nums[i]; //更新长度为lo+1的所有递增子序列中最小的序列尾部元素值
            if (lo == maxLen) ++maxLen;
        }
        return maxLen;
    }
};
```
运行效率大大提升：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了82.94% 的用户
内存消耗：10.8 MB, 在所有 C++ 提交中击败了14.42% 的用户
```
