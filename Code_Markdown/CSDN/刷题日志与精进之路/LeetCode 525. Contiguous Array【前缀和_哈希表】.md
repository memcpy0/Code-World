
<p>Given a binary array <code>nums</code>, return <em>the maximum length of a contiguous subarray with an equal number of </em><code>0</code><em> and </em><code>1</code>.</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: nums = [0,1]
Output: 2
Explanation: [0, 1] is the longest contiguous subarray with an equal number of 0 and 1.
```
 
<p><strong>Example 2:</strong></p>

```cpp
Input: nums = [0,1,0]
Output: 2
Explanation: [0, 1] (or [1, 0]) is a longest contiguous subarray with equal number of 0 and 1. 
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums.length &lt;= 10<sup>5</sup></code></li>
	<li><code>nums[i]</code> is either <code>0</code> or <code>1</code>.</li>
</ul>

  
题意：给定一个二进制数组, 找到含有相同数量的 `0` 和 `1` 的最长连续子数组（的长度）。
 

---
### 解法 前缀和+哈希表
这一题，我们将 `nums[]` 中所有 `0` 转换为 `-1` ，所以求含有相同数量的 `0` 和 `1` 的连续子数组，等价于求元素之和等于 `0` 的子数组，比较这些子数组的长度并取最大值作为题解。
```cpp
class Solution {
public:
    int findMaxLength(vector<int>& nums) {
        int n = nums.size(), sum = 0, ans = 0;
        unordered_map<int, int> rec; //rec[k]=i表示元素和为k的子数组,最左边的坐标是i
        rec[0] = 0;
        for (int i = 0; i < n; ++i) {
            sum += !nums[i] ? -1 : nums[i];
            if (rec.find(sum) == rec.end()) rec[sum] = i + 1;
            else ans = max(ans, i + 1 - rec[sum]);
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：140 ms, 在所有 C++ 提交中击败了73.21% 的用户
内存消耗：81.8 MB, 在所有 C++ 提交中击败了22.32% 的用户
```
