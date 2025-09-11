<div class="notranslate"><p>You are given a <strong>sorted unique</strong> integer array <code>nums</code>.</p>

<p>Return <em>the <strong>smallest sorted</strong> list of ranges that <strong>cover all the numbers in the array exactly</strong></em>. That is, each element of <code>nums</code> is covered by exactly one of the ranges, and there is no integer <code>x</code> such that <code>x</code> is in one of the ranges but not in <code>nums</code>.</p>

<p>Each range <code>[a,b]</code> in the list should be output as:</p>
<ul>
	<li><code>"a-&gt;b"</code> if <code>a != b</code></li>
	<li><code>"a"</code> if <code>a == b</code></li>
</ul>
 
<p><strong>Example 1:</strong></p>

```cpp
Input: nums = [0,1,2,4,5,7]
Output: ["0->2","4->5","7"]
Explanation: The ranges are:
[0,2] --> "0->2"
[4,5] --> "4->5"
[7,7] --> "7"
```
 
<p><strong>Example 2:</strong></p>

```cpp
Input: nums = [0,2,3,4,6,8,9]
Output: ["0","2->4","6","8->9"]
Explanation: The ranges are:
[0,0] --> "0"
[2,4] --> "2->4"
[6,6] --> "6"
[8,9] --> "8->9"
```
 
<p><strong>Example 3:</strong></p>

```cpp
Input: nums = []
Output: []
```
 
<p><strong>Example 4:</strong></p>

```cpp
Input: nums = [-1]
Output: ["-1"] 
```

<p><strong>Example 5:</strong></p>

```cpp
Input: nums = [0]
Output: ["0"] 
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>0 &lt;= nums.length &lt;= 20</code></li>
	<li><code>-2<sup>31</sup> &lt;= nums[i] &lt;= 2<sup>31</sup> - 1</code></li>
	<li>All the values of <code>nums</code> are <strong>unique</strong>.</li>
	<li><code>nums</code> is sorted in ascending order.</li>
</ul>
</div>

题意：给定一个无重复元素的有序整数数组 `nums` 。返回 **恰好覆盖数组中所有数字** 的 **最小有序** 区间范围列表。`nums` 的每个元素都恰好被某个区间范围所覆盖，并且不存在属于某个范围但不属于 `nums` 的数字 `x` 。列表中的每个区间范围 `[a,b]` 应该按如下格式输出：`"a->b"` ，如果 `a != b` ；`"a"` ，如果 `a == b` 。

---
# 解法 顺序遍历
使用while循环，避免最后一个区间的特殊处理：
```cpp
class Solution {
public:
    vector<string> summaryRanges(vector<int>& nums) {
        vector<string> ans; 
        int n = nums.size(), i = 0;
        while (i < n) {
            int j = i + 1;
            while (j < n && nums[j] == nums[j - 1] + 1) ++j; //[i,j)
            if (i + 1 < j) ans.push_back(to_string(nums[i]) + "->" + to_string(nums[j - 1]));
            else ans.push_back(to_string(nums[i]));
            i = j;
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7.1 MB, 在所有 C++ 提交中击败了41.71% 的用户
```



