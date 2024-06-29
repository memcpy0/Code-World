

Given a sorted array of strings that is interspersed with empty strings, write a method to find the location of a given string.

**Example1:**

```swift
 Input: words = ["at", "", "", "", "ball", "", "", "car", "", "","dad", "", ""], s = "ta"
 Output: -1
 Explanation: Return -1 if s is not in words.
```

**Example2:**

```swift
 Input: words = ["at", "", "", "", "ball", "", "", "car", "", "","dad", "", ""], s = "ball"
 Output: 4
```

**Note:** `1 <= words.length <= 1000000`


题意：稀疏数组搜索。有个排好序的字符串数组，其中散布着一些空字符串，编写一种方法，找出给定字符串的位置。

---
### 解法1 顺序遍历
```cpp
class Solution {
public:
    int findString(vector<string>& words, string s) {
        int n = words.size();
        for (int i = 0; i < n; ++i) 
            if (words[i] == s)
                return i;
        return -1;
    }
};
```
竟然能过，效率如下：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了90.33% 的用户
内存消耗：11.7 MB, 在所有 C++ 提交中击败了13.72% 的用户
```
---
### 解法2 二分查找
类似于[81. 搜索旋转排序数组 II](https://leetcode-cn.com/problems/search-in-rotated-sorted-array-ii/)，当遇到了**特殊值**导致正常的二分无法继续时，就临时退化为线性遍历：
```cpp
class Solution {
public:
    int findString(vector<string>& words, string s) {
        int lo = 0, hi = words.size() - 1;
        while (lo <= hi) {
            while (lo <= hi && words[lo] == "") ++lo; //特殊值
            while (lo <= hi && words[hi] == "") --hi; //特殊值
            if (lo > hi) return -1;
            int mid = (lo + hi) / 2;
            while (mid <= hi && words[mid] == "") ++mid; //特殊值
            
            if (words[mid] == s) return mid;
            else if (s < words[mid]) hi = mid - 1;
            else lo = mid + 1; //if (s > words[mid])
        }
        return -1;
    }
};
```
运行效率如下：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了90.33% 的用户
内存消耗：11.7 MB, 在所有 C++ 提交中击败了18.58% 的用户
```
