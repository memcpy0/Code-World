

We are given a list `nums` of integers representing a list compressed with run-length encoding.

Consider each adjacent pair of elements `[freq, val] = [nums[2*i], nums[2*i+1]]` (with `i >= 0`).  For each such pair, there are `freq` elements with value `val` concatenated in a sublist. Concatenate all the sublists from left to right to generate the decompressed list.

Return the decompressed list.

 

**Example 1:**

```swift
Input: nums = [1,2,3,4]
Output: [2,4,4,4]
Explanation: The first pair [1,2] means we have freq = 1 and val = 2 so we generate the array [2].
The second pair [3,4] means we have freq = 3 and val = 4 so we generate [4,4,4].
At the end the concatenation [2] + [4,4,4] is [2,4,4,4].
```

**Example 2:**

```swift
Input: nums = [1,1,2,3]
Output: [1,3,3]
```

**Constraints:**
-  `2 <= nums.length <= 100`
 - `nums.length % 2 == 0`
 - `1 <= nums[i] <= 100`

题意：给定一个以**行程长度编码** `run-length encoding` 压缩的整数列表 `nums` ，返回解压后的列表。

---
### 解法 辅助空间
```cpp
class Solution {
public:
    vector<int> decompressRLElist(vector<int>& nums) {
        vector<int> ans;
        int n = nums.size();
        for (int i = 0; i < n; i += 2) {
            int freq = nums[i], val = nums[i + 1];
            ans.insert(ans.end(), freq, val); //STL vector的insert
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了80.29% 的用户
内存消耗：10.3 MB, 在所有 C++ 提交中击败了15.11% 的用户
```
