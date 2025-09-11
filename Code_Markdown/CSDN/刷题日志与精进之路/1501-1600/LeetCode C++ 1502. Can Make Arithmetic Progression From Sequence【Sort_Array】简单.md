

Given an array of numbers `arr`. A sequence of numbers is called an arithmetic progression if the difference between any two consecutive elements is the same.

Return `true` if the array can be **rearranged** to form an arithmetic progression, otherwise, return `false`.

 

**Example 1:**

```swift
Input: arr = [3,5,1]
Output: true
Explanation: We can reorder the elements as [1,3,5] or [5,3,1] with differences 2 and -2 respectively, between each consecutive elements.
```

**Example 2:**

```swift
Input: arr = [1,2,4]
Output: false
Explanation: There is no way to reorder the elements to obtain an arithmetic progression.
```

 

**Constraints:**
- `2 <= arr.length <= 1000`
-  `-10^6 <= arr[i] <= 10^6`

题意：给出一个数字数组 `arr` 。如果一个数列中，任意相邻两项的差总等于同一个常数，那么这个数列就称为**等差数列** 。如果可以重新排列数组形成等差数列，请返回 `true` ；否则，返回 `false` 。

---
### 解法 排序
对数组从小到大排序，检查是否为等差数列即可：
```cpp
class Solution {
public:
    bool canMakeArithmeticProgression(vector<int>& arr) {
        sort(arr.begin(), arr.end());
        int diff = arr[1] - arr[0];
        for (int i = 1; i < arr.size(); ++i) 
            if (arr[i] - arr[i - 1] != diff) 
                return false;
        return true;
    }
};
```
效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了86.63% 的用户
内存消耗：8.9 MB, 在所有 C++ 提交中击败了40.27% 的用户
```
