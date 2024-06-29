

You are a product manager and currently leading a team to develop a new product. Unfortunately, the latest version of your product fails the quality check. Since each version is developed based on the previous version, all the versions after a bad version are also bad.

Suppose you have `n` versions `[1, 2, ..., n]` and you want to find out the first bad one, which causes all the following ones to be bad.

You are given an API `bool isBadVersion(version)` which returns whether `version` is bad. Implement a function to find the first bad version. You should minimize the number of calls to the API.

 

**Example 1:**

```swift
Input: n = 5, bad = 4
Output: 4
Explanation:
call isBadVersion(3) -> false
call isBadVersion(5) -> true
call isBadVersion(4) -> true
Then 4 is the first bad version.
```

**Example 2:**

```swift
Input: n = 1, bad = 1
Output: 1
```

 

**Constraints:** `1 <= bad <= n <= 2^31 - 1`

题意：实现一个函数来查找第一个错误的版本。尽量减少对调用 API 的次数。


---
### 解法 二分
```cpp
// The API isBadVersion is defined for you.
// bool isBadVersion(int version);
class Solution {
public:
    int firstBadVersion(int n) {
        int lo = 0, hi = n;
        while (lo < hi) {
            int m = lo + (hi - lo) / 2;
            if (isBadVersion(m)) hi = m;
            else lo = m + 1;
        }
        return lo;
    }
};
```
执行结果如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了17.83% 的用户
```
