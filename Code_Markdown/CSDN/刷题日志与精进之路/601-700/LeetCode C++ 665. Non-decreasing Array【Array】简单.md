<p>Given an array <code>nums</code> with <code>n</code> integers, your task is to check if it could become non-decreasing by modifying <strong>at most one element</strong>.</p>

<p>We define an array is non-decreasing if <code>nums[i] &lt;= nums[i + 1]</code> holds for every <code>i</code> (<strong>0-based</strong>) such that (<code>0 &lt;= i &lt;= n - 2</code>).</p>
 
<p><strong>Example 1:</strong></p>

```css
Input: nums = [4,2,3]
Output: true
Explanation: You could modify the first 4 to 1 to get a non-decreasing array. 
```

<p><strong>Example 2:</strong></p>

```css
Input: nums = [4,2,1]
Output: false
Explanation: You can't get a non-decreasing array by modify at most one element. 
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>n == nums.length</code></li>
	<li><code>1 &lt;= n &lt;= 10<sup>4</sup></code></li>
	<li><code>-10<sup>5</sup> &lt;= nums[i] &lt;= 10<sup>5</sup></code></li>
</ul>



题意：给出一个长度为 `n` 的整数数组，判断在**最多**改变 `1` 个元素的情况下，该数组能否变成一个非递减数列。

---
### 解法 遍历数组
容易想到，我们可以计算数组中 `nums[i] > nums[i + 1]` 这种情况出现的次数，然而本题没有这么简单，如 `[3,4,2,3]` 这个数组中只出现一次 `nums[i] > nums[i + 1]` 的情况，却无法通过修改一个元素使其成为非递减数组。

解题的关键在于实际修改数组+分类讨论，区分修改 `nums[i + 1]` 还是修改 `nums[i]` ，而后继续遍历：
```cpp
class Solution {
public:
    bool checkPossibility(vector<int>& nums) {
        int n = nums.size(), desc = 0;
        for (int i = 0; i < n - 1 && desc < 2; ++i) {
            if (nums[i] > nums[i + 1]) {
                ++desc; //i>0时有nums[i-1]<=nums[i]>nums[i+1],讨论nums[i-1]和nums[i+1]的关系
                if (i && nums[i - 1] > nums[i + 1]) nums[i + 1] = nums[i]; //保证非降序,必须增加nums[i+1]
                else nums[i] = nums[i + 1]; //i==0 || nums[i-1]<=nums[i+1],此时减少nums[i]为最佳选择
            }
        }
        return desc <= 1;
    }
};
```
运行效率如下：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了99.40% 的用户
内存消耗：26.3 MB, 在所有 C++ 提交中击败了74.94% 的用户
```
