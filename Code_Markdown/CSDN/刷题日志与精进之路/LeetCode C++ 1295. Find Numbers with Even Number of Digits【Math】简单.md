
Given an array `nums` of integers, return how many of them contain an **even number** of digits.

 

**Example 1:**

```swift
Input: nums = [12,345,2,6,7896]
Output: 2
Explanation: 
12 contains 2 digits (even number of digits). 
345 contains 3 digits (odd number of digits). 
2 contains 1 digit (odd number of digits). 
6 contains 1 digit (odd number of digits). 
7896 contains 4 digits (even number of digits). 
Therefore only 12 and 7896 contain an even number of digits.
```

**Example 2:**

```swift
Input: nums = [555,901,482,1771]
Output: 1 
Explanation: 
Only 1771 contains an even number of digits.
```

 

**Constraints:**
-  `1 <= nums.length <= 500`
 - `1 <= nums[i] <= 10^5`

题意：计算整数数组中，有多少整数的十进制位数为**偶数**。

---
### 解法 数学函数
```cpp
class Solution {
public:
    int findNumbers(vector<int>& nums) {
        int ans = 0;
        for (const int &v : nums) {
            int digits = static_cast<int>(log10(v)) + 1; 
            if (!(digits & 1)) ++ans;
        }
        return ans;
    }
};
```
提交后效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了96.80% 的用户
内存消耗：10.2 MB, 在所有 C++ 提交中击败了5.46% 的用户
```

