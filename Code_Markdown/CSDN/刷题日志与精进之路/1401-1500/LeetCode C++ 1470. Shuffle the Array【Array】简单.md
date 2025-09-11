

Given the array `nums` consisting of `2n` elements in the form `[x1,x2,...,xn,y1,y2,...,yn]`.

*Return the array in the form* `[x1,y1,x2,y2,...,xn,yn]`.

 

**Example 1:**

```swift
Input: nums = [2,5,1,3,4,7], n = 3
Output: [2,3,5,4,1,7] 
Explanation: Since x1=2, x2=5, x3=1, y1=3, y2=4, y3=7 then the answer is [2,3,5,4,1,7].
```

**Example 2:**

```swift
Input: nums = [1,2,3,4,4,3,2,1], n = 4
Output: [1,4,2,3,3,2,4,1]
```

**Example 3:**

```swift
Input: nums = [1,1,2,2], n = 2
Output: [1,2,1,2]
```

 

**Constraints:**
- `1 <= n <= 500`
-  `nums.length == 2n`
-  `1 <= nums[i] <= 10^3`

题意：给定一个数组 `nums` ，数组中有 `2n` 个元素，按 `[x1,x2,...,xn,y1,y2,...,yn]` 的格式排列。将数组按 `[x1,y1,x2,y2,...,xn,yn]` 格式重新排列，返回重排后的数组。
 
---
### 解法1 辅助空间+双指针
```cpp
class Solution {
public:
    vector<int> shuffle(vector<int>& nums, int n) {
        int f = 0, s = n, tn = 2 * n;
        vector<int> ans;
        while (f < n && s < tn) {
            ans.push_back(nums[f++]);
            ans.push_back(nums[s++]);
        }
        return ans;
    }
};
```
时间复杂度和空间复杂度都是 $O(n)$ ，执行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了95.16% 的用户
内存消耗：9.8 MB, 在所有 C++ 提交中击败了22.60% 的用户
```
---
### 解法2 原地工作

