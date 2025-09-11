Given an array `A` of integers, return true if and only if it is a valid mountain array. 


Recall that A is a mountain array if and only if:
- `A.length >= 3`
- There exists some `i` with `0 < i < A.length - 1` such that:
    - `A[0] < A[1] < ... A[i-1] < A[i]`
    - `A[i] > A[i+1] > ... > A[A.length - 1]`


 

**Example 1:**
```clike
Input: [2,1]
Output: false
```

**Example 2:**
```clike
Input: [3,5,5]
Output: false
```

**Example 3:**
```clike
Input: [0,3,2,1]
Output: true
```

**Note:**
- `0 <= A.length <= 10000`
- `0 <= A[i] <= 10000` 

 题意：给定一个整数数组 `A` ，如果它是有效的山脉数组，就返回 `true` ；否则返回 `false` 。


---
### 解法 向中间扫描
山脉数组，即高点在数组中间，向左右两侧严格递减：
```cpp
class Solution {
public:
    bool validMountainArray(vector<int>& A) {
        if (A.size() < 3) return false;
        int n = A.size(), lo = 0, hi = n - 1;
        while (lo < hi && A[lo] < A[lo + 1]) ++lo;
        if (lo == 0 || lo == n - 1) return false;
        while (lo < hi && A[hi] < A[hi - 1]) --hi;
        if (hi == 0 || hi == n - 1) return false;
        return lo == hi;
    }
};
```
效率如下：
```cpp
执行用时：56 ms, 在所有 C++ 提交中击败了94.35% 的用户
内存消耗：21.1 MB, 在所有 C++ 提交中击败了5.42% 的用户
```
----
**20201103 Update** 今日打卡代码：
```cpp
class Solution {
public:
    bool validMountainArray(vector<int>& A) {
        int n = A.size(), lo = 0, hi = n - 1;
        while (lo < hi && A[lo] < A[lo + 1]) ++lo;
        while (hi > lo && A[hi] < A[hi - 1]) --hi;
        return lo == hi && lo != n - 1 && hi != 0;
    }
};
```
效率如下：
```cpp
执行用时：52 ms, 在所有 C++ 提交中击败了96.94% 的用户
内存消耗：21 MB, 在所有 C++ 提交中击败了13.28% 的用户
```
