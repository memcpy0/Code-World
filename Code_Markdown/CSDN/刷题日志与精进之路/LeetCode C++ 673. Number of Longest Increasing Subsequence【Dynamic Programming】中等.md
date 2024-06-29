<p>Given an integer array&nbsp;<code>nums</code>, return <em>the number of longest increasing subsequences.</em></p>

<p><strong>Notice</strong> that the sequence has to be <strong>strictly</strong> increasing.</p>
 
<p><strong>Example 1:</strong></p>

```clike
Input: nums = [1,3,5,4,7]
Output: 2
Explanation: The two longest increasing subsequences are [1, 3, 4, 7] and [1, 3, 5, 7].
```
<p><strong>Example 2:</strong></p> 
</pre>

```clike
Input: nums = [2,2,2,2,2]
Output: 5
Explanation: The length of longest continuous increasing subsequence is 1, and there are 5 subsequences' length is 1, so output 5.
```

 
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums.length &lt;= 2000</code></li>
	<li><code>-10<sup>6</sup> &lt;= nums[i] &lt;= 10<sup>6</sup></code></li>
</ul>

题意：求最长递增子序列的个数。

---
### 解法 动态规划
问题很简单，类似求最短路径的条数，只要增加一个 `cnt[]`，`cnt[i]` 记录**以 `nums[i]` 为结尾元素的最长递增子序列的个数**：
```cpp
class Solution {
public:
    int findNumberOfLIS(vector<int>& nums) {
        int n = nums.size(), maxLen = 0, ansCnt = 0;
        vector<int> dp(n), cnt(n); //dp[i]为以nums[i]结尾的LIS的长度,cnt[i]为对应的个数
        for (int i = 0; i < n; ++i) {
            dp[i] = 1, cnt[i] = 1; //开始只有nums[i]一个元素
            for (int j = 0; j < i; ++j) {
                if (nums[i] > nums[j]) { //nums[i]可以加到以nums[j]为结尾的LIS之后
                    if (dp[j] + 1 > dp[i]) { //更长的LIS长度
                        dp[i] = dp[j] + 1; //更新以nums[i]结尾的LIS的长度
                        cnt[i] = cnt[j];   //继承cnt[j]
                    } else if (dp[j] + 1 == dp[i]) cnt[i] += cnt[j];
                }
            } //此时确定了以nums[i]结尾的LIS的长度和个数
            if (dp[i] > maxLen) { maxLen = dp[i]; ansCnt = cnt[i]; } //更长的LIS长度
            else if (dp[i] == maxLen) ansCnt += cnt[i]; 
        }
        return ansCnt;
    }
};
```
运行效率如下：
```cpp
执行用时：152 ms, 在所有 C++ 提交中击败了77.24% 的用户
内存消耗：12.8 MB, 在所有 C++ 提交中击败了81.65% 的用户
```
