<p>The <strong>XOR total</strong> of an array is defined as the bitwise <code>XOR</code> of<strong> all its elements</strong>, or <code>0</code> if the array is<strong> empty</strong>.</p>

For example, the <strong>XOR total</strong> of the array <code>[2,5,6]</code> is <code>2 XOR 5 XOR 6 = 1</code>.

<p>Given an array <code>nums</code>, return <em>the <strong>sum</strong> of all <strong>XOR totals</strong> for every <strong>subset</strong> of </em><code>nums</code>.&nbsp;</p>

<p><strong>Note:</strong> Subsets with the <strong>same</strong> elements should be counted <strong>multiple</strong> times.</p>

<p>An array <code>a</code> is a <strong>subset</strong> of an array <code>b</code> if <code>a</code> can be obtained from <code>b</code> by deleting some (possibly zero) elements of <code>b</code>.</p>
 
<p><strong>Example 1:</strong></p>

```clike
Input: nums = [1,3]
Output: 6
Explanation: The 4 subsets of [1,3] are:
- The empty subset has an XOR total of 0.
- [1] has an XOR total of 1.
- [3] has an XOR total of 3.
- [1,3] has an XOR total of 1 XOR 3 = 2.
0 + 1 + 3 + 2 = 6
```

 
<p><strong>Example 2:</strong></p>

```clike
Input: nums = [1,3]
Output: 6
Explanation: The 4 subsets of [1,3] are:
- The empty subset has an XOR total of 0.
- [1] has an XOR total of 1.
- [3] has an XOR total of 3.
- [1,3] has an XOR total of 1 XOR 3 = 2.
0 + 1 + 3 + 2 = 6 
```

<p><strong>Example 3:</strong></p>

```clike
Input: nums = [3,4,5,6,7,8]
Output: 480
Explanation: The sum of all XOR totals for every subset is 480.
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums.length &lt;= 12</code></li>
	<li><code>1 &lt;= nums[i] &lt;= 20</code></li>
</ul>

题意：一个数组的<strong> 异或总和</strong> 定义为数组中所有元素按位 <code>XOR</code> 的结果；如果数组为 <strong>空</strong> ，则异或总和为 <code>0</code> 。

<p>给你一个数组 <code>nums</code> ，请你求出 <code>nums</code> 中每个 <strong>子集</strong> 的 <strong>异或总和</strong> ，计算并返回这些值相加之 <strong>和</strong> 。<strong>注意：</strong>在本题中，元素 <strong>相同</strong> 的不同子集应 <strong>多次</strong> 计数。</p>

---
### 解法1 迭代求子集
```cpp
class Solution {
public:
    int subsetXORSum(vector<int>& nums) {
        int ans = 0, n = nums.size(), tot = (1 << n);
        for (int i = 0; i < tot; ++i) {
            int xorsum = 0;
            for (int j = 0; j < n; ++j) if ((i >> j) & 1) xorsum ^= nums[j]; 
            ans += xorsum;
        }
        return ans;
    }
};
```
运行效率如下：
```cpp 
执行用时：4 ms, 在所有 C++ 提交中击败了81.66% 的用户
内存消耗：6.8 MB, 在所有 C++ 提交中击败了86.40% 的用户
```
---
### 解法2 递归求子集
```cpp
class Solution {
private:
    int ans = 0;
    void dfs(const vector<int>& nums, int cur, int last, int val) {
        if (cur >= last) { ans += val; return ; }
        dfs(nums, cur + 1, last, val ^ nums[cur]);
        dfs(nums, cur + 1, last, val);
    }
public:
    int subsetXORSum(vector<int>& nums) { 
        dfs(nums, 0, nums.size(), 0);
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.9 MB, 在所有 C++ 提交中击败了69.54% 的用户
```
