

An array is *monotonic* if it is either monotone increasing or monotone decreasing.

An array `A` is monotone increasing if for all `i <= j`, `A[i] <= A[j]`.  An array A is monotone decreasing if for all `i <= j`, `A[i] >= A[j]`.

Return `true` if and only if the given array `A` is monotonic.

 

**Example 1:**

```swift
Input: [1,2,2,3]
Output: true
```

**Example 2:**

```swift
Input: [6,5,4,4]
Output: true
```

**Example 3:**

```swift
Input: [1,3,2]
Output: false
```

**Example 4:**

```swift
Input: [1,2,4,5]
Output: true
```

**Example 5:**

```swift
Input: [1,1,1]
Output: true
```
**Note:**
- `1 <= A.length <= 50000`
 - `-100000 <= A[i] <= 100000`

题意：当给定的数组 `A` 是单调数组时返回 `true`，否则返回 `false`。

---
### 解法 两个标记
很简单的题目，可以单次遍历解决：
```cpp
class Solution {
public:
    bool isMonotonic(vector<int>& A) {
        if (A.size() <= 1) return true;
        bool increasing = true, decreasing = true;
        for (int i = 0; i < A.size() - 1; ++i) {
            if (A[i] < A[i + 1]) decreasing = false;
            if (A[i] > A[i + 1]) increasing = false;
        }
        return (increasing || decreasing);
    }
};
```
效率如下，比较慢：
```cpp
执行用时：140 ms, 在所有 C++ 提交中击败了89.20% 的用户
内存消耗：48.9 MB, 在所有 C++ 提交中击败了9.13% 的用户
```
为了提高效率，使用临时变量保存 `A.size() - 1` ，且中间需要及时返回：
```cpp
class Solution {
public:
    bool isMonotonic(vector<int>& A) {
        if (A.size() <= 1) return true;
        bool increasing = true, decreasing = true;
        int n = A.size() - 1;
        for (int i = 0; i < n; ++i) {
            if (A[i] < A[i + 1]) decreasing = false;
            if (A[i] > A[i + 1]) increasing = false;
            if (!increasing && !decreasing) return false;
        }
        return true;
    }
};
```
效率有一些提升：
```cpp
执行用时：112 ms, 在所有 C++ 提交中击败了99.36% 的用户
内存消耗：48.9 MB, 在所有 C++ 提交中击败了8.60% 的用户
```
