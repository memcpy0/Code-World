

Given a binary array, find the maximum number of consecutive 1s in this array.

**Example 1:**

```swift
Input: [1,1,0,1,1,1]
Output: 3
Explanation: The first two digits or the last three digits are consecutive 1s.
    The maximum number of consecutive 1s is 3.
```

**Note:**
- The input array will only contain `0` and `1` .
-  The length of input array is a positive integer and will not exceed 10,000

题意：给定一个二进制数组， 计算其中最大连续1的个数。

---
### 解法1 顺序遍历
只需遍历一遍，遇到一就让 `tempLen` 计数加一，遇到零就比较最大长度，遍历完了最后比较一次即可。
```cpp
class Solution {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int maxOneLen = 0, tempLen = 0;
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] == 1) ++tempLen;
            else {
                maxOneLen = max(maxOneLen, tempLen);
                tempLen = 0;
            }
        }
        return max(maxOneLen, tempLen);
    }
};
```
效率如下：
```cpp
执行用时：84 ms, 在所有 C++ 提交中击败了89.39% 的用户
内存消耗：33.2 MB, 在所有 C++ 提交中击败了6.52% 的用户
```
或者写成如下形式：
```cpp
class Solution {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int maxOneLen = 0, tempLen = 0;
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] == 1) ++tempLen;
            else tempLen = 0; 
            maxOneLen = max(maxOneLen, tempLen);
        }
        return maxOneLen;
    }
};
```
就是效率稍低一些：
```cpp
执行用时：96 ms, 在所有 C++ 提交中击败了47.61% 的用户
内存消耗：33.2 MB, 在所有 C++ 提交中击败了6.49% 的用户
```
---
### 解法2 滑动窗口
就任何滑动窗口而言，需要考虑的只有两件事：一个是窗口的起点——如何检测到一个关于 `1` 的新窗口已经启动？另一个是这个窗口的结束点——如何检测现有窗口的结束点？如果把这两件事弄清楚，就可以检测出连续的窗口。剩下的就是找到最长的窗口并返回大小。
```cpp
class Solution {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int maxOneLen = 0;
        for (int l = 0, r = 0; r < nums.size(); ++r) {
            if (nums[r] == 0) l = r + 1; //不断移动滑动窗口的起点直到指向1
            maxOneLen = max(maxOneLen, r - l + 1); 
        }
        return maxOneLen;
    }
};
```
效率很好：
```cpp
执行用时：84 ms, 在所有 C++ 提交中击败了89.39% 的用户
内存消耗：33.2 MB, 在所有 C++ 提交中击败了6.49% 的用户
```
